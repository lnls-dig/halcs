########################################################################
# Library setup
########################################################################

# library basename
set (smio_table_OUTPUT_BASENAME "smio_table"
    CACHE STRING
    "Output smio_table library base name"
)

#########################################################################
# XXX dependency
#########################################################################

# list(APPEND smio_LIBRARIES XXX)

########################################################################
# Library compile options
########################################################################

########################################################################
# Sources/Headers definitions
########################################################################

# Source files

set(smio_table_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/acq/sm_io_acq_exports.c
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/dsp/sm_io_dsp_exports.c
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/fmc130m_4ch/sm_io_fmc130m_4ch_exports.c
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/fmc250m_4ch/sm_io_fmc250m_4ch_exports.c
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/fmc100m_4ch/sm_io_fmc100m_4ch_exports.c
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/fmcpico1m_4ch/sm_io_fmcpico1m_4ch_exports.c
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/fmc_adc_common/sm_io_fmc_adc_common_exports.c
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/fmc_active_clk/sm_io_fmc_active_clk_exports.c
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/swap/sm_io_swap_exports.c
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/rffe/sm_io_rffe_exports.c
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/rffe_scpi/sm_io_rffe_scpi_exports.c
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/afc_diag/sm_io_afc_diag_exports.c
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/trigger_iface/sm_io_trigger_iface_exports.c
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/trigger_mux/sm_io_trigger_mux_exports.c
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/afc_timing/sm_io_afc_timing_exports.c
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/acq_pm/sm_io_acq_pm_exports.c
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/init/sm_io_init_exports.c
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/afc_mgmt/sm_io_afc_mgmt_exports.c
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/tim_rcv/sm_io_tim_rcv_exports.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sm_io_codes.c
)

# for IDE project generation
source_group("Source Files" FILES
    ${smio_table_sources}
)

# Header files
list(APPEND smio_table_other_search_dirs "")

#######################################
# SMIO TABLE header definitions
#######################################

set(smio_table_public_headers
	${CMAKE_CURRENT_SOURCE_DIR}/modules/fmc130m_4ch/sm_io_fmc130m_4ch_exports.h
	${CMAKE_CURRENT_SOURCE_DIR}/modules/fmc250m_4ch/sm_io_fmc250m_4ch_exports.h
	${CMAKE_CURRENT_SOURCE_DIR}/modules/fmc100m_4ch/sm_io_fmc100m_4ch_exports.h
	${CMAKE_CURRENT_SOURCE_DIR}/modules/fmcpico1m_4ch/sm_io_fmcpico1m_4ch_exports.h
	${CMAKE_CURRENT_SOURCE_DIR}/modules/fmc_adc_common/sm_io_fmc_adc_common_exports.h
	${CMAKE_CURRENT_SOURCE_DIR}/modules/fmc_active_clk/sm_io_fmc_active_clk_exports.h
	${CMAKE_CURRENT_SOURCE_DIR}/modules/acq/sm_io_acq_exports.h
	${CMAKE_CURRENT_SOURCE_DIR}/modules/dsp/sm_io_dsp_exports.h
	${CMAKE_CURRENT_SOURCE_DIR}/modules/swap/sm_io_swap_exports.h
	${CMAKE_CURRENT_SOURCE_DIR}/modules/rffe/sm_io_rffe_exports.h
	${CMAKE_CURRENT_SOURCE_DIR}/modules/rffe_scpi/sm_io_rffe_scpi_exports.h
	${CMAKE_CURRENT_SOURCE_DIR}/modules/afc_diag/sm_io_afc_diag_exports.h
	${CMAKE_CURRENT_SOURCE_DIR}/modules/trigger_iface/sm_io_trigger_iface_exports.h
	${CMAKE_CURRENT_SOURCE_DIR}/modules/trigger_mux/sm_io_trigger_mux_exports.h
	${CMAKE_CURRENT_SOURCE_DIR}/modules/afc_timing/sm_io_afc_timing_exports.h
	${CMAKE_CURRENT_SOURCE_DIR}/modules/acq_pm/sm_io_acq_pm_exports.h
	${CMAKE_CURRENT_SOURCE_DIR}/modules/init/sm_io_init_exports.h
	${CMAKE_CURRENT_SOURCE_DIR}/modules/afc_mgmt/sm_io_afc_mgmt_exports.h
	${CMAKE_CURRENT_SOURCE_DIR}/modules/tim_rcv/sm_io_tim_rcv_exports.h
	${CMAKE_CURRENT_SOURCE_DIR}/sm_io_codes.h
)

# Get directory names only
list(APPEND smio_table_search_dirs "")

foreach(search_dir_header ${smio_table_public_headers})
    get_filename_component(search_dir
        ${search_dir_header}
        DIRECTORY
    )

    list(APPEND smio_table_search_dirs ${search_dir})
endforeach()

# remove, well, duplicates ...
list(REMOVE_DUPLICATES smio_table_search_dirs)

# append to global search list
list(APPEND smio_table_other_search_dirs ${smio_table_search_dirs})

#######################################
# HEADER Grouping
#######################################

# for IDE project generation
source_group("Header Files" FILES
    ${smio_table_public_headers}
)

# append to global search list
list(APPEND smio_table_other_search_dirs ${common_INCLUDE_DIR})

########################################################################
# Library declaration
########################################################################

# build static library
add_library(smio_table STATIC
    ${smio_table_sources}
    ${smio_table_public_headers}
)

set_target_properties(smio_table PROPERTIES
  PUBLIC_HEADER "${smio_table_public_headers}"
  OUTPUT_NAME "${smio_table_OUTPUT_BASENAME}"
  PREFIX "lib")

# smio_table_other_search_dirs is in quotes as this is a list
# and the first element would be concatenated with the previous
# generator expression.
target_include_directories(smio_table
    PRIVATE
    "$<BUILD_INTERFACE:${smio_table_other_search_dirs}>"
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>

)

# set debug posfix in case of debug builds
set_target_properties(smio_table PROPERTIES
    DEBUG_POSTFIX
    ${CMAKE_DEBUG_POSTFIX}
    POSITION_INDEPENDENT_CODE
    ON
)

# set compiler flags
target_link_libraries(smio_table
    PUBLIC
    halcs_compiler_flags
)

########################################################################
# Library dependencies
########################################################################

# set depedency to targets
target_link_libraries(smio_table
    PUBLIC
    ${smio_table_LIBRARIES}
    ${halcs_LIBRARIES}
)
