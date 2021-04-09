########################################################################
# Library setup
########################################################################

# extract version numbers from header file
include(TestHalcsVersion)

# library basename
set (boards_afcv3_1_OUTPUT_BASENAME "boards_afcv3_1"
    CACHE STRING
    "Output boards_afcv3_1 library base name"
)

#########################################################################
# XXX dependency
#########################################################################

# list(APPEND boards_afcv3_1_LIBRARIES XXX)

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

set(boards_afcv3_1_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/acq_chan.c
    ${CMAKE_CURRENT_SOURCE_DIR}/chips_addr.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ddr3_map.c
    ${CMAKE_CURRENT_SOURCE_DIR}/device_mapping.c
)

# for IDE project generation
source_group("Source Files" FILES
    ${boards_afcv3_1_sources}
)

# Header files
list(APPEND boards_afcv3_1_other_search_dirs "")

#######################################
# BOARD AFCV3_1 header definitions
#######################################

set(boards_afcv3_1_public_headers
    ${common_INCLUDE_DIR}/boards/afcv3_1/acq_chan.h
    ${common_INCLUDE_DIR}/boards/afcv3_1/board.h
    ${common_INCLUDE_DIR}/boards/afcv3_1/chips_addr.h
    ${common_INCLUDE_DIR}/boards/afcv3_1/ddr3_defs.h
    ${common_INCLUDE_DIR}/boards/afcv3_1/ddr3_map.h
    ${common_INCLUDE_DIR}/boards/afcv3_1/mem_layout.h
)

# Get directory names only
list(APPEND boards_afcv3_1_search_dirs "")

foreach(search_dir_header ${boards_afcv3_1_public_headers})
    get_filename_component(search_dir
        ${search_dir_header}
        DIRECTORY
    )

    list(APPEND boards_afcv3_1_search_dirs ${search_dir})
endforeach()

# remove, well, duplicates ...
list(REMOVE_DUPLICATES boards_afcv3_1_search_dirs)

# append to global search list
list(APPEND boards_afcv3_1_other_search_dirs ${boards_afcv3_1_search_dirs})

#######################################
# HEADER Grouping
#######################################

# for IDE project generation
source_group("Header Files" FILES
    ${boards_afcv3_1_public_headers}
)

########################################################################
# Library declaration
########################################################################

# build dynamic library
add_library(boards_afcv3_1 SHARED
    ${boards_afcv3_1_sources}
    ${boards_afcv3_1_public_headers}
)

set_target_properties(boards_afcv3_1 PROPERTIES
  PUBLIC_HEADER "${boards_afcv3_1_public_headers}"
  SOVERSION "1"
  VERSION "${halcs_VERSION}"
  COMPILE_DEFINITIONS "DLL_EXPORT"
  OUTPUT_NAME "${boards_afcv3_1_OUTPUT_BASENAME}"
  PREFIX "lib")

# boards_afcv3_1_other_search_dirs is in quotes as this is a list
# and the first element would be concatenated with the previous
# generator expression.
target_include_directories(boards_afcv3_1
    PRIVATE
    "$<BUILD_INTERFACE:${boards_afcv3_1_other_search_dirs}>"
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>
)

# setting compile options
target_compile_definitions(boards_afcv3_1
    PUBLIC
    ${boards_afcv3_1_definitions}
)

# set debug posfix in case of debug builds
set_target_properties(boards_afcv3_1 PROPERTIES
    DEBUG_POSTFIX
    ${CMAKE_DEBUG_POSTFIX}
    POSITION_INDEPENDENT_CODE
    ON
)

# set compiler flags
target_link_libraries(boards_afcv3_1
    PRIVATE
    boards_compiler_flags
)

########################################################################
# Library dependencies
########################################################################

# set depedency to targets
target_link_libraries(boards_afcv3_1
    PRIVATE
    ${boards_afcv3_1_LIBRARIES}
    ${halcs_LIBRARIES}
)
