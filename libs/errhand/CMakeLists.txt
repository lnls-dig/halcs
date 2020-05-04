########################################################################
# Project setup
########################################################################

cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(errhand)

# set modules path for CMkake to find
set(errhand_CMAKE_MODULES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)
list(APPEND CMAKE_MODULE_PATH ${errhand_CMAKE_MODULES_DIR})

# add module to prevent in-source builds
include(PreventInSourceBuilds)
# set installation variables compliant with distributions
include(GNUInstallDirs)
# configure and generate *.Config.cmake files
include(CMakePackageConfigHelpers)
# extract version numbers from header file
include(TestErrhandVersion)
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
set (errhand_OUTPUT_BASENAME "errhand"
    CACHE STRING
    "Output errhand library base name"
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

########################################################################
# User compile-time options
########################################################################

# should we use our errhand debug
option(errhand_DBG_OPT "Enable debug mode" ON)
if(errhand_DBG_OPT)
    set(errhand_DBG_OPT_VALUE y)
else()
    set(errhand_DBG_OPT_VALUE n)
endif()

message(STATUS
    "Setting compile-time option errhand_DBG='${errhand_DBG_OPT_VALUE}'")

# which debug level do we want
set(errhand_MIN_LEVEL_OPT "DBG_LVL_INFO" CACHE STRING
    "Minimum debug level")
set(errhand_MIN_LEVEL_VALUES
    "DBG_LVL_TRACE;DBG_LVL_INFO;DBG_LVL_WARN;DBG_LVL_ERR;DBG_LVL_FATAL"
)

set_property(CACHE errhand_MIN_LEVEL_OPT PROPERTY STRINGS
    ${errhand_MIN_LEVEL_VALUES}
)

message(STATUS
    "Setting compile-time option errhand_MIN_LEVEL='${errhand_MIN_LEVEL_OPT}'")

# which subsystem do we want on.
# Right now, the only option is all or none of them
set(errhand_SUBSYS_ON_OPT
    "(DBG_DEV_MNGR|DBG_DEV_IO|DBG_SM_IO|DBG_LIB_CLIENT|DBG_SM_PR|DBG_SM_CH|DBG_LL_IO|DBG_HAL_UTILS)"
    CACHE STRING
    "Set Subsystem On")
set(errhand_SUBSYS_ON_VALUES
    "(DBG_DEV_MNGR|DBG_DEV_IO|DBG_SM_IO|DBG_LIB_CLIENT|DBG_SM_PR|DBG_SM_CH|DBG_LL_IO|DBG_HAL_UTILS);"
)

set_property(CACHE errhand_SUBSYS_ON_OPT PROPERTY STRINGS
    ${errhand_SUBSYS_ON_VALUES}
)

message(STATUS
    "Setting compile-time option errhand_SUBSYS_ON='${errhand_SUBSYS_ON_OPT}'")

########################################################################
# Library compile options
########################################################################

add_library(errhand_compiler_flags INTERFACE)
target_compile_features(errhand_compiler_flags
    INTERFACE
    c_std_99
)

# add compiler warning flags just when building this project via
# the BUILD_INTERFACE genex
set(gcc_c "$<$<COMPILE_LANGUAGE:C>:$<C_COMPILER_ID:GNU>>")
set(clang_c "$<$<COMPILE_LANGUAGE:C>:$<C_COMPILER_ID:Clang>>")
set(msvc_c "$<$<COMPILE_LANGUAGE:C>:$<C_COMPILER_ID:MSVC>>")
target_compile_options(errhand_compiler_flags
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
    list(APPEND target_outputs "errhand")
endif()

if(BUILD_STATIC)
    list(APPEND target_outputs "errhand-static")
endif()

########################################################################
# Sources/Headers definitions
########################################################################

# Source files
set(errhand_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/src/errhand_local_print.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/errhand_print.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/errhand_subsys.c
)

# for IDE project generation
source_group("Source Files" FILES ${errhand_sources})

# Header files
set(errhand_public_headers
    ${CMAKE_CURRENT_SOURCE_DIR}/include/errhand_assert.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/errhand_classes.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/errhand_core.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/errhand.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/errhand_local_print.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/errhand_opts.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/errhand_prelude.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/errhand_print.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/errhand_subsys.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/varg_macros.h
)

# for IDE project generation
source_group("Header Files" FILES ${errhand_public_headers})

# README files
set(readme-docs
    COPYING
)

########################################################################
# Library declaration
########################################################################

# build shared library
if(BUILD_SHARED)
    add_library(errhand SHARED
        ${errhand_sources}
        ${errhand_public_headers}
        ${readme-docs}
        ${errhand-pkgconfig}
    )

    set_target_properties(errhand PROPERTIES
        PUBLIC_HEADER "${errhand_public_headers}"
        SOVERSION "1"
        VERSION "${errhand_VERSION}"
        COMPILE_DEFINITIONS "DLL_EXPORT"
        OUTPUT_NAME "${errhand_OUTPUT_BASENAME}"
        PREFIX "lib")
endif()

# build static library
if(BUILD_STATIC)
    add_library(errhand-static STATIC
        ${errhand_sources}
        ${errhand_public_headers}
        ${readme-docs}
        ${errhand-pkgconfig}
    )

    set_target_properties(errhand-static PROPERTIES
      PUBLIC_HEADER "${errhand_public_headers}"
      OUTPUT_NAME "${errhand_OUTPUT_BASENAME}"
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

    # setting PUBLIC compile options
    target_compile_definitions(${target} PUBLIC
        ERRHAND_DBG=${errhand_DBG_OPT_VALUE}
        ERRHAND_MIN_LEVEL=${errhand_MIN_LEVEL_OPT}
        ERRHAND_SUBSYS_ON=${errhand_SUBSYS_ON_OPT}
    )

    # for .pc file
    string(CONCAT
        pkg_config_defines
        " -DERRHAND_DBG=${errhand_DBG_OPT_VALUE}"
        " -DERRHAND_MIN_LEVEL=${errhand_MIN_LEVEL_OPT}"
        " -DERRHAND_SUBSYS_ON=\"${errhand_SUBSYS_ON_OPT}\""
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
        $<TARGET_PROPERTY:errhand_compiler_flags,INTERFACE_LINK_LIBRARIES>
    )
    target_include_directories(${target} PRIVATE
        $<TARGET_PROPERTY:errhand_compiler_flags,INTERFACE_INCLUDE_DIRECTORIES>
    )
    target_compile_options(${target} PRIVATE
        $<TARGET_PROPERTY:errhand_compiler_flags,INTERFACE_COMPILE_OPTIONS>
    )
    target_compile_definitions(${target} PRIVATE
        $<TARGET_PROPERTY:errhand_compiler_flags,INTERFACE_COMPILE_DEFINITIONS>
    )
    target_compile_features(${target} PRIVATE
        $<TARGET_PROPERTY:errhand_compiler_flags,INTERFACE_COMPILE_FEATURES>
    )
    target_sources(${target} PRIVATE
        $<TARGET_PROPERTY:errhand_compiler_flags,INTERFACE_SOURCES>
    )
endforeach()

########################################################################
# Library dependencies
########################################################################

# set depedency to targets
if(BUILD_SHARED)
    target_link_libraries(errhand
        PUBLIC
        ${MORE_LIBRARIES}
    )
endif()

if(BUILD_STATIC)
    target_link_libraries(errhand-static
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

install(TARGETS ${target_outputs} errhand_compiler_flags
    EXPORT ${PROJECT_NAME}-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FRAMEWORK DESTINATION "Library/Frameworks"
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# CMake search path wants either "share" (AKA GNUInstallDirs DATAROOTDIR)
# for arch-independent, or LIBDIR for arch-dependent, plus "cmake" as prefix
set(errhand_CMAKECONFIG_INSTALL_DIR
    "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
    CACHE STRING
    "install path for errhandConfig.cmake and errhandTargets.cmake"
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
    ${errhand_VERSION_MAJOR}.${errhand_VERSION_MINOR}.${errhand_VERSION_PATCH}
)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/liberrhand.pc.in
    ${CMAKE_CURRENT_BINARY_DIR}/liberrhand.pc @ONLY)
set(errhand-pkgconfig ${CMAKE_CURRENT_BINARY_DIR}/liberrhand.pc)

# install .pc file
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/liberrhand.pc
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
    ${errhand_CMAKECONFIG_INSTALL_DIR}
)

# configure and generate <PROJECT_NAME>ConfigVersion.cmake file
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION
    ${errhand_VERSION}
    COMPATIBILITY AnyNewerVersion
)

# actually install <PROJECT_NAME>Targets.cmake file
if(BUILD_SHARED OR BUILD_STATIC)
    install(EXPORT ${PROJECT_NAME}-targets
        FILE ${PROJECT_NAME}Targets.cmake
        DESTINATION ${errhand_CMAKECONFIG_INSTALL_DIR}
    )
    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION
        ${errhand_CMAKECONFIG_INSTALL_DIR}
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
    set(CPACK_PACKAGE_NAME "errhand")
    set(CPACK_PACKAGE_FILE_NAME
        "${CPACK_PACKAGE_NAME}-${errhand_VERSION}-${arch_name}"
    )
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Error Handling library")
    set(CPACK_PACKAGE_VENDOR "LNLS")
    set(CPACK_PACKAGE_CONTACT "Lucas Russo <lucas.russo@lnls.br>")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_BINARY_DIR}/COPYING.txt")
    # set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_BINARY_DIR}/README.txt")
    # set(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_CURRENT_BINARY_DIR}/WELCOME.txt")
    # set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/branding.bmp")
    set(CPACK_PACKAGE_VERSION ${errhand_VERSION})
    set(CPACK_PACKAGE_VERSION_MAJOR ${errhand_VERSION_MAJOR})
    set(CPACK_PACKAGE_VERSION_MINOR ${errhand_VERSION_MINOR})
    set(CPACK_PACKAGE_VERSION_PATCH ${errhand_VERSION_PATCH})
    # set(CPACK_PACKAGE_INSTALL_DIRECTORY "errhand Install Directory")
    # set(CPACK_TEMPORARY_DIRECTORY "errhand Temporary CPack Directory")

    include(CPack)

    cpack_add_component_group(Development
        DISPLAY_NAME "Errhand library"
        EXPANDED
    )
    cpack_add_component(SourceCode
        DISPLAY_NAME "Errhand source code"
        DISABLED
        INSTALL_TYPES FullInstall
    )
    cpack_add_component(SDK
        DISPLAY_NAME "Errhand headers and libraries"
        INSTALL_TYPES FullInstall DevInstall
        GROUP Development
    )
    cpack_add_component(Runtime
        DISPLAY_NAME "Errhand runtime files"
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
