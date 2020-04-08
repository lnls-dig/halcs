# Copied from:
# https://github.com/zeromq/malamute/blob/master/Findczmq.cmake

if (NOT MSVC)
    include(FindPkgConfig)
    pkg_check_modules(PC_llio "libllio")
    if (PC_llio_FOUND)
        # add CFLAGS from pkg-config file, e.g. draft api.
        add_definitions(${PC_llio_CFLAGS} ${PC_llio_CFLAGS_OTHER})
        # some libraries install the headers is a subdirectory of the include dir
        # returned by pkg-config, so use a wildcard match to improve chances of finding
        # headers and SOs.
        set(PC_llio_INCLUDE_HINTS ${PC_llio_INCLUDE_DIRS} ${PC_llio_INCLUDE_DIRS}/*)
        set(PC_llio_LIBRARY_HINTS ${PC_llio_LIBRARY_DIRS} ${PC_llio_LIBRARY_DIRS}/*)
    endif(PC_llio_FOUND)
endif (NOT MSVC)

find_path (
    llio_INCLUDE_DIRS
    NAMES ll_io.h
    HINTS ${PC_llio_INCLUDE_HINTS}
)

find_library (
    llio_LIBRARIES
    NAMES libllio llio
    HINTS ${PC_llio_LIBRARY_HINTS}
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    llio
    REQUIRED_VARS llio_LIBRARIES llio_INCLUDE_DIRS
)
mark_as_advanced(
    llio_FOUND
    llio_LIBRARIES llio_INCLUDE_DIRS
)
