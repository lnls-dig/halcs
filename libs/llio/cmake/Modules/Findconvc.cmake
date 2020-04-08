# Based on:
# https://github.com/zeromq/malamute/blob/master/Findczmq.cmake

if (NOT MSVC)
    include(FindPkgConfig)
    pkg_check_modules(PC_convc "libconvc")
    if (PC_convc_FOUND)
        # add CFLAGS from pkg-config file, e.g. draft api.
        add_definitions(${PC_convc_CFLAGS} ${PC_convc_CFLAGS_OTHER})
        # some libraries install the headers is a subdirectory of the include dir
        # returned by pkg-config, so use a wildcard match to improve chances of finding
        # headers and SOs.
        set(PC_convc_INCLUDE_HINTS ${PC_convc_INCLUDE_DIRS} ${PC_convc_INCLUDE_DIRS}/*)
        set(PC_convc_LIBRARY_HINTS ${PC_convc_LIBRARY_DIRS} ${PC_convc_LIBRARY_DIRS}/*)
    endif(PC_convc_FOUND)
endif (NOT MSVC)

find_path (
    convc_INCLUDE_DIRS
    NAMES convc.h
    HINTS ${PC_convc_INCLUDE_HINTS}
)

find_library (
    convc_LIBRARIES
    NAMES libconvc convc
    HINTS ${PC_convc_LIBRARY_HINTS}
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    convc
    REQUIRED_VARS convc_LIBRARIES convc_INCLUDE_DIRS
)
mark_as_advanced(
    convc_FOUND
    convc_LIBRARIES convc_INCLUDE_DIRS
)
