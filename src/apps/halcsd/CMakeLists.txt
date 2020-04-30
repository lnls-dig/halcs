########################################################################
# Binary setup
########################################################################

include(FindThreads)
# detect target architecture
include(TargetArch)
# set installation variables compliant with distributions
include(GNUInstallDirs)
# Get systemd service directories'
include(TestSystemd)
# extract version numbers from header file
include(TestHalcsVersion)

target_architecture(ARCH)

# halcsd basename
set (halcsd_OUTPUT_BASENAME "halcsd"
    CACHE STRING
    "Output halcsd binary base name"
)

# halcsd_cfg basename
set (halcsd_cfg_OUTPUT_BASENAME "halcsd_cfg"
    CACHE STRING
    "Output halcsd_cfg binary base name"
)

#########################################################################
# library dependencies
#########################################################################

list(APPEND halcsd_LIBRARIES
    ${apps_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)
list(APPEND halcsd_cfg_LIBRARIES
    ${apps_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)

########################################################################
# User compile-time options
########################################################################

#######################################
# HALCSD
#######################################

# which AFE version we want.

set(halcsd_AFE_RFFE_TYPE_OPT "2" CACHE STRING
    "AFE RFFE type")
set(halcsd_AFE_RFFE_TYPE_VALUES
    "1;2"
)

set_property(CACHE halcsd_AFE_RFFE_TYPE_OPT PROPERTY STRINGS
    ${halcsd_AFE_RFFE_TYPE_VALUES}
)

message(STATUS
    "Setting compile-time option halcsd_AFE_RFFE_TYPE='${halcsd_AFE_RFFE_TYPE_OPT}'")

# setting compile options
string(APPEND halcsd_board_definitions "")

# setting macros
if(halcsd_AFE_RFFE_TYPE_OPT STREQUAL 1)
    list(APPEND halcsd_board_definitions
        __AFE_RFFE_V1__
    )
elseif(halcsd_AFE_RFFE_TYPE_OPT STREQUAL 2)
    list(APPEND halcsd_board_definitions
        __AFE_RFFE_V2__
    )
endif()

# if we want HALCSD_CFG or not.

option(BUILD_halcsd_cfg "Whether or not to build halcsd_cfg" OFF)

if(BUILD_halcsd_cfg)
    list(APPEND apps_halcsd "halcsd_cfg")
endif()

option(BUILD_halcsd "Whether or not to build halcsd" ON)

if(BUILD_halcsd)
    list(APPEND apps_halcsd "halcsd")

    if(BUILD_halcsd_cfg)
        list(APPEND halcsd_board_definitions
            __WITH_APP_CFG__
        )
    endif()

endif()

# if we want to install idev rules or not
option(halcsd_INSTALL_UDEV_RULES
    "Install udev rules for halcsd"
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

add_library(halcsd_compiler_flags INTERFACE)

target_link_libraries(halcsd_compiler_flags
    INTERFACE
    apps_compiler_flags
)

add_library(halcsd_cfg_compiler_flags INTERFACE)

target_link_libraries(halcsd_cfg_compiler_flags
    INTERFACE
    apps_compiler_flags
)

########################################################################
# Sources/Headers definitions
########################################################################

########################################################################
# HALCSD
########################################################################

#######################################
# sources
#######################################

set(halcsd_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/halcsd.c
)

# for IDE project generation
source_group("Source Files" FILES
    ${halcsd_sources}
)

# Header files
list(APPEND halcsd_other_search_dirs "")

#######################################
# header definitions
#######################################

set(halcsd_public_headers
    ${common_INCLUDE_DIR}/halcs_server.h
)

# Get directory names only
list(APPEND halcsd_search_dirs "")

foreach(search_dir_header ${halcsd_public_headers})
    get_filename_component(search_dir
        ${search_dir_header}
        DIRECTORY
    )

    list(APPEND halcsd_search_dirs ${search_dir})
endforeach()

# remove, well, duplicates ...
list(REMOVE_DUPLICATES halcsd_search_dirs)

# append to global search list
list(APPEND halcsd_other_search_dirs ${halcsd_search_dirs})

#######################################
# LIBGEN header
#######################################

# find libgen.h header. It's part of libc.
find_path (
    _libgen_INCLUDE_DIRS
    NAMES libgen.h
)

mark_as_advanced(
    _libgen_INCLUDE_DIRS
)

# append to global search list
list(APPEND halcsd_other_search_dirs ${_libgen_INCLUDE_DIRS})

#######################################
# HEADER Grouping
#######################################

# for IDE project generation
source_group("Header Files" FILES
    ${halcsd_public_headers}
)

########################################################################
# HALCSD_CFG
########################################################################

#######################################
# sources
#######################################

# Source files

set(halcsd_cfg_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/halcsd_cfg.c
)

# for IDE project generation
source_group("Source Files" FILES
    ${halcsd_cfg_sources}
)

# Header files
list(APPEND halcsd_cfg_other_search_dirs "")

#######################################
# header definitions
#######################################

set(halcsd_cfg_public_headers
    ${common_INCLUDE_DIR}/halcs_server.h
)

# Get directory names only
list(APPEND halcsd_cfg_search_dirs "")

foreach(search_dir_header ${halcsd_cfg_public_headers})
    get_filename_component(search_dir
        ${search_dir_header}
        DIRECTORY
    )

    list(APPEND halcsd_cfg_search_dirs ${search_dir})
endforeach()

# remove, well, duplicates ...
list(REMOVE_DUPLICATES halcsd_cfg_search_dirs)

# append to global search list
list(APPEND halcsd_cfg_other_search_dirs ${halcsd_cfg_search_dirs})

#######################################
# HEADER Grouping
#######################################

# for IDE project generation
source_group("Header Files" FILES
    ${halcsd_cfg_public_headers}
)

########################################################################
# Binaries declaration
########################################################################

foreach(bin_target ${apps_halcsd})
    # build static library
    add_executable(${bin_target}
        ${${bin_target}_sources}
        ${${bin_target}_public_headers}
    )

    # ${bin_target}_other_search_dirs is in quotes as this is a list
    # and the first element would be concatenated with the previous
    # generator expression.
    target_include_directories(${bin_target}
        PRIVATE
        "$<BUILD_INTERFACE:${${bin_target}_other_search_dirs}>"
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:include>

    )

    target_compile_definitions(${bin_target} PRIVATE
        ${${bin_target}_board_definitions}
    )

    # set debug posfix in case of debug builds
    set_target_properties(${bin_target} PROPERTIES
        DEBUG_POSTFIX
        ${CMAKE_DEBUG_POSTFIX}
        POSITION_INDEPENDENT_CODE
        ON
    )

    # set PRIVATE compiler flags. It doesn't really work to link an
    # INTERFACE library with PRIVATE scope. See:
    # https://gitlab.kitware.com/cmake/cmake/-/issues/15415
    target_link_libraries(${bin_target} PRIVATE
        $<TARGET_PROPERTY:${bin_target}_compiler_flags,INTERFACE_LINK_LIBRARIES>
    )
    target_include_directories(${bin_target} PRIVATE
        $<TARGET_PROPERTY:${bin_target}_compiler_flags,INTERFACE_INCLUDE_DIRECTORIES>
    )
    target_compile_options(${bin_target} PRIVATE
        $<TARGET_PROPERTY:${bin_target}_compiler_flags,INTERFACE_COMPILE_OPTIONS>
    )
    target_compile_definitions(${bin_target} PRIVATE
        $<TARGET_PROPERTY:${bin_target}_compiler_flags,INTERFACE_COMPILE_DEFINITIONS>
    )
    target_compile_features(${bin_target} PRIVATE
        $<TARGET_PROPERTY:${bin_target}_compiler_flags,INTERFACE_COMPILE_FEATURES>
    )
    target_sources(${bin_target} PRIVATE
        $<TARGET_PROPERTY:${bin_target}_compiler_flags,INTERFACE_SOURCES>
    )

    ########################################################################
    # Library dependencies
    ########################################################################

    # set depedency to targets
    target_link_libraries(${bin_target}
        PRIVATE
        ${${bin_target}_LIBRARIES}
        ${halcs_LIBRARIES}
    )

    # install binary
    install(TARGETS ${bin_target}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT Binaries)

    # install config files
    if(TARGET halcsd)
        # general scripts location
        set(halcsd_INSTALL_DATADIR ${CMAKE_INSTALL_DATADIR}/halcs)
        install(DIRECTORY DESTINATION ${halcsd_INSTALL_DATADIR})

        # general scripts
        install(FILES
            ${CMAKE_CURRENT_SOURCE_DIR}/init-generic/share/halcs/scripts/eat-dashes.sh
            ${CMAKE_CURRENT_SOURCE_DIR}/init-generic/share/halcs/scripts/generate-board-halcs-idx.sh
            ${CMAKE_CURRENT_SOURCE_DIR}/init-generic/share/halcs/scripts/generate-dev-idx.sh
            DESTINATION ${halcsd_INSTALL_DATADIR}
            PERMISSIONS
            OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
        )

        if(WITH_SYSTEMD)
            # service files. Use an install script so we are able to change the
            # absolute paths of systemd unit files on install, which can be
            # prefixed by CMAKE_INSTALL_PREFIX. SYSTEMD variables are always fixed
            # to an absolute path.
            install(CODE "set(SYSTEMD_SERVICES_INSTALL_DIR ${SYSTEMD_SERVICES_INSTALL_DIR})")
            install(CODE "set(SYSTEMD_CONF_INSTALL_DIR ${SYSTEMD_CONF_INSTALL_DIR})")
            install(CODE "set(UDEV_SYSCONFDIR ${UDEV_SYSCONFDIR})")
            install(CODE "set(NOPREFIX_HALCS_SYSCONFDIR ${NOPREFIX_HALCS_SYSCONFDIR})")

            install(CODE "set(input_file
                \"${CMAKE_CURRENT_SOURCE_DIR}/init/systemd/etc/systemd/system/halcs-be@.service.in\")")
            install(CODE "set(output_file
                \"${CMAKE_CURRENT_BINARY_DIR}/init/systemd/halcs-be@.service\")")
            install(SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfigureInstall.cmake")

            install(CODE "set(input_file
                \"${CMAKE_CURRENT_SOURCE_DIR}/init/systemd/etc/systemd/system/halcs-fe@.service.in\")")
            install(CODE "set(output_file
                \"${CMAKE_CURRENT_BINARY_DIR}/init/systemd/halcs-fe@.service\")")
            install(SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfigureInstall.cmake")

            install(CODE "set(input_file
                \"${CMAKE_CURRENT_SOURCE_DIR}/init/systemd/etc/systemd/system/halcs@.target.in\")")
            install(CODE "set(output_file
                \"${CMAKE_CURRENT_BINARY_DIR}/init/systemd/halcs@.target\")")
            install(SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfigureInstall.cmake")

            install(FILES
                ${CMAKE_CURRENT_BINARY_DIR}/init/systemd/halcs-be@.service
                ${CMAKE_CURRENT_BINARY_DIR}/init/systemd/halcs-fe@.service
                ${CMAKE_CURRENT_BINARY_DIR}/init/systemd/halcs@.target
                DESTINATION ${SYSTEMD_SERVICES_INSTALL_DIR}
                PERMISSIONS
                OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
            )
            # sysconfig files
            install(CODE "set(input_file
                \"${CMAKE_CURRENT_SOURCE_DIR}/init/systemd/etc/sysconfig/halcs-board.in\")")
            install(CODE "set(output_file
                \"${CMAKE_CURRENT_BINARY_DIR}/init/systemd/halcs-board\")")
            install(SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfigureInstall.cmake")

            install(FILES
                ${CMAKE_CURRENT_BINARY_DIR}/init/systemd/halcs-board
                DESTINATION ${SYSTEMD_CONF_INSTALL_DIR}
                PERMISSIONS
                OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
            )

            # halcs.cfg file
            install(CODE "set(input_file
                \"${CMAKE_CURRENT_SOURCE_DIR}/init/systemd/etc/halcs/halcs.cfg.in\")")
            install(CODE "set(output_file
                \"${CMAKE_CURRENT_BINARY_DIR}/init/systemd/halcs.cfg\")")
            install(SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfigureInstall.cmake")

            install(FILES
                ${CMAKE_CURRENT_BINARY_DIR}/init/systemd/halcs.cfg
                DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/${NOPREFIX_HALCS_SYSCONFDIR}
                PERMISSIONS
                OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
            )

            # udev rules
            install(CODE "set(input_file
                \"${CMAKE_CURRENT_SOURCE_DIR}/init/systemd/etc/udev/rules.d/60-udev_fpga.rules.in\")")
            install(CODE "set(output_file
                \"${CMAKE_CURRENT_BINARY_DIR}/init/systemd/60-udev_fpga.rules\")")
            install(SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfigureInstall.cmake")

            install(FILES
                ${CMAKE_CURRENT_BINARY_DIR}/init/systemd/60-udev_fpga.rules
                DESTINATION ${UDEV_SYSCONFDIR}
                PERMISSIONS
                OWNER_READ OWNER_WRITE GROUP_READ GROUP_WRITE WORLD_READ
            )
        else()
            message(FATAL_ERROR "could not find systemd. Other init systems are not supported")
        endif()
    endif()

endforeach()

# set list to global, as linker options are passed through this
set(apps_halcsd ${apps_halcsd} PARENT_SCOPE)

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
    set(CPACK_PACKAGE_NAME "halcsd")
    set(CPACK_PACKAGE_FILE_NAME
        "${CPACK_PACKAGE_NAME}-${halcs_VERSION}-${halcs_BOARD_OPT}-${arch_name}"
    )
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "halcsd daemon")
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
    # set(CPACK_PACKAGE_INSTALL_DIRECTORY "halcsd Install Directory")
    # set(CPACK_TEMPORARY_DIRECTORY "halcsd Temporary CPack Directory")

    include(CPack)

    cpack_add_component_group(Development
        DISPLAY_NAME "halcsd library"
        EXPANDED
    )
    cpack_add_component(SourceCode
        DISPLAY_NAME "halcsd source code"
        DISABLED
        INSTALL_TYPES FullInstall
    )
    cpack_add_component(SDK
        DISPLAY_NAME "halcsd headers and libraries"
        INSTALL_TYPES FullInstall DevInstall
        GROUP Development
    )
    cpack_add_component(Runtime
        DISPLAY_NAME "halcsd runtime files"
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
