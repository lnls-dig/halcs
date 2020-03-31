########################################################################
# Project setup
########################################################################

cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(sdbfs)

# set modules path for CMkake to find
set(SDBFS_CMAKE_MODULES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)
list(APPEND CMAKE_MODULE_PATH ${SDBFS_CMAKE_MODULES_DIR})

# add module to prevent in-source builds
include(PreventInSourceBuilds)
# set installation variables compliant with distributions
include(GNUInstallDirs)
# configure and generate *.Config.cmake files
include(CMakePackageConfigHelpers)
# extract version numbers from header file
include(TestSdbfsVersion)
# detect target architecture
include(TargetArch)

target_architecture(ARCH)

# find pkg-config
find_package(PkgConfig)

# Will be used to add flags to pkg-config useful when apps want to statically link
set(pkg_config_libs_private "")
set(pkg_config_names_private "")
set(pkg_config_defines "")

set(CMAKE_DEBUG_POSTFIX d)

# library basename
set (SDBFS_OUTPUT_BASENAME "sdbfs"
    CACHE STRING
    "Output sdbfs library base name"
)

# set library dependencies
set(OPT_LIBRARIES_STATIC)

########################################################################
# Library compile options
########################################################################

add_library(sdbfs_compiler_flags INTERFACE)
target_compile_features(sdbfs_compiler_flags
    INTERFACE
    c_std_99
)

# add compiler warning flags just when building this project via
# the BUILD_INTERFACE genex
set(gcc_c "$<$<COMPILE_LANGUAGE:C>:$<C_COMPILER_ID:GNU>>")
set(clang_c "$<$<COMPILE_LANGUAGE:C>:$<C_COMPILER_ID:Clang>>")
target_compile_options(sdbfs_compiler_flags
    INTERFACE
    "$<${gcc_c}:$<BUILD_INTERFACE:-Wall;-ggdb;-O2;-ffunction-sections;-fdata-sections;-Wno-pointer-sign>>"
    "$<${clang_c}:$<BUILD_INTERFACE:-Wall;-ggdb;-O2;-ffunction-sections;-fdata-sections;-Wno-pointer-sign>>"
)

# control where the static and shared libraries are built so that on windows
# we don't need to tinker with the path to run the executable
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

option(BUILD_SHARED "Whether or not to build the shared object"  ON)
option(BUILD_STATIC "Whether or not to build the static archive" ON)

# add targets to target_outputs
list(APPEND target_outputs "")

if(BUILD_SHARED)
    list(APPEND target_outputs "sdbfs")
endif()

if(BUILD_STATIC)
    list(APPEND target_outputs "sdbfs-static")
endif()

########################################################################
# Sources/Headers definitions
########################################################################

# Source files
set(sdbfs_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/src/access.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/glue.c
)

# for IDE project generation
source_group("Source Files" FILES ${sdbfs_sources})

# Header files
set(sdbfs_public_headers
    ${CMAKE_CURRENT_SOURCE_DIR}/include/libsdbfs.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/libsdbfs-freestanding.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/libsdbfs-kernel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/libsdbfs-user.h
)

# Header directory
set(sdbfs_public_dir_headers
    ${CMAKE_CURRENT_SOURCE_DIR}/include/linux/sdb.h
)

# Get directory names only
list(APPEND sdbfs_public_dirs "")

foreach(public_dir_header ${sdbfs_public_dir_headers})
    get_filename_component(public_dir
        ${public_dir_header}
        DIRECTORY
    )

    list(APPEND sdbfs_public_dirs ${public_dir})
endforeach()

# remove, well, duplicates ...
list(REMOVE_DUPLICATES sdbfs_public_dirs)

# for IDE project generation
source_group("Header Files" FILES
    ${sdbfs_public_headers}
    ${sdbfs_public_dir_headers}
)

# README files
set(readme-docs
    COPYING
)

########################################################################
# Library declaration
########################################################################

# avoid building everything twice for shared + static
# only on *nix, as Windows needs different preprocessor
# defines in static builds

# add sources to "library" objects
add_library(objects OBJECT ${sdbfs_sources})

# always build objects with position independent code on
set_target_properties(objects PROPERTIES
    POSITION_INDEPENDENT_CODE
    ON
)

# add the binary tree to the search path for include files
# so that we will find our headers
target_include_directories(objects
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>)

# as we can't link an object library to anything, get
# the properties INTERFACE_COMPILE_FEATURES and
# INTERFACE_COMPILE_DEFINITONS and aplpy to the
# object library
get_target_property(sdbfs_compiler_features
    sdbfs_compiler_flags
    INTERFACE_COMPILE_FEATURES
)

target_compile_features(objects
    PRIVATE
    ${sdbfs_compiler_features}
)

get_target_property(sdbfs_compiler_options
    sdbfs_compiler_flags
    INTERFACE_COMPILE_OPTIONS
)

target_compile_options(objects
    PRIVATE
    ${sdbfs_compiler_options}
)

