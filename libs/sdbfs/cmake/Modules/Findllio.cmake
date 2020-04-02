# Copied from:
# https://github.com/zeromq/malamute/blob/master/Findczmq.cmake

if (NOT MSVC)
    include(FindPkgConfig)
    pkg_check_modules(PC_LLIO "libllio")
    if (PC_LLIO_FOUND)
        # add CFLAGS from pkg-config file, e.g. draft api.
        add_definitions(${PC_LLIO_CFLAGS} ${PC_LLIO_CFLAGS_OTHER})
        # some libraries install the headers is a subdirectory of the include dir
        # returned by pkg-config, so use a wildcard match to improve chances of finding
        # headers and SOs.
        set(PC_LLIO_INCLUDE_HINTS ${PC_LLIO_INCLUDE_DIRS} ${PC_LLIO_INCLUDE_DIRS}/*)
        set(PC_LLIO_LIBRARY_HINTS ${PC_LLIO_LIBRARY_DIRS} ${PC_LLIO_LIBRARY_DIRS}/*)
    endif(PC_LLIO_FOUND)
endif (NOT MSVC)

find_path (
    LLIO_INCLUDE_DIRS
    NAMES ll_io.h
    HINTS ${PC_LLIO_INCLUDE_HINTS}
)

find_library (
    LLIO_LIBRARIES
    NAMES libllio llio
    HINTS ${PC_LLIO_LIBRARY_HINTS}
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    LLIO
    REQUIRED_VARS LLIO_LIBRARIES LLIO_INCLUDE_DIRS
)
mark_as_advanced(
    LLIO_FOUND
    LLIO_LIBRARIES LLIO_INCLUDE_DIRS
)
