#######################################
# Library definition
#######################################

include(TestPciedriverVersion)

ExternalProject_Add(pciedriver_ext
    URL ${CMAKE_CURRENT_SOURCE_DIR}/foreign/pcie-driver
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/pciedriver
    CONFIGURE_COMMAND ""
    # Build only library, not the driver
    BUILD_COMMAND make lib_driver
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
)
