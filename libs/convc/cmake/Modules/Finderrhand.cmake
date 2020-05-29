# Based on:
# https://github.com/zeromq/malamute/blob/master/Findczmq.cmake

if (NOT MSVC)
    include(FindPkgConfig)
    pkg_check_modules(PC_errhand IMPORTED_TARGET "liberrhand")
    if (PC_errhand_FOUND)
        # To satisfy find_path()/find_library()
        set(PC_errhand_INCLUDE_HINTS ${PC_errhand_INCLUDE_DIRS} ${PC_errhand_INCLUDE_DIRS}/*)
        set(PC_errhand_LIBRARY_HINTS ${PC_errhand_LIBRARY_DIRS} ${PC_errhand_LIBRARY_DIRS}/*)
        # Imported target library name is all we need for
        # target_link_libraries().
        set(errhand_LIBRARIES PkgConfig::PC_errhand)
    endif(PC_errhand_FOUND)
endif (NOT MSVC)

# Try to find on our own
find_path (
    _errhand_INCLUDE_DIRS
    NAMES errhand.h
    HINTS ${PC_errhand_INCLUDE_HINTS}
)

find_library (
    _errhand_LIBRARIES
    NAMES liberrhand errhand
    HINTS ${PC_errhand_LIBRARY_HINTS}
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    errhand
    REQUIRED_VARS _errhand_LIBRARIES _errhand_INCLUDE_DIRS
)
mark_as_advanced(
    errhand_FOUND
    _errhand_LIBRARIES _errhand_INCLUDE_DIRS
)

# Create our INTERFACE library if not created by PkgConfig
if(errhand_FOUND AND NOT TARGET PkgConfig::PC_errhand AND NOT TARGET errhand::errhand)
    add_library(errhand::errhand
        INTERFACE IMPORTED
    )
    set_target_properties(errhand::errhand
        PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${_errhand_INCLUDE_DIRS}"
        INTERFACE_LINK_LIBRARIES "${_errhand_LIBRARIES}"
        # Must be hardcoded if not found by PkgConfig
        INTERFACE_COMPILE_DEFINITIONS "ERRHAND_DBG=y;ERRHAND_MIN_LEVEL=DBG_LVL_INFO;ERRHAND_SUBSYS_ON=(DBG_DEV_MNGR|DBG_DEV_IO|DBG_SM_IO|DBG_LIB_CLIENT|DBG_SM_PR|DBG_SM_CH|DBG_LL_IO|DBG_HAL_UTILS)"
    )

    set(errhand_LIBRARIES errhand::errhand)
endif()