# build shared library
if(BUILD_SHARED)
    add_library(sdbfs SHARED
        $<TARGET_OBJECTS:objects>
        ${sdbfs_public_headers}
        ${sdbfs_public_dir_headers}
        ${readme-docs}
        ${sdbfs-pkgconfig}
    )

    set_target_properties(sdbfs PROPERTIES
        PUBLIC_HEADER "${sdbfs_public_headers}"
        SOVERSION "1"
        VERSION "${SDBFS_VERSION}"
        COMPILE_DEFINITIONS "DLL_EXPORT"
        OUTPUT_NAME "${SDBFS_OUTPUT_BASENAME}"
        PREFIX "lib")
endif()

# build static library
if(BUILD_STATIC)
    add_library(sdbfs-static STATIC
        $<TARGET_OBJECTS:objects>
        ${sdbfs_public_headers}
        ${sdbfs_public_dir_headers}
        ${readme-docs}
        ${sdbfs-pkgconfig}
    )

    set_target_properties(sdbfs-static PROPERTIES
      PUBLIC_HEADER "${sdbfs_public_headers}"
      OUTPUT_NAME "${SDBFS_OUTPUT_BASENAME}"
      PREFIX "lib")
endif()

# set properties for all targets
foreach(target ${target_outputs})
    target_include_directories(${target}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:include>
    )

    # set debug posfix in case of debug builds
    set_target_properties(${target} PROPERTIES
        DEBUG_POSTFIX
        ${CMAKE_DEBUG_POSTFIX}
    )

    # set compiler flags
    target_link_libraries(${target}
        PUBLIC
        sdbfs_compiler_flags
    )
endforeach()

########################################################################
# Library dependencies
########################################################################

# set depedency to targets
if(BUILD_SHARED)
    target_link_libraries(sdbfs
        PUBLIC
        ${MORE_LIBRARIES}
    )
endif()

if(BUILD_STATIC)
    target_link_libraries(sdbfs-static
        PUBLIC
        ${MORE_LIBRARIES}
    )
endif()

########################################################################
# Binaries compile options
########################################################################

add_library(binaries_compiler_flags INTERFACE)
target_compile_features(binaries_compiler_flags
    INTERFACE
    c_std_99
)

# add compiler warning flags just when building this project via
# the BUILD_INTERFACE genex
set(gcc_c "$<$<COMPILE_LANGUAGE:C>:$<C_COMPILER_ID:GNU>>")
set(clang_c "$<$<COMPILE_LANGUAGE:C>:$<C_COMPILER_ID:Clang>>")
target_compile_options(binaries_compiler_flags
    INTERFACE
    "$<${gcc_c}:$<BUILD_INTERFACE:-Wall;-ggdb;-O2;>>"
    "$<${clang_c}:$<BUILD_INTERFACE:-Wall;-ggdb;-O2;>>"
)

# add binaries to bin_target_outputs
list(APPEND bin_target_outputs "")

########################################################################
# Binaries declaration
########################################################################

option(BUILD_GENSDBFS "Whether or not to build gensdbfs tool" ON)

if(BUILD_GENSDBFS)
    list(APPEND bin_target_outputs "gensdbfs")
endif()

option(BUILD_SDB_EXTRACT "Whether or not to build sdb-extract tool" ON)

if(BUILD_SDB_EXTRACT)
    list(APPEND bin_target_outputs "sdb-extract")
endif()

option(BUILD_SDB_READ "Whether or not to build sdb-read tool" ON)

if(BUILD_SDB_READ)
    list(APPEND bin_target_outputs "sdb-read")
endif()

option(BUILD_SDB_READ_LNLS "Whether or not to build sdb-read-lnls tool" ON)

if(BUILD_SDB_READ_LNLS)
    list(APPEND bin_target_outputs "sdb-read-lnls")
endif()

foreach(bin_target ${bin_target_outputs})
    # add binary tool
    add_executable(${bin_target} tools/${bin_target}.c)

    # add the binary tree to the search path for include files
    # so that we will find our headers
    target_include_directories(${bin_target}
        PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:include>)

    # set debug posfix in case of debug builds
    set_target_properties(${bin_target} PROPERTIES
        DEBUG_POSTFIX
        ${CMAKE_DEBUG_POSTFIX}
    )

    # set compiler flags
    target_link_libraries(${bin_target}
        PRIVATE
        binaries_compiler_flags
    )

    # library dependencies
    target_link_libraries(${bin_target}
        PRIVATE
        sdbfs
    )

    # install binary
    install(TARGETS ${bin_target}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT Tools)
endforeach()

# sdb-read-lnls needs more stuff
if(BUILD_SDB_READ_LNLS)
    target_link_libraries(sdb-read-lnls
        PRIVATE
        llio
        errhand
        convc
        czmq
        pcidriver
    )
endif()

########################################################################
# Library installer
########################################################################

