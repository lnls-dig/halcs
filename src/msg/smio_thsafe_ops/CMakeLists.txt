########################################################################
# Library setup
########################################################################

# library basename
set (msg_smio_thsafe_ops_OUTPUT_BASENAME "msg_smio_thsafe_ops"
    CACHE STRING
    "Output msg_smio_thsafe_ops library base name"
)

#########################################################################
# XXX dependency
#########################################################################

# list(APPEND msg_smio_thsafe_ops_LIBRARIES XXX)

########################################################################
# Library compile options
########################################################################

########################################################################
# Sources/Headers definitions
########################################################################

#######################################
# Our sources
#######################################

# Source files

set(msg_smio_thsafe_ops_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/smio_thsafe_zmq_server.c
    ${CMAKE_CURRENT_SOURCE_DIR}/smio_thsafe_zmq_client.c
    ${CMAKE_CURRENT_SOURCE_DIR}/thsafe_msg_zmq.c
)

# for IDE project generation
source_group("Source Files" FILES
    ${msg_smio_thsafe_ops_sources}
)

# Header files
list(APPEND msg_smio_thsafe_ops_other_search_dirs "")

#######################################
# MSG header definitions
#######################################

set(msg_smio_thsafe_ops_public_headers
    ${common_INCLUDE_DIR}/smio_thsafe_zmq_server.h
    ${common_INCLUDE_DIR}/smio_thsafe_zmq_client.h
    ${common_INCLUDE_DIR}/thsafe_msg_zmq.h
)

# Get directory names only
list(APPEND msg_smio_thsafe_ops_search_dirs "")

foreach(search_dir_header ${msg_smio_thsafe_ops_public_headers})
    get_filename_component(search_dir
        ${search_dir_header}
        DIRECTORY
    )

    list(APPEND msg_smio_thsafe_ops_search_dirs ${search_dir})
endforeach()

# remove, well, duplicates ...
list(REMOVE_DUPLICATES msg_smio_thsafe_ops_search_dirs)

# append to global search list
list(APPEND msg_smio_thsafe_ops_other_search_dirs ${msg_smio_thsafe_ops_search_dirs})

#######################################
# HEADER Grouping
#######################################

# for IDE project generation
source_group("Header Files" FILES
    ${msg_smio_thsafe_ops_public_headers}
)

########################################################################
# Library declaration
########################################################################

# build static library
add_library(msg_smio_thsafe_ops STATIC
    ${msg_smio_thsafe_ops_sources}
    ${msg_smio_thsafe_ops_public_headers}
)

set_target_properties(msg_smio_thsafe_ops PROPERTIES
  PUBLIC_HEADER "${msg_smio_thsafe_ops_public_headers}"
  OUTPUT_NAME "${msg_smio_thsafe_ops_OUTPUT_BASENAME}"
  PREFIX "lib")

# msg_smio_thsafe_ops_other_search_dirs is in quotes as this is a list
# and the first element would be concatenated with the previous
# generator expression.
target_include_directories(msg_smio_thsafe_ops
    PRIVATE
    "$<BUILD_INTERFACE:${msg_smio_thsafe_ops_other_search_dirs}>"
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>
)

# set debug posfix in case of debug builds
set_target_properties(msg_smio_thsafe_ops PROPERTIES
    DEBUG_POSTFIX
    ${CMAKE_DEBUG_POSTFIX}
    POSITION_INDEPENDENT_CODE
    ON
)

# set compiler flags
target_link_libraries(msg_smio_thsafe_ops
    PRIVATE
    msg_compiler_flags
)

########################################################################
# Library dependencies
########################################################################

# set depedency to targets
target_link_libraries(msg_smio_thsafe_ops
    PRIVATE
    ${msg_smio_thsafe_ops_LIBRARIES}
    ${halcs_LIBRARIES}
)
