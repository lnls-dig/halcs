#######################################
# Library definition
#######################################

# set installation variables compliant with distributions
include(GNUInstallDirs)
# get bsmp version
include(TestBsmpVersion)
# add external project
include(ExternalProject)

ExternalProject_Add(bsmp_ext
    URL ${CMAKE_CURRENT_SOURCE_DIR}/foreign/libbsmp
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/bsmp
    CONFIGURE_COMMAND ""
    BUILD_COMMAND make clean && make
    INSTALL_COMMAND ""
    # BSMP assumes in-source build
    BUILD_IN_SOURCE ON
)

ExternalProject_Get_property(bsmp_ext BINARY_DIR)

# this is not a CMake project, so no *Config.cmake or *Targets.cmake
# files will be generated. Hardcode library and include paths so when
# libraries try to search for bsmp they can find it locally!
set(bsmp_LIBRARY_DIRECTORIES
    ${BINARY_DIR}
)
set(bsmp_INCLUDE_DIRECTORIES
    ${BINARY_DIR}/include/bsmp
    ${BINARY_DIR}/include/bsmp/..
)

# Hack to make it work, otherwise INTERFACE_INCLUDE_DIRECTORIES will not be propagated
file(MAKE_DIRECTORY ${bsmp_INCLUDE_DIRECTORIES})

set(bsmp_public_dir_headers
    ${BINARY_DIR}/include/bsmp/bsmp.h
    ${BINARY_DIR}/include/bsmp/server.h
    ${BINARY_DIR}/include/bsmp/client.h
)

# Get directory names only
list(APPEND bsmp_public_dirs "")

foreach(public_dir_header ${bsmp_public_dir_headers})
    get_filename_component(public_dir
        ${public_dir_header}
        DIRECTORY
    )

    list(APPEND bsmp_public_dirs ${public_dir})
endforeach()

# remove, well, duplicates ...
list(REMOVE_DUPLICATES bsmp_public_dirs)

# add imported library so we can link to it later
add_library(bsmp SHARED IMPORTED)

# set library locations
set_target_properties(bsmp
    PROPERTIES
    IMPORTED_LOCATION
    ${bsmp_LIBRARY_DIRECTORIES}/libbsmp.so
    INTERFACE_INCLUDE_DIRECTORIES
    "${bsmp_INCLUDE_DIRECTORIES}"
    PUBLIC_HEADER
    ${bsmp_public_dirs}
)

add_dependencies(bsmp bsmp_ext)

#######################################
# Library installer
#######################################

set(VERSION
    ${bsmp_VERSION_MAJOR}.${bsmp_VERSION_MINOR}.${bsmp_VERSION_PATCH}
)

# install headers that are inside another directory
install(DIRECTORY
    ${bsmp_public_dirs}
    DESTINATION
    ${CMAKE_INSTALL_INCLUDEDIR}
)

# as we can't intall IMPORTED libraries as TARGETS, just use plain
# install(FILES)
install(FILES
    ${bsmp_LIBRARY_DIRECTORIES}/libbsmp.so
    ${bsmp_LIBRARY_DIRECTORIES}/libbsmp.a
    DESTINATION
    ${CMAKE_INSTALL_LIBDIR}
)
