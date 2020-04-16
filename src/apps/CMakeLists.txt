########################################################################
# Apps setup
########################################################################

# get all directories under a directory
macro(subdirlist result curdir)
  file(GLOB children RELATIVE ${curdir} ${curdir}/*)
  set(dirlist "")
  foreach(child ${children})
    if(IS_DIRECTORY ${curdir}/${child})
      list(APPEND dirlist ${child})
    endif()
  endforeach()
  set(${result} ${dirlist})
endmacro()

#########################################################################
# LIbrary dependencies
#########################################################################

list(APPEND apps_LIBRARIES
    boards
    dev_io
    msg
    revision
    sm_io
    sm_io_table
)

########################################################################
# Library compile options
########################################################################

add_library(apps_compiler_flags INTERFACE)

target_link_libraries(apps_compiler_flags
    INTERFACE
    halcs_compiler_flags
)

########################################################################
# Sources/Headers definitions
########################################################################

#######################################
# Other sources
#######################################

subdirlist(subdirs ${CMAKE_CURRENT_SOURCE_DIR})

foreach(subdir ${subdirs})
    add_subdirectory(${subdir})
    list(APPEND apps_targets ${apps_${subdir}})
endforeach()

message(STATUS "apps_targets: ${apps_targets}")

foreach(app_target ${apps_targets})
    # add linker options here as we can't use the more modern
    # target_link_options() that appeared only on CMake 3.13
    set_target_properties(${app_target}
        PROPERTIES LINK_FLAGS "-Wl,-T${halcs_LINKER_DIR}/halcs.ld"
    )
endforeach()

#######################################
# Our sources
#######################################

# Source files

set(apps_sources
)

# for IDE project generation
source_group("Source Files" FILES
    ${apps_sources}
)

# Header files
list(APPEND apps_other_search_dirs "")

#######################################
# apps header definitions
#######################################

set(apps_public_headers
)

# Get directory names only
list(APPEND apps_search_dirs "")

foreach(search_dir_header ${apps_public_headers})
    get_filename_component(search_dir
        ${search_dir_header}
        DIRECTORY
    )

    list(APPEND apps_search_dirs ${search_dir})
endforeach()

# remove, well, duplicates ...
list(REMOVE_DUPLICATES apps_search_dirs)

# append to global search list
list(APPEND apps_other_search_dirs ${apps_search_dirs})

#######################################
# HEADER Grouping
#######################################

# for IDE project generation
source_group("Header Files" FILES
    ${apps_public_headers}
)

########################################################################
# Binaries properties declaration
########################################################################

#foreach(app_target ${apps_targets})
#    # set PRIVATE compiler flags. It doesn't really work to link an
#    # INTERFACE library with PRIVATE scope. See:
#    # https://gitlab.kitware.com/cmake/cmake/-/issues/15415
#    target_link_libraries(${app_target} PRIVATE
#        $<TARGET_PROPERTY:apps_compiler_flags,INTERFACE_LINK_LIBRARIES>
#    )
#    target_include_directories(${app_target} PRIVATE
#        $<TARGET_PROPERTY:apps_compiler_flags,INTERFACE_INCLUDE_DIRECTORIES>
#    )
#    target_compile_options(${app_target} PRIVATE
#        $<TARGET_PROPERTY:apps_compiler_flags,INTERFACE_COMPILE_OPTIONS>
#    )
#    target_compile_definitions(${app_target} PRIVATE
#        $<TARGET_PROPERTY:apps_compiler_flags,INTERFACE_COMPILE_DEFINITIONS>
#    )
#    target_sources(${app_target} PRIVATE
#        $<TARGET_PROPERTY:apps_compiler_flags,INTERFACE_SOURCES>
#    )
#endforeach()

########################################################################
# Binary dependencies
########################################################################

#foreach(app_target ${apps_targets})
#    # set depedency to targets
#    target_link_libraries(${app_target}
#        PRIVATE
#        ${apps_LIBRARIES}
#        ${halcs_LIBRARIES}
#    )
#endforeach()
