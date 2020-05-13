########################################################################
# Library setup
########################################################################

# library basename
set (msg_OUTPUT_BASENAME "msg"
    CACHE STRING
    "Output msg library base name"
)

#########################################################################
# XXX dependency
#########################################################################

# list(APPEND msg_LIBRARIES XXX)

########################################################################
# User compile-time options
########################################################################

# should we use local debug or not
option(msg_LOCAL_MSG_DBG "Enable local message debug" OFF)
if(msg_LOCAL_MSG_DBG)
    set(msg_LOCAL_MSG_DBG_VALUE y)
else()
    set(msg_LOCAL_MSG_DBG_VALUE n)
endif()

message(STATUS
    "Setting compile-time option msg_LOCAL_MSG_DBG='${msg_LOCAL_MSG_DBG_VALUE}'")

########################################################################
# Library compile options
########################################################################

add_library(msg_compiler_flags INTERFACE)

target_link_libraries(msg_compiler_flags
    INTERFACE
    halcs_compiler_flags
)

########################################################################
# Sources/Headers definitions
########################################################################

#######################################
# Other sources
#######################################

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/exp_ops)
list(APPEND msg_LIBRARIES msg_exp_ops)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/smio_thsafe_ops)
list(APPEND msg_LIBRARIES msg_smio_thsafe_ops)

#######################################
# Our sources
#######################################

# Source files

set(msg_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/msg.c
    ${CMAKE_CURRENT_SOURCE_DIR}/msg_err.c
)

# for IDE project generation
source_group("Source Files" FILES
    ${msg_sources}
)

# Header files
list(APPEND msg_other_search_dirs "")

#######################################
# MSG header definitions
#######################################

set(msg_public_headers
    ${common_INCLUDE_DIR}/msg.h
    ${common_INCLUDE_DIR}/msg_err.h
    ${common_INCLUDE_DIR}/msg_macros.h
)

# Get directory names only
list(APPEND msg_search_dirs "")

foreach(search_dir_header ${msg_public_headers})
    get_filename_component(search_dir
        ${search_dir_header}
        DIRECTORY
    )

    list(APPEND msg_search_dirs ${search_dir})
endforeach()

# remove, well, duplicates ...
list(REMOVE_DUPLICATES msg_search_dirs)

# append to global search list
list(APPEND msg_other_search_dirs ${msg_search_dirs})

#######################################
# HEADER Grouping
#######################################

# for IDE project generation
source_group("Header Files" FILES
    ${msg_public_headers}
)

# append to global search list
list(APPEND msg_other_search_dirs ${common_INCLUDE_DIRS})

########################################################################
# Library declaration
########################################################################

# build static library
add_library(msg STATIC
    ${msg_sources}
    ${msg_public_headers}
)

set_target_properties(msg PROPERTIES
  PUBLIC_HEADER "${msg_public_headers}"
  OUTPUT_NAME "${msg_OUTPUT_BASENAME}"
  PREFIX "lib")

# msg_other_search_dirs is in quotes as this is a list
# and the first element would be concatenated with the previous
# generator expression.
target_include_directories(msg
    PRIVATE
    "$<BUILD_INTERFACE:${msg_other_search_dirs}>"
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>

)

# setting compile options
string(APPEND msg_definitions "")

if(msg_LOCAL_MSG_DBG_VALUE STREQUAL y)
    list(APPEND msg_definitions
        LOCAL_MSG_DBG=1
    )
endif()

# setting compile options
target_compile_definitions(msg PRIVATE
    ${msg_definitions}
)

# set debug posfix in case of debug builds
set_target_properties(msg PROPERTIES
    DEBUG_POSTFIX
    ${CMAKE_DEBUG_POSTFIX}
    POSITION_INDEPENDENT_CODE
    ON
)

# set PRIVATE compiler flags. It doesn't really work to link an
# INTERFACE library with PRIVATE scope. See:
# https://gitlab.kitware.com/cmake/cmake/-/issues/15415
target_link_libraries(msg PRIVATE
    $<TARGET_PROPERTY:msg_compiler_flags,INTERFACE_LINK_LIBRARIES>
)
target_include_directories(msg PRIVATE
    $<TARGET_PROPERTY:msg_compiler_flags,INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_options(msg PRIVATE
    $<TARGET_PROPERTY:msg_compiler_flags,INTERFACE_COMPILE_OPTIONS>
)
target_compile_definitions(msg PRIVATE
    $<TARGET_PROPERTY:msg_compiler_flags,INTERFACE_COMPILE_DEFINITIONS>
)
target_compile_features(msg PRIVATE
    $<TARGET_PROPERTY:msg_compiler_flags,INTERFACE_COMPILE_FEATURES>
)
target_sources(msg PRIVATE
    $<TARGET_PROPERTY:msg_compiler_flags,INTERFACE_SOURCES>
)

########################################################################
# Library dependencies
########################################################################

# set depedency to targets
target_link_libraries(msg
    PRIVATE
    ${msg_LIBRARIES}
    ${halcs_LIBRARIES}
)
