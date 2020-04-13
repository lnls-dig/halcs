########################################################################
# Library setup
########################################################################

# library basename
set (smio_module_acq_pm_OUTPUT_BASENAME "smio_module_acq_pm"
    CACHE STRING
    "Output smio_module_acq_pm library base name"
)

#########################################################################
# XXX dependency
#########################################################################

# list(APPEND smio_module_acq_pm_LIBRARIES XXX)

########################################################################
# Library compile options
########################################################################

########################################################################
# Sources/Headers definitions
########################################################################

#######################################
# Our sources
#######################################

# Source files

set(smio_module_acq_pm_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/sm_io_acq_pm_core.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sm_io_acq_pm_exp.c
)

# for IDE project generation
source_group("Source Files" FILES
    ${smio_module_acq_pm_sources}
)

# Header files
list(APPEND smio_module_acq_pm_other_search_dirs "")

#######################################
# SMIO header definitions
#######################################

set(smio_module_acq_pm_public_headers
    ${CMAKE_CURRENT_SOURCE_DIR}/sm_io_acq_pm_core.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sm_io_acq_pm_exp.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sm_io_acq_pm_codes.h
)

# Get directory names only
list(APPEND smio_module_acq_pm_search_dirs "")

foreach(search_dir_header ${smio_module_acq_pm_public_headers})
    get_filename_component(search_dir
        ${search_dir_header}
        DIRECTORY
    )

    list(APPEND smio_module_acq_pm_search_dirs ${search_dir})
endforeach()

# remove, well, duplicates ...
list(REMOVE_DUPLICATES smio_module_acq_pm_search_dirs)

# append to global search list
list(APPEND smio_module_acq_pm_other_search_dirs ${smio_module_acq_pm_search_dirs})

#######################################
# HEADER Grouping
#######################################

# for IDE project generation
source_group("Header Files" FILES
    ${smio_module_acq_pm_public_headers}
)

########################################################################
# Library declaration
########################################################################

# build static library
add_library(smio_module_acq_pm STATIC
    ${smio_module_acq_pm_sources}
    ${smio_module_acq_pm_public_headers}
)

set_target_properties(smio_module_acq_pm PROPERTIES
  PUBLIC_HEADER "${smio_module_acq_pm_public_headers}"
  OUTPUT_NAME "${smio_module_acq_pm_OUTPUT_BASENAME}"
  PREFIX "lib")

# smio_module_acq_pm_other_search_dirs is in quotes as this is a list
# and the first element would be concatenated with the previous
# generator expression.
target_include_directories(smio_module_acq_pm
    PRIVATE
    "$<BUILD_INTERFACE:${smio_module_acq_pm_other_search_dirs}>"
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>
)

# set debug posfix in case of debug builds
set_target_properties(smio_module_acq_pm PROPERTIES
    DEBUG_POSTFIX
    ${CMAKE_DEBUG_POSTFIX}
    POSITION_INDEPENDENT_CODE
    ON
)

# set compiler flags
target_link_libraries(smio_module_acq_pm
    PRIVATE
    smio_compiler_flags
)

########################################################################
# Library dependencies
########################################################################

# set depedency to targets
target_link_libraries(smio_module_acq_pm
    PRIVATE
    ${smio_module_acq_pm_LIBRARIES}
    ${halcs_LIBRARIES}
)
