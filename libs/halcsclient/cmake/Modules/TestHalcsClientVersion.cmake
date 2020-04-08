# Read header file and extract version numbers

file(READ
    "${PROJECT_SOURCE_DIR}/include/halcs_client_classes.h"
    _halcsclient_H_CONTENTS
)

string(REGEX REPLACE
    ".*#define HALCS_CLIENT_VERSION_MAJOR ([0-9]+).*"
    "\\1" halcsclient_VERSION_MAJOR
    "${_halcsclient_H_CONTENTS}"
)
string(REGEX REPLACE
    ".*#define HALCS_CLIENT_VERSION_MINOR ([0-9]+).*"
    "\\1" halcsclient_VERSION_MINOR
    "${_halcsclient_H_CONTENTS}"
)
string(REGEX REPLACE
    ".*#define HALCS_CLIENT_VERSION_PATCH ([0-9]+).*"
    "\\1" halcsclient_VERSION_PATCH
    "${_halcsclient_H_CONTENTS}"
)

set(halcsclient_VERSION
    "${halcsclient_VERSION_MAJOR}.${halcsclient_VERSION_MINOR}.${halcsclient_VERSION_PATCH}"
)

message(STATUS "Detected halcsclient Version - ${halcsclient_VERSION}")
