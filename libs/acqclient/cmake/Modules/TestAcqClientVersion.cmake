# Read header file and extract version numbers

file(READ
    "${PROJECT_SOURCE_DIR}/include/acq_client_classes.h"
    _acqclient_H_CONTENTS
)

string(REGEX REPLACE
    ".*#define ACQ_CLIENT_VERSION_MAJOR ([0-9]+).*"
    "\\1" acqclient_VERSION_MAJOR
    "${_acqclient_H_CONTENTS}"
)
string(REGEX REPLACE
    ".*#define ACQ_CLIENT_VERSION_MINOR ([0-9]+).*"
    "\\1" acqclient_VERSION_MINOR
    "${_acqclient_H_CONTENTS}"
)
string(REGEX REPLACE
    ".*#define ACQ_CLIENT_VERSION_PATCH ([0-9]+).*"
    "\\1" acqclient_VERSION_PATCH
    "${_acqclient_H_CONTENTS}"
)

set(acqclient_VERSION
    "${acqclient_VERSION_MAJOR}.${acqclient_VERSION_MINOR}.${acqclient_VERSION_PATCH}"
)

message(STATUS "Detected acqclient Version - ${acqclient_VERSION}")
