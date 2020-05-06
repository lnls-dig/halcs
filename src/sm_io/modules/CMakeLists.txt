########################################################################
# Library setup
########################################################################

# library basename
set (sm_io_modules_OUTPUT_BASENAME "sm_io_modules"
    CACHE STRING
    "Output sm_io_modules library base name"
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

# list(APPEND sm_io_modules_LIBRARIES XXX)

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
    # sm_io_module_<module_name>
    list(APPEND sm_io_modules_LIBRARIES sm_io_module_${subdir})
endforeach()

#######################################
# Our sources
#######################################

# Source files

set(sm_io_modules_sources
)

# for IDE project generation
source_group("Source Files" FILES
    ${sm_io_modules_sources}
)

# Header files
list(APPEND sm_io_modules_other_search_dirs "")

#######################################
# SMIO header definitions
#######################################

set(sm_io_modules_public_headers
)

# Get directory names only
list(APPEND sm_io_modules_search_dirs "")

foreach(search_dir_header ${sm_io_modules_public_headers})
    get_filename_component(search_dir
        ${search_dir_header}
        DIRECTORY
    )

    list(APPEND sm_io_modules_search_dirs ${search_dir})
endforeach()

# remove, well, duplicates ...
list(REMOVE_DUPLICATES sm_io_modules_search_dirs)

# append to global search list
list(APPEND sm_io_modules_other_search_dirs ${sm_io_modules_search_dirs})

#######################################
# HEADER Grouping
#######################################

# for IDE project generation
source_group("Header Files" FILES
    ${sm_io_modules_public_headers}
)

########################################################################
# Library declaration
########################################################################

# build static library
add_library(sm_io_modules INTERFACE)

# set compiler flags
target_link_libraries(sm_io_modules
    INTERFACE
    sm_io_compiler_flags
)

########################################################################
# Library dependencies
########################################################################

# set depedency to targets
target_link_libraries(sm_io_modules
    INTERFACE
    ${halcs_LIBRARIES}
)

########################################################################
# Module dependencies
########################################################################

# This techinique is required so we can link to all of the module unconditionally,
# as linker will add the symbol to the final binary if it has an unresolved symbol,
# but for modules this is always the case. This was extracted from:
# https://stackoverflow.com/questions/53071878/using-whole-archive-linker-option-with-cmake-and-libraries-with-other-library

target_link_libraries(sm_io_modules
    INTERFACE
    -Wl,--whole-archive
    ${sm_io_modules_LIBRARIES}
    -Wl,--no-whole-archive
)
