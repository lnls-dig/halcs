# Read header file and extract version numbers

file(READ
    "${PROJECT_SOURCE_DIR}/include/ll_io_classes.h"
    _llio_H_CONTENTS
)

string(REGEX REPLACE
    ".*#define LL_IO_VERSION_MAJOR ([0-9]+).*"
    "\\1" llio_VERSION_MAJOR
    "${_llio_H_CONTENTS}"
)
string(REGEX REPLACE
    ".*#define LL_IO_VERSION_MINOR ([0-9]+).*"
    "\\1" llio_VERSION_MINOR
    "${_llio_H_CONTENTS}"
)
string(REGEX REPLACE
    ".*#define LL_IO_VERSION_PATCH ([0-9]+).*"
    "\\1" llio_VERSION_PATCH
    "${_llio_H_CONTENTS}"
)

set(llio_VERSION
    "${llio_VERSION_MAJOR}.${llio_VERSION_MINOR}.${llio_VERSION_PATCH}"
)

message(STATUS "Detected llio Version - ${llio_VERSION}")
