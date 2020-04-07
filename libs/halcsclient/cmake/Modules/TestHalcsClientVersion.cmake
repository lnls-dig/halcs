# Read header file and extract version numbers

file(READ
    "${PROJECT_SOURCE_DIR}/include/halcs_client_classes.h"
    _HALCSCLIENT_H_CONTENTS
)

string(REGEX REPLACE
    ".*#define HALCS_CLIENT_VERSION_MAJOR ([0-9]+).*"
    "\\1" HALCSCLIENT_VERSION_MAJOR
    "${_HALCSCLIENT_H_CONTENTS}"
)
string(REGEX REPLACE
    ".*#define HALCS_CLIENT_VERSION_MINOR ([0-9]+).*"
    "\\1" HALCSCLIENT_VERSION_MINOR
    "${_HALCSCLIENT_H_CONTENTS}"
)
string(REGEX REPLACE
    ".*#define HALCS_CLIENT_VERSION_PATCH ([0-9]+).*"
    "\\1" HALCSCLIENT_VERSION_PATCH
    "${_HALCSCLIENT_H_CONTENTS}"
)

set(HALCSCLIENT_VERSION
    "${HALCSCLIENT_VERSION_MAJOR}.${HALCSCLIENT_VERSION_MINOR}.${HALCSCLIENT_VERSION_PATCH}"
)

message(STATUS "Detected HALCSCLIENT Version - ${HALCSCLIENT_VERSION}")