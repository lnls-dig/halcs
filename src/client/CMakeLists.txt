########################################################################
# Project setup
########################################################################

cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(halcsclient)

# set modules path for CMkake to find
set(halcsclient_CMAKE_MODULES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)
list(APPEND CMAKE_MODULE_PATH ${halcsclient_CMAKE_MODULES_DIR})

# add module to prevent in-source builds
include(PreventInSourceBuilds)
# set installation variables compliant with distributions
include(GNUInstallDirs)
# configure and generate *.Config.cmake files
include(CMakePackageConfigHelpers)
# extract version numbers from header file
include(TestHalcsClientVersion)
# detect target architecture
include(TargetArch)
# add git introspection functions
include(GetGitRevisionDescription)

target_architecture(ARCH)

# find pkg-config
find_package(PkgConfig)

# Will be used to add flags to pkg-config useful when apps want to statically link
set(pkg_config_libs_private "")
set(pkg_config_names_private "")
set(pkg_config_defines "")

set(CMAKE_DEBUG_POSTFIX d)

# library basename
set (halcsclient_OUTPUT_BASENAME "halcsclient"
    CACHE STRING
    "Output halcsclient library base name"
)

# set library dependencies
set(OPT_LIBRARIES_STATIC)

########################################################################
# libzmq dependency
########################################################################

find_package(libzmq)
if(NOT libzmq_FOUND)
    message(STATUS
        "libzmq not found with find_package MODULE mode. Trying CONFIG mode." )
    find_package(libzmq CONFIG)
endif()
if(libzmq_FOUND)
    list(APPEND MORE_LIBRARIES ${libzmq_LIBRARIES})
    if(PC_libzmq_FOUND)
        set(pkg_config_names_private "${pkg_config_names_private} libzmq")
        list(APPEND OPT_LIBRARIES_STATIC ${PC_libzmq_STATIC_LDFLAGS})
    else()
        set(pkg_config_libs_private "${pkg_config_libs_private} -lzmq")
    endif()
else()
    message(FATAL_ERROR
        "libzmq is not installed. Install it, then run CMake again." )
endif()

#########################################################################
# czmq dependency
#########################################################################

find_package(czmq)
if(NOT czmq_FOUND)
    message(STATUS
        "czmq not found with find_package MODULE mode. Trying CONFIG mode." )
    find_package(czmq CONFIG)
endif()
if(czmq_FOUND)
    list(APPEND MORE_LIBRARIES ${czmq_LIBRARIES})
    if(PC_czmq_FOUND)
        set(pkg_config_names_private "${pkg_config_names_private} libczmq")
        list(APPEND OPT_LIBRARIES_STATIC ${PC_czmq_STATIC_LDFLAGS})
    else()
        set(pkg_config_libs_private "${pkg_config_libs_private} -lczmq")
    endif()
else()
    message(FATAL_ERROR
        "czmq is not installed. Install it, then run CMake again." )
endif()

#########################################################################
# errhand dependency
#########################################################################

find_package(errhand)
if(NOT errhand_FOUND)
    message(STATUS
        "errhand not found with find_package MODULE mode. Trying CONFIG mode." )
    find_package(errhand CONFIG)
endif()
if(errhand_FOUND)
    list(APPEND MORE_LIBRARIES ${errhand_LIBRARIES})
    if(PC_errhand_FOUND)
        set(pkg_config_names_private "${pkg_config_names_private} liberrhand")
        list(APPEND OPT_LIBRARIES_STATIC ${PC_errhand_STATIC_LDFLAGS})
    else()
        set(pkg_config_libs_private "${pkg_config_libs_private} -lerrhand")
    endif()
else()
    message(FATAL_ERROR
        "liberrhand is not installed. Install it, then run CMake again." )
endif()

#########################################################################
# disptable dependency
#########################################################################

find_package(disptable)
if(NOT disptable_FOUND)
    message(STATUS
        "disptable not found with find_package MODULE mode. Trying CONFIG mode." )
    find_package(disptable CONFIG)
