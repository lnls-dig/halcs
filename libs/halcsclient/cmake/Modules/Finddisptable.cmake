# Based on:
# https://github.com/zeromq/malamute/blob/master/Findczmq.cmake

if (NOT MSVC)
    include(FindPkgConfig)
    pkg_check_modules(PC_disptable "libdisptable")
    if (PC_disptable_FOUND)
        # add CFLAGS from pkg-config file, e.g. draft api.
        add_definitions(${PC_disptable_CFLAGS} ${PC_disptable_CFLAGS_OTHER})
        # some libraries install the headers is a subdirectory of the include dir
        # returned by pkg-config, so use a wildcard match to improve chances of finding
        # headers and SOs.
        set(PC_disptable_INCLUDE_HINTS ${PC_disptable_INCLUDE_DIRS} ${PC_disptable_INCLUDE_DIRS}/*)
        set(PC_disptable_LIBRARY_HINTS ${PC_disptable_LIBRARY_DIRS} ${PC_disptable_LIBRARY_DIRS}/*)
    endif(PC_disptable_FOUND)
endif (NOT MSVC)

find_path (
    disptable_INCLUDE_DIRS
    NAMES disptable.h
    HINTS ${PC_disptable_INCLUDE_HINTS}
)

find_library (
    disptable_LIBRARIES
    NAMES libdisptable disptable
    HINTS ${PC_disptable_LIBRARY_HINTS}
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    disptable
    REQUIRED_VARS disptable_LIBRARIES disptable_INCLUDE_DIRS
)
mark_as_advanced(
    disptable_FOUND
    disptable_LIBRARIES disptable_INCLUDE_DIRS
)
