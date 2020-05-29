# Read header file and extract version numbers

file(READ
    "${PROJECT_SOURCE_DIR}/foreign/libbsmp/src/bsmp/c/server_priv.h"
    _bsmp_H_CONTENTS
)

string(REGEX REPLACE
    ".*#define VERSION +([0-9]+).*"
    "\\1" bsmp_VERSION_MAJOR
    "${_bsmp_H_CONTENTS}"
)
string(REGEX REPLACE
    ".*#define SUBVERSION +([0-9]+).*"
    "\\1" bsmp_VERSION_MINOR
    "${_bsmp_H_CONTENTS}"
)
string(REGEX REPLACE
    ".*#define REVISION +([0-9]+).*"
    "\\1" bsmp_VERSION_PATCH
    "${_bsmp_H_CONTENTS}"
)

set(bsmp_VERSION
    "${bsmp_VERSION_MAJOR}.${bsmp_VERSION_MINOR}.${bsmp_VERSION_PATCH}"
)

message(STATUS "Detected bsmp Version - ${bsmp_VERSION}")