endif()
if(disptable_FOUND)
    list(APPEND MORE_LIBRARIES ${disptable_LIBRARIES})
    if(PC_disptable_FOUND)
        set(pkg_config_names_private "${pkg_config_names_private} libdisptable")
        list(APPEND OPT_LIBRARIES_STATIC ${PC_disptable_STATIC_LDFLAGS})
    else()
        set(pkg_config_libs_private "${pkg_config_libs_private} -ldisptable")
    endif()
else()
    message(FATAL_ERROR
        "libdisptable is not installed. Install it, then run CMake again." )
endif()

#########################################################################
# hutils dependency
#########################################################################

find_package(hutils)
if(NOT hutils_FOUND)
    message(STATUS
        "hutils not found with find_package MODULE mode. Trying CONFIG mode." )
    find_package(hutils CONFIG)
endif()
if(hutils_FOUND)
    list(APPEND MORE_LIBRARIES ${hutils_LIBRARIES})
    if(PC_hutils_FOUND)
        set(pkg_config_names_private "${pkg_config_names_private} libhutils")
        list(APPEND OPT_LIBRARIES_STATIC ${PC_hutils_STATIC_LDFLAGS})
    else()
        set(pkg_config_libs_private "${pkg_config_libs_private} -lhutils")
    endif()
else()
    message(FATAL_ERROR
        "libhutils is not installed. Install it, then run CMake again." )
endif()

#########################################################################
# mlm dependency
#########################################################################

find_package(mlm)
if(NOT mlm_FOUND)
    message(STATUS
        "mlm not found with find_package MODULE mode. Trying CONFIG mode." )
    find_package(mlm CONFIG)
endif()
if(mlm_FOUND)
    list(APPEND MORE_LIBRARIES ${mlm_LIBRARIES})
    if(PC_mlm_FOUND)
        set(pkg_config_names_private "${pkg_config_names_private} libmlm")
        list(APPEND OPT_LIBRARIES_STATIC ${PC_mlm_STATIC_LDFLAGS})
    else()
        set(pkg_config_libs_private "${pkg_config_libs_private} -lmlm")
    endif()
else()
    message(FATAL_ERROR
        "libmlm is not installed. Install it, then run CMake again." )
endif()

########################################################################
# User compile-time options
########################################################################

########################################################################
# Library compile options
########################################################################

add_library(halcsclient_compiler_flags INTERFACE)
target_compile_features(halcsclient_compiler_flags
    INTERFACE
    c_std_99
)

# add compiler warning flags just when building this project via
# the BUILD_INTERFACE genex
set(gcc_c "$<$<COMPILE_LANGUAGE:C>:$<C_COMPILER_ID:GNU>>")
set(clang_c "$<$<COMPILE_LANGUAGE:C>:$<C_COMPILER_ID:Clang>>")
set(msvc_c "$<$<COMPILE_LANGUAGE:C>:$<C_COMPILER_ID:MSVC>>")
target_compile_options(halcsclient_compiler_flags
    INTERFACE
    "$<${gcc_c}:$<BUILD_INTERFACE:-Wall;-Wextra;-Werror;-O2;-g>>"
    "$<${clang_c}:$<BUILD_INTERFACE:-Wall;-Wextra;-Werror;-O2;-g>>"
    "$<${msvc_c}:$<BUILD_INTERFACE:-W3;-O2;-g>>"
)

# control where the static and shared libraries are built so that on windows
# we don't need to tinker with the /ath to run the executable
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

option(BUILD_SHARED "Whether or not to build the shared object"  ON)
option(BUILD_STATIC "Whether or not to build the static archive" ON)

# add targets to target_outputs
list(APPEND target_outputs "")

if(BUILD_SHARED)
    list(APPEND target_outputs "halcsclient")
endif()

if(BUILD_STATIC)
    list(APPEND target_outputs "halcsclient-static")
endif()

########################################################################
# Sources/Headers definitions
########################################################################

# Source files

#######################################
# halcsclient sources definitions
#######################################

set(halcsclient_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/src/halcs_client_err.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/halcs_client_core.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/halcs_client_revision.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/halcs_client_rw_param.c
)

#######################################
# ACQ sources definitions
#######################################

#######################################
# SMIO TABLE sources definitions
#######################################

