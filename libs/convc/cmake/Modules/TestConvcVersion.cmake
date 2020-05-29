# Read header file and extract version numbers

file(READ
    "${PROJECT_SOURCE_DIR}/include/convc_classes.h"
    _convc_H_CONTENTS
)

string(REGEX REPLACE
    ".*#define CONVC_VERSION_MAJOR ([0-9]+).*"
    "\\1" convc_VERSION_MAJOR
    "${_convc_H_CONTENTS}"
)
string(REGEX REPLACE
    ".*#define CONVC_VERSION_MINOR ([0-9]+).*"
    "\\1" convc_VERSION_MINOR
    "${_convc_H_CONTENTS}"
)
string(REGEX REPLACE
    ".*#define CONVC_VERSION_PATCH ([0-9]+).*"
    "\\1" convc_VERSION_PATCH
    "${_convc_H_CONTENTS}"
)

set(convc_VERSION
    "${convc_VERSION_MAJOR}.${convc_VERSION_MINOR}.${convc_VERSION_PATCH}"
)

message(STATUS "Detected convc Version - ${convc_VERSION}")
