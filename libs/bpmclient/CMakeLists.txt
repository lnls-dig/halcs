########################################################################
# Project setup
########################################################################

cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(bpmclient)

# set modules path for CMkake to find
set(bpmclient_CMAKE_MODULES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)
list(APPEND CMAKE_MODULE_PATH ${bpmclient_CMAKE_MODULES_DIR})

# add module to prevent in-source builds
include(PreventInSourceBuilds)
# set installation variables compliant with distributions
include(GNUInstallDirs)
# configure and generate *.Config.cmake files
include(CMakePackageConfigHelpers)
# extract version numbers from header file
include(TestBpmClientVersion)
# detect target architecture
include(TargetArch)
# add git introspection functions
include(GetGitRevisionDescription)

target_architecture(ARCH)

# find pkg-config
find_package(PkgConfig)

# Will be used to add flags to pkg-config useful when apps want to statically link
set(pkg_config_libs_private "")
set(pkg_config_names_private "")
set(pkg_config_defines "")

set(CMAKE_DEBUG_POSTFIX d)

# library basename
set (bpmclient_OUTPUT_BASENAME "bpmclient"
    CACHE STRING
    "Output bpmclient library base name"
)

# set library dependencies
set(OPT_LIBRARIES_STATIC)

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
    list(APPEND MORE_LIBRARIES ${libzmq_LIBRARIES})
    if(PC_libzmq_FOUND)
        set(pkg_config_names_private "${pkg_config_names_private} libzmq")
        list(APPEND OPT_LIBRARIES_STATIC ${PC_libzmq_STATIC_LDFLAGS})
    else()
        set(pkg_config_libs_private "${pkg_config_libs_private} -lzmq")
    endif()
else()
    message(FATAL_ERROR
        "libzmq is not installed. Install it, then run CMake again." )
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
    list(APPEND MORE_LIBRARIES ${czmq_LIBRARIES})
    if(PC_czmq_FOUND)
        set(pkg_config_names_private "${pkg_config_names_private} libczmq")
        list(APPEND OPT_LIBRARIES_STATIC ${PC_czmq_STATIC_LDFLAGS})
    else()
        set(pkg_config_libs_private "${pkg_config_libs_private} -lczmq")
    endif()
else()
    message(FATAL_ERROR
        "czmq is not installed. Install it, then run CMake again." )
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
    list(APPEND MORE_LIBRARIES ${errhand_LIBRARIES})
    if(PC_errhand_FOUND)
        set(pkg_config_names_private "${pkg_config_names_private} liberrhand")
        list(APPEND OPT_LIBRARIES_STATIC ${PC_errhand_STATIC_LDFLAGS})
    else()
        set(pkg_config_libs_private "${pkg_config_libs_private} -lerrhand")
    endif()
else()
    message(FATAL_ERROR
        "liberrhand is not installed. Install it, then run CMake again." )
endif()

#########################################################################
# disptable dependency
#########################################################################

find_package(disptable)
if(NOT disptable_FOUND)
    message(STATUS
        "disptable not found with find_package MODULE mode. Trying CONFIG mode." )
    find_package(disptable CONFIG)
endif()
if(disptable_FOUND)
    list(APPEND MORE_LIBRARIES ${disptable_LIBRARIES})
    if(PC_disptable_FOUND)
        set(pkg_config_names_private "${pkg_config_names_private} libdisptable")
        list(APPEND OPT_LIBRARIES_STATIC ${PC_disptable_STATIC_LDFLAGS})
    else()
        set(pkg_config_libs_private "${pkg_config_libs_private} -ldisptable")
    endif()
else()
    message(FATAL_ERROR
        "libdisptable is not installed. Install it, then run CMake again." )
endif()

#########################################################################
# hutils dependency
#########################################################################

find_package(hutils)
if(NOT hutils_FOUND)
    message(STATUS
        "hutils not found with find_package MODULE mode. Trying CONFIG mode." )
    find_package(hutils CONFIG)
