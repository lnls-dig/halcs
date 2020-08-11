# Read header file and extract version numbers

file(READ
    "${PROJECT_SOURCE_DIR}/include/halcs_examples_classes.h"
    _halcs_examples_H_CONTENTS
)

string(REGEX REPLACE
    ".*#define HALCS_EXAMPLES_VERSION_MAJOR ([0-9]+).*"
    "\\1" halcs_examples_VERSION_MAJOR
    "${_halcs_examples_H_CONTENTS}"
)
string(REGEX REPLACE
    ".*#define HALCS_EXAMPLES_VERSION_MINOR ([0-9]+).*"
    "\\1" halcs_examples_VERSION_MINOR
    "${_halcs_examples_H_CONTENTS}"
)
string(REGEX REPLACE
    ".*#define HALCS_EXAMPLES_VERSION_PATCH ([0-9]+).*"
    "\\1" halcs_examples_VERSION_PATCH
    "${_halcs_examples_H_CONTENTS}"
)

set(halcs_examples_VERSION
    "${halcs_examples_VERSION_MAJOR}.${halcs_examples_VERSION_MINOR}.${halcs_examples_VERSION_PATCH}"
)

message(STATUS "Detected halcs_examples Version - ${halcs_examples_VERSION}")
