########################################################################
# Binary setup
########################################################################

# detect target architecture
include(TargetArch)
# set installation variables compliant with distributions
include(GNUInstallDirs)
# Get systemd service directories'
include(TestSystemd)
# extract version numbers from header file
include(TestHalcsVersion)

target_architecture(ARCH)

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
    "UDEV rules system prefix (typically /etc ou /usr/lib)")

set(NOPREFIX_UDEV_SYSCONFDIR "udev/rules.d")

set(UDEV_SYSCONFDIR "${UDEV_RULES_PREFIX}/${NOPREFIX_UDEV_SYSCONFDIR}")

# relative directory for halcs config files
set(NOPREFIX_HALCS_SYSCONFDIR "halcs")

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
        set(halcs_generic_udev_INSTALL_DATADIR ${CMAKE_INSTALL_DATADIR}/halcs)
        install(DIRECTORY DESTINATION ${halcs_generic_udev_INSTALL_DATADIR})

        # general scripts
        install(FILES
            ${CMAKE_CURRENT_SOURCE_DIR}/init-generic/share/halcs/scripts/find-script.sh
            ${CMAKE_CURRENT_SOURCE_DIR}/init-generic/share/halcs/scripts/get-fpga-gateware-properties.sh
            ${CMAKE_CURRENT_SOURCE_DIR}/init-generic/share/halcs/scripts/get-halcs-idxs.sh
            ${CMAKE_CURRENT_SOURCE_DIR}/init-generic/share/halcs/scripts/run-fpga-program.sh
            ${CMAKE_CURRENT_SOURCE_DIR}/init-generic/share/halcs/scripts/stop-fpga-program.sh
            DESTINATION ${halcs_generic_udev_INSTALL_DATADIR}
            PERMISSIONS
            OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
        )

        # udev files. Use an install script so we are able to change the
        # absolute paths of systemd unit files on install, which can be
        # prefixed by CMAKE_INSTALL_PREFIX. SYSTEMD variables are always fixed
        # to an absolute path.
        install(CODE "set(SYSTEMD_SERVICES_INSTALL_DIR ${SYSTEMD_SERVICES_INSTALL_DIR})")
        install(CODE "set(SYSTEMD_CONF_INSTALL_DIR ${SYSTEMD_CONF_INSTALL_DIR})")
        install(CODE "set(UDEV_SYSCONFDIR ${UDEV_SYSCONFDIR})")
        install(CODE "set(NOPREFIX_HALCS_SYSCONFDIR ${NOPREFIX_HALCS_SYSCONFDIR})")

        # udev rules
        install(CODE "set(input_file
            \"${CMAKE_CURRENT_SOURCE_DIR}/init/systemd/etc/udev/rules.d/50-udev_fpga.rules.in\")")
        install(CODE "set(output_file
            \"${CMAKE_CURRENT_BINARY_DIR}/init/systemd/50-udev_fpga.rules\")")
        install(SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfigureInstall.cmake")

        install(FILES
            ${CMAKE_CURRENT_BINARY_DIR}/init/systemd/50-udev_fpga.rules
            DESTINATION ${UDEV_SYSCONFDIR}
            PERMISSIONS
            OWNER_READ OWNER_WRITE GROUP_READ GROUP_WRITE WORLD_READ
        )
    endif()
endif()

# set list to global, as linker options are passed through this
set(apps_halcs_generic_udev ${apps_halcs_generic_udev} PARENT_SCOPE)

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
    set(CPACK_RPM_PACKAGE_AUTOREQ yes)
    set(CPACK_RPM_PACKAGE_AUTOPROV yes)
    set(CPACK_PACKAGE_NAME "halcs_generic_udev")
    set(CPACK_PACKAGE_FILE_NAME
        "${CPACK_PACKAGE_NAME}-${halcs_VERSION}-${arch_name}"
    )
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "HALCS generic udev scripts")
    set(CPACK_PACKAGE_VENDOR "LNLS")
    set(CPACK_PACKAGE_CONTACT "Lucas Russo <lucas.russo@lnls.br>")
    #set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_BINARY_DIR}/COPYING.txt")
    # set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_BINARY_DIR}/README.txt")
    # set(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_CURRENT_BINARY_DIR}/WELCOME.txt")
    # set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/branding.bmp")
    set(CPACK_PACKAGE_VERSION ${halcs_VERSION})
    set(CPACK_PACKAGE_VERSION_MAJOR ${halcs_VERSION_MAJOR})
    set(CPACK_PACKAGE_VERSION_MINOR ${halcs_VERSION_MINOR})
    set(CPACK_PACKAGE_VERSION_PATCH ${halcs_VERSION_PATCH})
    # set(CPACK_PACKAGE_INSTALL_DIRECTORY "HALCS generic udev Install Directory")
    # set(CPACK_TEMPORARY_DIRECTORY "HALCS generic udev Temporary CPack Directory")

    include(CPack)

    cpack_add_component_group(Development
        DISPLAY_NAME "HALCS generic udev scripts"
        EXPANDED
    )
    cpack_add_component(SourceCode
        DISPLAY_NAME "HALCS generic udev source code"
        DISABLED
        INSTALL_TYPES FullInstall
    )
    cpack_add_component(SDK
        DISPLAY_NAME "HALCS generic udev headers and libraries"
        INSTALL_TYPES FullInstall DevInstall
        GROUP Development
    )
    cpack_add_component(Runtime
        DISPLAY_NAME "HALCS generic udev runtime files"
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
