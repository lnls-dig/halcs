# Based on:
# https://github.com/zeromq/malamute/blob/master/Findczmq.cmake

if (NOT MSVC)
    include(FindPkgConfig)
    pkg_check_modules(PC_MLM "libmlm")
    if (PC_MLM_FOUND)
        # add CFLAGS from pkg-config file, e.g. draft api.
        add_definitions(${PC_MLM_CFLAGS} ${PC_MLM_CFLAGS_OTHER})
        # some libraries install the headers is a subdirectory of the include dir
        # returned by pkg-config, so use a wildcard match to improve chances of finding
        # headers and SOs.
        set(PC_MLM_INCLUDE_HINTS ${PC_MLM_INCLUDE_DIRS} ${PC_MLM_INCLUDE_DIRS}/*)
        set(PC_MLM_LIBRARY_HINTS ${PC_MLM_LIBRARY_DIRS} ${PC_MLM_LIBRARY_DIRS}/*)
    endif(PC_MLM_FOUND)
endif (NOT MSVC)

find_path (
    MLM_INCLUDE_DIRS
    NAMES malamute.h
    HINTS ${PC_MLM_INCLUDE_HINTS}
)

find_library (
    MLM_LIBRARIES
    NAMES libmlm mlm
    HINTS ${PC_MLM_LIBRARY_HINTS}
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    MLM
    REQUIRED_VARS MLM_LIBRARIES MLM_INCLUDE_DIRS
)
mark_as_advanced(
    MLM_FOUND
    MLM_LIBRARIES MLM_INCLUDE_DIRS
)
