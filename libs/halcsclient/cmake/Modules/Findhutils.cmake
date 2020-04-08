# Based on:
# https://github.com/zeromq/malamute/blob/master/Findczmq.cmake

if (NOT MSVC)
    include(FindPkgConfig)
    pkg_check_modules(PC_hutils "libhutils")
    if (PC_hutils_FOUND)
        # add CFLAGS from pkg-config file, e.g. draft api.
        add_definitions(${PC_hutils_CFLAGS} ${PC_hutils_CFLAGS_OTHER})
        # some libraries install the headers is a subdirectory of the include dir
        # returned by pkg-config, so use a wildcard match to improve chances of finding
        # headers and SOs.
        set(PC_hutils_INCLUDE_HINTS ${PC_hutils_INCLUDE_DIRS} ${PC_hutils_INCLUDE_DIRS}/*)
        set(PC_hutils_LIBRARY_HINTS ${PC_hutils_LIBRARY_DIRS} ${PC_hutils_LIBRARY_DIRS}/*)
    endif(PC_hutils_FOUND)
endif (NOT MSVC)

find_path (
    hutils_INCLUDE_DIRS
    NAMES hutils.h
    HINTS ${PC_hutils_INCLUDE_HINTS}
)

find_library (
    hutils_LIBRARIES
    NAMES libhutils hutils
    HINTS ${PC_hutils_LIBRARY_HINTS}
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    hutils
    REQUIRED_VARS hutils_LIBRARIES hutils_INCLUDE_DIRS
)
mark_as_advanced(
    hutils_FOUND
    hutils_LIBRARIES hutils_INCLUDE_DIRS
)
