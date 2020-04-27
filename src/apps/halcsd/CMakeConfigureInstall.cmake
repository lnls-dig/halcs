#
# CMakeConfigureInstall.cmake
#
# This script calls configure_file() at CMake install time, making use of
# DESTDIR and CMAKE_INSTALL_PREFIX as appropriate. This is particulartly useful
# when cofiguring files that needs to have the absolute path of some binary
# that could be relocated according to the user or cpack, for instance
#
# This script can be used as part of the build via ADD_CUSTOM_COMMAND, or used
# only during make install via INSTALL SCRIPT.
#

include(GNUInstallDirs)

if(NOT DEFINED input_file)
    message(FATAL_ERROR "${CMAKE_CURRENT_LIST_FILE}(${CMAKE_CURRENT_LIST_LINE}): error: Variable input_file is not defined.")
endif()

if(NOT DEFINED output_file)
    message(FATAL_ERROR "${CMAKE_CURRENT_LIST_FILE}(${CMAKE_CURRENT_LIST_LINE}): error: Variable output_file is not defined.")
endif()

if(NOT DEFINED SYSTEMD_SERVICES_INSTALL_DIR)
    message(FATAL_ERROR "${CMAKE_CURRENT_LIST_FILE}(${CMAKE_CURRENT_LIST_LINE}): error: Variable SYSTEMD_SERVICES_INSTALL_DIR is not defined.")
endif()

if(NOT DEFINED SYSTEMD_CONF_INSTALL_DIR)
    message(FATAL_ERROR "${CMAKE_CURRENT_LIST_FILE}(${CMAKE_CURRENT_LIST_LINE}): error: Variable SYSTEMD_CONF_INSTALL_DIR is not defined.")
endif()

if(NOT DEFINED NOPREFIX_UDEV_SYSCONFDIR)
    message(FATAL_ERROR "${CMAKE_CURRENT_LIST_FILE}(${CMAKE_CURRENT_LIST_LINE}): error: Variable NOPREFIX_UDEV_SYSCONFDIR is not defined.")
endif()

if(NOT DEFINED NOPREFIX_HALCS_SYSCONFDIR)
    message(FATAL_ERROR "${CMAKE_CURRENT_LIST_FILE}(${CMAKE_CURRENT_LIST_LINE}): error: Variable NOPREFIX_HALCS_SYSCONFDIR is not defined.")
endif()

if(NOT DEFINED CMAKE_INSTALL_FULL_BINDIR)
    message(FATAL_ERROR "${CMAKE_CURRENT_LIST_FILE}(${CMAKE_CURRENT_LIST_LINE}): error: Variable CMAKE_INSTALL_FULL_BINDIR is not defined.")
endif()

if(NOT DEFINED CMAKE_INSTALL_FULL_DATADIR)
    message(FATAL_ERROR "${CMAKE_CURRENT_LIST_FILE}(${CMAKE_CURRENT_LIST_LINE}): error: Variable CMAKE_INSTALL_FULL_DATADIR is not defined.")
endif()

if(NOT DEFINED CMAKE_INSTALL_FULL_SYSCONFDIR)
    message(FATAL_ERROR "${CMAKE_CURRENT_LIST_FILE}(${CMAKE_CURRENT_LIST_LINE}): error: Variable CMAKE_INSTALL_FULL_SYSCONFDIR is not defined.")
endif()

# if we are in a cpack environment, cpack will first use DESTDIR to create a local build
# and after it will relocate them to build the final package. This is a problem for
# us, as we need to set DESTDIR to the final paths as we need it for setting
# absolute paths in .service units and udev rules.
#
# default CMAKE_FINAL_DESTDIR if not set by CPACK_PROJECT_CONFIG_FILE
set(CMAKE_FINAL_DESTDIR $ENV{DESTDIR})
set(CPACK_PROJECT_CONFIG_FILE ${CMAKE_CURRENT_SOURCE_DIR}/CMakeFixCPackPaths.cmake)

# prepend all variables with our CMAKE_FINAL_DESTDIR
set(halcsd_SYSTEMD_SERVICES_INSTALL_DIR "${CMAKE_FINAL_DESTDIR}${SYSTEMD_SERVICES_INSTALL_DIR}")
set(halcsd_SYSTEMD_CONF_INSTALL_DIR "${CMAKE_FINAL_DESTDIR}${SYSTEMD_CONF_INSTALL_DIR}")

set(halcsd_CMAKE_INSTALL_FULL_BINDIR "${CMAKE_FINAL_DESTDIR}${CMAKE_INSTALL_FULL_BINDIR}")
set(halcsd_CMAKE_INSTALL_FULL_DATADIR "${CMAKE_FINAL_DESTDIR}${CMAKE_INSTALL_FULL_DATADIR}")
set(halcsd_CMAKE_INSTALL_FULL_SYSCONFDIR "${CMAKE_FINAL_DESTDIR}${CMAKE_INSTALL_FULL_SYSCONFDIR}")
set(halcsd_HALCS_INSTALL_FULL_SYSCONFDIR "${CMAKE_FINAL_DESTDIR}${CMAKE_INSTALL_FULL_SYSCONFDIR}/${NOPREFIX_HALCS_SYSCONFDIR}")
set(halcsd_UDEV_INSTALL_FULL_SYSCONFDIR "${CMAKE_FINAL_DESTDIR}${CMAKE_INSTALL_FULL_SYSCONFDIR}/${NOPREFIX_UDEV_SYSCONFDIR}")

configure_file(${input_file}
    ${output_file}
    @ONLY
)
