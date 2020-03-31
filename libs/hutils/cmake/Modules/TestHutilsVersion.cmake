# Read header file and extract version numbers

file(READ
    "${PROJECT_SOURCE_DIR}/include/hutils_classes.h"
    _HUTILS_H_CONTENTS
)

string(REGEX REPLACE
    ".*#define HUTILS_VERSION_MAJOR ([0-9]+).*"
    "\\1" HUTILS_VERSION_MAJOR
    "${_HUTILS_H_CONTENTS}"
)
string(REGEX REPLACE
    ".*#define HUTILS_VERSION_MINOR ([0-9]+).*"
    "\\1" HUTILS_VERSION_MINOR
    "${_HUTILS_H_CONTENTS}"
)
string(REGEX REPLACE
    ".*#define HUTILS_VERSION_PATCH ([0-9]+).*"
    "\\1" HUTILS_VERSION_PATCH
    "${_HUTILS_H_CONTENTS}"
)

set(HUTILS_VERSION
    "${HUTILS_VERSION_MAJOR}.${HUTILS_VERSION_MINOR}.${HUTILS_VERSION_PATCH}"
)

message(STATUS "Detected HUTILS Version - ${HUTILS_VERSION}")
