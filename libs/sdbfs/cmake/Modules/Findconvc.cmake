# Based on:
# https://github.com/zeromq/malamute/blob/master/Findczmq.cmake

if (NOT MSVC)
    include(FindPkgConfig)
    pkg_check_modules(PC_CONVC "libconvc")
    if (PC_CONVC_FOUND)
        # add CFLAGS from pkg-config file, e.g. draft api.
        add_definitions(${PC_CONVC_CFLAGS} ${PC_CONVC_CFLAGS_OTHER})
        # some libraries install the headers is a subdirectory of the include dir
        # returned by pkg-config, so use a wildcard match to improve chances of finding
        # headers and SOs.
        set(PC_CONVC_INCLUDE_HINTS ${PC_CONVC_INCLUDE_DIRS} ${PC_CONVC_INCLUDE_DIRS}/*)
        set(PC_CONVC_LIBRARY_HINTS ${PC_CONVC_LIBRARY_DIRS} ${PC_CONVC_LIBRARY_DIRS}/*)
    endif(PC_CONVC_FOUND)
endif (NOT MSVC)

find_path (
    CONVC_INCLUDE_DIRS
    NAMES convc.h
    HINTS ${PC_CONVC_INCLUDE_HINTS}
)

find_library (
    CONVC_LIBRARIES
    NAMES libconvc convc
    HINTS ${PC_CONVC_LIBRARY_HINTS}
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    CONVC
    REQUIRED_VARS CONVC_LIBRARIES CONVC_INCLUDE_DIRS
)
mark_as_advanced(
    CONVC_FOUND
    CONVC_LIBRARIES CONVC_INCLUDE_DIRS
)
