# Based on:
# https://github.com/zeromq/malamute/blob/master/Findczmq.cmake

if (NOT MSVC)
    include(FindPkgConfig)
    pkg_check_modules(PC_errhand "liberrhand")
    if (PC_errhand_FOUND)
        # add CFLAGS from pkg-config file, e.g. draft api.
        add_definitions(${PC_errhand_CFLAGS} ${PC_errhand_CFLAGS_OTHER})
        # some libraries install the headers is a subdirectory of the include dir
        # returned by pkg-config, so use a wildcard match to improve chances of finding
        # headers and SOs.
        set(PC_errhand_INCLUDE_HINTS ${PC_errhand_INCLUDE_DIRS} ${PC_errhand_INCLUDE_DIRS}/*)
        set(PC_errhand_LIBRARY_HINTS ${PC_errhand_LIBRARY_DIRS} ${PC_errhand_LIBRARY_DIRS}/*)
    endif(PC_errhand_FOUND)
endif (NOT MSVC)

find_path (
    errhand_INCLUDE_DIRS
    NAMES errhand.h
    HINTS ${PC_errhand_INCLUDE_HINTS}
)

find_library (
    errhand_LIBRARIES
    NAMES liberrhand errhand
    HINTS ${PC_errhand_LIBRARY_HINTS}
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    errhand
    REQUIRED_VARS errhand_LIBRARIES errhand_INCLUDE_DIRS
)
mark_as_advanced(
    errhand_FOUND
    errhand_LIBRARIES errhand_INCLUDE_DIRS
)
