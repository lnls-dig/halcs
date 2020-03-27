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

# Will be used to add flags to pkg-config useful when apps want to statically link
set(pkg_config_libs_private "")
set(pkg_config_names_private "")
set(pkg_config_defines "")

set(CMAKE_DEBUG_POSTFIX d)

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

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

if(APPLE)
  set(CMAKE_INSTALL_RPATH "@executable_path/../lib")
elseif(UNIX)
  set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
endif()

# configure a header file to pass the version number only
#configure_file(errhandConfig.h.in errhandConfig.h)

# add the MathFunctions library
#add_subdirectory(MathFunctions)

set(ERRHAND_SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/src/errhand_local_print.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/errhand_print.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/errhand_subsys.c
)

# add the library
add_library(errhand ${ERRHAND_SRCS})

# set debug posfix in case of debug builds
set_target_properties(errhand PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

# add the binary tree to the search path for include files
# so that we will find our headers
target_include_directories(errhand
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

########################################################################
# Compile options
########################################################################

# should we use our errhand debug
option(ERRHAND_DBG_OPT "Enable debug mode" ON)
if(ERRHAND_DBG_OPT)

    set(ERRHAND_DBG_OPT_VALUE y)

else()

    set(ERRHAND_DBG_OPT_VALUE n)

endif()

target_compile_definitions(errhand PRIVATE
    ERRHAND_DBG=${ERRHAND_DBG_OPT_VALUE})

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

target_compile_definitions(errhand PRIVATE ERRHAND_MIN_LEVEL=${ERRHAND_MIN_LEVEL_OPT})

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

target_compile_definitions(errhand PRIVATE ERRHAND_SUBSYS_ON=${ERRHAND_SUBSYS_ON_OPT})

message(STATUS
    "Setting compile-time option ERRHAND_SUBSYS_ON='${ERRHAND_SUBSYS_ON_OPT}'")

# set library dependencies
set(OPT_LIBRARIES_STATIC)

########################################################################
# LIBZMQ dependency
########################################################################
find_package(libzmq)
if(LIBZMQ_FOUND)
    target_include_directories(errhand
        PUBLIC
        ${LIBZMQ_INCLUDE_DIRS}
    )
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
    target_include_directories(errhand
        PUBLIC
        ${CZMQ_INCLUDE_DIRS}
    )
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

# set depedency to all libraries
target_link_libraries(errhand
    PUBLIC
    ${MORE_LIBRARIES}
)

########################################################################
# Library
########################################################################

# state that errhand need PIC when the default is shared libraries
set_target_properties(errhand PROPERTIES
    POSITION_INDEPENDENT_CODE
    ${BUILD_SHARED_LIBS}
)

target_link_libraries(errhand PUBLIC errhand_compiler_flags)
