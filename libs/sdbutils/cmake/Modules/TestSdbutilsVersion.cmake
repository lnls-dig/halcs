# Read header file and extract version numbers

file(READ
    "${PROJECT_SOURCE_DIR}/include/sdbutils_classes.h"
    _sdbutils_H_CONTENTS
)

string(REGEX REPLACE
    ".*#define SDBUTILS_VERSION_MAJOR ([0-9]+).*"
    "\\1" sdbutils_VERSION_MAJOR
    "${_sdbutils_H_CONTENTS}"
)
string(REGEX REPLACE
    ".*#define SDBUTILS_VERSION_MINOR ([0-9]+).*"
    "\\1" sdbutils_VERSION_MINOR
    "${_sdbutils_H_CONTENTS}"
)
string(REGEX REPLACE
    ".*#define SDBUTILS_VERSION_PATCH ([0-9]+).*"
    "\\1" sdbutils_VERSION_PATCH
    "${_sdbutils_H_CONTENTS}"
)

set(sdbutils_VERSION
    "${sdbutils_VERSION_MAJOR}.${sdbutils_VERSION_MINOR}.${sdbutils_VERSION_PATCH}"
)

message(STATUS "Detected sdbutils Version - ${sdbutils_VERSION}")
