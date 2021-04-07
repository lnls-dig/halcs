########################################################################
# Library setup
########################################################################

# library basename
set (boards_ml605_OUTPUT_BASENAME "boards_ml605"
    CACHE STRING
    "Output boards_ml605 library base name"
)

#########################################################################
# XXX dependency
#########################################################################

# list(APPEND boards_ml605_LIBRARIES XXX)

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

set(boards_ml605_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/chips_addr.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ddr3_map.c
    ${CMAKE_CURRENT_SOURCE_DIR}/device_mapping.c
)

# for IDE project generation
source_group("Source Files" FILES
    ${boards_ml605_sources}
)

# Header files
list(APPEND boards_ml605_other_search_dirs "")

#######################################
# BOARD ML605 header definitions
#######################################

set(boards_ml605_public_headers
    ${common_INCLUDE_DIR}/boards/ml605/acq_chan.h
    ${common_INCLUDE_DIR}/boards/ml605/board.h
    ${common_INCLUDE_DIR}/boards/ml605/chips_addr.h
    ${common_INCLUDE_DIR}/boards/ml605/ddr3_defs.h
    ${common_INCLUDE_DIR}/boards/ml605/ddr3_map.h
    ${common_INCLUDE_DIR}/boards/ml605/defs.h
    ${common_INCLUDE_DIR}/boards/ml605/mem_layout.h
)

# Get directory names only
list(APPEND boards_ml605_search_dirs "")

foreach(search_dir_header ${boards_ml605_public_headers})
    get_filename_component(search_dir
        ${search_dir_header}
        DIRECTORY
    )

    list(APPEND boards_ml605_search_dirs ${search_dir})
endforeach()

# remove, well, duplicates ...
list(REMOVE_DUPLICATES boards_ml605_search_dirs)

# append to global search list
list(APPEND boards_ml605_other_search_dirs ${boards_ml605_search_dirs})

#######################################
# HEADER Grouping
#######################################

# for IDE project generation
source_group("Header Files" FILES
    ${boards_ml605_public_headers}
)

########################################################################
# Library declaration
########################################################################

# build static library
add_library(boards_ml605 STATIC
    ${boards_ml605_sources}
    ${boards_ml605_public_headers}
)

set_target_properties(boards_ml605 PROPERTIES
  PUBLIC_HEADER "${boards_ml605_public_headers}"
  OUTPUT_NAME "${boards_ml605_OUTPUT_BASENAME}"
  PREFIX "lib")

# boards_ml605_other_search_dirs is in quotes as this is a list
# and the first element would be concatenated with the previous
# generator expression.
target_include_directories(boards_ml605
    PRIVATE
    "$<BUILD_INTERFACE:${boards_ml605_other_search_dirs}>"
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>
)

# setting compile options
target_compile_definitions(boards_ml605
    PUBLIC
    ${boards_ml605_definitions}
)

# set debug posfix in case of debug builds
set_target_properties(boards_ml605 PROPERTIES
    DEBUG_POSTFIX
    ${CMAKE_DEBUG_POSTFIX}
    POSITION_INDEPENDENT_CODE
    ON
)

# set compiler flags
target_link_libraries(boards_ml605
    PRIVATE
    boards_compiler_flags
)

########################################################################
# Library dependencies
########################################################################

# set depedency to targets
target_link_libraries(boards_ml605
    PRIVATE
    ${boards_ml605_LIBRARIES}
    ${halcs_LIBRARIES}
)
