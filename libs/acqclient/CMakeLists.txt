########################################################################
# Project setup
########################################################################

cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(acqclient)

# set modules path for CMkake to find
set(ACQCLIENT_CMAKE_MODULES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)
list(APPEND CMAKE_MODULE_PATH ${ACQCLIENT_CMAKE_MODULES_DIR})

# add module to prevent in-source builds
include(PreventInSourceBuilds)
# set installation variables compliant with distributions
include(GNUInstallDirs)
# configure and generate *.Config.cmake files
include(CMakePackageConfigHelpers)
# extract version numbers from header file
include(TestAcqClientVersion)
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
set (ACQCLIENT_OUTPUT_BASENAME "acqclient"
    CACHE STRING
    "Output acqclient library base name"
)

# set library dependencies
set(OPT_LIBRARIES_STATIC)

########################################################################
# LIBZMQ dependency
########################################################################

find_package(libzmq)
if(LIBZMQ_FOUND)
    include_directories(${LIBZMQ_INCLUDE_DIRS})
    list(APPEND MORE_LIBRARIES ${LIBZMQ_LIBRARIES})
    if(PC_LIBZMQ_FOUND)
        set(pkg_config_names_private "${pkg_config_names_private} libzmq")
        list(APPEND OPT_LIBRARIES_STATIC ${PC_LIBZMQ_STATIC_LDFLAGS})
    else()
        set(pkg_config_libs_private "${pkg_config_libs_private} -lzmq")
    endif()
else()
    message(FATAL_ERROR
        "libzmq is not installed. Install it, then run CMake again." )
endif()

#########################################################################
# CZMQ dependency
#########################################################################

find_package(czmq)
if(CZMQ_FOUND)
    include_directories(${CZMQ_INCLUDE_DIRS})
    list(APPEND MORE_LIBRARIES ${CZMQ_LIBRARIES})
    if(PC_CZMQ_FOUND)
        set(pkg_config_names_private "${pkg_config_names_private} libczmq")
        list(APPEND OPT_LIBRARIES_STATIC ${PC_CZMQ_STATIC_LDFLAGS})
    else()
        set(pkg_config_libs_private "${pkg_config_libs_private} -lczmq")
    endif()
else()
    message(FATAL_ERROR
        "czmq is not installed. Install it, then run CMake again." )
endif()

#########################################################################
# ERRHAND dependency
#########################################################################

find_package(errhand)
if(ERRHAND_FOUND)
    include_directories(${ERRHAND_INCLUDE_DIRS})
    list(APPEND MORE_LIBRARIES ${ERRHAND_LIBRARIES})
    if(PC_ERRHAND_FOUND)
        set(pkg_config_names_private "${pkg_config_names_private} liberrhand")
        list(APPEND OPT_LIBRARIES_STATIC ${PC_ERRHAND_STATIC_LDFLAGS})
    else()
        set(pkg_config_libs_private "${pkg_config_libs_private} -lerrhand")
    endif()
else()
    message(FATAL_ERROR
        "liberrhand is not installed. Install it, then run CMake again." )
endif()

#########################################################################
# DISPTABLE dependency
#########################################################################

find_package(disptable)
if(DISPTABLE_FOUND)
    include_directories(${DISPTABLE_INCLUDE_DIRS})
    list(APPEND MORE_LIBRARIES ${DISPTABLE_LIBRARIES})
    if(PC_DISPTABLE_FOUND)
        set(pkg_config_names_private "${pkg_config_names_private} libdisptable")
        list(APPEND OPT_LIBRARIES_STATIC ${PC_DISPTABLE_STATIC_LDFLAGS})
    else()
        set(pkg_config_libs_private "${pkg_config_libs_private} -ldisptable")
    endif()
else()
    message(FATAL_ERROR
        "libdisptable is not installed. Install it, then run CMake again." )
endif()

#########################################################################
# HUTILS dependency
#########################################################################

find_package(hutils)
if(HUTILS_FOUND)
    include_directories(${HUTILS_INCLUDE_DIRS})
    list(APPEND MORE_LIBRARIES ${HUTILS_LIBRARIES})
    if(PC_HUTILS_FOUND)
        set(pkg_config_names_private "${pkg_config_names_private} libhutils")
        list(APPEND OPT_LIBRARIES_STATIC ${PC_HUTILS_STATIC_LDFLAGS})
    else()
        set(pkg_config_libs_private "${pkg_config_libs_private} -lhutils")
    endif()
else()
    message(FATAL_ERROR
        "libhutils is not installed. Install it, then run CMake again." )
