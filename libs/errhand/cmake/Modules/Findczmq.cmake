# Copied from:
# https://github.com/zeromq/malamute/blob/master/Findczmq.cmake

if (NOT MSVC)
    include(FindPkgConfig)
    pkg_check_modules(PC_czmq IMPORTED_TARGET "libczmq")
    if (PC_czmq_FOUND)
        # To satisfy find_path()/find_library()
        set(PC_czmq_INCLUDE_HINTS ${PC_czmq_INCLUDE_DIRS} ${PC_czmq_INCLUDE_DIRS}/*)
        set(PC_czmq_LIBRARY_HINTS ${PC_czmq_LIBRARY_DIRS} ${PC_czmq_LIBRARY_DIRS}/*)
        # Imported target library name is all we need for
        # target_link_libraries().
        set(czmq_LIBRARIES PkgConfig::PC_czmq)
    endif(PC_czmq_FOUND)
endif (NOT MSVC)

find_path (
    _czmq_INCLUDE_DIRS
    NAMES czmq.h
    HINTS ${PC_czmq_INCLUDE_HINTS}
)

find_library (
    _czmq_LIBRARIES
    NAMES libczmq czmq
    HINTS ${PC_czmq_LIBRARY_HINTS}
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    czmq
    REQUIRED_VARS _czmq_LIBRARIES _czmq_INCLUDE_DIRS
)
mark_as_advanced(
    czmq_FOUND
    _czmq_LIBRARIES _czmq_INCLUDE_DIRS
)

# Create our INTERFACE library if not created by PkgConfig
if(czmq_FOUND AND NOT TARGET PkgConfig::PC_czmq AND NOT TARGET czmq::czmq)
    add_library(czmq::czmq
        INTERFACE IMPORTED
    )
    set_target_properties(czmq::czmq
        PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${_czmq_INCLUDE_DIRS}"
        INTERFACE_LINK_LIBRARIES "${_czmq_LIBRARIES}"
    )

    set(czmq_LIBRARIES czmq::czmq)
endif()
