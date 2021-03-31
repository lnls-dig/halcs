#######################################
# Library definition
#######################################

# set installation variables compliant with distributions
include(GNUInstallDirs)
# get pciedriver version
include(TestPciedriverVersion)
# add external project
include(ExternalProject)
# detect target architecture
include(TargetArch)

target_architecture(ARCH)

ExternalProject_Add(pciedriver_ext
    URL ${CMAKE_CURRENT_SOURCE_DIR}/foreign/pcie-driver
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/pciedriver
    CONFIGURE_COMMAND ""
    # Build only library, not the driver
    BUILD_COMMAND make clean && make lib_driver
    INSTALL_COMMAND ""
    # PCIEDRIVER assumes in-source build
    BUILD_IN_SOURCE ON
)

ExternalProject_Get_property(pciedriver_ext BINARY_DIR)

# this is not a CMake project, so no *Config.cmake or *Targets.cmake
# files will be generated. Hardcode library and include paths so when
# libraries try to search for pciedriver they can find it locally!
set(pciedriver_LIBRARY_DIRECTORIES
    ${BINARY_DIR}/lib/pcie
)

set(pciedriver_INCLUDE_DIRECTORIES
    ${BINARY_DIR}/include/pciDriver/lib
    ${BINARY_DIR}/include/pciDriver/lib/../..
)

# Hack to make it work, otherwise INTERFACE_INCLUDE_DIRECTORIES will not be propagated
file(MAKE_DIRECTORY ${pciedriver_INCLUDE_DIRECTORIES})

set(pciedriver_public_headers
    ${BINARY_DIR}/include/pciDriver/lib/Exception.h
    ${BINARY_DIR}/include/pciDriver/lib/KernelMemory.h
    ${BINARY_DIR}/include/pciDriver/lib/Pcidefs.h
    ${BINARY_DIR}/include/pciDriver/lib/PciDevice.h
    ${BINARY_DIR}/include/pciDriver/lib/pciDriver.h
    ${BINARY_DIR}/include/pciDriver/lib/pciDriver_compat.h
    ${BINARY_DIR}/include/pciDriver/lib/UserMemory.h
)

# Get filenames only so we install only those listed above
list(APPEND pciedriver_public_files "")

foreach(public_dir_header ${pciedriver_public_headers})
    get_filename_component(public_dir
        ${public_dir_header}
        NAME
    )

    list(APPEND pciedriver_public_files ${public_dir})
endforeach()

# remove, well, duplicates ...
list(REMOVE_DUPLICATES pciedriver_public_files)

# Get directory names only. In this case, we must install
# pciDriver/lib directory structure, so list them here
# separately
list(APPEND pciedriver_public_dirs
    ${BINARY_DIR}/include/pciDriver
)

# add imported library so we can link to it later
add_library(pciedriver SHARED IMPORTED)

# set library locations
set_target_properties(pciedriver
    PROPERTIES
    IMPORTED_LOCATION
    ${pciedriver_LIBRARY_DIRECTORIES}/libpcidriver.so
    INTERFACE_INCLUDE_DIRECTORIES
    "${pciedriver_INCLUDE_DIRECTORIES}"
    PUBLIC_HEADER
    ${pciedriver_public_headers}
)

add_dependencies(pciedriver pciedriver_ext)

#######################################
# Library installer
#######################################

set(VERSION
    ${pciedriver_VERSION_MAJOR}.${pciedriver_VERSION_MINOR}.${pciedriver_VERSION_PATCH}
)

# install headers that are inside another directory
install(DIRECTORY
    ${pciedriver_public_dirs}
    DESTINATION
    ${CMAKE_INSTALL_INCLUDEDIR}
    COMPONENT Libs
    FILES_MATCHING PATTERN
    "*.h"
)

# as we can't intall IMPORTED libraries as TARGETS, just use plain
# install(FILES)
install(FILES
    ${pciedriver_LIBRARY_DIRECTORIES}/libpcidriver.so
    ${pciedriver_LIBRARY_DIRECTORIES}/libpcidriver.a
    DESTINATION
    ${CMAKE_INSTALL_LIBDIR}
    COMPONENT Libs
)

#######################################
# Driver definition
#######################################

option(BUILD_PCIE_DRIVER "Install PCIe driver" ON)