endif()

#########################################################################
# MLM dependency
#########################################################################

find_package(mlm)
if(MLM_FOUND)
    include_directories(${MLM_INCLUDE_DIRS})
    list(APPEND MORE_LIBRARIES ${MLM_LIBRARIES})
    if(PC_MLM_FOUND)
        set(pkg_config_names_private "${pkg_config_names_private} libmlm")
        list(APPEND OPT_LIBRARIES_STATIC ${PC_MLM_STATIC_LDFLAGS})
    else()
        set(pkg_config_libs_private "${pkg_config_libs_private} -lmlm")
    endif()
else()
    message(FATAL_ERROR
        "libmlm is not installed. Install it, then run CMake again." )
endif()

#########################################################################
# HALCSCLIENT dependency
#########################################################################

find_package(halcsclient)
if(HALCSCLIENT_FOUND)
    include_directories(${HALCSCLIENT_INCLUDE_DIRS})
    list(APPEND MORE_LIBRARIES ${HALCSCLIENT_LIBRARIES})
    if(PC_HALCSCLIENT_FOUND)
        set(pkg_config_names_private "${pkg_config_names_private} libhalcsclient")
        list(APPEND OPT_LIBRARIES_STATIC ${PC_HALCSCLIENT_STATIC_LDFLAGS})
    else()
        set(pkg_config_libs_private "${pkg_config_libs_private} -lhalcsclient")
    endif()
else()
    message(FATAL_ERROR
        "libhalcsclient is not installed. Install it, then run CMake again." )
endif()

########################################################################
# Library compile options
########################################################################

add_library(acqclient_compiler_flags INTERFACE)
target_compile_features(acqclient_compiler_flags
    INTERFACE
    c_std_99
)

