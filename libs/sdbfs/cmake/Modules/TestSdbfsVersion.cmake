# Read header file and extract version numbers

file(READ
    "${PROJECT_SOURCE_DIR}/include/libsdbfs.h"
    _sdbfs_H_CONTENTS
)

string(REGEX REPLACE
    ".*#define SDBFS_VERSION_MAJOR ([0-9]+).*"
    "\\1" sdbfs_VERSION_MAJOR
    "${_sdbfs_H_CONTENTS}"
)
string(REGEX REPLACE
    ".*#define SDBFS_VERSION_MINOR ([0-9]+).*"
    "\\1" sdbfs_VERSION_MINOR
    "${_sdbfs_H_CONTENTS}"
)
string(REGEX REPLACE
    ".*#define SDBFS_VERSION_PATCH ([0-9]+).*"
    "\\1" sdbfs_VERSION_PATCH
    "${_sdbfs_H_CONTENTS}"
)

set(sdbfs_VERSION
    "${sdbfs_VERSION_MAJOR}.${sdbfs_VERSION_MINOR}.${sdbfs_VERSION_PATCH}"
)

message(STATUS "Detected sdbfs Version - ${sdbfs_VERSION}")
