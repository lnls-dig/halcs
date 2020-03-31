# Based on:
# https://github.com/zeromq/malamute/blob/master/Findczmq.cmake

if (NOT MSVC)
    include(FindPkgConfig)
    pkg_check_modules(PC_PCIEDRIVER "libpciedriver")
    if (PC_PCIEDRIVER_FOUND)
        # add CFLAGS from pkg-config file, e.g. draft api.
        add_definitions(${PC_PCIEDRIVER_CFLAGS} ${PC_PCIEDRIVER_CFLAGS_OTHER})
        # some libraries install the headers is a subdirectory of the include dir
        # returned by pkg-config, so use a wildcard match to improve chances of finding
        # headers and SOs.
        set(PC_PCIEDRIVER_INCLUDE_HINTS ${PC_PCIEDRIVER_INCLUDE_DIRS} ${PC_PCIEDRIVER_INCLUDE_DIRS}/*)
        set(PC_PCIEDRIVER_LIBRARY_HINTS ${PC_PCIEDRIVER_LIBRARY_DIRS} ${PC_PCIEDRIVER_LIBRARY_DIRS}/*)
    endif(PC_PCIEDRIVER_FOUND)
endif (NOT MSVC)

find_path (
    PCIEDRIVER_INCLUDE_DIRS
    NAMES pciDriver.h
    HINTS ${PC_PCIEDRIVER_INCLUDE_HINTS}
    PATHS
        /usr/include/pciDriver/lib
        /usr/local/include/pciDriver/lib
)

find_library (
    PCIEDRIVER_LIBRARIES
    NAMES libpcidriver pcidriver
    HINTS ${PC_PCIEDRIVER_LIBRARY_HINTS}
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
    PCIEDRIVER
    REQUIRED_VARS PCIEDRIVER_LIBRARIES PCIEDRIVER_INCLUDE_DIRS
)
mark_as_advanced(
    PCIEDRIVER_FOUND
    PCIEDRIVER_LIBRARIES PCIEDRIVER_INCLUDE_DIRS
)
