# Based on:
# https://github.com/zeromq/malamute/blob/master/Findczmq.cmake

if (NOT MSVC)
    include(FindPkgConfig)
    pkg_check_modules(PC_SDBFS "libsdbfs")
    if (PC_SDBFS_FOUND)
        # add CFLAGS from pkg-config file, e.g. draft api.
        add_definitions(${PC_SDBFS_CFLAGS} ${PC_SDBFS_CFLAGS_OTHER})
        # some libraries install the headers is a subdirectory of the include dir
        # returned by pkg-config, so use a wildcard match to improve chances of finding
        # headers and SOs.
        set(PC_SDBFS_INCLUDE_HINTS ${PC_SDBFS_INCLUDE_DIRS} ${PC_SDBFS_INCLUDE_DIRS}/*)
        set(PC_SDBFS_LIBRARY_HINTS ${PC_SDBFS_LIBRARY_DIRS} ${PC_SDBFS_LIBRARY_DIRS}/*)
    endif(PC_SDBFS_FOUND)
endif (NOT MSVC)

find_path (
    SDBFS_INCLUDE_DIRS
    NAMES libsdbfs.h
    HINTS ${PC_SDBFS_INCLUDE_HINTS}
)

find_library (
    SDBFS_LIBRARIES
    NAMES libsdbfs sdbfs
    HINTS ${PC_SDBFS_LIBRARY_HINTS}
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    SDBFS
    REQUIRED_VARS SDBFS_LIBRARIES SDBFS_INCLUDE_DIRS
)
mark_as_advanced(
    SDBFS_FOUND
    SDBFS_LIBRARIES SDBFS_INCLUDE_DIRS
)
