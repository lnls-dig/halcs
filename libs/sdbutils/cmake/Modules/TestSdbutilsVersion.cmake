# Read header file and extract version numbers

file(READ
    "${PROJECT_SOURCE_DIR}/include/sdbutils_classes.h"
    _SDBUTILS_H_CONTENTS
)

string(REGEX REPLACE
    ".*#define SDBUTILS_VERSION_MAJOR ([0-9]+).*"
    "\\1" SDBUTILS_VERSION_MAJOR
    "${_SDBUTILS_H_CONTENTS}"
)
string(REGEX REPLACE
    ".*#define SDBUTILS_VERSION_MINOR ([0-9]+).*"
    "\\1" SDBUTILS_VERSION_MINOR
    "${_SDBUTILS_H_CONTENTS}"
)
string(REGEX REPLACE
    ".*#define SDBUTILS_VERSION_PATCH ([0-9]+).*"
    "\\1" SDBUTILS_VERSION_PATCH
    "${_SDBUTILS_H_CONTENTS}"
)

set(SDBUTILS_VERSION
    "${SDBUTILS_VERSION_MAJOR}.${SDBUTILS_VERSION_MINOR}.${SDBUTILS_VERSION_PATCH}"
)

message(STATUS "Detected SDBUTILS Version - ${SDBUTILS_VERSION}")
