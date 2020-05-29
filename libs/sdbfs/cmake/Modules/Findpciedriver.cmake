# Based on:
# https://github.com/zeromq/malamute/blob/master/Findczmq.cmake

if (NOT MSVC)
    include(FindPkgConfig)
    pkg_check_modules(PC_pciedriver IMPORTED_TARGET "libpciedriver")
    if (PC_pciedriver_FOUND)
        # To satisfy find_path()/find_library()
        set(PC_pciedriver_INCLUDE_HINTS ${PC_pciedriver_INCLUDE_DIRS} ${PC_pciedriver_INCLUDE_DIRS}/*)
        set(PC_pciedriver_LIBRARY_HINTS ${PC_pciedriver_LIBRARY_DIRS} ${PC_pciedriver_LIBRARY_DIRS}/*)
        # Imported target library name is all we need for
        # target_link_libraries().
        set(pciedriver_LIBRARIES PkgConfig::PC_pciedriver)
    endif(PC_pciedriver_FOUND)
endif (NOT MSVC)

# Try to find on our own
find_path (
    _pciedriver_INCLUDE_DIRS
    NAMES pciDriver.h
    HINTS ${PC_pciedriver_INCLUDE_HINTS}
    PATHS
        /usr/include/pciDriver/lib
        /usr/local/include/pciDriver/lib
)

# also include the grandparent directory, as some header files in
# pciedriver use the format <pciDriver/lib/<header>.h> to include
# the header
get_filename_component(_pciedriver_GRANDPARENT_INCLUDE_DIRS "${_pciedriver_INCLUDE_DIRS}/../../" ABSOLUTE)
list(APPEND _pciedriver_INCLUDE_DIRS "${_pciedriver_GRANDPARENT_INCLUDE_DIRS}")

find_library (
    _pciedriver_LIBRARIES
    NAMES libpcidriver pcidriver
    HINTS ${PC_pciedriver_LIBRARY_HINTS}
    PATHS
        /usr/lib
        /usr/lib/pciDriver
        /usr/lib/pciDriver/lib
        /usr/local/lib
        /usr/local/lib/pciDriver
        /usr/local/lib/pciDriver/lib
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    pciedriver
    REQUIRED_VARS _pciedriver_LIBRARIES _pciedriver_INCLUDE_DIRS
)
mark_as_advanced(
    pciedriver_FOUND
    _pciedriver_LIBRARIES _pciedriver_INCLUDE_DIRS
)

# Create our INTERFACE library if not created by PkgConfig
if(pciedriver_FOUND AND NOT TARGET PkgConfig::PC_pciedriver AND NOT TARGET pciedriver::pciedriver)
    add_library(pciedriver::pciedriver
        INTERFACE IMPORTED
    )
    set_target_properties(pciedriver::pciedriver
        PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${_pciedriver_INCLUDE_DIRS}"
        INTERFACE_LINK_LIBRARIES "${_pciedriver_LIBRARIES}"
    )

    set(pciedriver_LIBRARIES pciedriver::pciedriver)
endif()
