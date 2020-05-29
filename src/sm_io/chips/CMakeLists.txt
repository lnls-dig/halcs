########################################################################
# Library setup
########################################################################

# library basename
set (sm_io_chips_OUTPUT_BASENAME "sm_io_chips"
    CACHE STRING
    "Output sm_io_chips library base name"
)

#########################################################################
# XXX dependency
#########################################################################

# list(APPEND sm_io_chips_LIBRARIES XXX)

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

set(sm_io_chips_sources
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
    ${sm_io_chips_sources}
)

# Header files
list(APPEND sm_io_chips_other_search_dirs "")

#######################################
# sm_io header definitions
#######################################

set(sm_io_chips_public_headers
    ${common_INCLUDE_DIR}/sm_ch_err.h
    ${common_INCLUDE_DIR}/sm_ch_24aa64.h
    ${common_INCLUDE_DIR}/sm_ch_ad9510.h
    ${common_INCLUDE_DIR}/sm_ch_isla216p.h
    ${common_INCLUDE_DIR}/sm_ch_ltc2174.h
    ${common_INCLUDE_DIR}/sm_ch_pca9547.h
    ${common_INCLUDE_DIR}/sm_ch_si57x.h
    ${common_INCLUDE_DIR}/sm_ch_rffe.h
    ${common_INCLUDE_DIR}/sm_ch_rffe_scpi.h
    ${sm_io_chips_regs_INCLUDE_DIR}/e24aa64_regs.h
    ${sm_io_chips_regs_INCLUDE_DIR}/ad9510_regs.h
    ${sm_io_chips_regs_INCLUDE_DIR}/isla216p_regs.h
    ${sm_io_chips_regs_INCLUDE_DIR}/ltc2174_regs.h
    ${sm_io_chips_regs_INCLUDE_DIR}/pca9547_regs.h
    ${sm_io_chips_regs_INCLUDE_DIR}/si57x_regs.h
    ${sm_io_chips_regs_INCLUDE_DIR}/sm_ch_ad9510_defaults.h
    ${sm_io_chips_regs_INCLUDE_DIR}/sm_ch_isla216p_defaults.h
    ${sm_io_chips_regs_INCLUDE_DIR}/sm_ch_rffe_defaults.h
    ${sm_io_chips_regs_INCLUDE_DIR}/sm_ch_si57x_defaults.h
)

# Get directory names only
list(APPEND sm_io_chips_search_dirs "")

foreach(search_dir_header ${sm_io_chips_public_headers})
    get_filename_component(search_dir
        ${search_dir_header}
        DIRECTORY
    )

    list(APPEND sm_io_chips_search_dirs ${search_dir})
endforeach()

# remove, well, duplicates ...
list(REMOVE_DUPLICATES sm_io_chips_search_dirs)

# append to global search list
list(APPEND sm_io_chips_other_search_dirs ${sm_io_chips_search_dirs})

#######################################
# HEADER Grouping
#######################################

# for IDE project generation
source_group("Header Files" FILES
    ${sm_io_chips_public_headers}
)

########################################################################
# Library declaration
########################################################################

# build static library
add_library(sm_io_chips STATIC
    ${sm_io_chips_sources}
    ${sm_io_chips_public_headers}
)

set_target_properties(sm_io_chips PROPERTIES
  PUBLIC_HEADER "${sm_io_chips_public_headers}"
  OUTPUT_NAME "${sm_io_chips_OUTPUT_BASENAME}"
  PREFIX "lib")

# sm_io_chips_other_search_dirs is in quotes as this is a list
# and the first element would be concatenated with the previous
# generator expression.
target_include_directories(sm_io_chips
    PRIVATE
    "$<BUILD_INTERFACE:${sm_io_chips_other_search_dirs}>"
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>
)

# set debug posfix in case of debug builds
set_target_properties(sm_io_chips PROPERTIES
    DEBUG_POSTFIX
    ${CMAKE_DEBUG_POSTFIX}
    POSITION_INDEPENDENT_CODE
    ON
)

# set compiler flags
target_link_libraries(sm_io_chips
    PRIVATE
    sm_io_compiler_flags
)

########################################################################
# Library dependencies
########################################################################

# set depedency to targets
target_link_libraries(sm_io_chips
    PRIVATE
    ${sm_io_chips_LIBRARIES}
    ${halcs_LIBRARIES}
)