install(TARGETS ${target_outputs} sdbfs_compiler_flags
    EXPORT ${PROJECT_NAME}-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FRAMEWORK DESTINATION "Library/Frameworks"
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# install headers that are inside another directory
install(DIRECTORY
    ${sdbfs_public_dirs}
    DESTINATION
    ${CMAKE_INSTALL_INCLUDEDIR}
)

# CMake search path wants either "share" (AKA GNUInstallDirs DATAROOTDIR)
# for arch-independent, or LIBDIR for arch-dependent, plus "cmake" as prefix
set(SDBFS_CMAKECONFIG_INSTALL_DIR
    "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
    CACHE STRING
    "install path for sdbfsConfig.cmake and sdbfsTargets.cmake"
)

# Convert README files in *.txt ones
foreach(readme ${readme-docs})
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${readme}
        ${CMAKE_CURRENT_BINARY_DIR}/${readme}.txt
    )
endforeach()

# Generate .pc file
set(prefix ${CMAKE_INSTALL_PREFIX})
set(exec_prefix ${prefix})
set(libdir ${prefix}/lib)
set(includedir ${prefix}/include)
set(VERSION
    ${SDBFS_VERSION_MAJOR}.${SDBFS_VERSION_MINOR}.${SDBFS_VERSION_PATCH}
)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/libsdbfs.pc.in
    ${CMAKE_CURRENT_BINARY_DIR}/libsdbfs.pc @ONLY)
set(sdbfs-pkgconfig ${CMAKE_CURRENT_BINARY_DIR}/libsdbfs.pc)

# install .pc file
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/libsdbfs.pc
    DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)

# only available on CMake >= 3.0. Generates  <PROJECT_NAME>Targets.cmake
# file for other cmake projects using this.
export(EXPORT ${PROJECT_NAME}-targets
    FILE
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake"
)

# configure and generate <PROJECT_NAME>Config.cmake file
configure_package_config_file(
    cmake/${PROJECT_NAME}Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION
    ${SDBFS_CMAKECONFIG_INSTALL_DIR}
)

# configure and generate <PROJECT_NAME>ConfigVersion.cmake file
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION
    ${SDBFS_VERSION}
    COMPATIBILITY AnyNewerVersion
)

# actually install <PROJECT_NAME>Targets.cmake file
if(BUILD_SHARED OR BUILD_STATIC)
    install(EXPORT ${PROJECT_NAME}-targets
        FILE ${PROJECT_NAME}Targets.cmake
        DESTINATION ${SDBFS_CMAKECONFIG_INSTALL_DIR}
    )
    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION
        ${SDBFS_CMAKECONFIG_INSTALL_DIR}
    )
endif()

# CPack rules
option(ENABLE_CPACK "Enables cpack rules" ON)

if(ENABLE_CPACK)
    if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
        set(CMAKE_INSTALL_DEBUG_LIBRARIES_ONLY TRUE)
        set(CMAKE_INSTALL_DEBUG_LIBRARIES TRUE)
        set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE)
    endif()

    include(InstallRequiredSystemLibraries)

    # set architecture as detected by TargetArch module
    set(arch_name ${ARCH})

    list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_BINARY_DIR})
    set(CPACK_GENERATOR "DEB")
    set(CPACK_PACKAGE_NAME "sdbfs")
    set(CPACK_PACKAGE_FILE_NAME
        "${CPACK_PACKAGE_NAME}-${SDBFS_VERSION}-${arch_name}"
    )
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "SDB filesystem library")
    set(CPACK_PACKAGE_VENDOR "LNLS")
    set(CPACK_PACKAGE_CONTACT "Lucas Russo <lucas.russo@lnls.br>")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_BINARY_DIR}/COPYING.txt")
    # set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_BINARY_DIR}/README.txt")
    # set(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_CURRENT_BINARY_DIR}/WELCOME.txt")
    # set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/branding.bmp")
    set(CPACK_PACKAGE_VERSION ${SDBFS_VERSION})
    set(CPACK_PACKAGE_VERSION_MAJOR ${SDBFS_VERSION_MAJOR})
    set(CPACK_PACKAGE_VERSION_MINOR ${SDBFS_VERSION_MINOR})
    set(CPACK_PACKAGE_VERSION_PATCH ${SDBFS_VERSION_PATCH})
    # set(CPACK_PACKAGE_INSTALL_DIRECTORY "SDBFS Install Directory")
    # set(CPACK_TEMPORARY_DIRECTORY "SDBFS Temporary CPack Directory")

    include(CPack)

    cpack_add_component_group(Development
        DISPLAY_NAME "SDB Filesystem library"
        EXPANDED
    )
    cpack_add_component(SourceCode
        DISPLAY_NAME "SDB Filesystem source code"
        DISABLED
        INSTALL_TYPES FullInstall
    )
    cpack_add_component(SDK
        DISPLAY_NAME "SDB Filesystem headers and libraries"
        INSTALL_TYPES FullInstall DevInstall
        GROUP Development
    )
    cpack_add_component(Runtime
        DISPLAY_NAME "SDB Filesystem runtime files"
        REQUIRED
        INSTALL_TYPES FullInstall DevInstall MinInstall
    )
    cpack_add_install_type(FullInstall
        DISPLAY_NAME "Full install, including source code"
    )
    cpack_add_install_type(DevInstall
        DISPLAY_NAME "Developer install, headers and libraries"
    )
    cpack_add_install_type(MinInstall
        DISPLAY_NAME "Minimal install, runtime only"
    )
endif()