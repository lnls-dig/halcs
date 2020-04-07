# Based on:
# https://github.com/zeromq/malamute/blob/master/Findczmq.cmake

if (NOT MSVC)
    include(FindPkgConfig)
    pkg_check_modules(PC_HALCSCLIENT "libhalcsclient")
    if (PC_HALCSCLIENT_FOUND)
        # add CFLAGS from pkg-config file, e.g. draft api.
        add_definitions(${PC_HALCSCLIENT_CFLAGS} ${PC_HALCSCLIENT_CFLAGS_OTHER})
        # some libraries install the headers is a subdirectory of the include dir
        # returned by pkg-config, so use a wildcard match to improve chances of finding
        # headers and SOs.
        set(PC_HALCSCLIENT_INCLUDE_HINTS ${PC_HALCSCLIENT_INCLUDE_DIRS} ${PC_HALCSCLIENT_INCLUDE_DIRS}/*)
        set(PC_HALCSCLIENT_LIBRARY_HINTS ${PC_HALCSCLIENT_LIBRARY_DIRS} ${PC_HALCSCLIENT_LIBRARY_DIRS}/*)
    endif(PC_HALCSCLIENT_FOUND)
endif (NOT MSVC)

find_path (
    HALCSCLIENT_INCLUDE_DIRS
    NAMES halcs_client.h
    HINTS ${PC_HALCSCLIENT_INCLUDE_HINTS}
)

find_library (
    HALCSCLIENT_LIBRARIES
    NAMES libhalcsclient halcsclient
    HINTS ${PC_HALCSCLIENT_LIBRARY_HINTS}
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    HALCSCLIENT
    REQUIRED_VARS HALCSCLIENT_LIBRARIES HALCSCLIENT_INCLUDE_DIRS
)
mark_as_advanced(
    HALCSCLIENT_FOUND
    HALCSCLIENT_LIBRARIES HALCSCLIENT_INCLUDE_DIRS
)
