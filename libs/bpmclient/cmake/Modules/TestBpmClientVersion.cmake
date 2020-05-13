# Read header file and extract version numbers

file(READ
    "${PROJECT_SOURCE_DIR}/include/bpm_client_classes.h"
    _bpmclient_H_CONTENTS
)

string(REGEX REPLACE
    ".*#define BPM_CLIENT_VERSION_MAJOR ([0-9]+).*"
    "\\1" bpmclient_VERSION_MAJOR
    "${_bpmclient_H_CONTENTS}"
)
string(REGEX REPLACE
    ".*#define BPM_CLIENT_VERSION_MINOR ([0-9]+).*"
    "\\1" bpmclient_VERSION_MINOR
    "${_bpmclient_H_CONTENTS}"
)
string(REGEX REPLACE
    ".*#define BPM_CLIENT_VERSION_PATCH ([0-9]+).*"
    "\\1" bpmclient_VERSION_PATCH
    "${_bpmclient_H_CONTENTS}"
)

set(bpmclient_VERSION
    "${bpmclient_VERSION_MAJOR}.${bpmclient_VERSION_MINOR}.${bpmclient_VERSION_PATCH}"
)

message(STATUS "Detected bpmclient Version - ${bpmclient_VERSION}")
