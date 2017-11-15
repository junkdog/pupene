#set(CMAKE_CXX_CLANG_TIDY clang-tidy)

include_directories(
    ${pupene_SOURCE_DIR}/src
    ${pupene_SOURCE_DIR}/include)

set(source_path ${pupene_SOURCE_DIR}/src)

set(headers
    ${header_path}/pupene.h
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

hunter_add_package(GTest) # GTest::main
hunter_add_package(glm)
hunter_add_package(Boost COMPONENTS iostreams)

find_package(glm REQUIRED)
find_package(GTest CONFIG REQUIRED) # GTest::main
find_package(Boost CONFIG REQUIRED iostreams)

### libraries to link, PUPENE_BUILD_JSON appends to list
set(link_libraries
    GTest::main
    glm
    Boost::iostreams
)

if (${PUPENE_BUILD_JSON})
    hunter_add_package(nlohmann_json)
    find_package(nlohmann_json CONFIG REQUIRED)

    list(APPEND link_libraries
        nlohmann_json)

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
        ./puptest --gtest_filter="*"
    DEPENDS ./puptest
)