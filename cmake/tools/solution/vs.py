import os
import subprocess
from Solution import Solution
from pathlib import Path

def check_output(cmdline, **kwargs):
    """Logging version of subprocess.check_output"""
    return subprocess.check_output(cmdline, **kwargs)

class VisualStudioVersion:
  def __init__(self, year, cmnToolsEnvVarName, versionSpecificGeneratorArgs):
    self.year = year
    self.cmnToolsEnvVarName = cmnToolsEnvVarName
    self.versionSpecificGeneratorArgs = versionSpecificGeneratorArgs

class VisualStudioPaths:
    # The list of Visual Studio versions which are searched for if a build is running on the Windows platform.
    VS_VERSIONS = []

    def _visual_studio_default_tools_paths(self, year):
        """Default search paths for the Visual Studio tools directory."""
        return [
            r"C:\Program Files (x86)\Microsoft Visual Studio\{}\Professional\Common7\Tools".format(year),
            r"C:\Program Files (x86)\Microsoft Visual Studio\{}\Enterprise\Common7\Tools".format(year),
            r"C:\Program Files (x86)\Microsoft Visual Studio\{}\BuildTools\Common7\Tools".format(year),
            r"D:\Program Files (x86)\Microsoft Visual Studio\{}\Professional\Common7\Tools".format(year),
            r"D:\Program Files (x86)\Microsoft Visual Studio\{}\Enterprise\Common7\Tools".format(year),
            r"D:\Program Files (x86)\Microsoft Visual Studio\{}\BuildTools\Common7\Tools".format(year),
            r"C:\Program Files\Microsoft Visual Studio\{}\Professional\Common7\Tools".format(year),
            r"C:\Program Files\Microsoft Visual Studio\{}\Enterprise\Common7\Tools".format(year),
            r"C:\Program Files\Microsoft Visual Studio\{}\BuildTools\Common7\Tools".format(year),
            r"D:\Program Files\Microsoft Visual Studio\{}\Professional\Common7\Tools".format(year),
            r"D:\Program Files\Microsoft Visual Studio\{}\Enterprise\Common7\Tools".format(year),
            r"D:\Program Files\Microsoft Visual Studio\{}\BuildTools\Common7\Tools".format(year),
        ]

    def get_tools_path_and_args(self):
        """Returns Visual Studio tools directory with Visual Studio version specific build args or raises an exception."""

        for vsVersion in self.VS_VERSIONS:
            if vsVersion.cmnToolsEnvVarName in os.environ:
                cmnToolsDir = os.environ[vsVersion.cmnToolsEnvVarName]
                return (cmnToolsDir, vsVersion.versionSpecificGeneratorArgs)
            else:
                matchingPath = next((path for path in self._visual_studio_default_tools_paths(vsVersion.year) if os.path.isdir(path)), "")
                if matchingPath:
                    os.environ[vsVersion.cmnToolsEnvVarName] = matchingPath
                    return (matchingPath, vsVersion.versionSpecificGeneratorArgs)

        envVars = [vsVersion.cmnToolsEnvVarName for vsVersion in self.VS_VERSIONS]
        triedPaths = [path for vsVersion in self.VS_VERSIONS for path in self._visual_studio_default_tools_paths(vsVersion.year)]
        raise Exception("Visual Studio seems to be installed in a non-standard location (tried {}). Set the common tools environment variable matching your Visual Studio version ({}) to point to the Common7/Tools subdirectory of your Visual Studio installation.".format(", ".join(triedPaths), "|".join(envVars)))

    def get_vcvars_path(self):
        vc_path = self.get_vc_path()
        return os.path.join(vc_path, 'Auxiliary', 'Build', 'vcvars64.bat')

    def get_installed_path(self):
        toolsPathAndArgs = self.get_tools_path_and_args()
        return os.path.join(toolsPathAndArgs[0], '..', '..')

    def get_vc_path(self):
        installed_path = self.get_installed_path()
        return os.path.join(installed_path, 'VC')

    def get_ninja_path(self):
        installed_path = self.get_installed_path()
        return os.path.join(installed_path, 'Common7', 'IDE', 'CommonExtensions', 'Microsoft', 'CMake', 'Ninja', 'ninja.exe')

    def get_vstest_console_path(self):
        installed_path = self.get_installed_path()
        return os.path.join(installed_path, 'Common7', 'IDE', 'Extensions', 'TestPlatform', 'vstest.console.exe')

    def get_code_coverage_path(self):
        installed_path = self.get_installed_path()
        return os.path.join(installed_path, 'Team Tools', 'Dynamic Code Coverage Tools', 'CodeCoverage.exe')

    def __init__(self, vsVersions=[]):
        self.VS_VERSIONS = vsVersions

