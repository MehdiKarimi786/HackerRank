from Solution import Solution
import multiprocessing
import os
import platform
import importlib
from enum import Enum


class qnxVersions(Enum):
    QNX700 = 0,
    QNX710 = 1


QnxArchMap = {
    "x86": "amd64",
    "aarch64": "arm64"
}


class qnxSolution(Solution):

    def __init__(self, cmdline_args, version: qnxVersions, arch: str, build_mode="Debug", args=[]):
        Solution.__init__(self, cmdline_args, build_mode, args)
        self.architecture = f"{version.name.lower()}-{arch}"
        self.num_cpu = multiprocessing.cpu_count()
        self.native_build_options = ['-j' + str(self.num_cpu + 1)]
        self.version = version
        toolchainpath = os.path.dirname(os.path.abspath(__file__)) + "/../cmake/toolchain"
        self.extra_defines += ["-DCMAKE_TOOLCHAIN_FILE=" + toolchainpath + f"/{self.architecture}-toolchain.txt"]
        self.extra_defines += [f"-DETAS_CONAN_HOST_PROFILE=conan_profile_{version.name.lower()}_{QnxArchMap[arch]}"]
        if not cmdline_args.use_legacy_conan_profiles:
            if platform.system() == "Linux":
                linux_x86 = importlib.import_module("linux-x86")  # Cannot use "import" due to hyphen in the name.
                build_solution = linux_x86.linuxx86Solution(cmdline_args, build_mode)
                self.extra_defines += [f"-DETAS_CONAN_BUILD_PROFILE={build_solution.getConanProfileName('linux-x86')}"]
            elif platform.system() == "Windows":
                # TODO Can't detect VS version at the moment.
                self.extra_defines += [f"-DETAS_CONAN_BUILD_PROFILE=conan_profile_windows_x86_vs2022"]
            else:
                self.extra_defines += [f"-DETAS_CONAN_BUILD_PROFILE=default"]


    def prepare_env(self):
        base_dir = ""
        try:
            environ_var = self.version.name
            base_dir = os.environ[environ_var]
        except:
            print(f"{environ_var} environment variable must be set!")
            return

        if platform.system() == "Linux":
            os.environ["QNX_HOST"] = base_dir + '/host/linux/x86_64'
        elif platform.system() == "Darwin":
            os.environ["QNX_HOST"] = base_dir + '/host/darwin/x86_64'
        else:
            os.environ["QNX_HOST"] = base_dir + '/host/win64/x86_64'

        os.environ["QNX_TARGET"] = base_dir + '/target/qnx7'
        os.environ["MAKEFLAGS"] = '-I' + base_dir + '/target/qnx7/usr/include'
        os.environ["QNX_CONFIGURATION"] = os.path.expanduser('~').replace("\\", "/") + '/.qnx'
        os.environ["PATH"] = os.environ["QNX_HOST"] + '/usr/bin' + os.pathsep + os.environ["PATH"]
        os.environ["PATH"] = base_dir + '/jre/bin' + os.pathsep + os.environ["PATH"]
        os.environ["PATH"] = os.environ["QNX_CONFIGURATION"] + '/bin' + os.pathsep + os.environ["PATH"]
