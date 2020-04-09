# Based on:
# https://github.com/zeromq/malamute/blob/master/Findczmq.cmake

if (NOT MSVC)
    include(FindPkgConfig)
    pkg_check_modules(PC_pciedriver "libpciedriver")
    if (PC_pciedriver_FOUND)
        # add CFLAGS from pkg-config file, e.g. draft api.
        add_definitions(${PC_pciedriver_CFLAGS} ${PC_pciedriver_CFLAGS_OTHER})
        # some libraries install the headers is a subdirectory of the include dir
        # returned by pkg-config, so use a wildcard match to improve chances of finding
        # headers and SOs.
        set(PC_pciedriver_INCLUDE_HINTS ${PC_pciedriver_INCLUDE_DIRS} ${PC_pciedriver_INCLUDE_DIRS}/*)
        set(PC_pciedriver_LIBRARY_HINTS ${PC_pciedriver_LIBRARY_DIRS} ${PC_pciedriver_LIBRARY_DIRS}/*)
    endif(PC_pciedriver_FOUND)
endif (NOT MSVC)

find_path (
    pciedriver_INCLUDE_DIRS
    NAMES pciDriver.h
    HINTS ${PC_pciedriver_INCLUDE_HINTS}
    PATHS
        /usr/include/pciDriver/lib
        /usr/local/include/pciDriver/lib
)

find_library (
    pciedriver_LIBRARIES
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
    REQUIRED_VARS pciedriver_LIBRARIES pciedriver_INCLUDE_DIRS
)
mark_as_advanced(
    pciedriver_FOUND
    pciedriver_LIBRARIES pciedriver_INCLUDE_DIRS
)
