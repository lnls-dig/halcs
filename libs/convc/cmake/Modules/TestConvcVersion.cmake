# Read header file and extract version numbers

file(READ
    "${PROJECT_SOURCE_DIR}/include/convc_classes.h"
    _CONVC_H_CONTENTS
)

string(REGEX REPLACE
    ".*#define CONVC_VERSION_MAJOR ([0-9]+).*"
    "\\1" CONVC_VERSION_MAJOR
    "${_CONVC_H_CONTENTS}"
)
string(REGEX REPLACE
    ".*#define CONVC_VERSION_MINOR ([0-9]+).*"
    "\\1" CONVC_VERSION_MINOR
    "${_CONVC_H_CONTENTS}"
)
string(REGEX REPLACE
    ".*#define CONVC_VERSION_PATCH ([0-9]+).*"
    "\\1" CONVC_VERSION_PATCH
    "${_CONVC_H_CONTENTS}"
)

set(CONVC_VERSION
    "${CONVC_VERSION_MAJOR}.${CONVC_VERSION_MINOR}.${CONVC_VERSION_PATCH}"
)

message(STATUS "Detected CONVC Version - ${CONVC_VERSION}")
