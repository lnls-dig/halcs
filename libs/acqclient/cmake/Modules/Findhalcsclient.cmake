# Based on:
# https://github.com/zeromq/malamute/blob/master/Findczmq.cmake

if (NOT MSVC)
    include(FindPkgConfig)
    pkg_check_modules(PC_halcsclient IMPORTED_TARGET "libhalcsclient")
    if (PC_halcsclient_FOUND)
        # To satisfy find_path()/find_library()
        set(PC_halcsclient_INCLUDE_HINTS ${PC_halcsclient_INCLUDE_DIRS} ${PC_halcsclient_INCLUDE_DIRS}/*)
        set(PC_halcsclient_LIBRARY_HINTS ${PC_halcsclient_LIBRARY_DIRS} ${PC_halcsclient_LIBRARY_DIRS}/*)
        # Imported target library name is all we need for
        # target_link_libraries().
        set(halcsclient_LIBRARIES PkgConfig::PC_halcsclient)
    endif(PC_halcsclient_FOUND)
endif (NOT MSVC)

# Try to find on our own
find_path (
    _halcsclient_INCLUDE_DIRS
    NAMES halcs_client.h
    HINTS ${PC_halcsclient_INCLUDE_HINTS}
)

find_library (
    _halcsclient_LIBRARIES
    NAMES libhalcsclient halcsclient
    HINTS ${PC_halcsclient_LIBRARY_HINTS}
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    halcsclient
    REQUIRED_VARS _halcsclient_LIBRARIES _halcsclient_INCLUDE_DIRS
)
mark_as_advanced(
    halcsclient_FOUND
    _halcsclient_LIBRARIES _halcsclient_INCLUDE_DIRS
)

# Create our INTERFACE library if not created by PkgConfig
if(halcsclient_FOUND AND NOT TARGET PkgConfig::PC_halcsclient AND NOT TARGET halcsclient::halcsclient)
    add_library(halcsclient::halcsclient
        INTERFACE IMPORTED
    )
    set_target_properties(halcsclient::halcsclient
        PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${_halcsclient_INCLUDE_DIRS}"
        INTERFACE_LINK_LIBRARIES "${_halcsclient_LIBRARIES}"
        # Must be hardcoded if not found by PkgConfig
        INTERFACE_COMPILE_DEFINITIONS "__BOARD_AFCV3__;__BOARD_AFCV3_1__"
    )

    set(halcsclient_LIBRARIES halcsclient::halcsclient)
endif()
