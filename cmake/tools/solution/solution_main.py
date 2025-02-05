#!/usr/bin/python3
"""ETAS GETK CMake Build Tool"""

import sys
import os
import argparse
import importlib

script_path = os.path.dirname(os.path.abspath(__file__))

architectures = {'win32': ["qnx700-x86", "qnx700-aarch64", "qnx710-x86", "qnx710-aarch64", "vs2019", "vs2022"],
                 'cygwin': ["qnx700-x86", "qnx700-aarch64", "qnx710-x86", "qnx710-aarch64"],
                 'linux': ["linux-x86", "linux-arm64", "qnx700-x86", "qnx700-aarch64", "qnx710-x86", "qnx710-aarch64"],
                 'linux2': ["linux-x86", "linux-arm64"]}

linuxDistributions = ["buster", "bullseye", "bookworm", "bionic", "focal", "jammy"]


class ArgumentParserSolution(argparse.ArgumentParser):
    def error(self, message):
        print("Error: " + message + "\n", file=sys.stderr)
        self.print_help()
        sys.exit(1)


parser = ArgumentParserSolution(description=__doc__, usage="%(prog)s [--configure] [--build] [options]")

group = parser.add_argument_group('Build steps to execute')
group.add_argument("--configure", action='store_true', help='Configure CMake and create build paths')
group.add_argument("--build", action='store_true', help='Execute build')

group = parser.add_argument_group('Build options')
group.add_argument("--arch", help="Target OS/Architecture", required=True, choices=architectures[sys.platform])
group.add_argument("--distribution", help="Optional: Linux Target Distribution", choices=linuxDistributions)
group.add_argument("--generator", action='store', help='Configure CMake generator: Ninja [Windows Default], Unix Makefiles [Linux Default], Visual Studio 16 2019,...')
group.add_argument("--clean", action='store_true', help="Clean up before creating solution")
group.add_argument("--rebuild", action='store_true', help="Rebuild solution")
group.add_argument("--console", action='store_true', help="Print build output to stdout instead of log file")
group.add_argument("--target", dest="targets", nargs="+", help="Build specific target(s) instead of the default target 'all'. Multiple targets may be given, separated by spaces.")
group.add_argument("--virtualuc", action='store_true', help="Use virtual UC device for build and tests")
group.add_argument("--sonarscan", action='store_true', help="Configure build for sonar scan with CFamily plugin")
group.add_argument("--coverage", action='store_true', help="Configure build for code coverage analysis")
group.add_argument("--static", action='store_true', help='Configure build for static libraries')
group.add_argument("--srcdir", help="Specify the directory containing the sources (default: current working directory)")
group.add_argument("--profiles", nargs="+", help="Define one or more profiles separated by a space.")
group.add_argument("--macros", nargs="+", help="Define one or more CMAKE macros separated by a space.")

useLegacyConanProfilesArgument = group.add_argument('--use-legacy-conan-profiles', action='store_true', help=argparse.SUPPRESS)  # Use legacy conan profiles instead of build and host profiles. Hidden argument, intended to be set by the project calling solution_main: `solution_main.useLegacyConanProfilesArgument.default = True`.

group = parser.add_argument_group('CMake build types (CMAKE_BUILD_TYPE)')
exclusive_group = group.add_mutually_exclusive_group()
exclusive_group.add_argument("--release", action='store_true', help="Build in Release mode (default: Debug)")
exclusive_group.add_argument("--with-debug-info", action='store_true', help="Build in Release mode (default: True)")
exclusive_group.add_argument("--min-size", action='store_true', help="Build in MinSizeRel mode (default: Debug)")
exclusive_group.add_argument("--release-with-debug-info", action='store_true', help="Build in MinSizeRel mode (default: Debug)")

def main(args):
    build_mode = "Debug"
    build_args = []

    # these options are mutually exclusive
    if args.release:
        build_mode = "Release"
    if args.with_debug_info:
        build_mode = "RelWithDbgInfo"
    if args.min_size:
        build_mode = "MinSizeRel"

    if args.rebuild:
        build_args += ["rebuild"]
        args.clean = True
        args.build = True

    builder_module = importlib.import_module(args.arch)
    builder_class = getattr(builder_module, args.arch.replace("-", "") + 'Solution')
    builder = builder_class(args, build_mode, build_args)

    if args.srcdir is not None:
        if not os.path.exists(args.srcdir):
            print("Source directory '" + args.srcdir + "' does not exist")
            sys.exit(1)
        os.chdir(args.srcdir)

    print("Setting up global environment...")
    builder.prepare_global_env()

    print("Preparing environment...")
    builder.prepare_env()

    if not args.configure and not args.build:
        print("Not doing anything because neither --configure nor --build was passed.", file=sys.stderr)
        sys.exit(1)

    if args.configure:
        print("Configuring build system...")
        retval = builder.configure_buildsystem(args.clean, args.virtualuc)
        if not retval:
            sys.exit(-1)

    if args.build:
        print("Executing regular build...")
        retval = builder.exec_build()
        if not retval:
            sys.exit(-1)

        if not args.console:
            builder.post_build()


if __name__ == '__main__':
    main(parser.parse_args())
