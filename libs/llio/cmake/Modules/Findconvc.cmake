# Based on:
# https://github.com/zeromq/malamute/blob/master/Findczmq.cmake

if (NOT MSVC)
    include(FindPkgConfig)
    pkg_check_modules(PC_convc IMPORTED_TARGET "libconvc")
    if (PC_convc_FOUND)
        # To satisfy find_path()/find_library()
        set(PC_convc_INCLUDE_HINTS ${PC_convc_INCLUDE_DIRS} ${PC_convc_INCLUDE_DIRS}/*)
        set(PC_convc_LIBRARY_HINTS ${PC_convc_LIBRARY_DIRS} ${PC_convc_LIBRARY_DIRS}/*)
        # Imported target library name is all we need for
        # target_link_libraries().
        set(convc_LIBRARIES PkgConfig::PC_convc)
    endif(PC_convc_FOUND)
endif (NOT MSVC)

# Try to find on our own
find_path (
    _convc_INCLUDE_DIRS
    NAMES convc.h
    HINTS ${PC_convc_INCLUDE_HINTS}
)

find_library (
    _convc_LIBRARIES
    NAMES libconvc convc
    HINTS ${PC_convc_LIBRARY_HINTS}
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    convc
    REQUIRED_VARS _convc_LIBRARIES _convc_INCLUDE_DIRS
)
mark_as_advanced(
    convc_FOUND
    _convc_LIBRARIES _convc_INCLUDE_DIRS
)

# Create our INTERFACE library if not created by PkgConfig
if(convc_FOUND AND NOT TARGET PkgConfig::PC_convc AND NOT TARGET convc::convc)
    add_library(convc::convc
        INTERFACE IMPORTED
    )
    set_target_properties(convc::convc
        PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${_convc_INCLUDE_DIRS}"
        INTERFACE_LINK_LIBRARIES "${_convc_LIBRARIES}"
        # Must be hardcoded if not found by PkgConfig
        INTERFACE_COMPILE_DEFINITIONS "convc_DBG=y;convc_MIN_LEVEL=DBG_LVL_INFO;convc_SUBSYS_ON=(DBG_DEV_MNGR|DBG_DEV_IO|DBG_SM_IO|DBG_LIB_CLIENT|DBG_SM_PR|DBG_SM_CH|DBG_LL_IO|DBG_HAL_UTILS)"
    )

    set(convc_LIBRARIES convc::convc)
endif()
