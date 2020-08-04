########################################################################
# Library setup
########################################################################

# library basename
set (dev_io_OUTPUT_BASENAME "dev_io"
    CACHE STRING
    "Output dev_io library base name"
)

#########################################################################
# XXX dependency
#########################################################################

# list(APPEND dev_io_LIBRARIES XXX)

########################################################################
# Library compile options
########################################################################

add_library(dev_io_compiler_flags INTERFACE)

target_link_libraries(dev_io_compiler_flags
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

set(dev_io_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/dev_io_proto.c
    ${CMAKE_CURRENT_SOURCE_DIR}/dev_io_core.c
    ${CMAKE_CURRENT_SOURCE_DIR}/dev_io_err.c
    ${CMAKE_CURRENT_SOURCE_DIR}/dev_io_utils.c
)

# for IDE project generation
source_group("Source Files" FILES
    ${dev_io_sources}
)

# Header files
list(APPEND dev_io_other_search_dirs "")

#######################################
# dev_io header definitions
#######################################

set(dev_io_public_headers
    ${common_INCLUDE_DIR}/dev_io_proto.h
    ${common_INCLUDE_DIR}/dev_io_core.h
    ${common_INCLUDE_DIR}/dev_io_err.h
    ${common_INCLUDE_DIR}/dev_io_utils.h
)

# Get directory names only
list(APPEND dev_io_search_dirs "")

foreach(search_dir_header ${dev_io_public_headers})
    get_filename_component(search_dir
        ${search_dir_header}
        DIRECTORY
    )

    list(APPEND dev_io_search_dirs ${search_dir})
endforeach()

# remove, well, duplicates ...
list(REMOVE_DUPLICATES dev_io_search_dirs)

# append to global search list
list(APPEND dev_io_other_search_dirs ${dev_io_search_dirs})

#######################################
# HEADER Grouping
#######################################

# for IDE project generation
source_group("Header Files" FILES
    ${dev_io_public_headers}
)

# append to global search list
list(APPEND dev_io_other_search_dirs ${common_INCLUDE_DIRS})

########################################################################
# Library declaration
########################################################################

# build static library
add_library(dev_io STATIC
    ${dev_io_sources}
    ${dev_io_public_headers}
)

set_target_properties(dev_io PROPERTIES
  PUBLIC_HEADER "${dev_io_public_headers}"
  OUTPUT_NAME "${dev_io_OUTPUT_BASENAME}"
  PREFIX "lib")

# dev_io_other_search_dirs is in quotes as this is a list
# and the first element would be concatenated with the previous
# generator expression.
target_include_directories(dev_io
    PRIVATE
    "$<BUILD_INTERFACE:${dev_io_other_search_dirs}>"
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>

)

# set debug posfix in case of debug builds
set_target_properties(dev_io PROPERTIES
    DEBUG_POSTFIX
    ${CMAKE_DEBUG_POSTFIX}
    POSITION_INDEPENDENT_CODE
    ON
)

# set PRIVATE compiler flags. It doesn't really work to link an
# INTERFACE library with PRIVATE scope. See:
# https://gitlab.kitware.com/cmake/cmake/-/issues/15415
target_link_libraries(dev_io PRIVATE
    $<TARGET_PROPERTY:dev_io_compiler_flags,INTERFACE_LINK_LIBRARIES>
)
target_include_directories(dev_io PRIVATE
    $<TARGET_PROPERTY:dev_io_compiler_flags,INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_options(dev_io PRIVATE
    $<TARGET_PROPERTY:dev_io_compiler_flags,INTERFACE_COMPILE_OPTIONS>
)
target_compile_definitions(dev_io PRIVATE
    $<TARGET_PROPERTY:dev_io_compiler_flags,INTERFACE_COMPILE_DEFINITIONS>
)
target_compile_features(dev_io PRIVATE
    $<TARGET_PROPERTY:dev_io_compiler_flags,INTERFACE_COMPILE_FEATURES>
)
target_sources(dev_io PRIVATE
    $<TARGET_PROPERTY:dev_io_compiler_flags,INTERFACE_SOURCES>
)

########################################################################
# Library dependencies
########################################################################

# set depedency to targets
target_link_libraries(dev_io
    PRIVATE
    ${dev_io_LIBRARIES}
    ${halcs_LIBRARIES}
)
