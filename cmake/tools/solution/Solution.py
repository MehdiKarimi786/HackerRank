import importlib
import os
import socket
import subprocess
import sys
import platform
import os
import shutil
import ErrorParser
import datetime

class Solution:
    _defaultGenerator = 'Unix Makefiles'


    def __init__(self, cmdline_args, build_mode="Debug", args=[]):
        self.architecture = 'none'
        self.extra_defines = []
        self.install_conan_config = True
        self.cmdline_args = cmdline_args
        if self.cmdline_args.generator:
            self.cmake_generator = self.cmdline_args.generator
        else:
            self.cmake_generator = self._defaultGenerator
        self.build_mode = build_mode
        self.args = args
        self.native_build_options = []
        self.build_log = 'build_all.log'
        self.build_log_old = 'build_all_old.log'
        self.build_directory = ""
        if cmdline_args.static:
            self.static = True
        else:
            self.static = False
        if cmdline_args.profiles:
            extra_profiles= ';'.join(cmdline_args.profiles)
            self.extra_defines += [f"-DETAS_EXTRA_CONAN_PROFILES={extra_profiles}"]
        if cmdline_args.macros:
            for macro in cmdline_args.macros:
                self.extra_defines += [f"-D{macro}"]

    def configure_buildsystem(self, clean=False, virtual_uc=False):
        if not self.build_directory:
            self.build_directory = 'build/' + self.architecture + '/' + self.build_mode

        if clean:
            cmakecache = self.build_directory + '/CMakeCache.txt'
            if os.path.exists(cmakecache):
                os.unlink(cmakecache)
            if os.path.exists(self.build_directory):
                try:
                    shutil.rmtree(self.build_directory, True)
                except Exception as e:
                    print("Unable to remove build directory:" + str(e))
                    exit(-1)

        if not os.path.exists(self.build_directory):
            try:
                os.makedirs(self.build_directory)
            except Exception as e:
                print("configure_cmake --> Unable to create build directory:" + str(e))
                exit(-1)

        if self.install_conan_config:
            settingsYmlPath = "buildtools/conan-build-files/settings.yml"
            if os.path.exists(settingsYmlPath):
                print("Installing conan settings...")
                os.system("conan config install " + settingsYmlPath)
            else:
                print("No conan settings found, not installing them.")
        else:
            print("Installation of Conan settings.yml is disabled.")

        p = os.getcwd()
        os.chdir(self.build_directory)

        cmake_args = []
        if virtual_uc:
            cmake_args.append("-DETAS_VIRTUAL_UC_DEVICE=ON")
        else:
            cmake_args.append("-DETAS_VIRTUAL_UC_DEVICE=OFF")

        cmake_args.append("-DCMAKE_BUILD_TYPE=" + self.build_mode)

        if self.static:
            cmake_args.append("-DBUILD_SHARED_LIBS=OFF")
        else:
            cmake_args.append("-DBUILD_SHARED_LIBS=ON")

        cmake_args += self.extra_defines

        programArgs = ['cmake']
        programArgs += ['-G']
        programArgs += [self.cmake_generator]
        programArgs += cmake_args
        programArgs += [os.path.relpath(p)]

        env = os.environ.copy()
        if self.cmdline_args.console:
            env["CLICOLOR_FORCE"] = "yes"

        print("Starting cmake for " + self.architecture + ", command:'" + " ".join(programArgs) + "'.")
        print(datetime.datetime.now())
        process = subprocess.run(programArgs, env=env)
        print(datetime.datetime.now())
        os.chdir(p)
        return (process.returncode == 0)

    def prepare_global_env(self):
        if platform.system() == "Linux":
            return
        #
        # Load default configuration and prepare environment
        #
        try:
            print("Setting up default PATHs")
            cfg = importlib.import_module('default')
            try:
                os.environ["PATH"] = os.pathsep.join(
                    os.pathsep.split(cfg.host_settings["extra_paths"] + os.environ["PATH"]))
            except:
                print("No additional paths found to add to PATH")

            for var, val in cfg.host_settings['extra_env'].items():
                if var not in os.environ:
                    os.environ[var] = val
                else:
                    print("Not setting environment variable " + var + ". Already set!")
        except:
            print("Unable to load default configuration")
        #
        # Load configuration for this specific host and set environment variables correspondingly
        #
        try:
            print("Loading PATHs for host")
            cfg = importlib.import_module(socket.gethostname())
            try:
                p = os.environ["PATH"]
                os.environ["PATH"] = os.pathsep.join(cfg.host_settings["extra_paths"]) + os.pathsep + p
            except:
                print("No additional paths found to add to PATH")

            for var, val in cfg.host_settings['extra_env'].items():
                if var in os.environ:
                    print("Overwriting existing environment variable "+var)
                os.environ[var] = val
        except:
            print("Unable to load host specific configuration for " + socket.gethostname())
        os.environ["PATH"] = os.environ["PATH"].replace(";;", ";")

    def prepare_env(self):
        raise NotImplementedError()

    def rotate_build_log(self):
        if os.path.exists(self.build_log_old):
            os.unlink(self.build_log_old)
        if os.path.exists(self.build_log):
            os.rename(self.build_log, self.build_log_old)

    def exec_build(self, ):
        if not self.build_directory:
            self.build_directory = 'build/' + self.architecture + '/' + self.build_mode
        try:
            os.chdir(self.build_directory)
        except:
            print("could not change into build directory " + self.build_directory + " . Did you run with --configure first?")
            exit(-1)

        self.rotate_build_log()
        log = open(self.build_log, 'w')

        if self.cmdline_args.sonarscan:
            programArgs = self.scanprog
        else:
            programArgs = ['cmake']
        programArgs += ['--build']
        programArgs += ['.']
        programArgs += ['--config']
        programArgs += [self.build_mode]
        if 'rebuild' in self.args:
            programArgs += ['--clean-first']
        if self.cmdline_args.targets:
            programArgs += ['--target']
            programArgs += self.cmdline_args.targets
        programArgs += ['--']
        programArgs += self.native_build_options

        env = os.environ.copy()
        if self.cmdline_args.console:
            env["CLICOLOR_FORCE"] = "yes"

        print("Starting build for " + self.architecture + ", command:'" + " ".join(programArgs) + "'.")
        print(datetime.datetime.now())
        process = subprocess.Popen(programArgs, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, universal_newlines=True, env=env)

        while True:
            out = process.stdout.readline()
            if out == '' and process.poll() is not None:
                break
            else:
                log.write(out)
                log.flush()
                if self.cmdline_args.console:
                    sys.stdout.write(out)
                    sys.stdout.flush()

        print(datetime.datetime.now())
        log.close()

        return (process.returncode == 0)

    def post_build(self):
        ErrorParser.main(self.build_log, self.build_log_old, 2)
