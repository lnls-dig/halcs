# Based on:
# https://github.com/zeromq/malamute/blob/master/Findczmq.cmake

if (NOT MSVC)
    include(FindPkgConfig)
    pkg_check_modules(PC_DISPTABLE "libdisptable")
    if (PC_DISPTABLE_FOUND)
        # add CFLAGS from pkg-config file, e.g. draft api.
        add_definitions(${PC_DISPTABLE_CFLAGS} ${PC_DISPTABLE_CFLAGS_OTHER})
        # some libraries install the headers is a subdirectory of the include dir
        # returned by pkg-config, so use a wildcard match to improve chances of finding
        # headers and SOs.
        set(PC_DISPTABLE_INCLUDE_HINTS ${PC_DISPTABLE_INCLUDE_DIRS} ${PC_DISPTABLE_INCLUDE_DIRS}/*)
        set(PC_DISPTABLE_LIBRARY_HINTS ${PC_DISPTABLE_LIBRARY_DIRS} ${PC_DISPTABLE_LIBRARY_DIRS}/*)
    endif(PC_DISPTABLE_FOUND)
endif (NOT MSVC)

find_path (
    DISPTABLE_INCLUDE_DIRS
    NAMES disptable.h
    HINTS ${PC_DISPTABLE_INCLUDE_HINTS}
)

find_library (
    DISPTABLE_LIBRARIES
    NAMES libdisptable disptable
    HINTS ${PC_DISPTABLE_LIBRARY_HINTS}
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    DISPTABLE
    REQUIRED_VARS DISPTABLE_LIBRARIES DISPTABLE_INCLUDE_DIRS
)
mark_as_advanced(
    DISPTABLE_FOUND
    DISPTABLE_LIBRARIES DISPTABLE_INCLUDE_DIRS
)
