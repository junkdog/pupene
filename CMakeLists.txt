### 3.8.0 for smoother c++17
cmake_minimum_required(VERSION 3.8.0)

if(${CMAKE_BUILD_TYPE})
    set(CMAKE_BUILD_TYPE Debug)
endif(${CMAKE_BUILD_TYPE})

MESSAGE(STATUS
    "CMAKE_BUILD_TYPE: "
    ${CMAKE_BUILD_TYPE})


project(pupene VERSION 0.1.4)

### conan configuration
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    set(conan_download_url "junkdog/cmake-conan")
    message(STATUS "Downloading conan.cmake from https://github.com/${conan_download_url}")
    file(DOWNLOAD "https://raw.githubusercontent.com/${conan_download_url}/master/conan.cmake"
        "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)
conan_cmake_run(
    CONANFILE conanfile.py
    BASIC_SETUP CMAKE_TARGETS
    BUILD missing)


### properties ###
# auto-generated files
set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")
set(version_config "${generated_dir}/${PROJECT_NAME}-config-version.cmake")
set(project_config "${generated_dir}/${PROJECT_NAME}-config.cmake")

# config
set(targets_export_name "${PROJECT_NAME}Targets")
set(namespace "${PROJECT_NAME}::")
set(desc_name ${PROJECT_NAME}-${PROJECT_VERSION})

# install directories
set(config_install_dir "lib/cmake/${PROJECT_NAME}")
set(include_install_dir "include")
set(root_lib_dir "lib")
set(lib_install_dir "${root_lib_dir}")

# compile-related
set(header_path "${pupene_SOURCE_DIR}/include")

### compiler settings ###
set(CMAKE_CXX_STANDARD 17)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        -Wabstract-final-class
        -Wabstract-vbase-init
        -Wauto-import
        -Wdouble-promotion
        -Wfloat-conversion
        -Wfloat-equal
        -Wheader-hygiene
        -Widiomatic-parentheses
        -Wimplicit-fallthrough
        -Winconsistent-missing-override
        -fno-omit-frame-pointer
    )
else()
    message(WARNING "missing compiler-specific options")
    add_compile_options(
        -std=c++1z
        -fno-omit-frame-pointer
    )
endif()

### feature config
include(FeatureSummary)

option(PUPENE_BUILD_JSON "JsonReader and JsonWriter" true)
add_feature_info(json PUPENE_BUILD_JSON "JsonReader/Writer Puppers")

option(PUPENE_BUILD_TESTS "Builds the tests" true)
add_feature_info(tests PUPENE_BUILD_TESTS "Builds the tests")

feature_summary(WHAT ALL)

### build it
add_subdirectory(src)

if(${PUPENE_BUILD_TESTS})
    enable_testing()
    add_subdirectory(test)
endif(${PUPENE_BUILD_TESTS})
