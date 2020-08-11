# Read header file and extract version numbers

file(READ
    "${PROJECT_SOURCE_DIR}/include/revision.h"
    _halcs_H_CONTENTS
)

string(REGEX REPLACE
    ".*#define HALCS_VERSION_MAJOR ([0-9]+).*"
    "\\1" halcs_VERSION_MAJOR
    "${_halcs_H_CONTENTS}"
)
string(REGEX REPLACE
    ".*#define HALCS_VERSION_MINOR ([0-9]+).*"
    "\\1" halcs_VERSION_MINOR
    "${_halcs_H_CONTENTS}"
)
string(REGEX REPLACE
    ".*#define HALCS_VERSION_PATCH ([0-9]+).*"
    "\\1" halcs_VERSION_PATCH
    "${_halcs_H_CONTENTS}"
)

set(halcs_VERSION
    "${halcs_VERSION_MAJOR}.${halcs_VERSION_MINOR}.${halcs_VERSION_PATCH}"
)

message(STATUS "Detected halcs Version - ${halcs_VERSION}")
