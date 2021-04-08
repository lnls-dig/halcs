########################################################################
# Library setup
########################################################################

# library basename
set (boards_common_OUTPUT_BASENAME "boards_common"
    CACHE STRING
    "Output boards_common library base name"
)

#########################################################################
# XXX dependency
#########################################################################

# list(APPEND boards_common_LIBRARIES XXX)

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

set(boards_common_sources
)

# for IDE project generation
source_group("Source Files" FILES
    ${boards_common_sources}
)

# Header files
list(APPEND boards_common_other_search_dirs "")

#######################################
# BOARDS header definitions
#######################################

set(boards_common_public_headers
)

# Get directory names only
list(APPEND boards_common_search_dirs "")

foreach(search_dir_header ${boards_common_public_headers})
    get_filename_component(search_dir
        ${search_dir_header}
        DIRECTORY
    )

    list(APPEND boards_common_search_dirs ${search_dir})
endforeach()

# remove, well, duplicates ...
list(REMOVE_DUPLICATES boards_common_search_dirs)

# append to global search list
list(APPEND boards_common_other_search_dirs ${boards_common_search_dirs})

#######################################
# HEADER Grouping
#######################################

# for IDE project generation
source_group("Header Files" FILES
    ${boards_common_public_headers}
)

########################################################################
# Library declaration
########################################################################

# build static library
add_library(boards_common STATIC
    ${boards_common_sources}
    ${boards_common_public_headers}
)

set_target_properties(boards_common PROPERTIES
  PUBLIC_HEADER "${boards_common_public_headers}"
  OUTPUT_NAME "${boards_common_OUTPUT_BASENAME}"
  PREFIX "lib")

# boards_common_other_search_dirs is in quotes as this is a list
# and the first element would be concatenated with the previous
# generator expression.
target_include_directories(boards_common
    PRIVATE
    "$<BUILD_INTERFACE:${boards_common_other_search_dirs}>"
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>
)

# set debug posfix in case of debug builds
set_target_properties(boards_common PROPERTIES
    DEBUG_POSTFIX
    ${CMAKE_DEBUG_POSTFIX}
    POSITION_INDEPENDENT_CODE
    ON
)

# set compiler flags
target_link_libraries(boards_common
    PRIVATE
    boards_compiler_flags
)

########################################################################
# Library dependencies
########################################################################

# set depedency to targets
target_link_libraries(boards_common
    PRIVATE
    ${boards_common_LIBRARIES}
    ${halcs_LIBRARIES}
)