# add compiler warning flags just when building this project via
# the BUILD_INTERFACE genex
set(gcc_c "$<$<COMPILE_LANGUAGE:C>:$<C_COMPILER_ID:GNU>>")
set(clang_c "$<$<COMPILE_LANGUAGE:C>:$<C_COMPILER_ID:Clang>>")
set(msvc_c "$<$<COMPILE_LANGUAGE:C>:$<C_COMPILER_ID:MSVC>>")
target_compile_options(acqclient_compiler_flags
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
    list(APPEND target_outputs "acqclient")
endif()

if(BUILD_STATIC)
    list(APPEND target_outputs "acqclient-static")
endif()

########################################################################
# Sources/Headers definitions
########################################################################

# Source files
set(acqclient_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/src/acq_client_core.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/acq_client_revision.c
)

# for IDE project generation
source_group("Source Files" FILES ${acqclient_sources})

# Header files
set(acqclient_public_headers
    ${CMAKE_CURRENT_SOURCE_DIR}/include/acq_client_classes.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/acq_client_core.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/acq_client.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/acq_client_prelude.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/acq_client_revision.h
)

# for IDE project generation
source_group("Header Files" FILES ${acqclient_public_headers})

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
add_library(objects OBJECT ${acqclient_sources})

# always build objects with position independent code on
set_target_properties(objects PROPERTIES
    POSITION_INDEPENDENT_CODE
    ON
)

# set Git macros
git_describe(GIT_TAG --always --tags)
git_get_config(GIT_NAME "user.name")
git_get_config(GIT_EMAIL "user.email")

# add board compile definitions
target_compile_definitions(objects PRIVATE
    GIT_REVISION=\"${GIT_TAG}\"
    GIT_USER_NAME=\"${GIT_NAME}\"
    GIT_USER_EMAIL=\"${GIT_EMAIL}\"
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
get_target_property(acqclient_compiler_features
    acqclient_compiler_flags
    INTERFACE_COMPILE_FEATURES
)

target_compile_features(objects
    PRIVATE
    ${acqclient_compiler_features}
)

get_target_property(acqclient_compiler_options
    acqclient_compiler_flags
    INTERFACE_COMPILE_OPTIONS
)

target_compile_options(objects
    PRIVATE
    ${acqclient_compiler_options}
)

# build shared library
if(BUILD_SHARED)
    add_library(acqclient SHARED
        $<TARGET_OBJECTS:objects>
        ${acqclient_public_headers}
        ${readme-docs}
        ${acqclient-pkgconfig}
    )

    set_target_properties(acqclient PROPERTIES
        PUBLIC_HEADER "${acqclient_public_headers}"
        SOVERSION "1"
        VERSION "${ACQCLIENT_VERSION}"
        COMPILE_DEFINITIONS "DLL_EXPORT"
        OUTPUT_NAME "${ACQCLIENT_OUTPUT_BASENAME}"
        PREFIX "lib")
endif()

# build static library
if(BUILD_STATIC)
    add_library(acqclient-static STATIC
        $<TARGET_OBJECTS:objects>
        ${acqclient_public_headers}
        ${readme-docs}
        ${acqclient-pkgconfig}
    )

    set_target_properties(acqclient-static PROPERTIES
      PUBLIC_HEADER "${acqclient_public_headers}"
      OUTPUT_NAME "${ACQCLIENT_OUTPUT_BASENAME}"
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
        acqclient_compiler_flags
    )
endforeach()

########################################################################
# Library dependencies
########################################################################

# set depedency to targets
if(BUILD_SHARED)
    target_link_libraries(acqclient
        PUBLIC
        ${MORE_LIBRARIES}
    )
endif()

if(BUILD_STATIC)
    target_link_libraries(acqclient-static
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

install(TARGETS ${target_outputs} acqclient_compiler_flags
    EXPORT ${PROJECT_NAME}-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FRAMEWORK DESTINATION "Library/Frameworks"
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# CMake search path wants either "share" (AKA GNUInstallDirs DATAROOTDIR)
# for arch-independent, or LIBDIR for arch-dependent, plus "cmake" as prefix
set(ACQCLIENT_CMAKECONFIG_INSTALL_DIR
    "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
    CACHE STRING
    "install path for acqclientConfig.cmake and acqclientTargets.cmake"
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
    ${ACQCLIENT_VERSION_MAJOR}.${ACQCLIENT_VERSION_MINOR}.${ACQCLIENT_VERSION_PATCH}
)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/libacqclient.pc.in
    ${CMAKE_CURRENT_BINARY_DIR}/libacqclient.pc @ONLY)
set(acqclient-pkgconfig ${CMAKE_CURRENT_BINARY_DIR}/libacqclient.pc)

# install .pc file
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/libacqclient.pc
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
    ${ACQCLIENT_CMAKECONFIG_INSTALL_DIR}
)

# configure and generate <PROJECT_NAME>ConfigVersion.cmake file
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION
    ${ACQCLIENT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

# actually install <PROJECT_NAME>Targets.cmake file
if(BUILD_SHARED OR BUILD_STATIC)
    install(EXPORT ${PROJECT_NAME}-targets
        FILE ${PROJECT_NAME}Targets.cmake
        DESTINATION ${ACQCLIENT_CMAKECONFIG_INSTALL_DIR}
    )
    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION
        ${ACQCLIENT_CMAKECONFIG_INSTALL_DIR}
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
    set(CPACK_PACKAGE_NAME "acqclient")
    set(CPACK_PACKAGE_FILE_NAME
        "${CPACK_PACKAGE_NAME}-${ACQCLIENT_VERSION}-${arch_name}"
    )
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "HALCS acquisition library")
    set(CPACK_PACKAGE_VENDOR "LNLS")
    set(CPACK_PACKAGE_CONTACT "Lucas Russo <lucas.russo@lnls.br>")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_BINARY_DIR}/COPYING.txt")
    # set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_BINARY_DIR}/README.txt")
    # set(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_CURRENT_BINARY_DIR}/WELCOME.txt")
    # set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/branding.bmp")
    set(CPACK_PACKAGE_VERSION ${ACQCLIENT_VERSION})
    set(CPACK_PACKAGE_VERSION_MAJOR ${ACQCLIENT_VERSION_MAJOR})
    set(CPACK_PACKAGE_VERSION_MINOR ${ACQCLIENT_VERSION_MINOR})
    set(CPACK_PACKAGE_VERSION_PATCH ${ACQCLIENT_VERSION_PATCH})
    # set(CPACK_PACKAGE_INSTALL_DIRECTORY "ACQCLIENT Install Directory")
    # set(CPACK_TEMPORARY_DIRECTORY "ACQCLIENT Temporary CPack Directory")

    include(CPack)

    cpack_add_component_group(Development
        DISPLAY_NAME "Acquisition client library"
        EXPANDED
    )
    cpack_add_component(SourceCode
        DISPLAY_NAME "Acquisition client source code"
        DISABLED
        INSTALL_TYPES FullInstall
    )
    cpack_add_component(SDK
        DISPLAY_NAME "Acquisition client headers and libraries"
        INSTALL_TYPES FullInstall DevInstall
        GROUP Development
    )
    cpack_add_component(Runtime
        DISPLAY_NAME "Acquisition client runtime files"
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
