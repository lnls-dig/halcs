########################################################################
# Library setup
########################################################################

# library basename
set (sm_io_module_init_OUTPUT_BASENAME "sm_io_module_init"
    CACHE STRING
    "Output sm_io_module_init library base name"
)

#########################################################################
# XXX dependency
#########################################################################

# list(APPEND sm_io_module_init_LIBRARIES XXX)

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

set(sm_io_module_init_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/sm_io_init_core.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sm_io_init_exp.c
)

# for IDE project generation
source_group("Source Files" FILES
    ${sm_io_module_init_sources}
)

# Header files
list(APPEND sm_io_module_init_other_search_dirs "")

#######################################
# SMIO header definitions
#######################################

set(sm_io_module_init_public_headers
    ${CMAKE_CURRENT_SOURCE_DIR}/sm_io_init_core.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sm_io_init_exp.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sm_io_init_codes.h
)

# Get directory names only
list(APPEND sm_io_module_init_search_dirs "")

foreach(search_dir_header ${sm_io_module_init_public_headers})
    get_filename_component(search_dir
        ${search_dir_header}
        DIRECTORY
    )

    list(APPEND sm_io_module_init_search_dirs ${search_dir})
endforeach()

# remove, well, duplicates ...
list(REMOVE_DUPLICATES sm_io_module_init_search_dirs)

# append to global search list
list(APPEND sm_io_module_init_other_search_dirs ${sm_io_module_init_search_dirs})

#######################################
# HEADER Grouping
#######################################

# for IDE project generation
source_group("Header Files" FILES
    ${sm_io_module_init_public_headers}
)

########################################################################
# Library declaration
########################################################################

# build static library
add_library(sm_io_module_init STATIC
    ${sm_io_module_init_sources}
    ${sm_io_module_init_public_headers}
)

set_target_properties(sm_io_module_init PROPERTIES
  PUBLIC_HEADER "${sm_io_module_init_public_headers}"
  OUTPUT_NAME "${sm_io_module_init_OUTPUT_BASENAME}"
  PREFIX "lib")

# sm_io_module_init_other_search_dirs is in quotes as this is a list
# and the first element would be concatenated with the previous
# generator expression.
target_include_directories(sm_io_module_init
    PRIVATE
    "$<BUILD_INTERFACE:${sm_io_module_init_other_search_dirs}>"
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>
)

# set debug posfix in case of debug builds
set_target_properties(sm_io_module_init PROPERTIES
    DEBUG_POSTFIX
    ${CMAKE_DEBUG_POSTFIX}
    POSITION_INDEPENDENT_CODE
    ON
)

# set compiler flags
target_link_libraries(sm_io_module_init
    PRIVATE
    sm_io_compiler_flags
)

########################################################################
# Library dependencies
########################################################################

# set depedency to targets
target_link_libraries(sm_io_module_init
    PRIVATE
    ${sm_io_module_init_LIBRARIES}
    ${halcs_LIBRARIES}
)
