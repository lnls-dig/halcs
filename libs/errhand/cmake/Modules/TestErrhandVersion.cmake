# Read header file and extract version numbers

file(READ
    "${PROJECT_SOURCE_DIR}/include/errhand_classes.h"
    _ERRHAND_H_CONTENTS
)

string(REGEX REPLACE
    ".*#define ERRHAND_VERSION_MAJOR ([0-9]+).*"
    "\\1" ERRHAND_VERSION_MAJOR
    "${_ERRHAND_H_CONTENTS}"
)
string(REGEX REPLACE
    ".*#define ERRHAND_VERSION_MINOR ([0-9]+).*"
    "\\1" ERRHAND_VERSION_MINOR
    "${_ERRHAND_H_CONTENTS}"
)
string(REGEX REPLACE
    ".*#define ERRHAND_VERSION_PATCH ([0-9]+).*"
    "\\1" ERRHAND_VERSION_PATCH
    "${_ERRHAND_H_CONTENTS}"
)

set(ERRHAND_VERSION
    "${ERRHAND_VERSION_MAJOR}.${ERRHAND_VERSION_MINOR}.${ERRHAND_VERSION_PATCH}"
)

message(STATUS "Detected ERRHAND Version - ${ERRHAND_VERSION}")
