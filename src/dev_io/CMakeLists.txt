########################################################################
# Library setup
########################################################################

# library basename
set (devio_OUTPUT_BASENAME "devio"
    CACHE STRING
    "Output devio library base name"
)

#########################################################################
# XXX dependency
#########################################################################

# list(APPEND devio_LIBRARIES XXX)

########################################################################
# Library compile options
########################################################################

add_library(devio_compiler_flags INTERFACE)

target_link_libraries(devio_compiler_flags
    INTERFACE
    halcs_compiler_flags
)

########################################################################
# Sources/Headers definitions
########################################################################

#######################################
# Our sources
#######################################

# Source files

set(devio_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/dev_io_core.c
    ${CMAKE_CURRENT_SOURCE_DIR}/dev_io_err.c
    ${CMAKE_CURRENT_SOURCE_DIR}/dev_io_utils.c
)

# for IDE project generation
source_group("Source Files" FILES
    ${devio_sources}
)

# Header files
list(APPEND devio_other_search_dirs "")

#######################################
# devio header definitions
#######################################

set(devio_public_headers
    ${common_INCLUDE_DIR}/dev_io_core.h
    ${common_INCLUDE_DIR}/dev_io_err.h
    ${common_INCLUDE_DIR}/dev_io_utils.h
)

# Get directory names only
list(APPEND devio_search_dirs "")

foreach(search_dir_header ${devio_public_headers})
    get_filename_component(search_dir
        ${search_dir_header}
        DIRECTORY
    )

    list(APPEND devio_search_dirs ${search_dir})
endforeach()

# remove, well, duplicates ...
list(REMOVE_DUPLICATES devio_search_dirs)

# append to global search list
list(APPEND devio_other_search_dirs ${devio_search_dirs})

#######################################
# HEADER Grouping
#######################################

# for IDE project generation
source_group("Header Files" FILES
    ${devio_public_headers}
)

# append to global search list
list(APPEND devio_other_search_dirs ${common_INCLUDE_DIRS})

########################################################################
# Library declaration
########################################################################

# build static library
add_library(devio STATIC
    ${devio_sources}
    ${devio_public_headers}
)

set_target_properties(devio PROPERTIES
  PUBLIC_HEADER "${devio_public_headers}"
  OUTPUT_NAME "${devio_OUTPUT_BASENAME}"
  PREFIX "lib")

# devio_other_search_dirs is in quotes as this is a list
# and the first element would be concatenated with the previous
# generator expression.
target_include_directories(devio
    PRIVATE
    "$<BUILD_INTERFACE:${devio_other_search_dirs}>"
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>

)

# set debug posfix in case of debug builds
set_target_properties(devio PROPERTIES
    DEBUG_POSTFIX
    ${CMAKE_DEBUG_POSTFIX}
    POSITION_INDEPENDENT_CODE
    ON
)

# set PRIVATE compiler flags. It doesn't really work to link an
# INTERFACE library with PRIVATE scope. See:
# https://gitlab.kitware.com/cmake/cmake/-/issues/15415
target_link_libraries(devio PRIVATE
    $<TARGET_PROPERTY:devio_compiler_flags,INTERFACE_LINK_LIBRARIES>
)
target_include_directories(devio PRIVATE
    $<TARGET_PROPERTY:devio_compiler_flags,INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_options(devio PRIVATE
    $<TARGET_PROPERTY:devio_compiler_flags,INTERFACE_COMPILE_OPTIONS>
)
target_compile_definitions(devio PRIVATE
    $<TARGET_PROPERTY:devio_compiler_flags,INTERFACE_COMPILE_DEFINITIONS>
)
target_sources(devio PRIVATE
    $<TARGET_PROPERTY:devio_compiler_flags,INTERFACE_SOURCES>
)

########################################################################
# Library dependencies
########################################################################

# set depedency to targets
target_link_libraries(devio
    PRIVATE
    ${devio_LIBRARIES}
    ${halcs_LIBRARIES}
)
