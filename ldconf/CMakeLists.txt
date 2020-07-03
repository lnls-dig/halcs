########################################################################
# Project setup
########################################################################

cmake_minimum_required(VERSION 3.10)

# set installation variables compliant with distributions
include(GNUInstallDirs)

########################################################################
# Scripts declaration
########################################################################

option(INSTALL_LDCONFIG_halcs "Whether or not to install halcs ldconfig script" ON)

# ld.so.cond directories
set(LDCONF_RULES_PREFIX "/etc" CACHE STRING
    "LDCONF rules system prefix (typically /etc)")

# relative directory for udev rules
set(NOPREFIX_LDCONF_SYSCONFDIR "ld.so.conf.d")

set(LDCONF_SYSCONFDIR "${LDCONF_RULES_PREFIX}/${NOPREFIX_LDCONF_SYSCONFDIR}")

if(INSTALL_LDCONFIG_halcs)

    configure_file(
      "${CMAKE_CURRENT_SOURCE_DIR}/halcs.conf.in"
      "${CMAKE_CURRENT_BINARY_DIR}/ldconf/halcs.conf"
      @ONLY)

    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/ldconf/halcs.conf
        DESTINATION ${LDCONF_SYSCONFDIR}
        PERMISSIONS
        OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
    )
endif()
