from vs import vsSolution, VisualStudioVersion

class vs2022Solution(vsSolution):
    # The list of Visual Studio versions which are searched for if a build is running on the Windows platform.
    VS_VERSIONS = [
        VisualStudioVersion('2022', 'VS170COMNTOOLS', ['-G', 'Visual Studio 17 2022',       '-A', 'x64', '-T', 'v143,host=x64']),
    ]

    def __init__(self, cmdline_args, build_mode="Debug", args=[]):
        vsSolution.__init__(self, self.VS_VERSIONS, cmdline_args, build_mode, args)

        self.extra_defines += ["-DETAS_CONAN_HOST_PROFILE=conan_profile_windows_x86_vs2022"]
        if not cmdline_args.use_legacy_conan_profiles:
            self.extra_defines += ["-DETAS_CONAN_BUILD_PROFILE=conan_profile_windows_x86_vs2022"]
