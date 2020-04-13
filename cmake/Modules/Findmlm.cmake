# Copied from:
# https://github.com/zeromq/malamute/blob/master/Findczmq.cmake

if (NOT MSVC)
    include(FindPkgConfig)
    pkg_check_modules(PC_mlm IMPORTED_TARGET "libmlm")
    if (PC_mlm_FOUND)
        # To satisfy find_path()/find_library()
        set(PC_mlm_INCLUDE_HINTS ${PC_mlm_INCLUDE_DIRS} ${PC_mlm_INCLUDE_DIRS}/*)
        set(PC_mlm_LIBRARY_HINTS ${PC_mlm_LIBRARY_DIRS} ${PC_mlm_LIBRARY_DIRS}/*)
        # Imported target library name is all we need for
        # target_link_libraries().
        set(mlm_LIBRARIES PkgConfig::PC_mlm)
    endif(PC_mlm_FOUND)
endif (NOT MSVC)

find_path (
    _mlm_INCLUDE_DIRS
    NAMES malamute.h
    HINTS ${PC_mlm_INCLUDE_HINTS}
)

find_library (
    _mlm_LIBRARIES
    NAMES libmlm mlm
    HINTS ${PC_mlm_LIBRARY_HINTS}
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    mlm
    REQUIRED_VARS _mlm_LIBRARIES _mlm_INCLUDE_DIRS
)
mark_as_advanced(
    mlm_FOUND
    _mlm_LIBRARIES _mlm_INCLUDE_DIRS
)

# Create our INTERFACE library if not created by PkgConfig
if(mlm_FOUND AND NOT TARGET PkgConfig::PC_mlm AND NOT TARGET mlm::mlm)
    add_library(mlm::mlm
        INTERFACE IMPORTED
    )
    set_target_properties(mlm::mlm
        PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${_mlm_INCLUDE_DIRS}"
        INTERFACE_LINK_LIBRARIES "${_mlm_LIBRARIES}"
    )

    set(mlm_LIBRARIES mlm::mlm)
endif()
