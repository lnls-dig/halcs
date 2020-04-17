########################################################################
# Binary setup
########################################################################

include(FindThreads)

# halcsd basename
set (halcsd_OUTPUT_BASENAME "halcsd"
    CACHE STRING
    "Output halcsd binary base name"
)

# halcsd_cfg basename
set (halcsd_cfg_OUTPUT_BASENAME "halcsd_cfg"
    CACHE STRING
    "Output halcsd_cfg binary base name"
)

#########################################################################
# library dependencies
#########################################################################

list(APPEND halcsd_LIBRARIES
    ${apps_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)
list(APPEND halcsd_cfg_LIBRARIES
    ${apps_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)

########################################################################
# User compile-time options
########################################################################

#######################################
# HALCSD
#######################################

# which AFE version we want.

set(halcsd_AFE_RFFE_TYPE_OPT "2" CACHE STRING
    "AFE RFFE type")
set(halcsd_AFE_RFFE_TYPE_VALUES
    "1;2"
)

set_property(CACHE halcsd_AFE_RFFE_TYPE_OPT PROPERTY STRINGS
    ${halcsd_AFE_RFFE_TYPE_VALUES}
)

message(STATUS
    "Setting compile-time option halcsd_AFE_RFFE_TYPE='${halcsd_AFE_RFFE_TYPE_OPT}'")

# setting compile options
string(APPEND halcsd_board_definitions "")

# setting macros
if(halcsd_AFE_RFFE_TYPE_OPT STREQUAL 1)
    list(APPEND halcsd_board_definitions
        __AFE_RFFE_V1__
    )
elseif(halcsd_AFE_RFFE_TYPE_OPT STREQUAL 2)
    list(APPEND halcsd_board_definitions
        __AFE_RFFE_V2__
    )
endif()

# if we want HALCSD_CFG or not.

option(BUILD_halcsd_cfg "Whether or not to build halcsd_cfg" OFF)

if(BUILD_halcsd_cfg)
    list(APPEND apps_halcsd "halcsd_cfg")
endif()

option(BUILD_halcsd "Whether or not to build halcsd" ON)

if(BUILD_halcsd)
    list(APPEND apps_halcsd "halcsd")

    if(BUILD_halcsd_cfg)
        list(APPEND halcsd_board_definitions
            __WITH_APP_CFG__
        )
    endif()

endif()

########################################################################
# Library compile options
########################################################################

add_library(halcsd_compiler_flags INTERFACE)

target_link_libraries(halcsd_compiler_flags
    INTERFACE
    apps_compiler_flags
)

add_library(halcsd_cfg_compiler_flags INTERFACE)

target_link_libraries(halcsd_cfg_compiler_flags
    INTERFACE
    apps_compiler_flags
)

########################################################################
# Sources/Headers definitions
########################################################################

########################################################################
# HALCSD
########################################################################

#######################################
# sources
#######################################

set(halcsd_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/halcsd.c
)

# for IDE project generation
source_group("Source Files" FILES
    ${halcsd_sources}
)

# Header files
list(APPEND halcsd_other_search_dirs "")

#######################################
# header definitions
#######################################

set(halcsd_public_headers
    ${common_INCLUDE_DIR}/halcs_server.h
)

# Get directory names only
list(APPEND halcsd_search_dirs "")

foreach(search_dir_header ${halcsd_public_headers})
    get_filename_component(search_dir
        ${search_dir_header}
        DIRECTORY
    )

    list(APPEND halcsd_search_dirs ${search_dir})
endforeach()

# remove, well, duplicates ...
list(REMOVE_DUPLICATES halcsd_search_dirs)

# append to global search list
list(APPEND halcsd_other_search_dirs ${halcsd_search_dirs})

#######################################
# LIBGEN header
#######################################

# find libgen.h header. It's part of libc.
find_path (
    _libgen_INCLUDE_DIRS
    NAMES libgen.h
)

# append to global search list
list(APPEND halcsd_other_search_dirs ${_libgen_INCLUDE_DIRS})

#######################################
# HEADER Grouping
#######################################

# for IDE project generation
source_group("Header Files" FILES
    ${halcsd_public_headers}
)

########################################################################
# HALCSD_CFG
########################################################################

#######################################
# sources
#######################################

# Source files

set(halcsd_cfg_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/halcsd_cfg.c
)

# for IDE project generation
source_group("Source Files" FILES
    ${halcsd_cfg_sources}
)

# Header files
list(APPEND halcsd_cfg_other_search_dirs "")

#######################################
# header definitions
#######################################

set(halcsd_cfg_public_headers
    ${common_INCLUDE_DIR}/halcs_server.h
)

# Get directory names only
list(APPEND halcsd_cfg_search_dirs "")

foreach(search_dir_header ${halcsd_cfg_public_headers})
    get_filename_component(search_dir
        ${search_dir_header}
        DIRECTORY
    )

    list(APPEND halcsd_cfg_search_dirs ${search_dir})
endforeach()

# remove, well, duplicates ...
list(REMOVE_DUPLICATES halcsd_cfg_search_dirs)

# append to global search list
list(APPEND halcsd_cfg_other_search_dirs ${halcsd_cfg_search_dirs})

#######################################
# HEADER Grouping
#######################################

# for IDE project generation
source_group("Header Files" FILES
    ${halcsd_cfg_public_headers}
)

########################################################################
# Binaries declaration
########################################################################

foreach(bin_target ${apps_halcsd})
    # build static library
    add_executable(${bin_target}
        ${${bin_target}_sources}
        ${${bin_target}_public_headers}
    )

    # ${bin_target}_other_search_dirs is in quotes as this is a list
    # and the first element would be concatenated with the previous
    # generator expression.
    target_include_directories(${bin_target}
        PRIVATE
        "$<BUILD_INTERFACE:${${bin_target}_other_search_dirs}>"
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:include>

    )

    target_compile_definitions(${bin_target} PRIVATE
        ${${bin_target}_board_definitions}
    )

    # set debug posfix in case of debug builds
    set_target_properties(${bin_target} PROPERTIES
        DEBUG_POSTFIX
        ${CMAKE_DEBUG_POSTFIX}
        POSITION_INDEPENDENT_CODE
        ON
    )

    # set PRIVATE compiler flags. It doesn't really work to link an
    # INTERFACE library with PRIVATE scope. See:
    # https://gitlab.kitware.com/cmake/cmake/-/issues/15415
    target_link_libraries(${bin_target} PRIVATE
        $<TARGET_PROPERTY:${bin_target}_compiler_flags,INTERFACE_LINK_LIBRARIES>
    )
    target_include_directories(${bin_target} PRIVATE
        $<TARGET_PROPERTY:${bin_target}_compiler_flags,INTERFACE_INCLUDE_DIRECTORIES>
    )
    target_compile_options(${bin_target} PRIVATE
        $<TARGET_PROPERTY:${bin_target}_compiler_flags,INTERFACE_COMPILE_OPTIONS>
    )
    target_compile_definitions(${bin_target} PRIVATE
        $<TARGET_PROPERTY:${bin_target}_compiler_flags,INTERFACE_COMPILE_DEFINITIONS>
    )
    target_sources(${bin_target} PRIVATE
        $<TARGET_PROPERTY:${bin_target}_compiler_flags,INTERFACE_SOURCES>
    )

    ########################################################################
    # Library dependencies
    ########################################################################

    # set depedency to targets
    target_link_libraries(${bin_target}
        PRIVATE
        ${${bin_target}_LIBRARIES}
        ${halcs_LIBRARIES}
    )

    # install binary
    install(TARGETS ${bin_target}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT Binaries)
endforeach()

# set list to global, as linker options are passed through this
set(apps_halcsd ${apps_halcsd} PARENT_SCOPE)