endif()
if(hutils_FOUND)
    list(APPEND MORE_LIBRARIES ${hutils_LIBRARIES})
    if(PC_hutils_FOUND)
        set(pkg_config_names_private "${pkg_config_names_private} libhutils")
        list(APPEND OPT_LIBRARIES_STATIC ${PC_hutils_STATIC_LDFLAGS})
    else()
        set(pkg_config_libs_private "${pkg_config_libs_private} -lhutils")
    endif()
else()
    message(FATAL_ERROR
        "libhutils is not installed. Install it, then run CMake again." )
endif()

#########################################################################
# mlm dependency
#########################################################################

find_package(mlm)
if(NOT mlm_FOUND)
    message(STATUS
        "mlm not found with find_package MODULE mode. Trying CONFIG mode." )
    find_package(mlm CONFIG)
endif()
if(mlm_FOUND)
    list(APPEND MORE_LIBRARIES ${mlm_LIBRARIES})
    if(PC_mlm_FOUND)
        set(pkg_config_names_private "${pkg_config_names_private} libmlm")
        list(APPEND OPT_LIBRARIES_STATIC ${PC_mlm_STATIC_LDFLAGS})
    else()
        set(pkg_config_libs_private "${pkg_config_libs_private} -lmlm")
    endif()
else()
    message(FATAL_ERROR
        "libmlm is not installed. Install it, then run CMake again." )
endif()

#########################################################################
# halcsclient dependency
#########################################################################

find_package(halcsclient)
if(NOT halcsclient_FOUND)
    message(STATUS
        "halcsclient not found with find_package MODULE mode. Trying CONFIG mode." )
    find_package(halcsclient CONFIG)
endif()
if(halcsclient_FOUND)
    list(APPEND MORE_LIBRARIES ${halcsclient_LIBRARIES})
    if(PC_halcsclient_FOUND)
        set(pkg_config_names_private "${pkg_config_names_private} libhalcsclient")
        list(APPEND OPT_LIBRARIES_STATIC ${PC_halcsclient_STATIC_LDFLAGS})
    else()
        set(pkg_config_libs_private "${pkg_config_libs_private} -lhalcsclient")
    endif()
else()
    message(FATAL_ERROR
        "libhalcsclient is not installed. Install it, then run CMake again." )
endif()

#########################################################################
# acqclient dependency
#########################################################################

find_package(acqclient)
if(NOT acqclient_FOUND)
    message(STATUS
        "acqclient not found with find_package MODULE mode. Trying CONFIG mode." )
    find_package(acqclient CONFIG)
endif()
if(acqclient_FOUND)
    list(APPEND MORE_LIBRARIES ${acqclient_LIBRARIES})
    if(PC_acqclient_FOUND)
        set(pkg_config_names_private "${pkg_config_names_private} libacqclient")
        list(APPEND OPT_LIBRARIES_STATIC ${PC_acqclient_STATIC_LDFLAGS})
    else()
        set(pkg_config_libs_private "${pkg_config_libs_private} -lacqclient")
    endif()
else()
    message(FATAL_ERROR
        "libacqclient is not installed. Install it, then run CMake again." )
endif()

########################################################################
# Library compile options
########################################################################

add_library(bpmclient_compiler_flags INTERFACE)
target_compile_features(bpmclient_compiler_flags
    INTERFACE
    c_std_99
)

