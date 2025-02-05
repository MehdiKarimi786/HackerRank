import os
import platform
import subprocess
from Solution import Solution
from qnx import qnxSolution, qnxVersions


class qnx710aarch64Solution(qnxSolution):
    def __init__(self, cmdline_args, build_mode="Debug", args=[]):
        qnxSolution.__init__(self, cmdline_args, qnxVersions.QNX710, "aarch64", build_mode, args)
        self.install_conan_config = False
