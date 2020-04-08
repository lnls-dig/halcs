# Based on:
# https://github.com/zeromq/malamute/blob/master/Findczmq.cmake

if (NOT MSVC)
    include(FindPkgConfig)
    pkg_check_modules(PC_acqclient "libacqclient")
    if (PC_acqclient_FOUND)
        # add CFLAGS from pkg-config file, e.g. draft api.
        add_definitions(${PC_acqclient_CFLAGS} ${PC_acqclient_CFLAGS_OTHER})
        # some libraries install the headers is a subdirectory of the include dir
        # returned by pkg-config, so use a wildcard match to improve chances of finding
        # headers and SOs.
        set(PC_acqclient_INCLUDE_HINTS ${PC_acqclient_INCLUDE_DIRS} ${PC_acqclient_INCLUDE_DIRS}/*)
        set(PC_acqclient_LIBRARY_HINTS ${PC_acqclient_LIBRARY_DIRS} ${PC_acqclient_LIBRARY_DIRS}/*)
    endif(PC_acqclient_FOUND)
endif (NOT MSVC)

find_path (
    acqclient_INCLUDE_DIRS
    NAMES acq_client.h
    HINTS ${PC_acqclient_INCLUDE_HINTS}
)

find_library (
    acqclient_LIBRARIES
    NAMES libacqclient acqclient
    HINTS ${PC_acqclient_LIBRARY_HINTS}
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    acqclient
    REQUIRED_VARS acqclient_LIBRARIES acqclient_INCLUDE_DIRS
)
mark_as_advanced(
    acqclient_FOUND
    acqclient_LIBRARIES acqclient_INCLUDE_DIRS
)
