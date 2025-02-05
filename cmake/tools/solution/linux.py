from Solution import Solution
import multiprocessing
import re

class linuxSolution(Solution):
    def __init__(self, cmdline_args, build_mode="Debug", args=[]):
        Solution.__init__(self, cmdline_args, build_mode, args)
        self.architecture = "linux"
        self.num_cpu = multiprocessing.cpu_count()
        if cmdline_args.distribution:
            self.distribution = cmdline_args.distribution
        else:
            self.distribution = 'distribution-undefined'
            with open('/etc/os-release') as fp:
                for line in fp:
                    m = re.search(r'^VERSION_CODENAME=(\S+)$', line)
                    if m:
                        self.distribution = m.group(1)
                        break
                        
        self.native_build_options = ['-j' + str(self.num_cpu + 1)]
        if cmdline_args.coverage and build_mode == "Debug":
            self.extra_defines += ['-DETAS_ENABLE_GCOV=ON']

    def prepare_env(self):
        pass

    def getConanProfileName(self, arch):
        dist_mapping = {
            "bionic": "ubuntu18",
            "focal": "ubuntu20",
            "jammy": "ubuntu22"
        }
        distribution_name = self.distribution if self.distribution not in dist_mapping else dist_mapping[self.distribution]

        return f"conan_profile_{arch.replace('-','_')}_{distribution_name}"
