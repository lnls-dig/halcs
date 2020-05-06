########################################################################
# Library setup
########################################################################

# library basename
set (sm_io_OUTPUT_BASENAME "sm_io"
    CACHE STRING
    "Output sm_io library base name"
)

#########################################################################
# XXX dependency
#########################################################################

# list(APPEND sm_io_LIBRARIES XXX)

########################################################################
# Library compile options
########################################################################

add_library(sm_io_compiler_flags INTERFACE)

target_link_libraries(sm_io_compiler_flags
    INTERFACE
    halcs_compiler_flags
)

########################################################################
# Sources/Headers definitions
########################################################################

#######################################
# Other sources
#######################################

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules)
list(APPEND sm_io_LIBRARIES sm_io_modules)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/chips)
list(APPEND sm_io_LIBRARIES sm_io_chips)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/protocols)
list(APPEND sm_io_LIBRARIES sm_io_protocols)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/rw_param)
list(APPEND sm_io_LIBRARIES sm_io_rw_param)

#######################################
# Our sources
#######################################

# Source files

set(sm_io_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/sm_io.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sm_io_bootstrap.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sm_io_cfg.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sm_io_err.c
)

# for IDE project generation
source_group("Source Files" FILES
    ${sm_io_sources}
)

# Header files
list(APPEND sm_io_other_search_dirs "")

#######################################
# sm_io header definitions
#######################################

set(sm_io_public_headers
)

# Get directory names only
list(APPEND sm_io_search_dirs "")

foreach(search_dir_header ${sm_io_public_headers})
    get_filename_component(search_dir
        ${search_dir_header}
        DIRECTORY
    )

    list(APPEND sm_io_search_dirs ${search_dir})
endforeach()

# remove, well, duplicates ...
list(REMOVE_DUPLICATES sm_io_search_dirs)

# append to global search list
list(APPEND sm_io_other_search_dirs ${sm_io_search_dirs})

#######################################
# HEADER Grouping
#######################################

# for IDE project generation
source_group("Header Files" FILES
    ${sm_io_public_headers}
)

# append to global search list
list(APPEND sm_io_other_search_dirs ${common_INCLUDE_DIRS})

########################################################################
# Library declaration
########################################################################

# build static library
add_library(sm_io STATIC
    ${sm_io_sources}
    ${sm_io_public_headers}
)

set_target_properties(sm_io PROPERTIES
  PUBLIC_HEADER "${sm_io_public_headers}"
  OUTPUT_NAME "${sm_io_OUTPUT_BASENAME}"
  PREFIX "lib")

# sm_io_other_search_dirs is in quotes as this is a list
# and the first element would be concatenated with the previous
# generator expression.
target_include_directories(sm_io
    PRIVATE
    "$<BUILD_INTERFACE:${sm_io_other_search_dirs}>"
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>

)

# set debug posfix in case of debug builds
set_target_properties(sm_io PROPERTIES
    DEBUG_POSTFIX
    ${CMAKE_DEBUG_POSTFIX}
    POSITION_INDEPENDENT_CODE
    ON
)

# set PRIVATE compiler flags. It doesn't really work to link an
# INTERFACE library with PRIVATE scope. See:
# https://gitlab.kitware.com/cmake/cmake/-/issues/15415
target_link_libraries(sm_io PRIVATE
    $<TARGET_PROPERTY:sm_io_compiler_flags,INTERFACE_LINK_LIBRARIES>
)
target_include_directories(sm_io PRIVATE
    $<TARGET_PROPERTY:sm_io_compiler_flags,INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_options(sm_io PRIVATE
    $<TARGET_PROPERTY:sm_io_compiler_flags,INTERFACE_COMPILE_OPTIONS>
)
target_compile_definitions(sm_io PRIVATE
    $<TARGET_PROPERTY:sm_io_compiler_flags,INTERFACE_COMPILE_DEFINITIONS>
)
target_compile_features(sm_io PRIVATE
    $<TARGET_PROPERTY:sm_io_compiler_flags,INTERFACE_COMPILE_FEATURES>
)
target_sources(sm_io PRIVATE
    $<TARGET_PROPERTY:sm_io_compiler_flags,INTERFACE_SOURCES>
)

########################################################################
# Library dependencies
########################################################################

# set depedency to targets
target_link_libraries(sm_io
    PRIVATE
    ${sm_io_LIBRARIES}
    ${halcs_LIBRARIES}
)
