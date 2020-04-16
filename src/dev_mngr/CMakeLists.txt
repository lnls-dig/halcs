########################################################################
# Binary setup
########################################################################

include(FindThreads)

# library basename
set (dev_mngr_OUTPUT_BASENAME "dev_mngr"
    CACHE STRING
    "Output dev_mngr base name"
)

#########################################################################
# Libraries dependencies
#########################################################################

# threading library to use
list(APPEND dev_mngr_LIBRARIES
    ${CMAKE_THREAD_LIBS_INIT}
    dev_io
)

########################################################################
# Library compile options
########################################################################

add_library(dev_mngr_compiler_flags INTERFACE)

target_link_libraries(dev_mngr_compiler_flags
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

set(dev_mngr_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/dev_mngr.c
    ${CMAKE_CURRENT_SOURCE_DIR}/dev_mngr_core.c
    ${CMAKE_CURRENT_SOURCE_DIR}/dev_mngr_dev_info.c
    ${CMAKE_CURRENT_SOURCE_DIR}/dev_mngr_err.c
)

# for IDE project generation
source_group("Source Files" FILES
    ${dev_mngr_sources}
)

# Header files
list(APPEND dev_mngr_other_search_dirs "")

#######################################
# dev_mngr header definitions
#######################################

set(dev_mngr_public_headers
    ${common_INCLUDE_DIR}/dev_mngr.h
    ${common_INCLUDE_DIR}/dev_mngr_core.h
    ${common_INCLUDE_DIR}/dev_mngr_dev_info.h
    ${common_INCLUDE_DIR}/dev_mngr_err.h
)

# Get directory names only
list(APPEND dev_mngr_search_dirs "")

foreach(search_dir_header ${dev_mngr_public_headers})
    get_filename_component(search_dir
        ${search_dir_header}
        DIRECTORY
    )

    list(APPEND dev_mngr_search_dirs ${search_dir})
endforeach()

# remove, well, duplicates ...
list(REMOVE_DUPLICATES dev_mngr_search_dirs)

# append to global search list
list(APPEND dev_mngr_other_search_dirs ${dev_mngr_search_dirs})

#######################################
# HEADER Grouping
#######################################

# for IDE project generation
source_group("Header Files" FILES
    ${dev_mngr_public_headers}
)

# append to global search list
list(APPEND dev_mngr_other_search_dirs ${common_INCLUDE_DIRS})

########################################################################
# Binaries declaration
########################################################################

option(BUILD_dev_mngr "Whether or not to build dev_mngr" ON)

if(BUILD_dev_mngr)
    # build static library
    add_executable(dev_mngr
        ${dev_mngr_sources}
        ${dev_mngr_public_headers}
    )

    # dev_mngr_other_search_dirs is in quotes as this is a list
    # and the first element would be concatenated with the previous
    # generator expression.
    target_include_directories(dev_mngr
        PRIVATE
        "$<BUILD_INTERFACE:${dev_mngr_other_search_dirs}>"
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:include>

    )

    # set debug posfix in case of debug builds
    set_target_properties(dev_mngr PROPERTIES
        DEBUG_POSTFIX
        ${CMAKE_DEBUG_POSTFIX}
        POSITION_INDEPENDENT_CODE
        ON
    )

    # set PRIVATE compiler flags. It doesn't really work to link an
    # INTERFACE library with PRIVATE scope. See:
    # https://gitlab.kitware.com/cmake/cmake/-/issues/15415
    target_link_libraries(dev_mngr PRIVATE
        $<TARGET_PROPERTY:dev_mngr_compiler_flags,INTERFACE_LINK_LIBRARIES>
    )
    target_include_directories(dev_mngr PRIVATE
        $<TARGET_PROPERTY:dev_mngr_compiler_flags,INTERFACE_INCLUDE_DIRECTORIES>
    )
    target_compile_options(dev_mngr PRIVATE
        $<TARGET_PROPERTY:dev_mngr_compiler_flags,INTERFACE_COMPILE_OPTIONS>
    )
    target_compile_definitions(dev_mngr PRIVATE
        $<TARGET_PROPERTY:dev_mngr_compiler_flags,INTERFACE_COMPILE_DEFINITIONS>
    )
    target_sources(dev_mngr PRIVATE
        $<TARGET_PROPERTY:dev_mngr_compiler_flags,INTERFACE_SOURCES>
    )

    ########################################################################
    # Library dependencies
    ########################################################################

    # set depedency to targets
    target_link_libraries(dev_mngr
        PRIVATE
        ${dev_mngr_LIBRARIES}
        ${halcs_LIBRARIES}
    )

    # install binary
    install(TARGETS dev_mngr
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT Binaries)
endif()