# Ugly and weird set of sources outside the source directory
set(halcsclient_smio_table_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/acq/sm_io_acq_exports.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/dsp/sm_io_dsp_exports.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/fmc130m_4ch/sm_io_fmc130m_4ch_exports.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/fmc250m_4ch/sm_io_fmc250m_4ch_exports.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/fmc100m_4ch/sm_io_fmc100m_4ch_exports.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/fmcpico1m_4ch/sm_io_fmcpico1m_4ch_exports.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/fmc_adc_common/sm_io_fmc_adc_common_exports.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/fmc_active_clk/sm_io_fmc_active_clk_exports.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/swap/sm_io_swap_exports.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/rffe/sm_io_rffe_exports.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/rffe_scpi/sm_io_rffe_scpi_exports.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/afc_diag/sm_io_afc_diag_exports.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/trigger_iface/sm_io_trigger_iface_exports.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/trigger_mux/sm_io_trigger_mux_exports.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/afc_timing/sm_io_afc_timing_exports.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/acq_pm/sm_io_acq_pm_exports.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/init/sm_io_init_exports.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/afc_mgmt/sm_io_afc_mgmt_exports.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/tim_rcv/sm_io_tim_rcv_exports.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/orbit_intlk/sm_io_orbit_intlk_exports.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/fofb_ctrl/sm_io_fofb_ctrl_exports.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/rtmlamp_ohwr/sm_io_rtmlamp_ohwr_exports.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/fofb_processing/sm_io_fofb_processing_exports.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/clk_cnt/sm_io_clk_cnt_exports.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/sm_io_codes.c
)

# for IDE project generation
source_group("Source Files" FILES
    ${halcsclient_sources}
    ${halcsclient_acq_sources}
    ${halcsclient_smio_table_sources}
)

# Header files

list(APPEND halcsclient_public_dirs "")
list(APPEND halcsclient_other_search_dirs "")

#######################################
# halcsclient header definitions
#######################################

# Ugly and weird set of sources outside the source directory
set(halcsclient_public_headers
    ${CMAKE_CURRENT_SOURCE_DIR}/include/halcs_client_classes.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/halcs_client_codes.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/halcs_client_core.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/halcs_client_err.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/halcs_client.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/halcs_client_prelude.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/halcs_client_revision.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/halcs_client_rw_param_codes.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/halcs_client_rw_param.h
)

#######################################
# SMIO header definitions
#######################################

# Ugly and weird set of sources outside the source directory
set(halcsclient_smio_public_headers
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io/modules/fmc130m_4ch/sm_io_fmc130m_4ch_codes.h
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io/modules/fmc250m_4ch/sm_io_fmc250m_4ch_codes.h
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io/modules/fmc100m_4ch/sm_io_fmc100m_4ch_codes.h
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io/modules/fmcpico1m_4ch/sm_io_fmcpico1m_4ch_codes.h
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io/modules/fmc_adc_common/sm_io_fmc_adc_common_codes.h
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io/modules/fmc_active_clk/sm_io_fmc_active_clk_codes.h
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io/modules/acq/sm_io_acq_codes.h
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io/modules/dsp/sm_io_dsp_codes.h
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io/modules/swap/sm_io_swap_codes.h
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io/modules/rffe/sm_io_rffe_codes.h
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io/modules/afc_diag/sm_io_afc_diag_codes.h
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io/modules/trigger_iface/sm_io_trigger_iface_codes.h
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io/modules/trigger_mux/sm_io_trigger_mux_codes.h
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io/modules/afc_timing/sm_io_afc_timing_codes.h
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io/modules/acq_pm/sm_io_acq_pm_codes.h
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io/modules/init/sm_io_init_codes.h
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io/modules/afc_mgmt/sm_io_afc_mgmt_codes.h
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io/modules/tim_rcv/sm_io_tim_rcv_codes.h
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io/modules/orbit_intlk/sm_io_orbit_intlk_codes.h
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io/modules/fofb_ctrl/sm_io_fofb_ctrl_codes.h
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io/modules/rtmlamp_ohwr/sm_io_rtmlamp_ohwr_codes.h
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io/modules/fofb_processing/sm_io_fofb_processing_codes.h
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io/modules/clk_cnt/sm_io_clk_cnt_codes.h
)

# Get directory names only
list(APPEND halcsclient_smio_search_dirs "")