class vsSolution(Solution):
    Solution._defaultGenerator = 'Ninja'
    vcPath= ''
    vcToolsVersion= ''
    ninjaPath= ''
    vsVersions=[]

    def _load_visual_studio_environment(self):
        """Modifies os.environ by adding some paths from the Visual Studio Developer command prompt and sets Visual Studio version specific build parameters."""
        vsPath = VisualStudioPaths(self.vsVersions)
        toolsPathAndArgs = vsPath.get_tools_path_and_args()
        self.vcPath = vsPath.get_vc_path()
        self.ninjaPath = vsPath.get_ninja_path()
        vcvars_bat = vsPath.get_vcvars_path()

        if not os.path.exists(vcvars_bat):
            raise Exception("{} not found".format(vcvars_bat))
        else:
            print(vcvars_bat)

        # Execute vcvars64.bat in a sandbox and copy only relevant environment variables to the current environment
        output = check_output('CALL "{}" && SET'.format(vcvars_bat), shell=True).decode('utf-8')
        for line in output.splitlines():
            if line and line[0] != '!':
                name, value = line.partition('=')[::2]
                if name.upper() in ['INCLUDE', 'LIB', 'LIBPATH', 'PATH']:
                    os.environ[name] = value.strip()
                elif name.upper() == 'VCTOOLSVERSION':
                    self.vcToolsVersion = value.strip()

    def __init__(self, vsVersions, cmdline_args, build_mode="Debug", args=[]):
        Solution.__init__(self, cmdline_args, build_mode, args)
        self.architecture = "x64"
        self.install_conan_config = False
        self.vsVersions = vsVersions

        if self.cmake_generator == 'Ninja':
            self._load_visual_studio_environment()
            hostX64Path=os.path.join(self.vcPath,'Tools','MSVC',self.vcToolsVersion,'bin','Hostx64','x64')
            self.extra_defines.append(r'-DCMAKE_AR={}'.format(os.path.join(hostX64Path, 'lib.exe')).replace("\\","/"))
            self.extra_defines.append(r'-DCMAKE_CXX_COMPILER={}'.format(os.path.join(hostX64Path, 'cl.exe')).replace("\\","/"))
            self.extra_defines.append(r'-DCMAKE_C_COMPILER={}'.format(os.path.join(hostX64Path, 'cl.exe')).replace("\\","/"))
            self.extra_defines.append(r'-DCMAKE_LINKER={}'.format(os.path.join(hostX64Path, 'link.exe')).replace("\\","/"))
            self.extra_defines.append(r'-DCMAKE_MAKE_PROGRAM={}'.format(self.ninjaPath).replace("\\","/"))
        else:
            self.extra_defines.append(rf'-DCMAKE_CONFIGURATION_TYPES={self.build_mode}')
            self.native_build_options.append('/m')

        self.build_directory = 'build/' + self.architecture + '/' + self.build_mode
        if cmdline_args.sonarscan:
            self.scanprog = ['build-wrapper-win-x86-64.exe']
            self.scanprog += ['--out-dir']
            self.scanprog += ['sonaroutdir']
            self.scanprog += ['cmake']

        if cmdline_args.coverage and build_mode == "Debug":
            self.extra_defines += ['-DETAS_ENABLE_GCOV=ON']

    def prepare_env(self):
        pass