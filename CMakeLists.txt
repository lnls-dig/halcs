########################################################################
# Project setup
########################################################################

cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(halcs)

# set modules path for CMkake to find
set(halcs_CMAKE_MODULES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)
list(APPEND CMAKE_MODULE_PATH ${halcs_CMAKE_MODULES_DIR})

# add module to prevent in-source builds
include(PreventInSourceBuilds)
# detect target architecture
include(TargetArch)
# extract version numbers from header file
include(TestHalcsVersion)

target_architecture(ARCH)

# find pkg-config
find_package(PkgConfig)

set(CMAKE_DEBUG_POSTFIX d)

# library basename
set (halcs_OUTPUT_BASENAME "halcs"
    CACHE STRING
    "Output halcs base name"
)

# set library dependencies
set(OPT_LIBRARIES_STATIC)

########################################################################
# Macros
########################################################################

# Based on the lecture by Daniel Pfeifer - "Effective CMake"

# list all internal project libraries that we don't want to search for
# dependencies
set(as_subproject
    halcsclient
    acqclient
    bpmclient
    convc
    disptable
    errhand
    hutils
    llio
    sdbfs
    sdbutils
    # external projects, not CMake
    pciedriver
    bsmp
)

# override find_package to only call the built-in find_package()
# if not project listed above. Fool library checks to think
# find_package was executed and set the necessary variables>
# <lib_name>_LIBRARIES will have the target name which will be
# enough for the project to find all dependencies.
macro(find_package)
    if(NOT "${ARGV0}" IN_LIST as_subproject)
        _find_package(${ARGV})
    else()
        set(${ARGV0}_FOUND TRUE)
        set(${ARGV0}_LIBRARIES ${ARGV0})
        set(${ARGV0}_INCLUDE_DIRS "")
    endif()
endmacro()

########################################################################
# User compile-time options
########################################################################

########################################################################
# Library options
########################################################################

add_library(halcs_compiler_flags INTERFACE)
target_compile_features(halcs_compiler_flags
    INTERFACE
    c_std_99
)

# add compiler warning flags just when building this project via
# the BUILD_INTERFACE genex
set(gcc_c "$<$<COMPILE_LANGUAGE:C>:$<C_COMPILER_ID:GNU>>")
set(clang_c "$<$<COMPILE_LANGUAGE:C>:$<C_COMPILER_ID:Clang>>")
set(msvc_c "$<$<COMPILE_LANGUAGE:C>:$<C_COMPILER_ID:MSVC>>")
target_compile_options(halcs_compiler_flags
    INTERFACE
    "$<${gcc_c}:$<BUILD_INTERFACE:-Wall;-Wextra;-Werror;-O2;-g>>"
    "$<${clang_c}:$<BUILD_INTERFACE:-Wall;-Wextra;-Werror;-O2;-g>>"
    "$<${msvc_c}:$<BUILD_INTERFACE:-W3;-O2;-g>>"
)

#########################################################################
# PCIEDRIVER dependency
#########################################################################

include(AddExternalProjectPciedriver)

# Add "target" name library dependency
list(APPEND halcs_LIBRARIES pciedriver)

#########################################################################
# BSMP dependency
#########################################################################

include(AddExternalProjectBsmp)

# Add "target" name library dependency
list(APPEND halcs_LIBRARIES bsmp)

#########################################################################
# ERRHAND dependency
#########################################################################

add_subdirectory(libs/errhand)

# Add "target" name library dependency
list(APPEND halcs_LIBRARIES errhand)

#########################################################################
# CONVC dependency
#########################################################################

add_subdirectory(libs/convc)

# Add "target" name library dependency
list(APPEND halcs_LIBRARIES convc)

#########################################################################
# LLIO dependency
#########################################################################

add_subdirectory(libs/llio)

# Add "target" name library dependency
list(APPEND halcs_LIBRARIES llio)

#########################################################################
# SDBFS dependency
#########################################################################

add_subdirectory(libs/sdbfs)