foreach(search_dir_header ${halcsclient_smio_public_headers})
    get_filename_component(search_dir
        ${search_dir_header}
        DIRECTORY
    )

    list(APPEND halcsclient_smio_search_dirs ${search_dir})
endforeach()

# remove, well, duplicates ...
list(REMOVE_DUPLICATES halcsclient_smio_search_dirs)

# append to global search list
list(APPEND halcsclient_other_search_dirs ${halcsclient_smio_search_dirs})

# also, these headers are part of the project so include them
list(APPEND halcsclient_public_headers
    ${halcsclient_smio_public_headers}
)

#######################################
# SMIO TABLE header definitions
#######################################

# Ugly and weird set of sources outside the source directory
set(halcsclient_smio_table_public_headers
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/fmc130m_4ch/sm_io_fmc130m_4ch_exports.h
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/fmc250m_4ch/sm_io_fmc250m_4ch_exports.h
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/fmc100m_4ch/sm_io_fmc100m_4ch_exports.h
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/fmcpico1m_4ch/sm_io_fmcpico1m_4ch_exports.h
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/fmc_adc_common/sm_io_fmc_adc_common_exports.h
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/fmc_active_clk/sm_io_fmc_active_clk_exports.h
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/acq/sm_io_acq_exports.h
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/dsp/sm_io_dsp_exports.h
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/swap/sm_io_swap_exports.h
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/rffe/sm_io_rffe_exports.h
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/rffe_scpi/sm_io_rffe_scpi_exports.h
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/afc_diag/sm_io_afc_diag_exports.h
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/trigger_iface/sm_io_trigger_iface_exports.h
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/trigger_mux/sm_io_trigger_mux_exports.h
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/afc_timing/sm_io_afc_timing_exports.h
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/acq_pm/sm_io_acq_pm_exports.h
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/init/sm_io_init_exports.h
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/afc_mgmt/sm_io_afc_mgmt_exports.h
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/tim_rcv/sm_io_tim_rcv_exports.h
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/orbit_intlk/sm_io_orbit_intlk_exports.h
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/fofb_ctrl/sm_io_fofb_ctrl_exports.h
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/rtmlamp_ohwr/sm_io_rtmlamp_ohwr_exports.h
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/fofb_processing/sm_io_fofb_processing_exports.h
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/modules/clk_cnt/sm_io_clk_cnt_exports.h
	${CMAKE_CURRENT_SOURCE_DIR}/../sm_io_table/sm_io_codes.h
)

# Get directory names only
list(APPEND halcsclient_smio_table_search_dirs "")

foreach(search_dir_header ${halcsclient_smio_table_public_headers})
    get_filename_component(search_dir
        ${search_dir_header}
        DIRECTORY
    )

    list(APPEND halcsclient_smio_table_search_dirs ${search_dir})
endforeach()

# remove, well, duplicates ...
list(REMOVE_DUPLICATES halcsclient_smio_table_search_dirs)

# append to global search list
list(APPEND halcsclient_other_search_dirs ${halcsclient_smio_table_search_dirs})

# also, these headers are part of the project so include them
list(APPEND halcsclient_public_headers
    ${halcsclient_smio_table_public_headers}
)

#######################################
# ACQ header definitions
#######################################

# Ugly and weird set of sources outside the source directory
set(halcsclient_acq_public_headers
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/acq_chan_gen_defs.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/ddr3_map_structs.h
)

# Get directory names only
list(APPEND halcsclient_acq_search_dirs "")

foreach(search_dir_header ${halcsclient_acq_public_headers})
    get_filename_component(search_dir
        ${search_dir_header}
        DIRECTORY
    )

    list(APPEND halcsclient_acq_search_dirs ${search_dir})
endforeach()

# remove, well, duplicates ...
list(REMOVE_DUPLICATES halcsclient_acq_search_dirs)

# append to global search list
list(APPEND halcsclient_other_search_dirs ${halcsclient_acq_search_dirs})

# also, these headers are part of the project so include them
list(APPEND halcsclient_public_headers
    ${halcsclient_acq_public_headers}
)

#######################################
# ACQ Directories header definitions
#######################################

# Header directory
set(halcsclient_acq_public_dir_headers
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/boards/afcv3/acq_chan.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/boards/afcv3_1/acq_chan.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/boards/ml605/acq_chan.h
)

