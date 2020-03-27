########################################################################
# Project setup
########################################################################

cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(errhand)

# set modules path for CMkake to find
set(ERRHAND_CMAKE_MODULES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)
list(APPEND CMAKE_MODULE_PATH ${ERRHAND_CMAKE_MODULES_DIR})

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
set (ERRHAND_OUTPUT_BASENAME "errhand"
    CACHE STRING
    "Output errhand library base name"
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
    "$<${gcc_c}:$<BUILD_INTERFACE:-Wall;-Wextra;-Werror>>"
    "$<${clang_c}:$<BUILD_INTERFACE:-Wall;-Wextra;-Werror>>"
    "$<${msvc_c}:$<BUILD_INTERFACE:-W3>>"
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
# User compile-time options
########################################################################

# should we use our errhand debug
option(ERRHAND_DBG_OPT "Enable debug mode" ON)
if(ERRHAND_DBG_OPT)
    set(ERRHAND_DBG_OPT_VALUE y)
else()
    set(ERRHAND_DBG_OPT_VALUE n)
endif()

message(STATUS
    "Setting compile-time option ERRHAND_DBG='${ERRHAND_DBG_OPT_VALUE}'")

# which debug level do we want
set(ERRHAND_MIN_LEVEL_OPT "DBG_LVL_INFO" CACHE STRING
    "Minimum debug level")
set(ERRHAND_MIN_LEVEL_VALUES
    "DBG_LVL_TRACE;DBG_LVL_INFO;DBG_LVL_WARN;DBG_LVL_ERR;DBG_LVL_FATAL"
)

set_property(CACHE ERRHAND_MIN_LEVEL_OPT PROPERTY STRINGS
    ${ERRHAND_MIN_LEVEL_VALUES}
)

message(STATUS
    "Setting compile-time option ERRHAND_MIN_LEVEL='${ERRHAND_MIN_LEVEL_OPT}'")

# which subsystem do we want on.
# Right now, the only option is all or none of them
set(ERRHAND_SUBSYS_ON_OPT
    "(DBG_DEV_MNGR | DBG_DEV_IO | DBG_SM_IO | DBG_LIB_CLIENT | DBG_SM_PR | DBG_SM_CH | DBG_LL_IO | DBG_HAL_UTILS)"
    CACHE STRING
    "Set Subsystem On")
set(ERRHAND_SUBSYS_ON_VALUES
    "(DBG_DEV_MNGR | DBG_DEV_IO | DBG_SM_IO | DBG_LIB_CLIENT | DBG_SM_PR | DBG_SM_CH | DBG_LL_IO | DBG_HAL_UTILS);"
)

set_property(CACHE ERRHAND_SUBSYS_ON_OPT PROPERTY STRINGS
    ${ERRHAND_SUBSYS_ON_VALUES}
)

message(STATUS
    "Setting compile-time option ERRHAND_SUBSYS_ON='${ERRHAND_SUBSYS_ON_OPT}'")


########################################################################
# Library declaration
########################################################################

# avoid building everything twice for shared + static
# only on *nix, as Windows needs different preprocessor
# defines in static builds

# add sources to "library" objects
add_library(objects OBJECT ${errhand_sources})

# always build objects with position independent code on
set_target_properties(objects PROPERTIES
    POSITION_INDEPENDENT_CODE
    ON
)

# setting compile options
target_compile_definitions(objects PUBLIC
    ERRHAND_DBG=${ERRHAND_DBG_OPT_VALUE}
)

target_compile_definitions(objects PUBLIC
    ERRHAND_MIN_LEVEL=${ERRHAND_MIN_LEVEL_OPT}
)

target_compile_definitions(objects PUBLIC
    ERRHAND_SUBSYS_ON=${ERRHAND_SUBSYS_ON_OPT}
)

# add the binary tree to the search path for include files
# so that we will find our headers
target_include_directories(objects
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  $<INSTALL_INTERFACE:include>)

# build shared library
if(BUILD_SHARED)
    add_library(errhand SHARED
        $<TARGET_OBJECTS:objects>
        ${errhand_public_headers}
        ${readme-docs}
        ${errhand-pkgconfig}
    )

    set_target_properties(errhand PROPERTIES
        PUBLIC_HEADER "${errhand_public_headers}"
        SOVERSION "1"
        VERSION "${ERRHAND_VERSION}"
        COMPILE_DEFINITIONS "DLL_EXPORT"
        OUTPUT_NAME "${ERRHAND_OUTPUT_BASENAME}"
        PREFIX "lib")
endif()

# build static library
if(BUILD_STATIC)
    add_library(errhand-static STATIC
        $<TARGET_OBJECTS:objects>
        ${errhand_public_headers}
        ${readme-docs}
        ${errhand-pkgconfig}
    )

    set_target_properties(errhand-static PROPERTIES
      PUBLIC_HEADER "${errhand_public_headers}"
      OUTPUT_NAME "${ERRHAND_OUTPUT_BASENAME}"
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
        errhand_compiler_flags
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
set(ERRHAND_CMAKECONFIG_INSTALL_DIR
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
set(prefix ${CMAKE_INSTALL_PREFIX})
set(exec_prefix ${prefix})
set(libdir ${prefix}/lib)
set(includedir ${prefix}/include)
set(VERSION
    ${ERRHAND_VERSION_MAJOR}.${ERRHAND_VERSION_MINOR}.${ERRHAND_VERSION_PATCH}
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
    cmake/${PROJECT_NAME}Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION
    ${ERRHAND_CMAKECONFIG_INSTALL_DIR}
)

# configure and generate <PROJECT_NAME>ConfigVersion.cmake file
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION
    ${ERRHAND_VERSION}
    COMPATIBILITY AnyNewerVersion
)

# actually install <PROJECT_NAME>Targets.cmake file
if(BUILD_SHARED OR BUILD_STATIC)
    install(EXPORT ${PROJECT_NAME}-targets
        FILE ${PROJECT_NAME}Targets.cmake
        DESTINATION ${ERRHAND_CMAKECONFIG_INSTALL_DIR}
    )
    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION
        ${ERRHAND_CMAKECONFIG_INSTALL_DIR}
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
    set(CPACK_PACKAGE_NAME "errhand")
    set(CPACK_PACKAGE_FILE_NAME
        "${CPACK_PACKAGE_NAME}-${ERRHAND_VERSION}-${arch_name}"
    )
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Error Handling library")
    set(CPACK_PACKAGE_VENDOR "LNLS")
    set(CPACK_PACKAGE_CONTACT "Lucas Russo <lucas.russo@lnls.br>")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_BINARY_DIR}/COPYING.txt")
    # set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_BINARY_DIR}/README.txt")
    # set(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_CURRENT_BINARY_DIR}/WELCOME.txt")
    # set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/branding.bmp")
    set(CPACK_PACKAGE_VERSION ${ERRHAND_VERSION})
    set(CPACK_PACKAGE_VERSION_MAJOR ${ERRHAND_VERSION_MAJOR})
    set(CPACK_PACKAGE_VERSION_MINOR ${ERRHAND_VERSION_MINOR})
    set(CPACK_PACKAGE_VERSION_PATCH ${ERRHAND_VERSION_PATCH})
    # set(CPACK_PACKAGE_INSTALL_DIRECTORY "ERRHAND Install Directory")
    # set(CPACK_TEMPORARY_DIRECTORY "ERRHAND Temporary CPack Directory")

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