# Add "target" name library dependency
list(APPEND halcs_LIBRARIES sdbfs)

#########################################################################
# SDBUTILS dependency
#########################################################################

add_subdirectory(libs/sdbutils)

# Add "target" name library dependency
list(APPEND halcs_LIBRARIES sdbutils)

#########################################################################
# HUTILS dependency
#########################################################################

add_subdirectory(libs/hutils)

# Add "target" name library dependency
list(APPEND halcs_LIBRARIES hutils)

#########################################################################
# DISPTABLE dependency
#########################################################################

add_subdirectory(libs/disptable)

# Add "target" name library dependency
list(APPEND halcs_LIBRARIES disptable)

#########################################################################
# HALCSCLIENT dependency
#########################################################################

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/client)

# Add "target" name library dependency
list(APPEND halcs_LIBRARIES halcsclient)

#########################################################################
# ACQCLIENT dependency
#########################################################################

add_subdirectory(libs/acqclient)

#########################################################################
# BPMCLIENT dependency
#########################################################################

add_subdirectory(libs/bpmclient)

########################################################################
# Library compile options
########################################################################

########################################################################
# Sources/Headers definitions
########################################################################

# HALCS postinst file directory
set(halcs_POSTINST_FILE_DIR "${CMAKE_CURRENT_BINARY_DIR}/halcs")

set(common_INCLUDE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(sm_io_chips_regs_INCLUDE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/include/chips
)

set(sm_io_modules_regs_INCLUDE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/include/hw
)

set(sm_io_protocols_INCLUDE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/include/protocols
)

set(halcs_LINKER_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/linker
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/boards)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/dev_io)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/msg)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/revision)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/sm_io)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/sm_io_table)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ldconf)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/dev_mngr)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/apps)

#################################################
## CPack
#################################################

# ALL_COMPONENTS_IN_ONE or ONE_PER_GROUP
set(cpack_components_grouping_OPT "ALL_COMPONENTS_IN_ONE"
    CACHE STRING "CPack CPACK_COMPONENTS_GROUPING variable"
)
set(CPACK_COMPONENTS_GROUPING "${cpack_components_grouping_OPT}")

# Any of Binaries;Libs;Scripts;Tools;Pciedriver
set(cpack_components_all_OPT "Binaries;Libs;Scripts;Tools"
    CACHE STRING "CPack CPACK_COMPONENTS_ALL variable"
)
set(CPACK_COMPONENTS_ALL "${cpack_components_all_OPT}")

