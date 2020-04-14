########################################################################
# Library setup
########################################################################

# library basename
set (smio_OUTPUT_BASENAME "smio"
    CACHE STRING
    "Output smio library base name"
)

#########################################################################
# XXX dependency
#########################################################################

# list(APPEND smio_LIBRARIES XXX)

########################################################################
# Library compile options
########################################################################

add_library(smio_compiler_flags INTERFACE)

target_link_libraries(smio_compiler_flags
    INTERFACE
    halcs_compiler_flags
)

########################################################################
# Sources/Headers definitions
########################################################################

#######################################
# Other sources
#######################################

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/chips)
list(APPEND smio_LIBRARIES smio_chips)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/protocols)
list(APPEND smio_LIBRARIES smio_protocols)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/rw_param)
list(APPEND smio_LIBRARIES smio_rw_param)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules)
list(APPEND smio_LIBRARIES smio_modules)

#######################################
# Our sources
#######################################

# Source files

set(smio_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/sm_io.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sm_io_bootstrap.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sm_io_cfg.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sm_io_err.c
)

# for IDE project generation
source_group("Source Files" FILES
    ${smio_sources}
)

# Header files
list(APPEND smio_other_search_dirs "")

#######################################
# SMIO header definitions
#######################################

set(smio_public_headers
)

# Get directory names only
list(APPEND smio_search_dirs "")

foreach(search_dir_header ${smio_public_headers})
    get_filename_component(search_dir
        ${search_dir_header}
        DIRECTORY
    )

    list(APPEND smio_search_dirs ${search_dir})
endforeach()

# remove, well, duplicates ...
list(REMOVE_DUPLICATES smio_search_dirs)

# append to global search list
list(APPEND smio_other_search_dirs ${smio_search_dirs})

#######################################
# HEADER Grouping
#######################################

# for IDE project generation
source_group("Header Files" FILES
    ${smio_public_headers}
)

# append to global search list
list(APPEND smio_other_search_dirs ${common_INCLUDE_DIRS})

########################################################################
# Library declaration
########################################################################

# build static library
add_library(smio STATIC
    ${smio_sources}
    ${smio_public_headers}
)

set_target_properties(smio PROPERTIES
  PUBLIC_HEADER "${smio_public_headers}"
  OUTPUT_NAME "${smio_OUTPUT_BASENAME}"
  PREFIX "lib")

# smio_other_search_dirs is in quotes as this is a list
# and the first element would be concatenated with the previous
# generator expression.
target_include_directories(smio
    PRIVATE
    "$<BUILD_INTERFACE:${smio_other_search_dirs}>"
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>

)

# set debug posfix in case of debug builds
set_target_properties(smio PROPERTIES
    DEBUG_POSTFIX
    ${CMAKE_DEBUG_POSTFIX}
    POSITION_INDEPENDENT_CODE
    ON
)

# set PRIVATE compiler flags. It doesn't really work to link an
# INTERFACE library with PRIVATE scope. See:
# https://gitlab.kitware.com/cmake/cmake/-/issues/15415
target_link_libraries(smio PRIVATE
    $<TARGET_PROPERTY:smio_compiler_flags,INTERFACE_LINK_LIBRARIES>
)
target_include_directories(smio PRIVATE
    $<TARGET_PROPERTY:smio_compiler_flags,INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_options(smio PRIVATE
    $<TARGET_PROPERTY:smio_compiler_flags,INTERFACE_COMPILE_OPTIONS>
)
target_compile_definitions(smio PRIVATE
    $<TARGET_PROPERTY:smio_compiler_flags,INTERFACE_COMPILE_DEFINITIONS>
)
target_sources(smio PRIVATE
    $<TARGET_PROPERTY:smio_compiler_flags,INTERFACE_SOURCES>
)

########################################################################
# Library dependencies
########################################################################

# set depedency to targets
target_link_libraries(smio
    PUBLIC
    ${smio_LIBRARIES}
    ${halcs_LIBRARIES}
)
