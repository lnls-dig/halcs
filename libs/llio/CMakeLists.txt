########################################################################
# Project setup
########################################################################

cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(llio)

# set modules path for CMkake to find
set(LLIO_CMAKE_MODULES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)
list(APPEND CMAKE_MODULE_PATH ${LLIO_CMAKE_MODULES_DIR})

# add module to prevent in-source builds
include(PreventInSourceBuilds)
# set installation variables compliant with distributions
include(GNUInstallDirs)
# configure and generate *.Config.cmake files
include(CMakePackageConfigHelpers)
# extract version numbers from header file
include(TestLlioVersion)
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
set (LLIO_OUTPUT_BASENAME "llio"
    CACHE STRING
    "Output llio library base name"
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
# CONVC dependency
#########################################################################

find_package(convc)
if(CONVC_FOUND)
    include_directories(${CONVC_INCLUDE_DIRS})
    list(APPEND MORE_LIBRARIES ${CONVC_LIBRARIES})
  if(PC_CONVC_FOUND)
      set(pkg_config_names_private "${pkg_config_names_private} libconvc")
      list(APPEND OPT_LIBRARIES_STATIC ${PC_CONVC_STATIC_LDFLAGS})
  else()
      set(pkg_config_libs_private "${pkg_config_libs_private} -lconvc")
  endif()
else()
    message(FATAL_ERROR
        "libconvc is not installed. Install it, then run CMake again." )
endif()

#########################################################################
# PCIE Driver lib dependency
#########################################################################

find_package(pciedriver)
if(PCIEDRIVER_FOUND)
    include_directories(${PCIEDRIVER_INCLUDE_DIRS})
    list(APPEND MORE_LIBRARIES ${PCIEDRIVER_LIBRARIES})
  if(PC_PCIEDRIVER_FOUND)
      set(pkg_config_names_private "${pkg_config_names_private} libpciedriver")
      list(APPEND OPT_LIBRARIES_STATIC ${PC_PCIEDRIVER_STATIC_LDFLAGS})
  else()
      set(pkg_config_libs_private "${pkg_config_libs_private} -lpciedriver")
  endif()
  message(STATUS "PCIEDRIVER_LIBRARIES: ${PCIEDRIVER_LIBRARIES}")
  message(STATUS "PCIEDRIVER_INCLUDE_DIRS: ${PCIEDRIVER_INCLUDE_DIRS}")
else()
    message(FATAL_ERROR
        "libpciedriver is not installed. Install it, then run CMake again." )
endif()

########################################################################
# Library compile options
########################################################################

add_library(llio_compiler_flags INTERFACE)
target_compile_features(llio_compiler_flags
    INTERFACE
    c_std_99
)

# add compiler warning flags just when building this project via
# the BUILD_INTERFACE genex
set(gcc_c "$<$<COMPILE_LANGUAGE:C>:$<C_COMPILER_ID:GNU>>")
set(clang_c "$<$<COMPILE_LANGUAGE:C>:$<C_COMPILER_ID:Clang>>")
set(msvc_c "$<$<COMPILE_LANGUAGE:C>:$<C_COMPILER_ID:MSVC>>")
target_compile_options(llio_compiler_flags
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
    list(APPEND target_outputs "llio")
endif()

if(BUILD_STATIC)
    list(APPEND target_outputs "llio-static")
endif()

########################################################################
# Sources/Headers definitions
########################################################################

# Source files
set(llio_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ll_io_core.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ll_io_dev_info.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ll_io_endpoint.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ll_io_err.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ops/ll_io_eth.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ops/ll_io_eth_utils.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ops/ll_io_pcie.c
)

# for IDE project generation
source_group("Source Files" FILES ${llio_sources})

# Header files
set(llio_public_headers
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ll_io_classes.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ll_io_core.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ll_io_dev_info.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ll_io_endpoint.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ll_io_err.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ll_io_eth.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ll_io_eth_utils.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ll_io.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ll_io_pcie.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ll_io_prelude.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ll_io_utils.h
)

# Header directory
set(llio_public_dir_headers
    ${CMAKE_CURRENT_SOURCE_DIR}/include/hw/pcie_regs.h
)

# Get directory names only
list(APPEND llio_public_dirs "")

foreach(public_dir_header ${llio_public_dir_headers})
    get_filename_component(public_dir
        ${public_dir_header}
        DIRECTORY
    )

    list(APPEND llio_public_dirs ${public_dir})
endforeach()

# remove, well, duplicates ...
list(REMOVE_DUPLICATES llio_public_dirs)

# for IDE project generation
source_group("Header Files" FILES
    ${llio_public_headers}
    ${llio_public_dir_headers}
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
add_library(objects OBJECT ${llio_sources})

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
get_target_property(llio_compiler_features
    llio_compiler_flags
    INTERFACE_COMPILE_FEATURES
)

target_compile_features(objects
    PRIVATE
    ${llio_compiler_features}
)

get_target_property(llio_compiler_options
    llio_compiler_flags
    INTERFACE_COMPILE_OPTIONS
)

target_compile_options(objects
    PRIVATE
    ${llio_compiler_options}
)

# build shared library
if(BUILD_SHARED)
    add_library(llio SHARED
        $<TARGET_OBJECTS:objects>
        ${llio_public_headers}
        ${llio_public_dir_headers}
        ${readme-docs}
        ${llio-pkgconfig}
    )

    set_target_properties(llio PROPERTIES
        PUBLIC_HEADER "${llio_public_headers}"
        SOVERSION "1"
        VERSION "${LLIO_VERSION}"
        COMPILE_DEFINITIONS "DLL_EXPORT"
        OUTPUT_NAME "${LLIO_OUTPUT_BASENAME}"
        PREFIX "lib")
endif()

# build static library
if(BUILD_STATIC)
    add_library(llio-static STATIC
        $<TARGET_OBJECTS:objects>
        ${llio_public_headers}
        ${llio_public_dir_headers}
        ${readme-docs}
        ${llio-pkgconfig}
    )

    set_target_properties(llio-static PROPERTIES
      PUBLIC_HEADER "${llio_public_headers}"
      OUTPUT_NAME "${LLIO_OUTPUT_BASENAME}"
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
        llio_compiler_flags
    )
endforeach()

########################################################################
# Library dependencies
########################################################################

# set depedency to targets
if(BUILD_SHARED)
    target_link_libraries(llio
        PUBLIC
        ${MORE_LIBRARIES}
    )
endif()

if(BUILD_STATIC)
    target_link_libraries(llio-static
        PUBLIC
        ${MORE_LIBRARIES}
    )
endif()

########################################################################
# Installer
########################################################################

install(TARGETS ${target_outputs} llio_compiler_flags
    EXPORT ${PROJECT_NAME}-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FRAMEWORK DESTINATION "Library/Frameworks"
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# install headers that are inside another directory
install(DIRECTORY
    ${llio_public_dirs}
    DESTINATION
    ${CMAKE_INSTALL_INCLUDEDIR}
)

# CMake search path wants either "share" (AKA GNUInstallDirs DATAROOTDIR)
# for arch-independent, or LIBDIR for arch-dependent, plus "cmake" as prefix
set(LLIO_CMAKECONFIG_INSTALL_DIR
    "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
    CACHE STRING
    "install path for llioConfig.cmake and llioTargets.cmake"
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
    ${LLIO_VERSION_MAJOR}.${LLIO_VERSION_MINOR}.${LLIO_VERSION_PATCH}
)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/libllio.pc.in
    ${CMAKE_CURRENT_BINARY_DIR}/libllio.pc @ONLY)
set(llio-pkgconfig ${CMAKE_CURRENT_BINARY_DIR}/libllio.pc)

# install .pc file
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/libllio.pc
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
    ${LLIO_CMAKECONFIG_INSTALL_DIR}
)

# configure and generate <PROJECT_NAME>ConfigVersion.cmake file
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION
    ${LLIO_VERSION}
    COMPATIBILITY AnyNewerVersion
)

# actually install <PROJECT_NAME>Targets.cmake file
if(BUILD_SHARED OR BUILD_STATIC)
    install(EXPORT ${PROJECT_NAME}-targets
        FILE ${PROJECT_NAME}Targets.cmake
        DESTINATION ${LLIO_CMAKECONFIG_INSTALL_DIR}
    )
    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION
        ${LLIO_CMAKECONFIG_INSTALL_DIR}
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
    set(CPACK_PACKAGE_NAME "llio")
    set(CPACK_PACKAGE_FILE_NAME
        "${CPACK_PACKAGE_NAME}-${LLIO_VERSION}-${arch_name}"
    )
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Low-level I/O library")
    set(CPACK_PACKAGE_VENDOR "LNLS")
    set(CPACK_PACKAGE_CONTACT "Lucas Russo <lucas.russo@lnls.br>")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_BINARY_DIR}/COPYING.txt")
    # set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_BINARY_DIR}/README.txt")
    # set(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_CURRENT_BINARY_DIR}/WELCOME.txt")
    # set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/branding.bmp")
    set(CPACK_PACKAGE_VERSION ${LLIO_VERSION})
    set(CPACK_PACKAGE_VERSION_MAJOR ${LLIO_VERSION_MAJOR})
    set(CPACK_PACKAGE_VERSION_MINOR ${LLIO_VERSION_MINOR})
    set(CPACK_PACKAGE_VERSION_PATCH ${LLIO_VERSION_PATCH})
    # set(CPACK_PACKAGE_INSTALL_DIRECTORY "LLIO Install Directory")
    # set(CPACK_TEMPORARY_DIRECTORY "LLIO Temporary CPack Directory")

    include(CPack)

    cpack_add_component_group(Development
        DISPLAY_NAME "Low-level I/O library"
        EXPANDED
    )
    cpack_add_component(SourceCode
        DISPLAY_NAME "Low-level I/O source code"
        DISABLED
        INSTALL_TYPES FullInstall
    )
    cpack_add_component(SDK
        DISPLAY_NAME "Low-level I/O headers and libraries"
        INSTALL_TYPES FullInstall DevInstall
        GROUP Development
    )
    cpack_add_component(Runtime
        DISPLAY_NAME "Low-level I/O runtime files"
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