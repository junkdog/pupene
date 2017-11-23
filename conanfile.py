from conans import ConanFile, CMake, tools

class PupeneConan(ConanFile):
    name = "pupene"
    version = "0.1.4"
    license = "MIT"
    url = "https://github.com/junkdog/pupene"
    description = "A non-invasive, multi-format serialization library for C++17"
    settings = "os", "compiler", "build_type", "arch"
    # options = {"shared": [True, False]}
    # default_options = "shared=False"
    generators = "cmake"
    exports_sources = "*"
    requires = "jsonformoderncpp/2.1.1@vthiery/stable"
    build_requires = "Boost/1.64.0@conan/stable", \
                     "glm/0.9.8.5@g-truc/stable", \
                     "gtest/1.8.0@lasote/stable"

    default_options = "Boost:shared=False", \
                      "Boost:without_atomic=True", \
                      "Boost:without_chrono=True", \
                      "Boost:without_container=True", \
                      "Boost:without_context=True", \
                      "Boost:without_coroutine=True", \
                      "Boost:without_coroutine2=True", \
                      "Boost:without_date_time=True", \
                      "Boost:without_exception=True", \
                      "Boost:without_fiber=True", \
                      "Boost:without_filesystem=True", \
                      "Boost:without_graph=True", \
                      "Boost:without_graph_parallel=True", \
                      "Boost:without_iostreams=False", \
                      "Boost:without_locale=True", \
                      "Boost:without_log=True", \
                      "Boost:without_math=True", \
                      "Boost:without_metaparse=True", \
                      "Boost:without_mpi=True", \
                      "Boost:without_program_options=True", \
                      "Boost:without_random=True", \
                      "Boost:without_regex=True", \
                      "Boost:without_serialization=True", \
                      "Boost:without_signals=True", \
                      "Boost:without_system=True", \
                      "Boost:without_test=True", \
                      "Boost:without_thread=True", \
                      "Boost:without_timer=True", \
                      "Boost:without_type_erasure=True", \
                      "Boost:without_wave=True", \
                      "gtest:shared=False", \
                      "gtest:no_gmock=True"

    def config_options(self):
        self.settings.compiler.libcxx = "libstdc++11"


    def build(self):
        cmake = CMake(self)
        # cmake.configure(source_dir="%s/pupene" % self.source_folder, build_dir="./")
        cmake.configure(source_dir=self.source_folder)
        cmake.build(target="install")


    def package(self):
        print("hello")


    def package_info(self):
        self.cpp_info.libs = ["pupene"]

