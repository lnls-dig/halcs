########################################################################
# Library setup
########################################################################

# library basename
set (smio_modules_OUTPUT_BASENAME "smio_modules"
    CACHE STRING
    "Output smio_modules library base name"
)

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
# XXX dependency
#########################################################################

# list(APPEND smio_modules_LIBRARIES XXX)

########################################################################
# Library compile options
########################################################################

########################################################################
# Sources/Headers definitions
########################################################################

#######################################
# Other sources
#######################################

subdirlist(subdirs ${CMAKE_CURRENT_SOURCE_DIR})

foreach(subdir ${subdirs})
    add_subdirectory(${subdir})
    # modules are expected to generate a library with the name
    # smio_module_<module_name>
    list(APPEND smio_modules_LIBRARIES smio_module_${subdir})
endforeach()

#######################################
# Our sources
#######################################

# Source files

set(smio_modules_sources
)

# for IDE project generation
source_group("Source Files" FILES
    ${smio_modules_sources}
)

# Header files
list(APPEND smio_modules_other_search_dirs "")

#######################################
# SMIO header definitions
#######################################

set(smio_modules_public_headers
)

# Get directory names only
list(APPEND smio_modules_search_dirs "")

foreach(search_dir_header ${smio_modules_public_headers})
    get_filename_component(search_dir
        ${search_dir_header}
        DIRECTORY
    )

    list(APPEND smio_modules_search_dirs ${search_dir})
endforeach()

# remove, well, duplicates ...
list(REMOVE_DUPLICATES smio_modules_search_dirs)

# append to global search list
list(APPEND smio_modules_other_search_dirs ${smio_modules_search_dirs})

#######################################
# HEADER Grouping
#######################################

# for IDE project generation
source_group("Header Files" FILES
    ${smio_modules_public_headers}
)

########################################################################
# Library declaration
########################################################################

# build static library
add_library(smio_modules INTERFACE)

# set compiler flags
target_link_libraries(smio_modules
    INTERFACE
    smio_compiler_flags
)

########################################################################
# Library dependencies
########################################################################

# set depedency to targets
target_link_libraries(smio_modules
    INTERFACE
    ${smio_modules_LIBRARIES}
    ${halcs_LIBRARIES}
)
