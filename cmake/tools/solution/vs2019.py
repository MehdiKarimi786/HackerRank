from vs import vsSolution, VisualStudioVersion

class vs2019Solution(vsSolution):
    # The list of Visual Studio versions which are searched for if a build is running on the Windows platform.
    VS_VERSIONS = [
        VisualStudioVersion('2019', 'VS160COMNTOOLS', ['-G', 'Visual Studio 16 2019',       '-A', 'x64', '-T', 'v142,host=x64']),
    ]

    def __init__(self, cmdline_args, build_mode="Debug", args=[]):
        vsSolution.__init__(self, self.VS_VERSIONS, cmdline_args, build_mode, args)

        self.extra_defines += ["-DETAS_CONAN_HOST_PROFILE=conan_profile_windows_x86_vs2019"]
        if not cmdline_args.use_legacy_conan_profiles:
            self.extra_defines += ["-DETAS_CONAN_BUILD_PROFILE=conan_profile_windows_x86_vs2019"]