# Get filenames only so we install only those listed above
list(APPEND halcsclient_acq_files "")

foreach(public_dir_header ${halcsclient_acq_public_dir_headers})
    get_filename_component(public_dir
        ${public_dir_header}
        NAME
    )

    list(APPEND halcsclient_acq_files ${public_dir})
endforeach()

# remove, well, duplicates ...
list(REMOVE_DUPLICATES halcsclient_acq_files)

# Get directory names only. In this case, we must install
# boards/<board name> directory structure, so list them here
# separately
list(APPEND halcsclient_acq_public_dirs
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/boards
)

# Group header installation directories
list(APPEND halcsclient_public_dirs
    ${halcsclient_acq_public_dirs}
)

# Group filenames inside dirs that will be installed.
# This will be used to match the filenames, so only
# those listed here will be installed
list(APPEND halcsclient_public_files_in_dirs
    ${halcsclient_acq_files}
)

#######################################
# HEADER Grouping
#######################################

# for IDE project generation
source_group("Header Files" FILES
    ${halcsclient_public_headers}
)

# README files
set(readme-docs
    COPYING
)

########################################################################
# Library declaration
########################################################################

# build shared library
if(BUILD_SHARED)
    add_library(halcsclient SHARED
        ${halcsclient_sources}
        ${halcsclient_acq_sources}
        ${halcsclient_smio_table_sources}
        ${halcsclient_public_headers}
        ${readme-docs}
        ${halcsclient-pkgconfig}
    )

    set_target_properties(halcsclient PROPERTIES
        PUBLIC_HEADER "${halcsclient_public_headers}"
        SOVERSION "1"
        VERSION "${halcsclient_VERSION}"
        COMPILE_DEFINITIONS "DLL_EXPORT"
        OUTPUT_NAME "${halcsclient_OUTPUT_BASENAME}"
        PREFIX "lib")
endif()

# build static library
if(BUILD_STATIC)
    add_library(halcsclient-static STATIC
        ${halcsclient_sources}
        ${halcsclient_acq_sources}
        ${halcsclient_smio_table_sources}
        ${halcsclient_public_headers}
        ${readme-docs}
        ${halcsclient-pkgconfig}
    )

    set_target_properties(halcsclient-static PROPERTIES
      PUBLIC_HEADER "${halcsclient_public_headers}"
      OUTPUT_NAME "${halcsclient_OUTPUT_BASENAME}"
      PREFIX "lib")
endif()

# set Git macros
git_describe(GIT_TAG --always --tags)
git_get_config(GIT_NAME "user.name")
git_get_config(GIT_EMAIL "user.email")

# set properties for all targets
foreach(target ${target_outputs})
    # halcsclient_other_search_dirs is in quotes as this is a list
    # and the first element would be concatenated with the previous
    # generator expression.
    target_include_directories(${target}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        "$<BUILD_INTERFACE:${halcsclient_other_search_dirs}>"
        $<INSTALL_INTERFACE:include>
    )

    # setting PUBLIC compile options
    string(APPEND halcsclient_board_definitions "")

    target_compile_definitions(${target} PUBLIC
        ${halcsclient_board_definitions}
    )

    # set debug posfix in case of debug builds
    set_target_properties(${target} PROPERTIES
        DEBUG_POSTFIX
        ${CMAKE_DEBUG_POSTFIX}
        POSITION_INDEPENDENT_CODE
        ON
    )

    # add board compile definitions
    target_compile_definitions(${target} PRIVATE
        GIT_REVISION=\"${GIT_TAG}\"
        GIT_USER_NAME=\"${GIT_NAME}\"
        GIT_USER_EMAIL=\"${GIT_EMAIL}\"
    )

    # set PRIVATE compiler flags. It doesn't really work to link an
    # INTERFACE library with PRIVATE scope. See:
    # https://gitlab.kitware.com/cmake/cmake/-/issues/15415
    target_link_libraries(${target} PRIVATE
        $<TARGET_PROPERTY:halcsclient_compiler_flags,INTERFACE_LINK_LIBRARIES>
    )
    target_include_directories(${target} PRIVATE
        $<TARGET_PROPERTY:halcsclient_compiler_flags,INTERFACE_INCLUDE_DIRECTORIES>
    )
    target_compile_options(${target} PRIVATE
        $<TARGET_PROPERTY:halcsclient_compiler_flags,INTERFACE_COMPILE_OPTIONS>
    )
    target_compile_definitions(${target} PRIVATE
        $<TARGET_PROPERTY:halcsclient_compiler_flags,INTERFACE_COMPILE_DEFINITIONS>
    )
    target_compile_features(${target} PRIVATE
        $<TARGET_PROPERTY:halcsclient_compiler_flags,INTERFACE_COMPILE_FEATURES>
    )
    target_sources(${target} PRIVATE
        $<TARGET_PROPERTY:halcsclient_compiler_flags,INTERFACE_SOURCES>
    )
