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
# set installation variables compliant with distributions
include(GNUInstallDirs)
# configure and generate *.Config.cmake files
include(CMakePackageConfigHelpers)
# extract version numbers from header file
include(TestHalcsVersion)
# detect target architecture
include(TargetArch)
# add git introspection functions
include(GetGitRevisionDescription)
### add FetchContent to enabla configure-time populating dependencies
##include(FetchContent)
include(ExternalProject)

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

#########################################################################
# PCIEDRIVER dependency
#########################################################################

ExternalProject_Add(pciedriver
    URL ${CMAKE_CURRENT_SOURCE_DIR}/foreign/pcie-driver
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/pciedriver
    CONFIGURE_COMMAND ""
    # Build only library, not the driver
    BUILD_COMMAND make lib_driver
    INSTALL_COMMAND ""
    # PCIEDRIVER assumes in-source build
    BUILD_IN_SOURCE ON
)

ExternalProject_Get_property(pciedriver BINARY_DIR)

# this is not a CMake project, so no *Config.cmake or *Targets.cmake
# files will be generated. Hardcode library and include paths so when
# libraries try to search for pciedriver they can find it locally!
set(pciedriver_LIBRARY_PATH ${BINARY_DIR}/lib/pcie)
set(pciedriver_INCLUDE_PATH ${BINARY_DIR}/include/pciDriver/lib)

#########################################################################
# BSMP dependency
#########################################################################

ExternalProject_Add(bsmp
    URL ${CMAKE_CURRENT_SOURCE_DIR}/foreign/libbsmp
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/bsmp
    CONFIGURE_COMMAND ""
    BUILD_COMMAND make
    INSTALL_COMMAND ""
    # BSMP assumes in-source build
    BUILD_IN_SOURCE ON
)

ExternalProject_Get_property(bsmp BINARY_DIR)

# this is not a CMake project, so no *Config.cmake or *Targets.cmake
# files will be generated. Hardcode library and include paths so when
# libraries try to search for bsmp they can find it locally!
set(bsmp_LIBRARY_PATH ${BINARY_DIR})
set(bsmp_INCLUDE_PATH ${BINARY_DIR}/include/bsmp)

# Project libraries treated as External Projects with ExternalProject
#
# References:
# https://cmake.org/pipermail/cmake/2012-June/050930.html
# https://coderwall.com/p/qk2eog/use-cmake-enabled-libraries-in-your-cmake-project-ii
#
# We use ExternalProject, as the libraries themselves have dependencies between
# them. Even exporting *Config.cmake and *Targets.cmake is not enough to use
# add_subdirectories() approach, as *Targets.cmake are only cretaed at configure
# time, after the initial CMake parsing is done is done. So, libs will not find
# its dependencies via find_package() in CONFIG mode.

#########################################################################
# ERRHAND dependency
#########################################################################

# add external project
ExternalProject_Add(errhand
    URL ${CMAKE_CURRENT_SOURCE_DIR}/libs/errhand
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/errhand
    INSTALL_COMMAND ""
)

# get binary directory where we can use for searching *Config.cmake files
ExternalProject_Get_property(errhand BINARY_DIR)

# add project to search path for *Config.cmake files
list(APPEND errhand_DIR ${BINARY_DIR})

# Add "target" name library dependency
list(APPEND MORE_LIBRARIES errhand)

#########################################################################
# CONVC dependency
#########################################################################

# add external project
ExternalProject_Add(convc
    URL ${CMAKE_CURRENT_SOURCE_DIR}/libs/convc
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/convc
    INSTALL_COMMAND ""
    CMAKE_ARGS
        -Derrhand_DIR=${errhand_DIR}
    # Specify other targets on which the external project depends.
    # The other targets will be brought up to date before any of
    # the external project’s steps are executed. Because the external
    # project uses additional custom targets internally for each step,
    # the DEPENDS option is the most convenient way to ensure all of
    # those steps depend on the other targets. Simply doing
    # add_dependencies(<name> <targets>) will not make any of the
    # steps dependent on <targets>.
    DEPENDS errhand
)

# get binary directory where we can use for searching *Config.cmake files
ExternalProject_Get_property(convc BINARY_DIR)

# add project to search path for *Config.cmake files
list(APPEND convc_DIR ${BINARY_DIR})

# Add "target" name library dependency
list(APPEND MORE_LIBRARIES convc)

#########################################################################
# LLIO dependency
#########################################################################

# add external project
ExternalProject_Add(llio
    URL ${CMAKE_CURRENT_SOURCE_DIR}/libs/llio
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/llio
    INSTALL_COMMAND ""
    CMAKE_ARGS
        -Derrhand_DIR=${errhand_DIR}
        -Dconvc_DIR=${convc_DIR}
        # Location of PCIEDRIVER lib and include
        -DCMAKE_LIBRARY_PATH=${pciedriver_LIBRARY_PATH}
        -DCMAKE_INCLUDE_PATH=${pciedriver_INCLUDE_PATH}
    # Specify other targets on which the external project depends.
    # The other targets will be brought up to date before any of
    # the external project’s steps are executed. Because the external
    # project uses additional custom targets internally for each step,
    # the DEPENDS option is the most convenient way to ensure all of
    # those steps depend on the other targets. Simply doing
    # add_dependencies(<name> <targets>) will not make any of the
    # steps dependent on <targets>.
    DEPENDS errhand convc pciedriver
)

