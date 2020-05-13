# Copied from:
# https://github.com/zeromq/czmq/blob/master/Findlibzmq.cmake

if (NOT MSVC)
    include(FindPkgConfig)
    pkg_check_modules(PC_libzmq IMPORTED_TARGET "libzmq")
    if (PC_libzmq_FOUND)
        # To satisfy find_path()/find_library()
        set(PC_libzmq_INCLUDE_HINTS ${PC_libzmq_INCLUDE_DIRS} ${PC_libzmq_INCLUDE_DIRS}/*)
        set(PC_libzmq_LIBRARY_HINTS ${PC_libzmq_LIBRARY_DIRS} ${PC_libzmq_LIBRARY_DIRS}/*)
        # Imported target library name is all we need for
        # target_link_libraries().
        set(libzmq_LIBRARIES PkgConfig::PC_libzmq)
    endif(PC_libzmq_FOUND)
endif (NOT MSVC)

find_path (
    _libzmq_INCLUDE_DIRS
    NAMES zmq.h
    HINTS ${PC_libzmq_INCLUDE_HINTS}
)

if (MSVC)
    # libzmq dll/lib built with MSVC is named using the Boost convention.
    # https://github.com/zeromq/czmq/issues/577
    # https://github.com/zeromq/czmq/issues/1972
    if (MSVC_IDE)
        set(MSVC_TOOLSET "-${CMAKE_VS_PLATFORM_TOOLSET}")
    else ()
        set(MSVC_TOOLSET "")
    endif ()

    # Retrieve ZeroMQ version number from zmq.h
    file(STRINGS "${_libzmq_INCLUDE_DIRS}/zmq.h" zmq_version_defines
        REGEX "#define ZMQ_VERSION_(MAJOR|MINOR|PATCH)")
    foreach(ver ${zmq_version_defines})
        if(ver MATCHES "#define ZMQ_VERSION_(MAJOR|MINOR|PATCH) +([^ ]+)$")
            set(ZMQ_VERSION_${CMAKE_MATCH_1} "${CMAKE_MATCH_2}" CACHE INTERNAL "")
        endif()
    endforeach()

    set(_zmq_version ${ZMQ_VERSION_MAJOR}_${ZMQ_VERSION_MINOR}_${ZMQ_VERSION_PATCH})

    set(_zmq_debug_names
        "libzmq${MSVC_TOOLSET}-mt-gd-${_zmq_version}" # Debug, BUILD_SHARED
        "libzmq${MSVC_TOOLSET}-mt-sgd-${_zmq_version}" # Debug, BUILD_STATIC
        "libzmq-mt-gd-${_zmq_version}" # Debug, BUILD_SHARED
        "libzmq-mt-sgd-${_zmq_version}" # Debug, BUILD_STATIC
    )

    set(_zmq_release_names
        "libzmq${MSVC_TOOLSET}-mt-${_zmq_version}" # Release|RelWithDebInfo|MinSizeRel, BUILD_SHARED
        "libzmq${MSVC_TOOLSET}-mt-s-${_zmq_version}" # Release|RelWithDebInfo|MinSizeRel, BUILD_STATIC
        "libzmq-mt-${_zmq_version}" # Release|RelWithDebInfo|MinSizeRel, BUILD_SHARED
        "libzmq-mt-s-${_zmq_version}" # Release|RelWithDebInfo|MinSizeRel, BUILD_STATIC
    )

    find_library (libzmq_LIBRARY_DEBUG
        NAMES ${_zmq_debug_names}
    )

    find_library (libzmq_LIBRARY_RELEASE
        NAMES ${_zmq_release_names}
    )

    include(SelectLibraryConfigurations)
    select_library_configurations(libzmq)
endif ()

if (NOT _libzmq_LIBRARIES)
    find_library (
        _libzmq_LIBRARIES
        NAMES libzmq zmq
        HINTS ${PC_libzmq_LIBRARY_HINTS}
    )
endif ()

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    libzmq
    REQUIRED_VARS _libzmq_LIBRARIES _libzmq_INCLUDE_DIRS
)
mark_as_advanced(
    libzmq_FOUND
    _libzmq_LIBRARIES _libzmq_INCLUDE_DIRS
)

# Create our INTERFACE library if not created by PkgConfig
if(libzmq_FOUND AND NOT TARGET PkgConfig::PC_libzmq AND NOT TARGET libzmq::libzmq)
    add_library(libzmq::libzmq
        INTERFACE IMPORTED
    )
    set_target_properties(libzmq::libzmq
        PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${_libzmq_INCLUDE_DIRS}"
        INTERFACE_LINK_LIBRARIES "${_libzmq_LIBRARIES}"
    )

    set(libzmq_LIBRARIES libzmq::libzmq)
endif()
