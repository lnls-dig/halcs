# Read header file and extract version numbers

file(READ
    "${PROJECT_SOURCE_DIR}/include/ll_io_classes.h"
    _LLIO_H_CONTENTS
)

string(REGEX REPLACE
    ".*#define LL_IO_VERSION_MAJOR ([0-9]+).*"
    "\\1" LLIO_VERSION_MAJOR
    "${_LLIO_H_CONTENTS}"
)
string(REGEX REPLACE
    ".*#define LL_IO_VERSION_MINOR ([0-9]+).*"
    "\\1" LLIO_VERSION_MINOR
    "${_LLIO_H_CONTENTS}"
)
string(REGEX REPLACE
    ".*#define LL_IO_VERSION_PATCH ([0-9]+).*"
    "\\1" LLIO_VERSION_PATCH
    "${_LLIO_H_CONTENTS}"
)

set(LLIO_VERSION
    "${LLIO_VERSION_MAJOR}.${LLIO_VERSION_MINOR}.${LLIO_VERSION_PATCH}"
)

message(STATUS "Detected LLIO Version - ${LLIO_VERSION}")
