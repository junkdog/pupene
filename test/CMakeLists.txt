#set(CMAKE_CXX_CLANG_TIDY clang-tidy)

include_directories(
    ${pupene_SOURCE_DIR}/src
    ${pupene_SOURCE_DIR}/include)

set(source_path ${pupene_SOURCE_DIR}/src)

set(headers
    helpers.h
    testdata.h
    puptest-binary.h
    pupperdefs.h
    models.h
    specialized-pupper.h
)

set(sources
    ${source_path}/debug.cpp
    helpers.cpp
    puptest-binary.cpp
    traits-test.cpp
    specialized-pupper-test.cpp
    specialized-pupper.cpp
)

### libraries to link, PUPENE_BUILD_JSON appends to list
set(link_libraries
    CONAN_PKG::gtest
    CONAN_PKG::glm
    CONAN_PKG::Boost
)

if (${PUPENE_BUILD_JSON})
    list(APPEND link_libraries
        CONAN_PKG::jsonformoderncpp)

    list(APPEND headers
        ${header_path}/json.h
        ${header_path}/json-puppers.h
        puptest-json.h)
    list(APPEND sources
        ${source_path}/json-puppers.cpp
        puptest-json.cpp)
endif(${PUPENE_BUILD_JSON})

add_executable(puptest ${sources} ${headers})
target_link_libraries(puptest ${link_libraries})

add_test(tests puptest)

add_custom_target(valgrind
    COMMAND valgrind
        --track-origins=yes
        --tool=memcheck
        --leak-check=full
        --num-callers=40
        ${CMAKE_BINARY_DIR}/bin/puptest --gtest_filter="*"
    DEPENDS puptest
)