import os
import platform
from Solution import Solution
from qnx import qnxSolution, qnxVersions


class qnx700x86Solution(qnxSolution):
    def __init__(self, cmdline_args, build_mode="Debug", args=[]):
        qnxSolution.__init__(self, cmdline_args, qnxVersions.QNX700, "x86", build_mode, args)
        self.install_conan_config = True
