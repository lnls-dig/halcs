########################################################################
# Library setup
########################################################################

# library basename
set (smio_modules_OUTPUT_BASENAME "smio_modules"
    CACHE STRING
    "Output smio_modules library base name"
)

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

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/acq)
list(APPEND smio_modules_LIBRARIES smio_module_acq)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/acq_pm)
list(APPEND smio_modules_LIBRARIES smio_module_acq_pm)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/afc_diag)
list(APPEND smio_modules_LIBRARIES smio_module_afc_diag)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/afc_mgmt)
list(APPEND smio_modules_LIBRARIES smio_module_afc_mgmt)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/afc_timing)
list(APPEND smio_modules_LIBRARIES smio_module_afc_timing)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/dsp)
list(APPEND smio_modules_LIBRARIES smio_module_dsp)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/fmc100m_4ch)
list(APPEND smio_modules_LIBRARIES smio_module_fmc100m_4ch)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/fmc130m_4ch)
list(APPEND smio_modules_LIBRARIES smio_module_fmc130m_4ch)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/fmc250m_4ch)
list(APPEND smio_modules_LIBRARIES smio_module_fmc250m_4ch)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/fmc_active_clk)
list(APPEND smio_modules_LIBRARIES smio_module_fmc_active_clk)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/fmc_adc_common)
list(APPEND smio_modules_LIBRARIES smio_module_fmc_adc_common)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/fmcpico1m_4ch)
list(APPEND smio_modules_LIBRARIES smio_module_fmcpico1m_4ch)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/init)
list(APPEND smio_modules_LIBRARIES smio_module_init)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/rffe)
list(APPEND smio_modules_LIBRARIES smio_module_rffe)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/rffe_scpi)
list(APPEND smio_modules_LIBRARIES smio_module_rffe_scpi)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/swap)
list(APPEND smio_modules_LIBRARIES smio_module_swap)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tim_rcv)
list(APPEND smio_modules_LIBRARIES smio_module_tim_rcv)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/trigger_iface)
list(APPEND smio_modules_LIBRARIES smio_module_trigger_iface)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/trigger_mux)
list(APPEND smio_modules_LIBRARIES smio_module_trigger_mux)

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

########################################################################
# Library dependencies
########################################################################

# set depedency to targets
target_link_libraries(smio_modules
    INTERFACE
    ${smio_modules_LIBRARIES}
    ${halcs_LIBRARIES}
)