# add compiler warning flags just when building this project via
# the BUILD_INTERFACE genex
set(gcc_c "$<$<COMPILE_LANGUAGE:C>:$<C_COMPILER_ID:GNU>>")
set(clang_c "$<$<COMPILE_LANGUAGE:C>:$<C_COMPILER_ID:Clang>>")
set(msvc_c "$<$<COMPILE_LANGUAGE:C>:$<C_COMPILER_ID:MSVC>>")
target_compile_options(bpmclient_compiler_flags
    INTERFACE
    "$<${gcc_c}:$<BUILD_INTERFACE:-Wall;-Wextra;-Werror;-O2>>"
    "$<${clang_c}:$<BUILD_INTERFACE:-Wall;-Wextra;-Werror;-O2>>"
    "$<${msvc_c}:$<BUILD_INTERFACE:-W3;-O2>>"
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
    list(APPEND target_outputs "bpmclient")
endif()

if(BUILD_STATIC)
    list(APPEND target_outputs "bpmclient-static")
endif()

########################################################################
# Sources/Headers definitions
########################################################################

# Source files
set(bpmclient_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/src/bpm_client_core.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/bpm_client_revision.c
)

# for IDE project generation
source_group("Source Files" FILES ${bpmclient_sources})

# Header files
set(bpmclient_public_headers
    ${CMAKE_CURRENT_SOURCE_DIR}/include/bpm_client_classes.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/bpm_client_core.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/bpm_client.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/bpm_client_prelude.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/bpm_client_revision.h
)

# for IDE project generation
source_group("Header Files" FILES ${bpmclient_public_headers})

# README files
set(readme-docs
    COPYING
)

########################################################################
# Library declaration
########################################################################

# build shared library
if(BUILD_SHARED)
    add_library(bpmclient SHARED
        ${bpmclient_sources}
        ${bpmclient_public_headers}
        ${readme-docs}
        ${bpmclient-pkgconfig}
    )

    set_target_properties(bpmclient PROPERTIES
        PUBLIC_HEADER "${bpmclient_public_headers}"
        SOVERSION "1"
        VERSION "${bpmclient_VERSION}"
        COMPILE_DEFINITIONS "DLL_EXPORT"
        OUTPUT_NAME "${bpmclient_OUTPUT_BASENAME}"
        PREFIX "lib")
endif()

# build static library
if(BUILD_STATIC)
    add_library(bpmclient-static STATIC
        ${bpmclient_sources}
        ${bpmclient_public_headers}
        ${readme-docs}
        ${bpmclient-pkgconfig}
    )

    set_target_properties(bpmclient-static PROPERTIES
      PUBLIC_HEADER "${bpmclient_public_headers}"
      OUTPUT_NAME "${bpmclient_OUTPUT_BASENAME}"
      PREFIX "lib")
endif()

# set Git macros
git_describe(GIT_TAG --always --tags)
git_get_config(GIT_NAME "user.name")
git_get_config(GIT_EMAIL "user.email")

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

    # add board compile definitions
    target_compile_definitions(${target} PRIVATE
        GIT_REVISION=\"${GIT_TAG}\"
        GIT_USER_NAME=\"${GIT_NAME}\"
        GIT_USER_EMAIL=\"${GIT_EMAIL}\"
    )

    # set PRIVATE compiler flags. It doesn't really work to link an
    # INTERFACE library with PRIVATE scope. See:
    # https://gitlab.kitware.com/cmake/cmake/-/issues/15415
    target_link_libraries(${target} PRIVATE
        $<TARGET_PROPERTY:bpmclient_compiler_flags,INTERFACE_LINK_LIBRARIES>
    )
    target_include_directories(${target} PRIVATE
        $<TARGET_PROPERTY:bpmclient_compiler_flags,INTERFACE_INCLUDE_DIRECTORIES>
    )
    target_compile_options(${target} PRIVATE
        $<TARGET_PROPERTY:bpmclient_compiler_flags,INTERFACE_COMPILE_OPTIONS>
    )
    target_compile_definitions(${target} PRIVATE
        $<TARGET_PROPERTY:bpmclient_compiler_flags,INTERFACE_COMPILE_DEFINITIONS>
    )
    target_compile_features(${target} PRIVATE
        $<TARGET_PROPERTY:bpmclient_compiler_flags,INTERFACE_COMPILE_FEATURES>
    )
    target_sources(${target} PRIVATE
        $<TARGET_PROPERTY:bpmclient_compiler_flags,INTERFACE_SOURCES>
    )
endforeach()

########################################################################
# Library dependencies
########################################################################

# set depedency to targets
if(BUILD_SHARED)
    target_link_libraries(bpmclient
        PUBLIC
        ${MORE_LIBRARIES}
    )
endif()

if(BUILD_STATIC)
    target_link_libraries(bpmclient-static
        PUBLIC
        ${MORE_LIBRARIES}
    )
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
# Installer
########################################################################

install(TARGETS ${target_outputs} bpmclient_compiler_flags
    EXPORT ${PROJECT_NAME}-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FRAMEWORK DESTINATION "Library/Frameworks"
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# CMake search path wants either "share" (AKA GNUInstallDirs DATAROOTDIR)
# for arch-independent, or LIBDIR for arch-dependent, plus "cmake" as prefix
set(bpmclient_CMAKECONFIG_INSTALL_DIR
    "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
    CACHE STRING
    "install path for bpmclientConfig.cmake and bpmclientTargets.cmake"
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
    ${bpmclient_VERSION_MAJOR}.${bpmclient_VERSION_MINOR}.${bpmclient_VERSION_PATCH}
)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/libbpmclient.pc.in
    ${CMAKE_CURRENT_BINARY_DIR}/libbpmclient.pc @ONLY)
set(bpmclient-pkgconfig ${CMAKE_CURRENT_BINARY_DIR}/libbpmclient.pc)

# install .pc file
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/libbpmclient.pc
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
    ${bpmclient_CMAKECONFIG_INSTALL_DIR}
)

