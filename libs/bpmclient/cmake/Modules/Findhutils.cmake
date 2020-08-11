# Based on:
# https://github.com/zeromq/malamute/blob/master/Findczmq.cmake

if (NOT MSVC)
    include(FindPkgConfig)
    pkg_check_modules(PC_hutils IMPORTED_TARGET "libhutils")
    if (PC_hutils_FOUND)
        # To satisfy find_path()/find_library()
        set(PC_hutils_INCLUDE_HINTS ${PC_hutils_INCLUDE_DIRS} ${PC_hutils_INCLUDE_DIRS}/*)
        set(PC_hutils_LIBRARY_HINTS ${PC_hutils_LIBRARY_DIRS} ${PC_hutils_LIBRARY_DIRS}/*)
        # Imported target library name is all we need for
        # target_link_libraries().
        set(hutils_LIBRARIES PkgConfig::PC_hutils)
    endif(PC_hutils_FOUND)
endif (NOT MSVC)

# Try to find on our own
find_path (
    _hutils_INCLUDE_DIRS
    NAMES hutils.h
    HINTS ${PC_hutils_INCLUDE_HINTS}
)

find_library (
    _hutils_LIBRARIES
    NAMES libhutils hutils
    HINTS ${PC_hutils_LIBRARY_HINTS}
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    hutils
    REQUIRED_VARS _hutils_LIBRARIES _hutils_INCLUDE_DIRS
)
mark_as_advanced(
    hutils_FOUND
    _hutils_LIBRARIES _hutils_INCLUDE_DIRS
)

# Create our INTERFACE library if not created by PkgConfig
if(hutils_FOUND AND NOT TARGET PkgConfig::PC_hutils AND NOT TARGET hutils::hutils)
    add_library(hutils::hutils
        INTERFACE IMPORTED
    )
    set_target_properties(hutils::hutils
        PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${_hutils_INCLUDE_DIRS}"
        INTERFACE_LINK_LIBRARIES "${_hutils_LIBRARIES}"
    )

    set(hutils_LIBRARIES hutils::hutils)
endif()
