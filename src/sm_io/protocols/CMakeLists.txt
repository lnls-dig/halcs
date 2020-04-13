########################################################################
# Library setup
########################################################################

# library basename
set (smio_protocols_OUTPUT_BASENAME "smio_protocols"
    CACHE STRING
    "Output smio_protocols library base name"
)

#########################################################################
# XXX dependency
#########################################################################

# list(APPEND smio_protocols_LIBRARIES XXX)

########################################################################
# Library compile options
########################################################################

########################################################################
# Sources/Headers definitions
########################################################################

#######################################
# Other sources
#######################################

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ops)
list(APPEND smio_protocols_LIBRARIES smio_protocols_ops)

#######################################
# Our sources
#######################################

# Source files

set(smio_protocols_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/sm_pr.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sm_pr_err.c
)

# for IDE project generation
source_group("Source Files" FILES
    ${smio_protocols_sources}
)

# Header files
list(APPEND smio_protocols_other_search_dirs "")

#######################################
# SMIO header definitions
#######################################

set(smio_protocols_public_headers
    ${common_INCLUDE_DIR}/sm_pr.h
    ${common_INCLUDE_DIR}/sm_pr_err.h
)

# Get directory names only
list(APPEND smio_protocols_search_dirs "")

foreach(search_dir_header ${smio_protocols_public_headers})
    get_filename_component(search_dir
        ${search_dir_header}
        DIRECTORY
    )

    list(APPEND smio_protocols_search_dirs ${search_dir})
endforeach()

# remove, well, duplicates ...
list(REMOVE_DUPLICATES smio_protocols_search_dirs)

# append to global search list
list(APPEND smio_protocols_other_search_dirs ${smio_protocols_search_dirs})

#######################################
# HEADER Grouping
#######################################

# for IDE project generation
source_group("Header Files" FILES
    ${smio_protocols_public_headers}
)

########################################################################
# Library declaration
########################################################################

# build static library
add_library(smio_protocols STATIC
    ${smio_protocols_sources}
    ${smio_protocols_public_headers}
)

set_target_properties(smio_protocols PROPERTIES
  PUBLIC_HEADER "${smio_protocols_public_headers}"
  OUTPUT_NAME "${smio_protocols_OUTPUT_BASENAME}"
  PREFIX "lib")

# smio_protocols_other_search_dirs is in quotes as this is a list
# and the first element would be concatenated with the previous
# generator expression.
target_include_directories(smio_protocols
    PRIVATE
    "$<BUILD_INTERFACE:${smio_protocols_other_search_dirs}>"
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>
)

# set debug posfix in case of debug builds
set_target_properties(smio_protocols PROPERTIES
    DEBUG_POSTFIX
    ${CMAKE_DEBUG_POSTFIX}
    POSITION_INDEPENDENT_CODE
    ON
)

# set compiler flags
target_link_libraries(smio_protocols
    PRIVATE
    smio_compiler_flags
)

########################################################################
# Library dependencies
########################################################################

# set depedency to targets
target_link_libraries(smio_protocols
    PRIVATE
    ${smio_protocols_LIBRARIES}
    ${halcs_LIBRARIES}
)
