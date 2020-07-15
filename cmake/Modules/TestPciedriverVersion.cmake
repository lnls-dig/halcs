# Read header file and extract version numbers

file(READ
    "${PROJECT_SOURCE_DIR}/foreign/pcie-driver/include/pcie/driver/pciDriver.h"
    _pciedriver_H_CONTENTS
)

string(REGEX REPLACE
    ".*#define PCIEDRIVER_VERSION_MAJOR ([0-9]+).*"
    "\\1" pciedriver_VERSION_MAJOR
    "${_pciedriver_H_CONTENTS}"
)
string(REGEX REPLACE
    ".*#define PCIEDRIVER_VERSION_MINOR ([0-9]+).*"
    "\\1" pciedriver_VERSION_MINOR
    "${_pciedriver_H_CONTENTS}"
)
string(REGEX REPLACE
    ".*#define PCIEDRIVER_VERSION_PATCH ([0-9]+).*"
    "\\1" pciedriver_VERSION_PATCH
    "${_pciedriver_H_CONTENTS}"
)

set(pciedriver_VERSION
    "${pciedriver_VERSION_MAJOR}.${pciedriver_VERSION_MINOR}.${pciedriver_VERSION_PATCH}"
)

message(STATUS "Detected pciedriver Version - ${pciedriver_VERSION}")
