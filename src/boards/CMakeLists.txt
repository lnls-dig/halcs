########################################################################
# Library setup
########################################################################

# library basename
set (boards_OUTPUT_BASENAME "boards"
    CACHE STRING
    "Output boards library base name"
)

#########################################################################
# XXX dependency
#########################################################################

# list(APPEND boards_LIBRARIES XXX)

########################################################################
# Library compile options
########################################################################

add_library(boards_compiler_flags INTERFACE)

target_link_libraries(boards_compiler_flags
    INTERFACE
    halcs_compiler_flags
)

########################################################################
# Sources/Headers definitions
########################################################################

#######################################
# Other sources
#######################################

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/common)
list(APPEND boards_LIBRARIES boards_common)

# select board directory to include
string(TOLOWER ${halcs_BOARD_OPT} halcs_BOARD_OPT_LOWER)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/${halcs_BOARD_OPT_LOWER})
list(APPEND boards_LIBRARIES boards_${halcs_BOARD_OPT_LOWER})

#######################################
# Our sources
#######################################

# Source files

set(boards_sources
)

# for IDE project generation
source_group("Source Files" FILES
    ${boards_sources}
)

# Header files
list(APPEND boards_other_search_dirs "")

#######################################
# BOARDS header definitions
#######################################

set(boards_public_headers
)

# Get directory names only
list(APPEND boards_search_dirs "")

foreach(search_dir_header ${boards_public_headers})
    get_filename_component(search_dir
        ${search_dir_header}
        DIRECTORY
    )

    list(APPEND boards_search_dirs ${search_dir})
endforeach()

# remove, well, duplicates ...
list(REMOVE_DUPLICATES boards_search_dirs)

# append to global search list
list(APPEND boards_other_search_dirs ${boards_search_dirs})

#######################################
# HEADER Grouping
#######################################

# for IDE project generation
source_group("Header Files" FILES
    ${boards_public_headers}
)

########################################################################
# Library declaration
########################################################################

# build static library
add_library(boards INTERFACE)

# set compiler flags
target_link_libraries(boards
    INTERFACE
    boards_compiler_flags
)

########################################################################
# Library dependencies
########################################################################

# set depedency to targets
target_link_libraries(boards
    INTERFACE
    ${boards_LIBRARIES}
    ${halcs_LIBRARIES}
)
