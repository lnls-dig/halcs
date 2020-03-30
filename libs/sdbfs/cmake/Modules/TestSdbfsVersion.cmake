# Read header file and extract version numbers

file(READ
    "${PROJECT_SOURCE_DIR}/include/libsdbfs.h"
    _SDBFS_H_CONTENTS
)

string(REGEX REPLACE
    ".*#define SDBFS_VERSION_MAJOR ([0-9]+).*"
    "\\1" SDBFS_VERSION_MAJOR
    "${_SDBFS_H_CONTENTS}"
)
string(REGEX REPLACE
    ".*#define SDBFS_VERSION_MINOR ([0-9]+).*"
    "\\1" SDBFS_VERSION_MINOR
    "${_SDBFS_H_CONTENTS}"
)
string(REGEX REPLACE
    ".*#define SDBFS_VERSION_PATCH ([0-9]+).*"
    "\\1" SDBFS_VERSION_PATCH
    "${_SDBFS_H_CONTENTS}"
)

set(SDBFS_VERSION
    "${SDBFS_VERSION_MAJOR}.${SDBFS_VERSION_MINOR}.${SDBFS_VERSION_PATCH}"
)

message(STATUS "Detected SDBFS Version - ${SDBFS_VERSION}")
