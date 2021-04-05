########################################################################
# Binary setup
########################################################################

# set installation variables compliant with distributions
include(GNUInstallDirs)
# Get systemd service directories'
include(TestSystemd)

# halcs_generic_udev basename
set (halcs_generic_udev_OUTPUT_BASENAME "halcs_generic_udev"
    CACHE STRING
    "Output halcs_generic_udev binary base name"
)

#########################################################################
# library dependencies
#########################################################################

########################################################################
# User compile-time options
########################################################################

option(BUILD_halcs_generic_udev "Whether or not to build halcs_generic_udev" ON)

if(BUILD_halcs_generic_udev)
    list(APPEND apps_halcs_generic_udev "halcs_generic_udev")
endif()

#######################################
# HALCS_GENERIC_UDEV
#######################################

# if we want to install idev rules or not
option(halcs_generic_udev_INSTALL_UDEV_RULES
    "Install udev rules for halcs_generic_udev"
    ON
)

# udev directories
set(UDEV_RULES_PREFIX "/etc" CACHE STRING
    "UDEV rules system prefix (typically /etc or /usr/lib)")

# relative directory for udev rules
set(NOPREFIX_UDEV_SYSCONFDIR "udev/rules.d")

set(UDEV_SYSCONFDIR "${UDEV_RULES_PREFIX}/${NOPREFIX_UDEV_SYSCONFDIR}")

# halcs config directory
set(HALCS_CFG_PREFIX "/etc" CACHE STRING
    "HALCS config files prefix (typically /etc, /usr/etc or /usr/local/etc)")

# relative directory for halcs config files
set(NOPREFIX_HALCS_SYSCONFDIR "halcs")

set(HALCS_SYSCONFDIR ${HALCS_CFG_PREFIX}/${NOPREFIX_HALCS_SYSCONFDIR})

# relative directory for halcs data files
set(NOPREFIX_HALCS_DATADIR "halcs")

########################################################################
# Library compile options
########################################################################

########################################################################
# Sources/Headers definitions
########################################################################

########################################################################
# Binaries declaration
########################################################################

if(BUILD_halcs_generic_udev)
    # install config files
    if(halcs_generic_udev_INSTALL_UDEV_RULES)
        # general scripts location
        set(halcs_generic_udev_INSTALL_DATADIR ${CMAKE_INSTALL_DATADIR}/${NOPREFIX_HALCS_DATADIR})
        install(DIRECTORY
            DESTINATION ${halcs_generic_udev_INSTALL_DATADIR}
            COMPONENT Scripts
        )

        # general scripts
        install(FILES
            ${CMAKE_CURRENT_SOURCE_DIR}/init-generic/share/halcs/scripts/find-script.sh
            ${CMAKE_CURRENT_SOURCE_DIR}/init-generic/share/halcs/scripts/get-fpga-gateware-properties.sh
            ${CMAKE_CURRENT_SOURCE_DIR}/init-generic/share/halcs/scripts/get-halcs-idxs.sh
            ${CMAKE_CURRENT_SOURCE_DIR}/init-generic/share/halcs/scripts/run-fpga-program.sh
            ${CMAKE_CURRENT_SOURCE_DIR}/init-generic/share/halcs/scripts/stop-fpga-program.sh
            DESTINATION ${halcs_generic_udev_INSTALL_DATADIR}
            COMPONENT Scripts
            PERMISSIONS
            OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
        )

        # udev files. Use an install script so we are able to change the
        # absolute paths of systemd unit files on install, which can be
        # prefixed by CMAKE_INSTALL_PREFIX. SYSTEMD variables are always fixed
        # to an absolute path.
        install(CODE "set(SYSTEMD_SERVICES_INSTALL_DIR ${SYSTEMD_SERVICES_INSTALL_DIR})"
            COMPONENT Scripts
        )
        install(CODE "set(SYSTEMD_CONF_INSTALL_DIR ${SYSTEMD_CONF_INSTALL_DIR})"
            COMPONENT Scripts
        )
        install(CODE "set(HALCS_SYSCONFDIR ${HALCS_SYSCONFDIR})"
            COMPONENT Scripts
        )
        install(CODE "set(UDEV_SYSCONFDIR ${UDEV_SYSCONFDIR})"
            COMPONENT Scripts
        )
        install(CODE "set(NOPREFIX_HALCS_SYSCONFDIR ${NOPREFIX_HALCS_SYSCONFDIR})"
            COMPONENT Scripts
        )
        install(CODE "set(NOPREFIX_HALCS_DATADIR ${NOPREFIX_HALCS_DATADIR})"
            COMPONENT Scripts
        )

        # udev rules
        install(CODE "set(input_file
            \"${CMAKE_CURRENT_SOURCE_DIR}/init/systemd/etc/udev/rules.d/81-udev_fpga.rules.in\")"
            COMPONENT Scripts
        )
        install(CODE "set(output_file
            \"${CMAKE_CURRENT_BINARY_DIR}/init/systemd/81-udev_fpga.rules\")"
            COMPONENT Scripts
        )
        install(SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfigureInstall.cmake"
            COMPONENT Scripts
        )

        install(FILES
            ${CMAKE_CURRENT_BINARY_DIR}/init/systemd/81-udev_fpga.rules
            DESTINATION ${UDEV_SYSCONFDIR}
            COMPONENT Scripts
            PERMISSIONS
            OWNER_READ OWNER_WRITE GROUP_READ GROUP_WRITE WORLD_READ
        )
    endif()
endif()

# set list to global, as linker options are passed through this
set(apps_halcs_generic_udev ${apps_halcs_generic_udev} PARENT_SCOPE)
