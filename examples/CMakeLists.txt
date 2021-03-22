########################################################################
# Project setup
########################################################################

cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(halcs_examples)

# set modules path for CMkake to find
set(halcs_examples_CMAKE_MODULES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)
list(APPEND CMAKE_MODULE_PATH ${halcs_examples_CMAKE_MODULES_DIR})

# add module to prevent in-source builds
include(PreventInSourceBuilds)
# set installation variables compliant with distributions
include(GNUInstallDirs)
# configure and generate *.Config.cmake files
include(CMakePackageConfigHelpers)
# extract version numbers from header file
include(TestHalcsexamplesVersion)
# detect target architecture
include(TargetArch)

target_architecture(ARCH)

set(CMAKE_DEBUG_POSTFIX d)

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
        list(APPEND OPT_LIBRARIES_STATIC ${PC_libzmq_STATIC_LDFLAGS})
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
        list(APPEND OPT_LIBRARIES_STATIC ${PC_czmq_STATIC_LDFLAGS})
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
        list(APPEND OPT_LIBRARIES_STATIC ${PC_errhand_STATIC_LDFLAGS})
    endif()
else()
    message(FATAL_ERROR
        "liberrhand is not installed. Install it, then run CMake again." )
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
        list(APPEND OPT_LIBRARIES_STATIC ${PC_hutils_STATIC_LDFLAGS})
    endif()
else()
    message(FATAL_ERROR
        "libhutils is not installed. Install it, then run CMake again." )
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
        list(APPEND OPT_LIBRARIES_STATIC ${PC_disptable_STATIC_LDFLAGS})
    endif()
else()
    message(FATAL_ERROR
        "libdisptable is not installed. Install it, then run CMake again." )
endif()

#########################################################################
# halcsclient dependency
#########################################################################

find_package(halcsclient)
if(NOT halcsclient_FOUND)
    message(STATUS
        "halcsclient not found with find_package MODULE mode. Trying CONFIG mode." )
    find_package(halcsclient CONFIG)
endif()
if(halcsclient_FOUND)
    list(APPEND MORE_LIBRARIES ${halcsclient_LIBRARIES})
    if(PC_halcsclient_FOUND)
        list(APPEND OPT_LIBRARIES_STATIC ${PC_halcsclient_STATIC_LDFLAGS})
    endif()
else()
    message(FATAL_ERROR
        "libhalcsclient is not installed. Install it, then run CMake again." )
endif()

#########################################################################
# acqclient dependency
#########################################################################

find_package(acqclient)
if(NOT acqclient_FOUND)
    message(STATUS
        "acqclient not found with find_package MODULE mode. Trying CONFIG mode." )
    find_package(acqclient CONFIG)
endif()
if(acqclient_FOUND)
    list(APPEND MORE_LIBRARIES ${acqclient_LIBRARIES})
    if(PC_acqclient_FOUND)
        list(APPEND OPT_LIBRARIES_STATIC ${PC_acqclient_STATIC_LDFLAGS})
    endif()
else()
    message(FATAL_ERROR
        "libacqclient is not installed. Install it, then run CMake again." )
endif()

#########################################################################
# bpmclient dependency
#########################################################################

find_package(bpmclient)
if(NOT bpmclient_FOUND)
    message(STATUS
        "bpmclient not found with find_package MODULE mode. Trying CONFIG mode." )
    find_package(bpmclient CONFIG)
endif()
if(bpmclient_FOUND)
    list(APPEND MORE_LIBRARIES ${bpmclient_LIBRARIES})
    if(PC_bpmclient_FOUND)
        list(APPEND OPT_LIBRARIES_STATIC ${PC_bpmclient_STATIC_LDFLAGS})
    endif()
else()
    message(FATAL_ERROR
        "libbpmclient is not installed. Install it, then run CMake again." )
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
        list(APPEND OPT_LIBRARIES_STATIC ${PC_mlm_STATIC_LDFLAGS})
    endif()
else()
    message(FATAL_ERROR
        "libmlm is not installed. Install it, then run CMake again." )
endif()

#########################################################################
# libm dependency (possibly)
#########################################################################

