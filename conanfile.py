from conans import ConanFile, CMake

class PupeneConan(ConanFile):
    name = "pupene"
    version = "0.2.0"
    license = "MIT"
    url = "https://github.com/junkdog/pupene"
    description = "A non-invasive, multi-format serialization library for C++17"
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"
    exports_sources = "*"

    options = {"json": [True, False],
               "build_tests": [True, False]}
    default_options = tuple(
        ["Boost:shared=False",
         "Boost:without_atomic=True",
         "Boost:without_chrono=True",
         "Boost:without_container=True",
         "Boost:without_context=True",
         "Boost:without_coroutine=True",
         "Boost:without_coroutine2=True",
         "Boost:without_date_time=True",
         "Boost:without_exception=True",
         "Boost:without_fiber=True",
         "Boost:without_filesystem=True",
         "Boost:without_graph=True",
         "Boost:without_graph_parallel=True",
         "Boost:without_iostreams=False",
         "Boost:without_locale=True",
         "Boost:without_log=True",
         "Boost:without_math=True",
         "Boost:without_metaparse=True",
         "Boost:without_mpi=True",
         "Boost:without_program_options=True",
         "Boost:without_random=True",
         "Boost:without_regex=True",
         "Boost:without_serialization=True",
         "Boost:without_signals=True",
         "Boost:without_system=True",
         "Boost:without_test=True",
         "Boost:without_thread=True",
         "Boost:without_timer=True",
         "Boost:without_type_erasure=True",
         "Boost:without_wave=True",

         "gtest:shared=False",
         "gtest:no_gmock=True",

         "json=True",
         "build_tests=False"])

    def config_options(self):
        self.settings.compiler.libcxx = "libstdc++11"

    def build_requirements(self):
        if self.options.build_tests:
            self.build_requires("Boost/1.64.0@conan/stable",
                                "glm/0.9.8.5@g-truc/stable",
                                "gtest/1.8.0@lasote/stable")

    def requirements(self):
        if self.options.json:
            self.requires("jsonformoderncpp/2.1.1@vthiery/stable")



    def build(self):
        cmake = CMake(self)
        cmake.configure(source_dir=self.source_folder,
                        defs={"PUPENE_BUILD_JSON": self.options.json,
                              "PUPENE_BUILD_TESTS": self.options.build_tests})
        cmake.build(target="install")

    def package_info(self):
        self.cpp_info.libs = ["pupene"]
