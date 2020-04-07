# Based on:
# https://github.com/zeromq/malamute/blob/master/Findczmq.cmake

if (NOT MSVC)
    include(FindPkgConfig)
    pkg_check_modules(PC_HUTILS "libhutils")
    if (PC_HUTILS_FOUND)
        # add CFLAGS from pkg-config file, e.g. draft api.
        add_definitions(${PC_HUTILS_CFLAGS} ${PC_HUTILS_CFLAGS_OTHER})
        # some libraries install the headers is a subdirectory of the include dir
        # returned by pkg-config, so use a wildcard match to improve chances of finding
        # headers and SOs.
        set(PC_HUTILS_INCLUDE_HINTS ${PC_HUTILS_INCLUDE_DIRS} ${PC_HUTILS_INCLUDE_DIRS}/*)
        set(PC_HUTILS_LIBRARY_HINTS ${PC_HUTILS_LIBRARY_DIRS} ${PC_HUTILS_LIBRARY_DIRS}/*)
    endif(PC_HUTILS_FOUND)
endif (NOT MSVC)

find_path (
    HUTILS_INCLUDE_DIRS
    NAMES hutils.h
    HINTS ${PC_HUTILS_INCLUDE_HINTS}
)

find_library (
    HUTILS_LIBRARIES
    NAMES libhutils hutils
    HINTS ${PC_HUTILS_LIBRARY_HINTS}
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    HUTILS
    REQUIRED_VARS HUTILS_LIBRARIES HUTILS_INCLUDE_DIRS
)
mark_as_advanced(
    HUTILS_FOUND
    HUTILS_LIBRARIES HUTILS_INCLUDE_DIRS
)