if(BUILD_PCIE_DRIVER)

    set(pciedriver_driver_ext_DIR
        ${CMAKE_CURRENT_SOURCE_DIR}/foreign/pcie-driver
    )

    # Driver directory
    set(pciedriver_driver_KO_DIRECTORY
        ${pciedriver_driver_ext_DIR}/drivers/pcie/src
    )

    # DKMS install directory
    set (pciedriver_DKMS_INSTALL_DIR "/usr/src/pciDriver-${VERSION}")

    # DKMS files
    set(DKMS_FILE_NAME "dkms.conf")
    set(DKMS_POSTINST "postinst")
    set(DKMS_PRERM "prerm")

    configure_file (
        "${pciedriver_driver_KO_DIRECTORY}/${DKMS_FILE_NAME}.in"
        "${CMAKE_CURRENT_BINARY_DIR}/${DKMS_FILE_NAME}"
        @ONLY
    )

    set(PCIDRIVER_VERSION_STRING
        ${VERSION}
    )

    configure_file (
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/${DKMS_POSTINST}.in"
        "${CMAKE_CURRENT_BINARY_DIR}/${DKMS_POSTINST}"
        @ONLY
    )

    configure_file (
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/${DKMS_PRERM}.in"
        "${CMAKE_CURRENT_BINARY_DIR}/${DKMS_PRERM}"
        @ONLY
    )


    #######################################
    # Driver installer
    #######################################

    # FIXME. Get all filenames REALPATHs?
    get_filename_component(pciedriver_driver_HEADER_ABS
        ${pciedriver_driver_KO_DIRECTORY}/pciDriver.h
        REALPATH
    )

    get_filename_component(pciedriver_driver_UDEV_ABS
        ${pciedriver_driver_KO_DIRECTORY}/60-udev_fpga.rules
        REALPATH
    )

    set(pciedriver_driver_SRCS
        ${pciedriver_driver_KO_DIRECTORY}/base.c
        ${pciedriver_driver_KO_DIRECTORY}/base.h
        ${pciedriver_driver_KO_DIRECTORY}/common.h
        ${pciedriver_driver_KO_DIRECTORY}/compat.h
        ${pciedriver_driver_KO_DIRECTORY}/config.h
        ${pciedriver_driver_KO_DIRECTORY}/int.c
        ${pciedriver_driver_KO_DIRECTORY}/int.h
        ${pciedriver_driver_KO_DIRECTORY}/ioctl.c
        ${pciedriver_driver_KO_DIRECTORY}/ioctl.h
        ${pciedriver_driver_KO_DIRECTORY}/kmem.c
        ${pciedriver_driver_KO_DIRECTORY}/kmem.h
        ${pciedriver_driver_KO_DIRECTORY}/sysfs.c
        ${pciedriver_driver_KO_DIRECTORY}/sysfs.h
        ${pciedriver_driver_KO_DIRECTORY}/umem.c
        ${pciedriver_driver_KO_DIRECTORY}/umem.h
        ${pciedriver_driver_KO_DIRECTORY}/Makefile
        ${pciedriver_driver_HEADER_ABS}
        ${pciedriver_driver_UDEV_ABS}
    )

    set(pciedriver_driver_SCRIPTS
        ${pciedriver_driver_KO_DIRECTORY}/version.sh
    )

    install(FILES ${pciedriver_driver_SRCS}
        DESTINATION ${pciedriver_DKMS_INSTALL_DIR}
        COMPONENT Pciedriver
    )
    install(FILES ${pciedriver_driver_SCRIPT} DESTINATION ${pciedriver_DKMS_INSTALL_DIR}
        PERMISSIONS
        OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
        COMPONENT Pciedriver
    )
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${DKMS_FILE_NAME}
        DESTINATION ${pciedriver_DKMS_INSTALL_DIR}
        COMPONENT Pciedriver
    )

endif()

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

    set(pciedriver_DISTRO_VERSION "" CACHE STRING "pciedriver distribution version")
    string(APPEND pciedriver_VERSION "${pciedriver_DISTRO_VERSION}")
    set(CPACK_GENERATOR "DEB")

    set(CPACK_DEB_COMPONENT_INSTALL ON)
    set(CPACK_DEBIAN_ENABLE_COMPONENT_DEPENDS ON)
    set(CPACK_DEBIAN_PACKAGE_GENERATE_SHLIBS_POLICY ">=")
    set(CPACK_DEBIAN_PACKAGE_GENERATE_SHLIBS ON)
    set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS OFF)
    set(CPACK_DEBIAN_PCIEDRIVER_PACKAGE_DEPENDS "dkms")
    set(CPACK_DEBIAN_PCIEDRIVER_PACKAGE_CONTROL_EXTRA
        "${CMAKE_CURRENT_BINARY_DIR}/postinst;${CMAKE_CURRENT_BINARY_DIR}/prerm"
    )
    set(CPACK_DEBIAN_PCIEDRIVER_PACKAGE_NAME "pcieDriver")
    set(CPACK_DEBIAN_PCIEDRIVER_FILE_NAME
        "${CPACK_DEBIAN_PCIEDRIVER_PACKAGE_NAME}_${pciedriver_VERSION}_${arch_name}.deb"
    )

    set(CPACK_RPM_COMPONENT_INSTALL ON)
    set(CPACK_RPM_PACKAGE_AUTOREQ no)
    set(CPACK_RPM_PACKAGE_AUTOPROV yes)
    set(CPACK_RPM_PCIEDRIVER_PACKAGE_REQUIRE "dkms")
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
    set(CPACK_RPM_PCIEDRIVER_POST_INSTALL_SCRIPT_FILE "${CMAKE_CURRENT_BINARY_DIR}/postinst")
    set(CPACK_RPM_PCIEDRIVER_PRE_UNINSTALL_SCRIPT_FILE "${CMAKE_CURRENT_BINARY_DIR}/prerm")
    set(CPACK_RPM_PCIEDRIVER_PACKAGE_NAME "pcieDriver")
    set(CPACK_RPM_PCIEDRIVER_FILE_NAME
        "${CPACK_RPM_PCIEDRIVER_PACKAGE_NAME}_${pciedriver_VERSION}_${arch_name}.rpm"
    )

    set(CPACK_COMPONENT_PCIEDRIVER_DESCRIPTION "pcieDriver library/driver")
    set(CPACK_PACKAGE_VENDOR "LNLS")
    set(CPACK_PACKAGE_CONTACT "Lucas Russo <lucas.russo@lnls.br>")

    # if(CMAKE_CURRENT_BINARY_DIR STREQUAL CMAKE_BINARY_DIR)
    #     include(CPack)
    #
    #        cpack_add_component_group(PciedriverAll
    #            DISPLAY_NAME "pcieDriver drivers, headers and libraries"
    #        )
    #
    #        cpack_add_component(Pciedriver
    #            DISPLAY_NAME "pcieDriver drivers, headers and libraries"
    #            GROUP PciedriverAll
    #            INSTALL_TYPES FullDriver
    #        )
    #
    #        cpack_add_install_type(FullDriver
    #            DISPLAY_NAME "Full drivers, headers and libraries"
    #        )
    #
    # endif()

endif()