include(CheckSymbolExists)

if(NOT FLOOR_FUNCTION_EXISTS AND NOT NEED_LINKING_AGAINST_LIBM)
    check_symbol_exists(floor math.h FLOOR_FUNCTION_EXISTS)
    if(NOT FLOOR_FUNCTION_EXISTS)
        unset(FLOOR_FUNCTION_EXISTS CACHE)
        list(APPEND CMAKE_REQUIRED_LIBRARIES m)
        check_symbol_exists(floor math.h FLOOR_FUNCTION_EXISTS)
        if(FLOOR_FUNCTION_EXISTS)
            set(NEED_LINKING_AGAINST_LIBM ON
                CACHE BOOL
                "Whether we need to link against libm or not" FORCE
            )
        else()
            message(FATAL_ERROR
                "could not find floor function on system libm")
        endif()
    endif()
endif()

if (NEED_LINKING_AGAINST_LIBM)
    list(APPEND MORE_LIBRARIES m)
endif()

########################################################################
# Binaries compile options
########################################################################

add_library(halcs_examples_compiler_flags INTERFACE)
target_compile_features(halcs_examples_compiler_flags
    INTERFACE
    c_std_99
)

# add compiler warning flags just when building this project via
# the BUILD_INTERFACE genex
set(gcc_c "$<$<COMPILE_LANGUAGE:C>:$<C_COMPILER_ID:GNU>>")
set(clang_c "$<$<COMPILE_LANGUAGE:C>:$<C_COMPILER_ID:Clang>>")
target_compile_options(halcs_examples_compiler_flags
    INTERFACE
    "$<${gcc_c}:$<BUILD_INTERFACE:-Wall;-O2;-g;-Wno-missing-field-initializers;-Wno-missing-braces;-Wno-cpp>>"
    "$<${clang_c}:$<BUILD_INTERFACE:-Wall;-O2;-g;-Wno-missing-field-initializers;-Wno-missing-braces;-Wno-error=\#warnings>>"
)

# add binaries to bin_target_outputs
list(APPEND bin_target_outputs "")

# README files
set(readme-docs
    COPYING
)

########################################################################
# Binaries declaration
########################################################################

option(BUILD_EXAMPLE_acq "Whether or not to build example acq tool" ON)

if(BUILD_EXAMPLE_acq)
    list(APPEND bin_target_outputs "acq")
endif()

option(BUILD_EXAMPLE_acq_gen_sw_trigger "Whether or not to build example acq_gen_sw_trigger tool" ON)

if(BUILD_EXAMPLE_acq_gen_sw_trigger)
    list(APPEND bin_target_outputs "acq_gen_sw_trigger")
endif()

option(BUILD_EXAMPLE_acq_prop "Whether or not to build example acq_prop tool" ON)

if(BUILD_EXAMPLE_acq_prop)
    list(APPEND bin_target_outputs "acq_prop")
endif()

option(BUILD_EXAMPLE_acq_sp "Whether or not to build example acq_sp tool" ON)

if(BUILD_EXAMPLE_acq_sp)
    list(APPEND bin_target_outputs "acq_sp")
endif()

option(BUILD_EXAMPLE_acq_trigger "Whether or not to build example acq_trigger tool" ON)

if(BUILD_EXAMPLE_acq_trigger)
    list(APPEND bin_target_outputs "acq_trigger")
endif()

option(BUILD_EXAMPLE_ad9510_ctl "Whether or not to build example ad9510_ctl tool" ON)

if(BUILD_EXAMPLE_ad9510_ctl)
    list(APPEND bin_target_outputs "ad9510_ctl")
endif()

option(BUILD_EXAMPLE_adc_data "Whether or not to build example adc_data tool" ON)

if(BUILD_EXAMPLE_adc_data)
    list(APPEND bin_target_outputs "adc_data")
endif()

option(BUILD_EXAMPLE_adc_dly "Whether or not to build example adc_dly tool" ON)

if(BUILD_EXAMPLE_adc_dly)
    list(APPEND bin_target_outputs "adc_dly")
endif()

