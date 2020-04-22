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

if(NOT DEFINED CMAKE_INSTALL_FULL_BINDIR)
    message(FATAL_ERROR "${CMAKE_CURRENT_LIST_FILE}(${CMAKE_CURRENT_LIST_LINE}): error: Variable MAKE_INSTALL_FULL_BINDIR is not defined.")
endif()

if(NOT DEFINED CMAKE_INSTALL_FULL_DATADIR)
    message(FATAL_ERROR "${CMAKE_CURRENT_LIST_FILE}(${CMAKE_CURRENT_LIST_LINE}): error: Variable MAKE_INSTALL_FULL_DATADIR is not defined.")
endif()

if(NOT DEFINED CMAKE_INSTALL_FULL_SYSCONFDIR)
    message(FATAL_ERROR "${CMAKE_CURRENT_LIST_FILE}(${CMAKE_CURRENT_LIST_LINE}): error: Variable MAKE_INSTALL_FULL_SYSCONFDIR is not defined.")
endif()

if(NOT DEFINED SYSTEMD_SERVICES_INSTALL_DIR_SET)
    set(SYSTEMD_SERVICES_INSTALL_DIR "$ENV{DESTDIR}${SYSTEMD_SERVICES_INSTALL_DIR}")
    set(SYSTEMD_SERVICES_INSTALL_DIR_SET ON)
endif()

if(NOT DEFINED SYSTEMD_CONF_INSTALL_DIR_SET)
    set(SYSTEMD_CONF_INSTALL_DIR "$ENV{DESTDIR}${SYSTEMD_CONF_INSTALL_DIR}")
    set(SYSTEMD_CONF_INSTALL_DIR_SET ON)
endif()

if(NOT DEFINED CMAKE_INSTALL_FULL_BINDIR_SET)
    set(CMAKE_INSTALL_FULL_BINDIR "$ENV{DESTDIR}${CMAKE_INSTALL_FULL_BINDIR}")
    set(CMAKE_INSTALL_FULL_BINDIR_SET ON)
endif()

if(NOT DEFINED CMAKE_INSTALL_FULL_DATADIR_SET)
    set(CMAKE_INSTALL_FULL_DATADIR "$ENV{DESTDIR}${CMAKE_INSTALL_FULL_DATADIR}")
    set(CMAKE_INSTALL_FULL_DATADIR_SET ON)
endif()

if(NOT DEFINED CMAKE_INSTALL_FULL_SYSCONFDIR_SET)
    set(CMAKE_INSTALL_FULL_SYSCONFDIR "$ENV{DESTDIR}${CMAKE_INSTALL_FULL_SYSCONFDIR}")
    set(CMAKE_INSTALL_FULL_SYSCONFDIR_SET ON)
endif()

configure_file(${input_file}
    ${output_file}
    @ONLY
)
