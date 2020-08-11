# Read header file and extract version numbers

file(READ
    "${PROJECT_SOURCE_DIR}/include/disptable_classes.h"
    _disptable_H_CONTENTS
)

string(REGEX REPLACE
    ".*#define DISPTABLE_VERSION_MAJOR ([0-9]+).*"
    "\\1" disptable_VERSION_MAJOR
    "${_disptable_H_CONTENTS}"
)
string(REGEX REPLACE
    ".*#define DISPTABLE_VERSION_MINOR ([0-9]+).*"
    "\\1" disptable_VERSION_MINOR
    "${_disptable_H_CONTENTS}"
)
string(REGEX REPLACE
    ".*#define DISPTABLE_VERSION_PATCH ([0-9]+).*"
    "\\1" disptable_VERSION_PATCH
    "${_disptable_H_CONTENTS}"
)

set(disptable_VERSION
    "${disptable_VERSION_MAJOR}.${disptable_VERSION_MINOR}.${disptable_VERSION_PATCH}"
)

message(STATUS "Detected disptable Version - ${disptable_VERSION}")
