import os
import platform
from Solution import Solution
from qnx import qnxSolution, qnxVersions


class qnx710x86Solution(qnxSolution):
    def __init__(self, cmdline_args, build_mode="Debug", args=[]):
        qnxSolution.__init__(self, cmdline_args, qnxVersions.QNX710, "x86", build_mode, args)
        self.install_conan_config = True
