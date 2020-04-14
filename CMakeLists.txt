########################################################################
# Project setup
########################################################################

cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(halcs)

# set modules path for CMkake to find
set(halcs_CMAKE_MODULES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)
list(APPEND CMAKE_MODULE_PATH ${halcs_CMAKE_MODULES_DIR})

# add module to prevent in-source builds
include(PreventInSourceBuilds)
# set installation variables compliant with distributions
include(GNUInstallDirs)
# configure and generate *.Config.cmake files
include(CMakePackageConfigHelpers)
# extract version numbers from header file
include(TestHalcsVersion)
# detect target architecture
include(TargetArch)
# add git introspection functions
include(GetGitRevisionDescription)
### add FetchContent to enabla configure-time populating dependencies
##include(FetchContent)
include(ExternalProject)

target_architecture(ARCH)

# find pkg-config
find_package(PkgConfig)

set(CMAKE_DEBUG_POSTFIX d)

# library basename
set (halcs_OUTPUT_BASENAME "halcs"
    CACHE STRING
    "Output halcs base name"
)

# set library dependencies
set(OPT_LIBRARIES_STATIC)

########################################################################
# Macros
########################################################################

# Based on the lecture by Daniel Pfeifer - "Effective CMake"

# list all internal project libraries that we don't want to search for
# dependencies
set(as_subproject
    halcsclient
    acqclient
    bpmclient
    convc
    disptable
    errhand
    hutils
    llio
    sdbfs
    sdbutils
    # external projects, not CMake
    pciedriver
    bsmp
)

# override find_package to only call the built-in find_package()
# if not project listed above. Fool library checks to think
# find_package was executed and set the necessary variables>
# <lib_name>_LIBRARIES will have the target name which will be
# enough for the project to find all dependencies.
macro(find_package)
    if(NOT "${ARGV0}" IN_LIST as_subproject)
        _find_package(${ARGV})
    else()
        set(${ARGV0}_FOUND TRUE)
        set(${ARGV0}_LIBRARIES ${ARGV0})
        set(${ARGV0}_INCLUDE_DIRS "")
    endif()
endmacro()

########################################################################
# User compile-time options
########################################################################

# which board do we want.

set(halcs_BOARD_OPT "AFCV3_1" CACHE STRING
    "HALCS board")
set(halcs_BOARD_VALUES
    "ML605;AFCV3;AFCV3_1"
)

set_property(CACHE halcs_BOARD_OPT PROPERTY STRINGS
    ${halcs_BOARD_VALUES}
)

message(STATUS
    "Setting compile-time option halcs_BOARD='${halcs_BOARD_OPT}'")


########################################################################
# Library options
########################################################################

add_library(halcs_compiler_flags INTERFACE)
target_compile_features(halcs_compiler_flags
    INTERFACE
    c_std_99
)

# add compiler warning flags just when building this project via
# the BUILD_INTERFACE genex
set(gcc_c "$<$<COMPILE_LANGUAGE:C>:$<C_COMPILER_ID:GNU>>")
set(clang_c "$<$<COMPILE_LANGUAGE:C>:$<C_COMPILER_ID:Clang>>")
set(msvc_c "$<$<COMPILE_LANGUAGE:C>:$<C_COMPILER_ID:MSVC>>")
target_compile_options(halcs_compiler_flags
    INTERFACE
    "$<${gcc_c}:$<BUILD_INTERFACE:-Wall;-Wextra;-Werror;-O2>>"
    "$<${clang_c}:$<BUILD_INTERFACE:-Wall;-Wextra;-Werror;-O2>>"
    "$<${msvc_c}:$<BUILD_INTERFACE:-W3;-O2>>"
)

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
        list(APPEND OPT_LIBRARIES_STATIC ${PC_libzmq_STATIC_LDFLAGS})
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
        list(APPEND OPT_LIBRARIES_STATIC ${PC_czmq_STATIC_LDFLAGS})
    endif()
else()
    message(FATAL_ERROR
        "czmq is not installed. Install it, then run CMake again." )
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
        list(APPEND OPT_LIBRARIES_STATIC ${PC_mlm_STATIC_LDFLAGS})
    endif()
else()
    message(FATAL_ERROR
        "libmlm is not installed. Install it, then run CMake again." )
endif()

#########################################################################
# PCIEDRIVER dependency
#########################################################################

ExternalProject_Add(pciedriver_ext
    URL ${CMAKE_CURRENT_SOURCE_DIR}/foreign/pcie-driver
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/pciedriver
    CONFIGURE_COMMAND ""
    # Build only library, not the driver
    BUILD_COMMAND make lib_driver
    INSTALL_COMMAND ""
    # PCIEDRIVER assumes in-source build
    BUILD_IN_SOURCE ON
)

