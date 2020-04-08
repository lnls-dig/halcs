# Read header file and extract version numbers

file(READ
    "${PROJECT_SOURCE_DIR}/include/hutils_classes.h"
    _hutils_H_CONTENTS
)

string(REGEX REPLACE
    ".*#define HUTILS_VERSION_MAJOR ([0-9]+).*"
    "\\1" hutils_VERSION_MAJOR
    "${_hutils_H_CONTENTS}"
)
string(REGEX REPLACE
    ".*#define HUTILS_VERSION_MINOR ([0-9]+).*"
    "\\1" hutils_VERSION_MINOR
    "${_hutils_H_CONTENTS}"
)
string(REGEX REPLACE
    ".*#define HUTILS_VERSION_PATCH ([0-9]+).*"
    "\\1" hutils_VERSION_PATCH
    "${_hutils_H_CONTENTS}"
)

set(hutils_VERSION
    "${hutils_VERSION_MAJOR}.${hutils_VERSION_MINOR}.${hutils_VERSION_PATCH}"
)

message(STATUS "Detected hutils Version - ${hutils_VERSION}")
