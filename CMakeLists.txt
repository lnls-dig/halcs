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

#########################################################################
# PCIEDRIVER dependency
#########################################################################

include(AddExternalProjectPciedriver)

# Add "target" name library dependency
list(APPEND halcs_LIBRARIES pciedriver)

#########################################################################
# BSMP dependency
#########################################################################

include(AddExternalProjectBsmp)

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

set(sm_io_chips_regs_INCLUDE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/include/chips
)

set(sm_io_modules_regs_INCLUDE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/include/hw
)

set(sm_io_protocols_INCLUDE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/include/protocols
)

set(halcs_LINKER_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/linker
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/boards)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/dev_io)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/msg)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/revision)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/sm_io)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/sm_io_table)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/dev_mngr)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/apps)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples)

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