endforeach()

########################################################################
# Library dependencies
########################################################################

# set depedency to targets
if(BUILD_SHARED)
    target_link_libraries(halcsclient
        PUBLIC
        ${MORE_LIBRARIES}
    )
endif()

if(BUILD_STATIC)
    target_link_libraries(halcsclient-static
        PUBLIC
        ${MORE_LIBRARIES}
    )
endif()

########################################################################
# Custom targets
########################################################################

# uninstall target
if(NOT TARGET uninstall)
  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

  add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()

########################################################################
# Installer
########################################################################

install(TARGETS ${target_outputs} halcsclient_compiler_flags
    EXPORT ${PROJECT_NAME}-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Libs
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Libs
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Libs
    FRAMEWORK DESTINATION "Library/Frameworks"
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT Libs
)

# install headers that are inside another directory
install(DIRECTORY
    ${halcsclient_public_dirs}
    DESTINATION
    ${CMAKE_INSTALL_INCLUDEDIR}
    COMPONENT Libs
    FILES_MATCHING PATTERN
    ${halcsclient_public_files_in_dirs}
)

# CMake search path wants either "share" (AKA GNUInstallDirs DATAROOTDIR)
# for arch-independent, or LIBDIR for arch-dependent, plus "cmake" as prefix
set(halcsclient_CMAKECONFIG_INSTALL_DIR
    "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
    CACHE STRING
    "install path for halcsclientConfig.cmake and halcsclientTargets.cmake"
)

# Convert README files in *.txt ones
foreach(readme ${readme-docs})
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${readme}
        ${CMAKE_CURRENT_BINARY_DIR}/${readme}.txt
    )
endforeach()

# Generate .pc file
set(prefix \${pcfiledir}/../..)
set(exec_prefix \${prefix})
set(libdir \${prefix}/lib)
set(includedir \${prefix}/include)
set(VERSION
    ${halcsclient_VERSION_MAJOR}.${halcsclient_VERSION_MINOR}.${halcsclient_VERSION_PATCH}
)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/libhalcsclient.pc.in
    ${CMAKE_CURRENT_BINARY_DIR}/libhalcsclient.pc @ONLY)
set(halcsclient-pkgconfig ${CMAKE_CURRENT_BINARY_DIR}/libhalcsclient.pc)

# install .pc file
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/libhalcsclient.pc
    DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/pkgconfig
    COMPONENT Libs
)

# only available on CMake >= 3.0. Generates  <PROJECT_NAME>Targets.cmake
# file for other cmake projects using this.
export(EXPORT ${PROJECT_NAME}-targets
    FILE
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake"
)

# configure and generate <PROJECT_NAME>Config.cmake file
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION
    ${halcsclient_CMAKECONFIG_INSTALL_DIR}
)

# configure and generate <PROJECT_NAME>ConfigVersion.cmake file
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION
    ${halcsclient_VERSION}
    COMPATIBILITY AnyNewerVersion
)

# actually install <PROJECT_NAME>Targets.cmake file
if(BUILD_SHARED OR BUILD_STATIC)
    install(EXPORT ${PROJECT_NAME}-targets
        FILE ${PROJECT_NAME}Targets.cmake
        DESTINATION ${halcsclient_CMAKECONFIG_INSTALL_DIR}
        COMPONENT Libs
    )
    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION
        ${halcsclient_CMAKECONFIG_INSTALL_DIR}
        COMPONENT Libs
    )
endif()

# CPack rules
option(ENABLE_CPACK "Enables cpack rules" ON)

