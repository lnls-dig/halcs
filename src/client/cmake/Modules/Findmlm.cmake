# Based on:
# https://github.com/zeromq/malamute/blob/master/Findczmq.cmake

if (NOT MSVC)
    include(FindPkgConfig)
    pkg_check_modules(PC_mlm "libmlm")
    if (PC_mlm_FOUND)
        # add CFLAGS from pkg-config file, e.g. draft api.
        add_definitions(${PC_mlm_CFLAGS} ${PC_mlm_CFLAGS_OTHER})
        # some libraries install the headers is a subdirectory of the include dir
        # returned by pkg-config, so use a wildcard match to improve chances of finding
        # headers and SOs.
        set(PC_mlm_INCLUDE_HINTS ${PC_mlm_INCLUDE_DIRS} ${PC_mlm_INCLUDE_DIRS}/*)
        set(PC_mlm_LIBRARY_HINTS ${PC_mlm_LIBRARY_DIRS} ${PC_mlm_LIBRARY_DIRS}/*)
    endif(PC_mlm_FOUND)
endif (NOT MSVC)

find_path (
    mlm_INCLUDE_DIRS
    NAMES malamute.h
    HINTS ${PC_mlm_INCLUDE_HINTS}
)

find_library (
    mlm_LIBRARIES
    NAMES libmlm mlm
    HINTS ${PC_mlm_LIBRARY_HINTS}
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    mlm
    REQUIRED_VARS mlm_LIBRARIES mlm_INCLUDE_DIRS
)
mark_as_advanced(
    mlm_FOUND
    mlm_LIBRARIES mlm_INCLUDE_DIRS
)
