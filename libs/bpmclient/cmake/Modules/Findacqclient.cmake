# Based on:
# https://github.com/zeromq/malamute/blob/master/Findczmq.cmake

if (NOT MSVC)
    include(FindPkgConfig)
    pkg_check_modules(PC_ACQCLIENT "libacqclient")
    if (PC_ACQCLIENT_FOUND)
        # add CFLAGS from pkg-config file, e.g. draft api.
        add_definitions(${PC_ACQCLIENT_CFLAGS} ${PC_ACQCLIENT_CFLAGS_OTHER})
        # some libraries install the headers is a subdirectory of the include dir
        # returned by pkg-config, so use a wildcard match to improve chances of finding
        # headers and SOs.
        set(PC_ACQCLIENT_INCLUDE_HINTS ${PC_ACQCLIENT_INCLUDE_DIRS} ${PC_ACQCLIENT_INCLUDE_DIRS}/*)
        set(PC_ACQCLIENT_LIBRARY_HINTS ${PC_ACQCLIENT_LIBRARY_DIRS} ${PC_ACQCLIENT_LIBRARY_DIRS}/*)
    endif(PC_ACQCLIENT_FOUND)
endif (NOT MSVC)

find_path (
    ACQCLIENT_INCLUDE_DIRS
    NAMES acq_client.h
    HINTS ${PC_ACQCLIENT_INCLUDE_HINTS}
)

find_library (
    ACQCLIENT_LIBRARIES
    NAMES libacqclient acqclient
    HINTS ${PC_ACQCLIENT_LIBRARY_HINTS}
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    ACQCLIENT
    REQUIRED_VARS ACQCLIENT_LIBRARIES ACQCLIENT_INCLUDE_DIRS
)
mark_as_advanced(
    ACQCLIENT_FOUND
    ACQCLIENT_LIBRARIES ACQCLIENT_INCLUDE_DIRS
)