# get binary directory where we can use for searching *Config.cmake files
ExternalProject_Get_property(llio BINARY_DIR)

# add project to search path for *Config.cmake files
list(APPEND llio_DIR ${BINARY_DIR})

# Add "target" name library dependency
list(APPEND MORE_LIBRARIES llio)

#########################################################################
# SDBFS dependency
#########################################################################

# add external project
ExternalProject_Add(sdbfs
    URL ${CMAKE_CURRENT_SOURCE_DIR}/libs/sdbfs
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/sdbfs
    INSTALL_COMMAND ""
    CMAKE_ARGS
        -Derrhand_DIR=${errhand_DIR}
        -Dconvc_DIR=${convc_DIR}
        -Dllio_DIR=${llio_DIR}
    # Specify other targets on which the external project depends.
    # The other targets will be brought up to date before any of
    # the external project’s steps are executed. Because the external
    # project uses additional custom targets internally for each step,
    # the DEPENDS option is the most convenient way to ensure all of
    # those steps depend on the other targets. Simply doing
    # add_dependencies(<name> <targets>) will not make any of the
    # steps dependent on <targets>.
    DEPENDS errhand convc llio
)

# get binary directory where we can use for searching *Config.cmake files
ExternalProject_Get_property(sdbfs BINARY_DIR)

# add project to search path for *Config.cmake files
list(APPEND sdbfs_DIR ${BINARY_DIR})

# Add "target" name library dependency
list(APPEND MORE_LIBRARIES sdbfs)

#########################################################################
# SDBUTILS dependency
#########################################################################

# add external project
ExternalProject_Add(sdbutils
    URL ${CMAKE_CURRENT_SOURCE_DIR}/libs/sdbutils
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/sdbutils
    INSTALL_COMMAND ""
    CMAKE_ARGS
        -Derrhand_DIR=${errhand_DIR}
        -Dsdbfs_DIR=${sdbfs_DIR}
    # Specify other targets on which the external project depends.
    # The other targets will be brought up to date before any of
    # the external project’s steps are executed. Because the external
    # project uses additional custom targets internally for each step,
    # the DEPENDS option is the most convenient way to ensure all of
    # those steps depend on the other targets. Simply doing
    # add_dependencies(<name> <targets>) will not make any of the
    # steps dependent on <targets>.
    DEPENDS errhand sdbfs
)

# get binary directory where we can use for searching *Config.cmake files
ExternalProject_Get_property(sdbutils BINARY_DIR)

# add project to search path for *Config.cmake files
list(APPEND sdbutils_DIR ${BINARY_DIR})

# Add "target" name library dependency
list(APPEND MORE_LIBRARIES sdbutils)

#########################################################################
# HUTILS dependency
#########################################################################

# add external project
ExternalProject_Add(hutils
    URL ${CMAKE_CURRENT_SOURCE_DIR}/libs/hutils
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/hutils
    INSTALL_COMMAND ""
    CMAKE_ARGS
        -Derrhand_DIR=${errhand_DIR}
    # Specify other targets on which the external project depends.
    # The other targets will be brought up to date before any of
    # the external project’s steps are executed. Because the external
    # project uses additional custom targets internally for each step,
    # the DEPENDS option is the most convenient way to ensure all of
    # those steps depend on the other targets. Simply doing
    # add_dependencies(<name> <targets>) will not make any of the
    # steps dependent on <targets>.
    DEPENDS errhand
)

# get binary directory where we can use for searching *Config.cmake files
ExternalProject_Get_property(hutils BINARY_DIR)

# add project to search path for *Config.cmake files
list(APPEND hutils_DIR ${BINARY_DIR})

# Add "target" name library dependency
list(APPEND MORE_LIBRARIES hutils)

#########################################################################
# DISPTABLE dependency
#########################################################################

# add external project
ExternalProject_Add(disptable
    URL ${CMAKE_CURRENT_SOURCE_DIR}/libs/disptable
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/disptable
    INSTALL_COMMAND ""
    CMAKE_ARGS
        -Derrhand_DIR=${errhand_DIR}
        -Dhutils_DIR=${hutils_DIR}
    # Specify other targets on which the external project depends.
    # The other targets will be brought up to date before any of
    # the external project’s steps are executed. Because the external
    # project uses additional custom targets internally for each step,
    # the DEPENDS option is the most convenient way to ensure all of
    # those steps depend on the other targets. Simply doing
    # add_dependencies(<name> <targets>) will not make any of the
    # steps dependent on <targets>.
    DEPENDS errhand hutils
)

# get binary directory where we can use for searching *Config.cmake files
ExternalProject_Get_property(disptable BINARY_DIR)

# add project to search path for *Config.cmake files
list(APPEND disptable_DIR ${BINARY_DIR})

# Add "target" name library dependency
list(APPEND MORE_LIBRARIES disptable)

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
