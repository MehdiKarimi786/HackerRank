from linux import linuxSolution
import os
import platform
import subprocess


class linuxx86Solution(linuxSolution):
    def __init__(self, cmdline_args, build_mode="Debug", args=[]):
        linuxSolution.__init__(self, cmdline_args, build_mode, args)
        self.architecture = "linux-x86"
        self.install_conan_config = True
        if cmdline_args.sonarscan:
            self.scanprog = ['build-wrapper-linux-x86-64']
            self.scanprog += ['--out-dir']
            self.scanprog += ['sonaroutdir']
            self.scanprog += ['cmake']
        self.extra_defines += [f"-DETAS_CONAN_HOST_PROFILE={self.getConanProfileName(cmdline_args.arch)}"]
        if not cmdline_args.use_legacy_conan_profiles:
            self.extra_defines += [f"-DETAS_CONAN_BUILD_PROFILE={self.getConanProfileName(cmdline_args.arch)}"]