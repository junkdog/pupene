### linter
set(CMAKE_CXX_CLANG_TIDY clang-tidy)

include_directories(${header_path})

set(headers
    ${header_path}/pupene.h
    ${header_path}/pupper.h
    ${header_path}/pup-pupper.h
    ${header_path}/pup-core.h
    ${header_path}/pup.h
    ${header_path}/traits.h
    ${header_path}/null-pupper.h
    ${header_path}/debug-pupper.h
    ${header_path}/debug.h
    ${header_path}/binary-puppers.h
    ${header_path}/binary.h
    ${header_path}/util.h
)

set(sources
    debug.cpp
)

if (${PUPENE_BUILD_JSON})
    list(APPEND headers
        ${header_path}/json.h
        ${header_path}/json-puppers.h)
    list(APPEND sources
        json-puppers.cpp)
endif(${PUPENE_BUILD_JSON})

add_library(pupene
    ${sources}
    ${headers})

### Generate pupene_export.h
include(GenerateExportHeader)
generate_export_header(pupene)

### Global includes
target_include_directories(
    pupene PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
)

if (${PUPENE_BUILD_JSON})
    target_link_libraries(pupene CONAN_PKG::jsonformoderncpp)
endif(${PUPENE_BUILD_JSON})


### write version file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${version_config}" COMPATIBILITY SameMajorVersion
)

# Configure '<PROJECT-NAME>-config.cmake'
configure_package_config_file(
    "../cmake/${PROJECT_NAME}-config.cmake.in"
    "${project_config}"
    INSTALL_DESTINATION "${config_install_dir}"
)

### installing
# targets
install(
    TARGETS pupene
    EXPORT "${targets_export_name}"
    LIBRARY DESTINATION "${lib_install_dir}"
    ARCHIVE DESTINATION "${lib_install_dir}"
    RUNTIME DESTINATION "bin"
    INCLUDES DESTINATION "${include_install_dir}"
)

# headers
install(
    FILES ${headers}
    DESTINATION "${include_install_dir}/${PROJECT_NAME}"
)

# export headers
install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_export.h"
    DESTINATION "${include_install_dir}"
)

# pupene cmake files
install(
    FILES "${project_config}" "${version_config}"
    DESTINATION "${config_install_dir}"
)

# pupeneTargets.cmake
install(
    EXPORT "${targets_export_name}"
    NAMESPACE "${namespace}"
    DESTINATION "${config_install_dir}"
)

### doxygen
find_package(Doxygen REQUIRED dot)
if (DOXYGEN_FOUND)
    # general config
    # NB: cat README.md | sed  -E 's/```([a-z]+)/```{.\1}/g' > README.docs.md
    set(readme ${CMAKE_CURRENT_SOURCE_DIR}/../README.docs.md)

    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE ${readme})
    set(DOXYGEN_MARKDOWN_SUPPORT YES)
    set(DOXYGEN_CLANG_OPTIONS
        "-std=c++17 -Wno-pragma-once-outside-header")
    set(DOXYGEN_GENERATE_LATEX NO)

    # diagrams
    set(DOXYGEN_BUILTIN_STL_SUPPORT YES)
    set(DOXYGEN_CLASS_DIAGRAMS NO)
    set(DOXYGEN_COLLABORATION_GRAPH NO)

    # docs
    set(DOXYGEN_CLANG_ASSISTED_PARSING YES)
    set(DOXYGEN_JAVADOC_AUTOBRIEF YES)

    set(DOXYGEN_EXTRACT_ALL YES)
    set(DOXYGEN_EXTRACT_PRIVATE NO)
    set(DOXYGEN_EXTRACT_PACKAGE YES)
    set(DOXYGEN_EXTRACT_STATIC YES)
    set(DOXYGEN_STRIP_FROM_PATH
        ${PROJECT_SOURCE_DIR}
        ${PROJECT_BINARY_DIR})

    set(DOXYGEN_SORT_MEMBER_DOCS YES)
    set(DOXYGEN_SORT_BRIEF_DOCS YES)
    set(DOXYGEN_REFERENCED_BY_RELATION YES)
    set(DOXYGEN_REFERENCES_RELATION YES)
    set(DOXYGEN_REFERENCES_LINK_SOURCE NO)

    set(DOXYGEN_SHOW_INCLUDE_FILES YES)
    set(DOXYGEN_SOURCE_BROWSER YES)

    if (${CMAKE_VERSION} VERSION_GREATER_EQUAL 3.9)
        doxygen_add_docs(
            docs
            ${headers} ${sources} ${readme}
            COMMENT "Generate API docs"
        )
    else()
        message("CMAKE VERSION >= 3.9 required to build the docs")
    endif()
else (DOXYGEN_FOUND)
    message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)
