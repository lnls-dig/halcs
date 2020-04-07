# Based on:
# https://github.com/zeromq/malamute/blob/master/Findczmq.cmake

if (NOT MSVC)
    include(FindPkgConfig)
    pkg_check_modules(PC_ERRHAND "liberrhand")
    if (PC_ERRHAND_FOUND)
        # add CFLAGS from pkg-config file, e.g. draft api.
        add_definitions(${PC_ERRHAND_CFLAGS} ${PC_ERRHAND_CFLAGS_OTHER})
        # some libraries install the headers is a subdirectory of the include dir
        # returned by pkg-config, so use a wildcard match to improve chances of finding
        # headers and SOs.
        set(PC_ERRHAND_INCLUDE_HINTS ${PC_ERRHAND_INCLUDE_DIRS} ${PC_ERRHAND_INCLUDE_DIRS}/*)
        set(PC_ERRHAND_LIBRARY_HINTS ${PC_ERRHAND_LIBRARY_DIRS} ${PC_ERRHAND_LIBRARY_DIRS}/*)
    endif(PC_ERRHAND_FOUND)
endif (NOT MSVC)

find_path (
    ERRHAND_INCLUDE_DIRS
    NAMES errhand.h
    HINTS ${PC_ERRHAND_INCLUDE_HINTS}
)

find_library (
    ERRHAND_LIBRARIES
    NAMES liberrhand errhand
    HINTS ${PC_ERRHAND_LIBRARY_HINTS}
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    ERRHAND
    REQUIRED_VARS ERRHAND_LIBRARIES ERRHAND_INCLUDE_DIRS
)
mark_as_advanced(
    ERRHAND_FOUND
    ERRHAND_LIBRARIES ERRHAND_INCLUDE_DIRS
)
