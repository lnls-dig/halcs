########################################################################
# Library setup
########################################################################

# add git introspection functions
include(GetGitRevisionDescription)

# library basename
set (revision_OUTPUT_BASENAME "revision"
    CACHE STRING
    "Output revision library base name"
)

#########################################################################
# XXX dependency
#########################################################################

# list(APPEND revision_LIBRARIES XXX)

########################################################################
# Library compile options
########################################################################

add_library(revision_compiler_flags INTERFACE)

target_link_libraries(revision_compiler_flags
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

set(revision_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/revision.c
)

# for IDE project generation
source_group("Source Files" FILES
    ${revision_sources}
)

# Header files
list(APPEND revision_other_search_dirs "")

#######################################
# revision header definitions
#######################################

set(revision_public_headers
    ${common_INCLUDE_DIR}/revision.h
)

# Get directory names only
list(APPEND revision_search_dirs "")

foreach(search_dir_header ${revision_public_headers})
    get_filename_component(search_dir
        ${search_dir_header}
        DIRECTORY
    )

    list(APPEND revision_search_dirs ${search_dir})
endforeach()

# remove, well, duplicates ...
list(REMOVE_DUPLICATES revision_search_dirs)

# append to global search list
list(APPEND revision_other_search_dirs ${revision_search_dirs})

#######################################
# HEADER Grouping
#######################################

# for IDE project generation
source_group("Header Files" FILES
    ${revision_public_headers}
)

# append to global search list
list(APPEND revision_other_search_dirs ${common_INCLUDE_DIRS})

########################################################################
# Library declaration
########################################################################

# build static library
add_library(revision STATIC
    ${revision_sources}
    ${revision_public_headers}
)

set_target_properties(revision PROPERTIES
  PUBLIC_HEADER "${revision_public_headers}"
  OUTPUT_NAME "${revision_OUTPUT_BASENAME}"
  PREFIX "lib")

# set Git macros
git_describe(GIT_TAG --always --tags)
git_get_config(GIT_NAME "user.name")
git_get_config(GIT_EMAIL "user.email")

# revision_other_search_dirs is in quotes as this is a list
# and the first element would be concatenated with the previous
# generator expression.
target_include_directories(revision
    PRIVATE
    "$<BUILD_INTERFACE:${revision_other_search_dirs}>"
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>
)

# set debug posfix in case of debug builds
set_target_properties(revision PROPERTIES
    DEBUG_POSTFIX
    ${CMAKE_DEBUG_POSTFIX}
    POSITION_INDEPENDENT_CODE
    ON
)

# add board compile definitions
target_compile_definitions(revision PRIVATE
    GIT_REVISION=\"${GIT_TAG}\"
    GIT_USER_NAME=\"${GIT_NAME}\"
    GIT_USER_EMAIL=\"${GIT_EMAIL}\"
)

# set PRIVATE compiler flags. It doesn't really work to link an
# INTERFACE library with PRIVATE scope. See:
# https://gitlab.kitware.com/cmake/cmake/-/issues/15415
target_link_libraries(revision PRIVATE
    $<TARGET_PROPERTY:revision_compiler_flags,INTERFACE_LINK_LIBRARIES>
)
target_include_directories(revision PRIVATE
    $<TARGET_PROPERTY:revision_compiler_flags,INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_options(revision PRIVATE
    $<TARGET_PROPERTY:revision_compiler_flags,INTERFACE_COMPILE_OPTIONS>
)
target_compile_definitions(revision PRIVATE
    $<TARGET_PROPERTY:revision_compiler_flags,INTERFACE_COMPILE_DEFINITIONS>
)
target_sources(revision PRIVATE
    $<TARGET_PROPERTY:revision_compiler_flags,INTERFACE_SOURCES>
)

########################################################################
# Library dependencies
########################################################################

# set depedency to targets
target_link_libraries(revision
    PRIVATE
    ${revision_LIBRARIES}
    ${halcs_LIBRARIES}
)
