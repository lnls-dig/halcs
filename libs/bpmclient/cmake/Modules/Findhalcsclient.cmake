# Based on:
# https://github.com/zeromq/malamute/blob/master/Findczmq.cmake

if (NOT MSVC)
    include(FindPkgConfig)
    pkg_check_modules(PC_halcsclient "libhalcsclient")
    if (PC_halcsclient_FOUND)
        # add CFLAGS from pkg-config file, e.g. draft api.
        add_definitions(${PC_halcsclient_CFLAGS} ${PC_halcsclient_CFLAGS_OTHER})
        # some libraries install the headers is a subdirectory of the include dir
        # returned by pkg-config, so use a wildcard match to improve chances of finding
        # headers and SOs.
        set(PC_halcsclient_INCLUDE_HINTS ${PC_halcsclient_INCLUDE_DIRS} ${PC_halcsclient_INCLUDE_DIRS}/*)
        set(PC_halcsclient_LIBRARY_HINTS ${PC_halcsclient_LIBRARY_DIRS} ${PC_halcsclient_LIBRARY_DIRS}/*)
    endif(PC_halcsclient_FOUND)
endif (NOT MSVC)

find_path (
    halcsclient_INCLUDE_DIRS
    NAMES halcs_client.h
    HINTS ${PC_halcsclient_INCLUDE_HINTS}
)

find_library (
    halcsclient_LIBRARIES
    NAMES libhalcsclient halcsclient
    HINTS ${PC_halcsclient_LIBRARY_HINTS}
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    halcsclient
    REQUIRED_VARS halcsclient_LIBRARIES halcsclient_INCLUDE_DIRS
)
mark_as_advanced(
    halcsclient_FOUND
    halcsclient_LIBRARIES halcsclient_INCLUDE_DIRS
)
