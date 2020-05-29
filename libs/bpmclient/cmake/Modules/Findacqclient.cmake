# Based on:
# https://github.com/zeromq/malamute/blob/master/Findczmq.cmake

if (NOT MSVC)
    include(FindPkgConfig)
    pkg_check_modules(PC_acqclient IMPORTED_TARGET "libacqclient")
    if (PC_acqclient_FOUND)
        # To satisfy find_path()/find_library()
        set(PC_acqclient_INCLUDE_HINTS ${PC_acqclient_INCLUDE_DIRS} ${PC_acqclient_INCLUDE_DIRS}/*)
        set(PC_acqclient_LIBRARY_HINTS ${PC_acqclient_LIBRARY_DIRS} ${PC_acqclient_LIBRARY_DIRS}/*)
        # Imported target library name is all we need for
        # target_link_libraries().
        set(acqclient_LIBRARIES PkgConfig::PC_acqclient)
    endif(PC_acqclient_FOUND)
endif (NOT MSVC)

# Try to find on our own
find_path (
    _acqclient_INCLUDE_DIRS
    NAMES acq_client.h
    HINTS ${PC_acqclient_INCLUDE_HINTS}
)

find_library (
    _acqclient_LIBRARIES
    NAMES libacqclient acqclient
    HINTS ${PC_acqclient_LIBRARY_HINTS}
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    acqclient
    REQUIRED_VARS _acqclient_LIBRARIES _acqclient_INCLUDE_DIRS
)
mark_as_advanced(
    acqclient_FOUND
    _acqclient_LIBRARIES _acqclient_INCLUDE_DIRS
)

# Create our INTERFACE library if not created by PkgConfig
if(acqclient_FOUND AND NOT TARGET PkgConfig::PC_acqclient AND NOT TARGET acqclient::acqclient)
    add_library(acqclient::acqclient
        INTERFACE IMPORTED
    )
    set_target_properties(acqclient::acqclient
        PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${_acqclient_INCLUDE_DIRS}"
        INTERFACE_LINK_LIBRARIES "${_acqclient_LIBRARIES}"
    )

    set(acqclient_LIBRARIES acqclient::acqclient)
endif()
