########
# Find systemd service dir
#
# from: https://github.com/ximion/limba/blob/master/data/cmake/systemdservice.cmake

pkg_check_modules(SYSTEMD "systemd")
if (SYSTEMD_FOUND AND "${SYSTEMD_SERVICES_INSTALL_DIR}" STREQUAL "" AND
        "${SYSTEMD_CONF_INSTALL_DIR}" STREQUAL "")
    # service files
    execute_process(COMMAND ${PKG_CONFIG_EXECUTABLE}
        --variable=systemdsystemunitdir systemd
        OUTPUT_VARIABLE SYSTEMD_SERVICES_INSTALL_DIR)
    string(REGEX REPLACE "[ \t\n]+" "" SYSTEMD_SERVICES_INSTALL_DIR
        "${SYSTEMD_SERVICES_INSTALL_DIR}")
    # conf files
    execute_process(COMMAND ${PKG_CONFIG_EXECUTABLE}
        --variable=systemdsystemconfdir systemd
        OUTPUT_VARIABLE SYSTEMD_CONF_INSTALL_DIR)
    string(REGEX REPLACE "[ \t\n]+" "" SYSTEMD_CONF_INSTALL_DIR
        "${SYSTEMD_CONF_INSTALL_DIR}")
elseif (NOT SYSTEMD_FOUND AND SYSTEMD_SERVICES_INSTALL_DIR AND
        SYSTEMD_CONF_INSTALL_DIR)
    message (FATAL_ERROR "Variable SYSTEMD_SERVICES_INSTALL_DIR and \
        SYSTEMD_CONF_INSTALL_DIR are defined, but we can't find systemd \
        using pkg-config")
endif()

if (SYSTEMD_FOUND)
    set(WITH_SYSTEMD "ON")
    message(STATUS "systemd services install dir: ${SYSTEMD_SERVICES_INSTALL_DIR}")
    message(STATUS "systemd configuration files install dir: ${SYSTEMD_CONF_INSTALL_DIR}")
else()
    set(WITH_SYSTEMD "OFF")
endif (SYSTEMD_FOUND)