# configure and generate <PROJECT_NAME>ConfigVersion.cmake file
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION
    ${bpmclient_VERSION}
    COMPATIBILITY AnyNewerVersion
)

# actually install <PROJECT_NAME>Targets.cmake file
if(BUILD_SHARED OR BUILD_STATIC)
    install(EXPORT ${PROJECT_NAME}-targets
        FILE ${PROJECT_NAME}Targets.cmake
        DESTINATION ${bpmclient_CMAKECONFIG_INSTALL_DIR}
    )
    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION
        ${bpmclient_CMAKECONFIG_INSTALL_DIR}
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
    set(CPACK_RPM_PACKAGE_AUTOREQ no)
    set(CPACK_RPM_PACKAGE_AUTOPROV yes)
    set(CPACK_PACKAGE_NAME "bpmclient")
    set(CPACK_PACKAGE_FILE_NAME
        "${CPACK_PACKAGE_NAME}-${bpmclient_VERSION}-${arch_name}"
    )
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "HALCS BPM library")
    set(CPACK_PACKAGE_VENDOR "LNLS")
    set(CPACK_PACKAGE_CONTACT "Lucas Russo <lucas.russo@lnls.br>")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_BINARY_DIR}/COPYING.txt")
    # set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_BINARY_DIR}/README.txt")
    # set(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_CURRENT_BINARY_DIR}/WELCOME.txt")
    # set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/branding.bmp")
    set(CPACK_PACKAGE_VERSION ${bpmclient_VERSION})
    set(CPACK_PACKAGE_VERSION_MAJOR ${bpmclient_VERSION_MAJOR})
    set(CPACK_PACKAGE_VERSION_MINOR ${bpmclient_VERSION_MINOR})
    set(CPACK_PACKAGE_VERSION_PATCH ${bpmclient_VERSION_PATCH})
    # set(CPACK_PACKAGE_INSTALL_DIRECTORY "bpmclient Install Directory")
    # set(CPACK_TEMPORARY_DIRECTORY "bpmclient Temporary CPack Directory")

    include(CPack)

    cpack_add_component_group(Development
        DISPLAY_NAME "BPM client library"
        EXPANDED
    )
    cpack_add_component(SourceCode
        DISPLAY_NAME "BPM client source code"
        DISABLED
        INSTALL_TYPES FullInstall
    )
    cpack_add_component(SDK
        DISPLAY_NAME "BPM client headers and libraries"
        INSTALL_TYPES FullInstall DevInstall
        GROUP Development
    )
    cpack_add_component(Runtime
        DISPLAY_NAME "BPM client runtime files"
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