option(BUILD_EXAMPLE_dsp_swap "Whether or not to build example dsp_swap tool" ON)

if(BUILD_EXAMPLE_dsp_swap)
    list(APPEND bin_target_outputs "dsp_swap")
endif()

option(BUILD_EXAMPLE_fmc100m_4ch_ctl "Whether or not to build example fmc100m_4ch_ctl tool" ON)

if(BUILD_EXAMPLE_fmc100m_4ch_ctl)
    list(APPEND bin_target_outputs "fmc100m_4ch_ctl")
endif()

option(BUILD_EXAMPLE_fmc130m_4ch_ctl "Whether or not to build example fmc130m_4ch_ctl tool" ON)

if(BUILD_EXAMPLE_fmc130m_4ch_ctl)
    list(APPEND bin_target_outputs "fmc130m_4ch_ctl")
endif()

option(BUILD_EXAMPLE_fmc130m_eeprom_ctl "Whether or not to build example fmc130m_eeprom_ctl tool" ON)

if(BUILD_EXAMPLE_fmc130m_eeprom_ctl)
    list(APPEND bin_target_outputs "fmc130m_eeprom_ctl")
endif()

option(BUILD_EXAMPLE_fmc250m_4ch_sleep "Whether or not to build example fmc250m_4ch_sleep tool" ON)

if(BUILD_EXAMPLE_fmc250m_4ch_sleep)
    list(APPEND bin_target_outputs "fmc250m_4ch_sleep")
endif()

option(BUILD_EXAMPLE_fmc250m_4ch_test_mode "Whether or not to build example fmc250m_4ch_test_mode tool" ON)

if(BUILD_EXAMPLE_fmc250m_4ch_test_mode)
    list(APPEND bin_target_outputs "fmc250m_4ch_test_mode")
endif()

option(BUILD_EXAMPLE_fmc_active_clk_ad9510_ctl "Whether or not to build example fmc_active_clk_ad9510_ctl tool" ON)

if(BUILD_EXAMPLE_fmc_active_clk_ad9510_ctl)
    list(APPEND bin_target_outputs "fmc_active_clk_ad9510_ctl")
endif()

option(BUILD_EXAMPLE_fpga_info "Whether or not to build example fpga_info tool" ON)

if(BUILD_EXAMPLE_fpga_info)
    list(APPEND bin_target_outputs "fpga_info")
endif()

option(BUILD_EXAMPLE_leds "Whether or not to build example leds tool" ON)

if(BUILD_EXAMPLE_leds)
    list(APPEND bin_target_outputs "leds")
endif()

option(BUILD_EXAMPLE_monit_amp "Whether or not to build example monit_amp tool" ON)

if(BUILD_EXAMPLE_monit_amp)
    list(APPEND bin_target_outputs "monit_amp")
endif()

option(BUILD_EXAMPLE_monit_pos "Whether or not to build example monit_pos tool" ON)

if(BUILD_EXAMPLE_monit_pos)
    list(APPEND bin_target_outputs "monit_pos")
endif()

option(BUILD_EXAMPLE_rffe_ctl "Whether or not to build example rffe_ctl tool" ON)

if(BUILD_EXAMPLE_rffe_ctl)
    list(APPEND bin_target_outputs "rffe_ctl")
endif()

option(BUILD_EXAMPLE_si571_ctl "Whether or not to build example si571_ctl tool" ON)

if(BUILD_EXAMPLE_si571_ctl)
    list(APPEND bin_target_outputs "si571_ctl")
endif()

option(BUILD_EXAMPLE_test_data_en "Whether or not to build example test_data_en tool" ON)

if(BUILD_EXAMPLE_test_data_en)
    list(APPEND bin_target_outputs "test_data_en")
endif()

option(BUILD_EXAMPLE_timing "Whether or not to build example timing tool" ON)

if(BUILD_EXAMPLE_timing)
    list(APPEND bin_target_outputs "timing")
endif()

option(BUILD_EXAMPLE_tim_rcv "Whether or not to build example tim_rcv tool" ON)

if(BUILD_EXAMPLE_tim_rcv)
    list(APPEND bin_target_outputs "tim_rcv")
