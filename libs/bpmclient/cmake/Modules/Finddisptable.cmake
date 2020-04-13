# Based on:
# https://github.com/zeromq/malamute/blob/master/Findczmq.cmake

if (NOT MSVC)
    include(FindPkgConfig)
    pkg_check_modules(PC_disptable IMPORTED_TARGET "libdisptable")
    if (PC_disptable_FOUND)
        # To satisfy find_path()/find_library()
        set(PC_disptable_INCLUDE_HINTS ${PC_disptable_INCLUDE_DIRS} ${PC_disptable_INCLUDE_DIRS}/*)
        set(PC_disptable_LIBRARY_HINTS ${PC_disptable_LIBRARY_DIRS} ${PC_disptable_LIBRARY_DIRS}/*)
        # Imported target library name is all we need for
        # target_link_libraries().
        set(disptable_LIBRARIES PkgConfig::PC_disptable)
    endif(PC_disptable_FOUND)
endif (NOT MSVC)

# Try to find on our own
find_path (
    _disptable_INCLUDE_DIRS
    NAMES disptable.h
    HINTS ${PC_disptable_INCLUDE_HINTS}
)

find_library (
    _disptable_LIBRARIES
    NAMES libdisptable disptable
    HINTS ${PC_disptable_LIBRARY_HINTS}
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    disptable
    REQUIRED_VARS _disptable_LIBRARIES _disptable_INCLUDE_DIRS
)
mark_as_advanced(
    disptable_FOUND
    _disptable_LIBRARIES _disptable_INCLUDE_DIRS
)

# Create our INTERFACE library if not created by PkgConfig
if(disptable_FOUND AND NOT TARGET PkgConfig::PC_disptable AND NOT TARGET disptable::disptable)
    add_library(disptable::disptable
        INTERFACE IMPORTED
    )
    set_target_properties(disptable::disptable
        PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${_disptable_INCLUDE_DIRS}"
        INTERFACE_LINK_LIBRARIES "${_disptable_LIBRARIES}"
    )

    set(disptable_LIBRARIES disptable::disptable)
endif()
