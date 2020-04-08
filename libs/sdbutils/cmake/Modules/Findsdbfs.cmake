# Based on:
# https://github.com/zeromq/malamute/blob/master/Findczmq.cmake

if (NOT MSVC)
    include(FindPkgConfig)
    pkg_check_modules(PC_sdbfs "libsdbfs")
    if (PC_sdbfs_FOUND)
        # add CFLAGS from pkg-config file, e.g. draft api.
        add_definitions(${PC_sdbfs_CFLAGS} ${PC_sdbfs_CFLAGS_OTHER})
        # some libraries install the headers is a subdirectory of the include dir
        # returned by pkg-config, so use a wildcard match to improve chances of finding
        # headers and SOs.
        set(PC_sdbfs_INCLUDE_HINTS ${PC_sdbfs_INCLUDE_DIRS} ${PC_sdbfs_INCLUDE_DIRS}/*)
        set(PC_sdbfs_LIBRARY_HINTS ${PC_sdbfs_LIBRARY_DIRS} ${PC_sdbfs_LIBRARY_DIRS}/*)
    endif(PC_sdbfs_FOUND)
endif (NOT MSVC)

find_path (
    sdbfs_INCLUDE_DIRS
    NAMES libsdbfs.h
    HINTS ${PC_sdbfs_INCLUDE_HINTS}
)

find_library (
    sdbfs_LIBRARIES
    NAMES libsdbfs sdbfs
    HINTS ${PC_sdbfs_LIBRARY_HINTS}
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    sdbfs
    REQUIRED_VARS sdbfs_LIBRARIES sdbfs_INCLUDE_DIRS
)
mark_as_advanced(
    sdbfs_FOUND
    sdbfs_LIBRARIES sdbfs_INCLUDE_DIRS
)