# Any CPACK generator
set(cpack_generator_OPT "RPM"
    CACHE STRING "CPack CPACK_GENERATOR variable"
)

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
    
    set(halcs_DISTRO_VERSION "" CACHE STRING "halcs distribution version")

    list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_BINARY_DIR})

    set(CPACK_GENERATOR "${cpack_generator_OPT}")
    set(CPACK_PACKAGE_NAME "halcsd")

    set(CPACK_DEB_COMPONENT_INSTALL ON)
    set(CPACK_DEBIAN_PACKAGE_GENERATE_SHLIBS_POLICY ">=")
    set(CPACK_DEBIAN_PACKAGE_GENERATE_SHLIBS ON)
    set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
    # we also depend on malamute >= 1.0, but this has not been packaged into the official
    # repositories yet
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libsodium-dev (>= 1.0.8), libzmq3-dev (>= 4.2.5), libczmq-dev (>= 4.0.2)")
    set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${halcs_POSTINST_FILE_DIR}/debian/postinst")
    # Seems broken in CMake and crashes with 
    # CPack: Create package
    # terminate called after throwing an instance of 'std::logic_error'
    #   what():  basic_string::_M_construct null not valid
    # Makefile:81: recipe for target 'package' failed
    # make: *** [package] Aborted (core dumped)
    # set(CPACK_DEBIAN_DEBUGINFO_PACKAGE ON)

    set(CPACK_RPM_COMPONENT_INSTALL ON)
    set(CPACK_RPM_PACKAGE_AUTOREQ no)
    set(CPACK_RPM_PACKAGE_AUTOPROV yes)
    # we also depend on malamute >= 1.0, but this has not been packaged into the official
    # repositories yet
    set(CPACK_RPM_PACKAGE_REQUIRE "libsodium-devel >= 1.0.8, zeromq-devel >= 4.2.5, czmq-devel >= 4.0.2")
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
    set(CPACK_RPM_POST_INSTALL_SCRIPT_FILE "${halcs_POSTINST_FILE_DIR}/redhat/postinst")
    set(CPACK_RPM_BUILD_SOURCE_DIRS_PREFIX "/usr/src/debug/${CPACK_PACKAGE_NAME}-${halcs_VERSION}")
    set(CPACK_RPM_DEBUGINFO_PACKAGE ON)

    set(CPACK_PACKAGE_FILE_NAME
        "${CPACK_PACKAGE_NAME}_${halcs_VERSION}${halcs_DISTRO_VERSION}_${arch_name}"
    )
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "halcsd daemon")
    set(CPACK_PACKAGE_VENDOR "LNLS")
    set(CPACK_PACKAGE_CONTACT "Lucas Russo <lucas.russo@lnls.br>")
    #set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_BINARY_DIR}/COPYING.txt")
    # set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_BINARY_DIR}/README.txt")
    # set(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_CURRENT_BINARY_DIR}/WELCOME.txt")
    # set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/branding.bmp")

    # if we are packaging Pciedriver use it's own version
    if("${CPACK_COMPONENTS_ALL}" STREQUAL "Pciedriver")
        set(CPACK_PACKAGE_VERSION ${pciedriver_VERSION})
        set(CPACK_PACKAGE_VERSION_MAJOR ${pciedriver_VERSION_MAJOR})
        set(CPACK_PACKAGE_VERSION_MINOR ${pciedriver_VERSION_MINOR})
        set(CPACK_PACKAGE_VERSION_PATCH ${pciedriver_VERSION_PATCH})
    else()
        set(CPACK_PACKAGE_VERSION ${halcs_VERSION})
        set(CPACK_PACKAGE_VERSION_MAJOR ${halcs_VERSION_MAJOR})
        set(CPACK_PACKAGE_VERSION_MINOR ${halcs_VERSION_MINOR})
        set(CPACK_PACKAGE_VERSION_PATCH ${halcs_VERSION_PATCH})
    endif()
    # set(CPACK_PACKAGE_INSTALL_DIRECTORY "halcsd Install Directory")
    # set(CPACK_TEMPORARY_DIRECTORY "halcsd Temporary CPack Directory")

    include(CPack)

    # For HALCS
    cpack_add_component_group(HalcsFull
        DISPLAY_NAME "HALCS libs, scripts, tools, headers and binaries"
    )

    cpack_add_component(Binaries
        DISPLAY_NAME "Halcs binaries"
        GROUP HalcsFull
        INSTALL_TYPES FullHalcs
    )

    cpack_add_component(Libs
        DISPLAY_NAME "Halcs libraries"
        GROUP HalcsFull
        INSTALL_TYPES FullHalcs
    )

    cpack_add_component(Scripts
        DISPLAY_NAME "Halcs scripts"
        GROUP HalcsFull
        INSTALL_TYPES FullHalcs
    )

    cpack_add_component(Tools
        DISPLAY_NAME "Halcs tools"
        GROUP HalcsFull
        INSTALL_TYPES FullHalcs
    )

    cpack_add_install_type(FullHalcs
        DISPLAY_NAME "Full Halcs"
    )

    # For Pciedriver
    cpack_add_component_group(Pciedriver
        DISPLAY_NAME "pcieDriver drivers, headers and libraries"
    )

    cpack_add_component(Pciedriver
        DISPLAY_NAME "pcieDriver drivers, headers and libraries"
        GROUP Pciedriver
        INSTALL_TYPES FullDriver
    )

    cpack_add_install_type(FullDriver
        DISPLAY_NAME "Full drivers, headers and libraries"
    )

endif()

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
