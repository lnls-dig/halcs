########################################################################
# Project setup
########################################################################

cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(sdbfs)

# set modules path for CMkake to find
set(sdbfs_CMAKE_MODULES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)
list(APPEND CMAKE_MODULE_PATH ${sdbfs_CMAKE_MODULES_DIR})

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
set (sdbfs_OUTPUT_BASENAME "sdbfs"
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

# build shared library
if(BUILD_SHARED)
    add_library(sdbfs SHARED
        ${sdbfs_sources}
        ${sdbfs_public_headers}
        ${sdbfs_public_dir_headers}
        ${readme-docs}
        ${sdbfs-pkgconfig}
    )

    set_target_properties(sdbfs PROPERTIES
        PUBLIC_HEADER "${sdbfs_public_headers}"
        SOVERSION "1"
        VERSION "${sdbfs_VERSION}"
        COMPILE_DEFINITIONS "DLL_EXPORT"
        OUTPUT_NAME "${sdbfs_OUTPUT_BASENAME}"
        PREFIX "lib")
endif()

# build static library
if(BUILD_STATIC)
    add_library(sdbfs-static STATIC
        ${sdbfs_sources}
        ${sdbfs_public_headers}
        ${sdbfs_public_dir_headers}
        ${readme-docs}
        ${sdbfs-pkgconfig}
    )

    set_target_properties(sdbfs-static PROPERTIES
      PUBLIC_HEADER "${sdbfs_public_headers}"
      OUTPUT_NAME "${sdbfs_OUTPUT_BASENAME}"
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
        POSITION_INDEPENDENT_CODE
        ON
    )

    # set PRIVATE compiler flags. It doesn't really work to link an
    # INTERFACE library with PRIVATE scope. See:
    # https://gitlab.kitware.com/cmake/cmake/-/issues/15415
    target_link_libraries(${target} PRIVATE
        $<TARGET_PROPERTY:sdbfs_compiler_flags,INTERFACE_LINK_LIBRARIES>
    )
    target_include_directories(${target} PRIVATE
        $<TARGET_PROPERTY:sdbfs_compiler_flags,INTERFACE_INCLUDE_DIRECTORIES>
    )
    target_compile_options(${target} PRIVATE
        $<TARGET_PROPERTY:sdbfs_compiler_flags,INTERFACE_COMPILE_OPTIONS>
    )
    target_compile_definitions(${target} PRIVATE
        $<TARGET_PROPERTY:sdbfs_compiler_flags,INTERFACE_COMPILE_DEFINITIONS>
    )
    target_sources(${target} PRIVATE
        $<TARGET_PROPERTY:sdbfs_compiler_flags,INTERFACE_SOURCES>
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

option(BUILD_GENsdbfs "Whether or not to build gensdbfs tool" ON)

if(BUILD_GENsdbfs)
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

    # lots of dependencies for sdb-read-lnls
    ########################################################################
    # libzmq dependency
    ########################################################################

    find_package(libzmq)
    if(NOT libzmq_FOUND)
        message(STATUS
            "libzmq not found with find_package MODULE mode. Trying CONFIG mode." )
        find_package(libzmq CONFIG)
    endif()
    if(libzmq_FOUND)
        target_include_directories(sdb-read-lnls
            PRIVATE
            ${libzmq_INCLUDE_DIRS}
        )
        list(APPEND MORE_LIBRARIES ${libzmq_LIBRARIES})
        if(PC_libzmq_FOUND)
            set(pkg_config_names_private "${pkg_config_names_private} libzmq")
            list(APPEND OPT_LIBRARIES_STATIC ${PC_libzmq_STATIC_LDFLAGS})
        else()
            set(pkg_config_libs_private "${pkg_config_libs_private} -lzmq")
        endif()
    else()
        message(FATAL_ERROR
            "libzmq is not installed for sdb-read-lnls. Install it, then run CMake again." )
    endif()

    #########################################################################
    # czmq dependency
    #########################################################################

    find_package(czmq)
    if(NOT czmq_FOUND)
        message(STATUS
            "czmq not found with find_package MODULE mode. Trying CONFIG mode." )
        find_package(czmq CONFIG)
    endif()
    if(czmq_FOUND)
        target_include_directories(sdb-read-lnls
            PRIVATE
            ${czmq_INCLUDE_DIRS}
        )
        list(APPEND MORE_LIBRARIES ${czmq_LIBRARIES})
        if(PC_czmq_FOUND)
            set(pkg_config_names_private "${pkg_config_names_private} libczmq")
            list(APPEND OPT_LIBRARIES_STATIC ${PC_czmq_STATIC_LDFLAGS})
        else()
            set(pkg_config_libs_private "${pkg_config_libs_private} -lczmq")
        endif()
    else()
        message(FATAL_ERROR
            "czmq is not installed for sdb-read-lnls. Install it, then run CMake again." )
    endif()

    #########################################################################
    # errhand dependency
    #########################################################################

    find_package(errhand)
    if(NOT errhand_FOUND)
        message(STATUS
            "errhand not found with find_package MODULE mode. Trying CONFIG mode." )
        find_package(errhand CONFIG)
    endif()
    if(errhand_FOUND)
        target_include_directories(sdb-read-lnls
            PRIVATE
            ${errhand_INCLUDE_DIRS}
        )
        list(APPEND MORE_LIBRARIES ${errhand_LIBRARIES})
        if(PC_errhand_FOUND)
            set(pkg_config_names_private "${pkg_config_names_private} liberrhand")
            list(APPEND OPT_LIBRARIES_STATIC ${PC_errhand_STATIC_LDFLAGS})
        else()
            set(pkg_config_libs_private "${pkg_config_libs_private} -lerrhand")
        endif()
    else()
        message(FATAL_ERROR
            "liberrhand is not installed for sdb-read-lnls. Install it, then run CMake again." )
    endif()

    #########################################################################
    # convc dependency
    #########################################################################

    find_package(convc)
    if(NOT convc_FOUND)
        message(STATUS
            "convc not found with find_package MODULE mode. Trying CONFIG mode." )
        find_package(convc CONFIG)
    endif()
    if(convc_FOUND)
        target_include_directories(sdb-read-lnls
            PRIVATE
            ${convc_INCLUDE_DIRS}
        )
        list(APPEND MORE_LIBRARIES ${convc_LIBRARIES})
        if(PC_convc_FOUND)
            set(pkg_config_names_private "${pkg_config_names_private} libconvc")
            list(APPEND OPT_LIBRARIES_STATIC ${PC_convc_STATIC_LDFLAGS})
        else()
            set(pkg_config_libs_private "${pkg_config_libs_private} -lconvc")
        endif()
    else()
        message(FATAL_ERROR
            "libconvc is not installed for sdb-read-lnls. Install it, then run CMake again." )
    endif()

    # Is this needed or is it better to leave this unspecified in case
    # libs depending on sdbfs that do not use sdb-read-lnls can be left without
    # requiring this PCIE driver lib?
    ##########################################################################
    ## PCIE Driver lib dependency
    ##########################################################################

    #find_package(pciedriver)
    #if(NOT pciedriver_FOUND)
    #    message(STATUS
    #        "pciedriver not found with find_package MODULE mode. Trying CONFIG mode." )
    #    find_package(pciedriver CONFIG)
    #endif()
    #if(PCIEDRIVER_FOUND)
    #    target_include_directories(sdb-read-lnls
    #        PRIVATE
    #        ${PCIEDRIVER_INCLUDE_DIRS}
    #    )
    #    list(APPEND MORE_LIBRARIES ${PCIEDRIVER_LIBRARIES})
    #    if(PC_PCIEDRIVER_FOUND)
    #        set(pkg_config_names_private "${pkg_config_names_private} libpciedriver")
    #        list(APPEND OPT_LIBRARIES_STATIC ${PC_PCIEDRIVER_STATIC_LDFLAGS})
    #    else()
    #        set(pkg_config_libs_private "${pkg_config_libs_private} -lpciedriver")
    #    endif()
    #else()
    #    message(FATAL_ERROR
    #        "libpciedriver is not installed for sdb-read-lnls. Install it, then run CMake again." )
    #endif()

    #########################################################################
    # llio dependency
    #########################################################################

    find_package(llio)
    if(NOT llio_FOUND)
        message(STATUS
            "llio not found with find_package MODULE mode. Trying CONFIG mode." )
        find_package(llio CONFIG)
    endif()
    if(llio_FOUND)
        target_include_directories(sdb-read-lnls
            PRIVATE
            ${llio_INCLUDE_DIRS}
        )
        list(APPEND MORE_LIBRARIES ${llio_LIBRARIES})
        if(PC_llio_FOUND)
            set(pkg_config_names_private "${pkg_config_names_private} libllio")
            list(APPEND OPT_LIBRARIES_STATIC ${PC_llio_STATIC_LDFLAGS})
        else()
            set(pkg_config_libs_private "${pkg_config_libs_private} -lllio")
        endif()
    else()
        message(FATAL_ERROR
            "libllio is not installed for sdb-read-lnls. Install it, then run CMake again." )
    endif()

    # dynamic library
    if(TARGET sdbfs)
        target_link_libraries(sdb-read-lnls
            PRIVATE
            ${MORE_LIBRARIES}
        )
    endif()

    # only building static lib
    if(NOT TARGET sdbfs AND TARGET sdbfs-static)
        target_link_libraries(sdb-read-lnls
            PRIVATE
            ${MORE_LIBRARIES}
            ${OPT_LIBRARIES_STATIC}
        )
    endif()
endif()

########################################################################
# Custom targets
########################################################################

# uninstall target
if(NOT TARGET uninstall)
  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

  add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
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
set(sdbfs_CMAKECONFIG_INSTALL_DIR
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
set(prefix \${pcfiledir}/../..)
set(exec_prefix \${prefix})
set(libdir \${prefix}/lib)
set(includedir \${prefix}/include)
set(VERSION
    ${sdbfs_VERSION_MAJOR}.${sdbfs_VERSION_MINOR}.${sdbfs_VERSION_PATCH}
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
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION
    ${sdbfs_CMAKECONFIG_INSTALL_DIR}
)

# configure and generate <PROJECT_NAME>ConfigVersion.cmake file
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION
    ${sdbfs_VERSION}
    COMPATIBILITY AnyNewerVersion
)

# actually install <PROJECT_NAME>Targets.cmake file
if(BUILD_SHARED OR BUILD_STATIC)
    install(EXPORT ${PROJECT_NAME}-targets
        FILE ${PROJECT_NAME}Targets.cmake
        DESTINATION ${sdbfs_CMAKECONFIG_INSTALL_DIR}
    )
    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION
        ${sdbfs_CMAKECONFIG_INSTALL_DIR}
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
    set(CPACK_DEBIAN_PACKAGE_GENERATE_SHLIBS_POLICY ">=")
    set(CPACK_DEBIAN_PACKAGE_GENERATE_SHLIBS ON)
    set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
    set(CPACK_PACKAGE_NAME "sdbfs")
    set(CPACK_PACKAGE_FILE_NAME
        "${CPACK_PACKAGE_NAME}-${sdbfs_VERSION}-${arch_name}"
    )
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "SDB filesystem library")
    set(CPACK_PACKAGE_VENDOR "LNLS")
    set(CPACK_PACKAGE_CONTACT "Lucas Russo <lucas.russo@lnls.br>")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_BINARY_DIR}/COPYING.txt")
    # set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_BINARY_DIR}/README.txt")
    # set(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_CURRENT_BINARY_DIR}/WELCOME.txt")
    # set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/branding.bmp")
    set(CPACK_PACKAGE_VERSION ${sdbfs_VERSION})
    set(CPACK_PACKAGE_VERSION_MAJOR ${sdbfs_VERSION_MAJOR})
    set(CPACK_PACKAGE_VERSION_MINOR ${sdbfs_VERSION_MINOR})
    set(CPACK_PACKAGE_VERSION_PATCH ${sdbfs_VERSION_PATCH})
    # set(CPACK_PACKAGE_INSTALL_DIRECTORY "sdbfs Install Directory")
    # set(CPACK_TEMPORARY_DIRECTORY "sdbfs Temporary CPack Directory")

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