ExternalProject_Get_property(pciedriver_ext BINARY_DIR)

# this is not a CMake project, so no *Config.cmake or *Targets.cmake
# files will be generated. Hardcode library and include paths so when
# libraries try to search for pciedriver they can find it locally!
set(pciedriver_LIBRARY_DIRECTORIES
    ${BINARY_DIR}/lib/pcie
)

set(pciedriver_INCLUDE_DIRECTORIES
    ${BINARY_DIR}/include/pciDriver/lib
    ${BINARY_DIR}/include/pciDriver/lib/../..
)

# Hack to make it work, otherwise INTERFACE_INCLUDE_DIRECTORIES will not be propagated
file(MAKE_DIRECTORY ${pciedriver_INCLUDE_DIRECTORIES})

# add imported library so we can link to it later
add_library(pciedriver SHARED IMPORTED)

# set library locations
set_target_properties(pciedriver
    PROPERTIES
    IMPORTED_LOCATION
    ${pciedriver_LIBRARY_DIRECTORIES}/libpcidriver.so
    INTERFACE_INCLUDE_DIRECTORIES
    "${pciedriver_INCLUDE_DIRECTORIES}"
)

add_dependencies(pciedriver pciedriver_ext)

# Add "target" name library dependency
list(APPEND halcs_LIBRARIES pciedriver)

#########################################################################
# BSMP dependency
#########################################################################

ExternalProject_Add(bsmp_ext
    URL ${CMAKE_CURRENT_SOURCE_DIR}/foreign/libbsmp
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/bsmp
    CONFIGURE_COMMAND ""
    BUILD_COMMAND make
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

# add imported library so we can link to it later
add_library(bsmp SHARED IMPORTED)

# set library locations
set_target_properties(bsmp
    PROPERTIES
    IMPORTED_LOCATION
    ${bsmp_LIBRARY_DIRECTORIES}/libbsmp.so
    INTERFACE_INCLUDE_DIRECTORIES
    "${bsmp_INCLUDE_DIRECTORIES}"
)

add_dependencies(bsmp bsmp_ext)

# Add "target" name library dependency
list(APPEND halcs_LIBRARIES bsmp)

#########################################################################
# ERRHAND dependency
#########################################################################

add_subdirectory(libs/errhand)

# Add "target" name library dependency
list(APPEND halcs_LIBRARIES errhand)

#########################################################################
# CONVC dependency
#########################################################################

add_subdirectory(libs/convc)

# Add "target" name library dependency
list(APPEND halcs_LIBRARIES convc)

#########################################################################
# LLIO dependency
#########################################################################

add_subdirectory(libs/llio)

# Add "target" name library dependency
list(APPEND halcs_LIBRARIES llio)

#########################################################################
# SDBFS dependency
#########################################################################

add_subdirectory(libs/sdbfs)

# Add "target" name library dependency
list(APPEND halcs_LIBRARIES sdbfs)

#########################################################################
# SDBUTILS dependency
#########################################################################

add_subdirectory(libs/sdbutils)

# Add "target" name library dependency
list(APPEND halcs_LIBRARIES sdbutils)

#########################################################################
# HUTILS dependency
#########################################################################

add_subdirectory(libs/hutils)

# Add "target" name library dependency
list(APPEND halcs_LIBRARIES hutils)

#########################################################################
# DISPTABLE dependency
#########################################################################

add_subdirectory(libs/disptable)

# Add "target" name library dependency
list(APPEND halcs_LIBRARIES disptable)

#########################################################################
# HALCSCLIENT dependency
#########################################################################

# set compile options to halcsclient
set(halcsclient_BOARD_OPT ${halcs_BOARD_OPT})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/client)

# Add "target" name library dependency
list(APPEND halcs_LIBRARIES halcsclient)

#########################################################################
# ACQCLIENT dependency
#########################################################################

add_subdirectory(libs/acqclient)

#########################################################################
# BPMCLIENT dependency
#########################################################################

add_subdirectory(libs/bpmclient)

########################################################################
# Library compile options
########################################################################

########################################################################
# Sources/Headers definitions
########################################################################

set(common_INCLUDE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(smio_chips_regs_INCLUDE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/include/chips
)

set(smio_modules_regs_INCLUDE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/include/hw
)

set(smio_protocols_INCLUDE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/include/protocols
)

#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/cfg)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/apps)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/boards)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/dev_io)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/dev_mngr)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/msg)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/revision)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/sm_io)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/sm_io_table)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/linker)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples)

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
