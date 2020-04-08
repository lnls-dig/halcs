# Read header file and extract version numbers

file(READ
    "${PROJECT_SOURCE_DIR}/include/errhand_classes.h"
    _errhand_H_CONTENTS
)

string(REGEX REPLACE
    ".*#define ERRHAND_VERSION_MAJOR ([0-9]+).*"
    "\\1" errhand_VERSION_MAJOR
    "${_errhand_H_CONTENTS}"
)
string(REGEX REPLACE
    ".*#define ERRHAND_VERSION_MINOR ([0-9]+).*"
    "\\1" errhand_VERSION_MINOR
    "${_errhand_H_CONTENTS}"
)
string(REGEX REPLACE
    ".*#define ERRHAND_VERSION_PATCH ([0-9]+).*"
    "\\1" errhand_VERSION_PATCH
    "${_errhand_H_CONTENTS}"
)

set(errhand_VERSION
    "${errhand_VERSION_MAJOR}.${errhand_VERSION_MINOR}.${errhand_VERSION_PATCH}"
)

message(STATUS "Detected errhand Version - ${errhand_VERSION}")
