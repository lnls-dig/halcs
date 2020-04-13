# Based on:
# https://github.com/zeromq/malamute/blob/master/Findczmq.cmake

if (NOT MSVC)
    include(FindPkgConfig)
    pkg_check_modules(PC_llio IMPORTED_TARGET "libllio")
    if (PC_llio_FOUND)
        # To satisfy find_path()/find_library()
        set(PC_llio_INCLUDE_HINTS ${PC_llio_INCLUDE_DIRS} ${PC_llio_INCLUDE_DIRS}/*)
        set(PC_llio_LIBRARY_HINTS ${PC_llio_LIBRARY_DIRS} ${PC_llio_LIBRARY_DIRS}/*)
        # Imported target library name is all we need for
        # target_link_libraries().
        set(llio_LIBRARIES PkgConfig::PC_llio)
    endif(PC_llio_FOUND)
endif (NOT MSVC)

# Try to find on our own
find_path (
    _llio_INCLUDE_DIRS
    NAMES ll_io.h
    HINTS ${PC_llio_INCLUDE_HINTS}
)

find_library (
    _llio_LIBRARIES
    NAMES libllio llio
    HINTS ${PC_llio_LIBRARY_HINTS}
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    llio
    REQUIRED_VARS _llio_LIBRARIES _llio_INCLUDE_DIRS
)
mark_as_advanced(
    llio_FOUND
    _llio_LIBRARIES _llio_INCLUDE_DIRS
)

# Create our INTERFACE library if not created by PkgConfig
if(llio_FOUND AND NOT TARGET PkgConfig::PC_llio AND NOT TARGET llio::llio)
    add_library(llio::llio
        INTERFACE IMPORTED
    )
    set_target_properties(llio::llio
        PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${_llio_INCLUDE_DIRS}"
        INTERFACE_LINK_LIBRARIES "${_llio_LIBRARIES}"
    )

    set(llio_LIBRARIES llio::llio)
endif()
