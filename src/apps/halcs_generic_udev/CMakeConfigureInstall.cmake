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

if(NOT DEFINED UDEV_SYSCONFDIR)
    message(FATAL_ERROR "${CMAKE_CURRENT_LIST_FILE}(${CMAKE_CURRENT_LIST_LINE}): error: Variable UDEV_SYSCONFDIR is not defined.")
endif()

if(NOT DEFINED NOPREFIX_HALCS_SYSCONFDIR)
    message(FATAL_ERROR "${CMAKE_CURRENT_LIST_FILE}(${CMAKE_CURRENT_LIST_LINE}): error: Variable NOPREFIX_HALCS_SYSCONFDIR is not defined.")
endif()

if(NOT DEFINED NOPREFIX_HALCS_DATADIR)
    message(FATAL_ERROR "${CMAKE_CURRENT_LIST_FILE}(${CMAKE_CURRENT_LIST_LINE}): error: Variable NOPREFIX_HALCS_DATADIR is not defined.")
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

# None of the variables will add DESDIR to the files. This is on purpose, as
# on a CPack packaging there is no way to distinguish the intermediate CPack
# DESTDIR variable from a non CPack packaging (e.g., make DESDIR=<dir> install)
set(halcs_generic_udev_SYSTEMD_SERVICES_INSTALL_DIR "${SYSTEMD_SERVICES_INSTALL_DIR}")
set(halcs_generic_udev_SYSTEMD_CONF_INSTALL_DIR "${SYSTEMD_CONF_INSTALL_DIR}")

set(halcs_generic_udev_CMAKE_INSTALL_FULL_BINDIR "${CMAKE_INSTALL_FULL_BINDIR}")
set(halcs_generic_udev_CMAKE_INSTALL_FULL_DATADIR "${CMAKE_INSTALL_FULL_DATADIR}")
set(halcs_generic_udev_CMAKE_INSTALL_FULL_SYSCONFDIR "${CMAKE_INSTALL_FULL_SYSCONFDIR}")
set(halcs_generic_udev_HALCS_INSTALL_FULL_SYSCONFDIR "${CMAKE_INSTALL_FULL_SYSCONFDIR}/${NOPREFIX_HALCS_SYSCONFDIR}")
set(halcs_generic_udev_CMAKE_INSTALL_FULL_DATADIR "${CMAKE_INSTALL_FULL_DATADIR}")
set(halcs_generic_udev_HALCS_INSTALL_FULL_DATADIR "${CMAKE_INSTALL_FULL_DATADIR}/${NOPREFIX_HALCS_DATADIR}")
set(halcs_generic_udev_UDEV_INSTALL_FULL_SYSCONFDIR "${UDEV_SYSCONFDIR}")

configure_file(${input_file}
    ${output_file}
    @ONLY
)
