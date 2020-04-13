# Based on:
# https://github.com/zeromq/malamute/blob/master/Findczmq.cmake

if (NOT MSVC)
    include(FindPkgConfig)
    pkg_check_modules(PC_sdbfs IMPORTED_TARGET "libsdbfs")
    if (PC_sdbfs_FOUND)
        # To satisfy find_path()/find_library()
        set(PC_sdbfs_INCLUDE_HINTS ${PC_sdbfs_INCLUDE_DIRS} ${PC_sdbfs_INCLUDE_DIRS}/*)
        set(PC_sdbfs_LIBRARY_HINTS ${PC_sdbfs_LIBRARY_DIRS} ${PC_sdbfs_LIBRARY_DIRS}/*)
        # Imported target library name is all we need for
        # target_link_libraries().
        set(sdbfs_LIBRARIES PkgConfig::PC_sdbfs)
    endif(PC_sdbfs_FOUND)
endif (NOT MSVC)

# Try to find on our own
find_path (
    _sdbfs_INCLUDE_DIRS
    NAMES libsdbfs.h
    HINTS ${PC_sdbfs_INCLUDE_HINTS}
)

find_library (
    _sdbfs_LIBRARIES
    NAMES libsdbfs sdbfs
    HINTS ${PC_sdbfs_LIBRARY_HINTS}
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    sdbfs
    REQUIRED_VARS _sdbfs_LIBRARIES _sdbfs_INCLUDE_DIRS
)
mark_as_advanced(
    sdbfs_FOUND
    _sdbfs_LIBRARIES _sdbfs_INCLUDE_DIRS
)

# Create our INTERFACE library if not created by PkgConfig
if(sdbfs_FOUND AND NOT TARGET PkgConfig::PC_sdbfs AND NOT TARGET sdbfs::sdbfs)
    add_library(sdbfs::sdbfs
        INTERFACE IMPORTED
    )
    set_target_properties(sdbfs::sdbfs
        PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${_sdbfs_INCLUDE_DIRS}"
        INTERFACE_LINK_LIBRARIES "${_sdbfs_LIBRARIES}"
    )

    set(sdbfs_LIBRARIES sdbfs::sdbfs)
endif()