endif()

option(BUILD_EXAMPLE_trigger "Whether or not to build example trigger tool" ON)

if(BUILD_EXAMPLE_trigger)
    list(APPEND bin_target_outputs "trigger")
endif()

option(BUILD_EXAMPLE_fofb_ctrl "Whether or not to build example fofb_ctrl tool" ON)

if(BUILD_EXAMPLE_fofb_ctrl)
    list(APPEND bin_target_outputs "fofb_ctrl")
endif()

option(BUILD_EXAMPLE_rtmlamp_ohwr_ctl "Whether or not to build example rtmlamp_ohwr_ctl tool" ON)

if(BUILD_EXAMPLE_rtmlamp_ohwr_ctl)
    list(APPEND bin_target_outputs "rtmlamp_ohwr_ctl")
endif()

foreach(bin_target ${bin_target_outputs})
    # add binary tool
    add_executable(${bin_target} src/${bin_target}.c)

    # add the binary tree to the search path for include files
    # so that we will find our headers
    target_include_directories(${bin_target}
        PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:include>)

    # set debug posfix in case of debug builds
    set_target_properties(${bin_target} PROPERTIES
        DEBUG_POSTFIX
        ${CMAKE_DEBUG_POSTFIX}
    )

    # set compiler flags
    target_link_libraries(${bin_target}
        PRIVATE
        halcs_examples_compiler_flags
    )

    # library dependencies
    target_link_libraries(${bin_target}
        PRIVATE
        ${MORE_LIBRARIES}
    )

    # install binary
    install(TARGETS ${bin_target}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT Tools)
endforeach()

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
# Other files installer
########################################################################

# Convert README files in *.txt ones
foreach(readme ${readme-docs})
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${readme}
        ${CMAKE_CURRENT_BINARY_DIR}/${readme}.txt
    )
endforeach()

# CPack rules
option(ENABLE_CPACK "Enables cpack rules" ON)

if(ENABLE_CPACK)
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
    set(CPACK_PACKAGE_NAME "halcs_examples")
    set(halcs_examples_DISTRO_VERSION "" CACHE STRING "halcs_examples distribution version")
    string(APPEND halcs_examples_VERSION "${halcs_examples_DISTRO_VERSION}")
    set(CPACK_PACKAGE_FILE_NAME
        "${CPACK_PACKAGE_NAME}_${halcs_examples_VERSION}_${arch_name}"
    )
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "SDB filesystem library")
    set(CPACK_PACKAGE_VENDOR "LNLS")
    set(CPACK_PACKAGE_CONTACT "Lucas Russo <lucas.russo@lnls.br>")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_BINARY_DIR}/COPYING.txt")
    # set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_BINARY_DIR}/README.txt")
    # set(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_CURRENT_BINARY_DIR}/WELCOME.txt")
    # set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/branding.bmp")
    set(CPACK_PACKAGE_VERSION ${halcs_examples_VERSION})
    set(CPACK_PACKAGE_VERSION_MAJOR ${halcs_examples_VERSION_MAJOR})
    set(CPACK_PACKAGE_VERSION_MINOR ${halcs_examples_VERSION_MINOR})
    set(CPACK_PACKAGE_VERSION_PATCH ${halcs_examples_VERSION_PATCH})
    # set(CPACK_PACKAGE_INSTALL_DIRECTORY "HALCS examples Install Directory")
    # set(CPACK_TEMPORARY_DIRECTORY "HALCS examples Temporary CPack Directory")

    include(CPack)

    cpack_add_component_group(Development
        DISPLAY_NAME "HALCS examples library"
        EXPANDED
    )
    cpack_add_component(SourceCode
        DISPLAY_NAME "HALCS examples source code"
        DISABLED
        INSTALL_TYPES FullInstall
    )
    cpack_add_component(SDK
        DISPLAY_NAME "HALCS examples headers and libraries"
        INSTALL_TYPES FullInstall DevInstall
        GROUP Development
    )
    cpack_add_component(Runtime
        DISPLAY_NAME "HALCS examples runtime files"
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
