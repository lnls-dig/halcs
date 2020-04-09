# Copied from:
# https://github.com/zeromq/malamute/blob/master/Findczmq.cmake

if (NOT MSVC)
    include(FindPkgConfig)
    pkg_check_modules(PC_czmq "libczmq")
    if (PC_czmq_FOUND)
        # add CFLAGS from pkg-config file, e.g. draft api.
        add_definitions(${PC_czmq_CFLAGS} ${PC_czmq_CFLAGS_OTHER})
        # some libraries install the headers is a subdirectory of the include dir
        # returned by pkg-config, so use a wildcard match to improve chances of finding
        # headers and SOs.
        set(PC_czmq_INCLUDE_HINTS ${PC_czmq_INCLUDE_DIRS} ${PC_czmq_INCLUDE_DIRS}/*)
        set(PC_czmq_LIBRARY_HINTS ${PC_czmq_LIBRARY_DIRS} ${PC_czmq_LIBRARY_DIRS}/*)
    endif(PC_czmq_FOUND)
endif (NOT MSVC)

find_path (
    czmq_INCLUDE_DIRS
    NAMES czmq.h
    HINTS ${PC_czmq_INCLUDE_HINTS}
)

find_library (
    czmq_LIBRARIES
    NAMES libczmq czmq
    HINTS ${PC_czmq_LIBRARY_HINTS}
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    czmq
    REQUIRED_VARS czmq_LIBRARIES czmq_INCLUDE_DIRS
)
mark_as_advanced(
    czmq_FOUND
    czmq_LIBRARIES czmq_INCLUDE_DIRS
)
