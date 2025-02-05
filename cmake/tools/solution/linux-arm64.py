from linux import linuxSolution
import os
import platform
import subprocess



def getDistribution():
    path="/etc/debian_version"
    if os.path.exists(path):
        with open(path) as f:
            rel=f.readline()
            print("Debian release: " + str(rel))
            if rel.startswith("11."):
                return "bullseye"
            elif rel.startswith("10."):
                return "buster"
            elif rel.startswith("9."):
                return "stretch"
    return ""


class linuxarm64Solution(linuxSolution):
    def __init__(self, cmdline_args, build_mode="Debug", args=[]):
        linuxSolution.__init__(self, cmdline_args, build_mode, args)
        self.architecture = "linux-arm64"
        toolchainpath = os.path.dirname(os.path.abspath(__file__)) + "/../cmake/toolchain"
        toolchain_name = 'gcc'
        self.extra_defines += ["-DCMAKE_TOOLCHAIN_FILE=" + toolchainpath + "/" + toolchain_name + "-arm64-toolchain.txt"]
        self.extra_defines += [f"-DETAS_CONAN_HOST_PROFILE={self.getConanProfileName(cmdline_args.arch)}"]
        if not cmdline_args.use_legacy_conan_profiles:
            self.extra_defines += [f"-DETAS_CONAN_BUILD_PROFILE={self.getConanProfileName('linux-x86')}"]