if(ENABLE_CPACK)
    if(CMAKE_CURRENT_BINARY_DIR STREQUAL CMAKE_BINARY_DIR)
        if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
            set(CMAKE_INSTALL_DEBUG_LIBRARIES_ONLY TRUE)
            set(CMAKE_INSTALL_DEBUG_LIBRARIES TRUE)
            set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE)
        endif()

        include(InstallRequiredSystemLibraries)

        # set architecture as detected by TargetArch module
        set(arch_name ${ARCH})

        list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_BINARY_DIR})
        set(CPACK_GENERATOR "DEB")
        set(CPACK_DEBIAN_PACKAGE_GENERATE_SHLIBS_POLICY ">=")
        set(CPACK_DEBIAN_PACKAGE_GENERATE_SHLIBS ON)
        set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
        # Seems broken in CMake and crashes with 
        # CPack: Create package
        # terminate called after throwing an instance of 'std::logic_error'
        #   what():  basic_string::_M_construct null not valid
        # Makefile:81: recipe for target 'package' failed
        # make: *** [package] Aborted (core dumped)
        # set(CPACK_DEBIAN_DEBUGINFO_PACKAGE ON)
        set(CPACK_RPM_PACKAGE_AUTOREQ no)
        set(CPACK_RPM_PACKAGE_AUTOPROV yes)
        set(CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION
            /lib
            /usr/etc
            /etc/systemd
            /lib/systemd
            /etc/systemd/system
            /lib/systemd/system
            /etc/udev
            /etc/udev/rules.d
        )
        set(CPACK_PACKAGE_NAME "halcsclient")
        set(halcsclient_DISTRO_VERSION "" CACHE STRING "halcsclient distribution version")
        set(CPACK_PACKAGE_FILE_NAME
            "${CPACK_PACKAGE_NAME}_${halcsclient_VERSION}${halcsclient_DISTRO_VERSION}_${arch_name}"
        )
        set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "HALCS Conversion library")
        set(CPACK_PACKAGE_VENDOR "LNLS")
        set(CPACK_PACKAGE_CONTACT "Lucas Russo <lucas.russo@lnls.br>")
        set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_BINARY_DIR}/COPYING.txt")
        # set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_BINARY_DIR}/README.txt")
        # set(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_CURRENT_BINARY_DIR}/WELCOME.txt")
        # set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/branding.bmp")
        set(CPACK_PACKAGE_VERSION ${halcsclient_VERSION})
        set(CPACK_PACKAGE_VERSION_MAJOR ${halcsclient_VERSION_MAJOR})
        set(CPACK_PACKAGE_VERSION_MINOR ${halcsclient_VERSION_MINOR})
        set(CPACK_PACKAGE_VERSION_PATCH ${halcsclient_VERSION_PATCH})
        # set(CPACK_PACKAGE_INSTALL_DIRECTORY "halcsclient Install Directory")
        # set(CPACK_TEMPORARY_DIRECTORY "halcsclient Temporary CPack Directory")
        # Generate debuginfo package
        set(CPACK_RPM_DEBUGINFO_PACKAGE ON)
        set(CPACK_RPM_BUILD_SOURCE_DIRS_PREFIX "/usr/src/debug/${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")

        include(CPack)

        cpack_add_component_group(Development
            DISPLAY_NAME "HALCS client library"
            EXPANDED
        )
        cpack_add_component(SourceCode
            DISPLAY_NAME "HALCS client source code"
            DISABLED
            INSTALL_TYPES FullInstall
        )
        cpack_add_component(SDK
            DISPLAY_NAME "HALCS client headers and libraries"
            INSTALL_TYPES FullInstall DevInstall
            GROUP Development
        )
        cpack_add_component(Runtime
            DISPLAY_NAME "HALCS client runtime files"
            REQUIRED
            INSTALL_TYPES FullInstall DevInstall MinInstall
        )
        cpack_add_install_type(FullInstall
            DISPLAY_NAME "Full install, including source code"
        )
        cpack_add_install_type(DevInstall
            DISPLAY_NAME "Developer install, headers and libraries"
        )
        cpack_add_install_type(MinInstall
            DISPLAY_NAME "Minimal install, runtime only"
        )
    endif()

endif()
