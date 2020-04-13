########################################################################
# Library setup
########################################################################

# library basename
set (smio_chips_OUTPUT_BASENAME "smio_chips"
    CACHE STRING
    "Output smio_chips library base name"
)

#########################################################################
# XXX dependency
#########################################################################

# list(APPEND smio_chips_LIBRARIES XXX)

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

set(smio_chips_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/sm_ch_err.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sm_ch_24aa64.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sm_ch_ad9510.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sm_ch_isla216p.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sm_ch_ltc2174.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sm_ch_pca9547.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sm_ch_rffe.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sm_ch_rffe_scpi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sm_ch_si57x.c
)

# for IDE project generation
source_group("Source Files" FILES
    ${smio_chips_sources}
)

# Header files
list(APPEND smio_chips_other_search_dirs "")

#######################################
# SMIO header definitions
#######################################

set(smio_chips_public_headers
    ${common_INCLUDE_DIR}/sm_ch_err.h
    ${common_INCLUDE_DIR}/sm_ch_24aa64.h
    ${common_INCLUDE_DIR}/sm_ch_ad9510.h
    ${common_INCLUDE_DIR}/sm_ch_isla216p.h
    ${common_INCLUDE_DIR}/sm_ch_ltc2174.h
    ${common_INCLUDE_DIR}/sm_ch_pca9547.h
    ${common_INCLUDE_DIR}/sm_ch_si57x.h
    ${common_INCLUDE_DIR}/sm_ch_rffe.h
    ${common_INCLUDE_DIR}/sm_ch_rffe_scpi.h
    ${smio_chips_regs_INCLUDE_DIR}/e24aa64_regs.h
    ${smio_chips_regs_INCLUDE_DIR}/ad9510_regs.h
    ${smio_chips_regs_INCLUDE_DIR}/isla216p_regs.h
    ${smio_chips_regs_INCLUDE_DIR}/ltc2174_regs.h
    ${smio_chips_regs_INCLUDE_DIR}/pca9547_regs.h
    ${smio_chips_regs_INCLUDE_DIR}/si57x_regs.h
    ${smio_chips_regs_INCLUDE_DIR}/sm_ch_ad9510_defaults.h
    ${smio_chips_regs_INCLUDE_DIR}/sm_ch_isla216p_defaults.h
    ${smio_chips_regs_INCLUDE_DIR}/sm_ch_rffe_defaults.h
    ${smio_chips_regs_INCLUDE_DIR}/sm_ch_si57x_defaults.h
)

# Get directory names only
list(APPEND smio_chips_search_dirs "")

foreach(search_dir_header ${smio_chips_public_headers})
    get_filename_component(search_dir
        ${search_dir_header}
        DIRECTORY
    )

    list(APPEND smio_chips_search_dirs ${search_dir})
endforeach()

# remove, well, duplicates ...
list(REMOVE_DUPLICATES smio_chips_search_dirs)

# append to global search list
list(APPEND smio_chips_other_search_dirs ${smio_chips_search_dirs})

#######################################
# HEADER Grouping
#######################################

# for IDE project generation
source_group("Header Files" FILES
    ${smio_chips_public_headers}
)

########################################################################
# Library declaration
########################################################################

# build static library
add_library(smio_chips STATIC
    ${smio_chips_sources}
    ${smio_chips_public_headers}
)

set_target_properties(smio_chips PROPERTIES
  PUBLIC_HEADER "${smio_chips_public_headers}"
  OUTPUT_NAME "${smio_chips_OUTPUT_BASENAME}"
  PREFIX "lib")

# smio_chips_other_search_dirs is in quotes as this is a list
# and the first element would be concatenated with the previous
# generator expression.
target_include_directories(smio_chips
    PRIVATE
    "$<BUILD_INTERFACE:${smio_chips_other_search_dirs}>"
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>
)

# set debug posfix in case of debug builds
set_target_properties(smio_chips PROPERTIES
    DEBUG_POSTFIX
    ${CMAKE_DEBUG_POSTFIX}
    POSITION_INDEPENDENT_CODE
    ON
)

# set compiler flags
target_link_libraries(smio_chips
    PRIVATE
    smio_compiler_flags
)

########################################################################
# Library dependencies
########################################################################

# set depedency to targets
target_link_libraries(smio_chips
    PRIVATE
    ${smio_chips_LIBRARIES}
    ${halcs_LIBRARIES}
)
