# Read header file and extract version numbers

# PCIe Driver doesn't have the notion of version numbers....
# assign 1.0.0

set(pciedriver_VERSION_MAJOR
    1
)
set(pciedriver_VERSION_MINOR
    0
)
set(pciedriver_VERSION_PATCH
    0
)

set(pciedriver_VERSION
    "${pciedriver_VERSION_MAJOR}.${pciedriver_VERSION_MINOR}.${pciedriver_VERSION_PATCH}"
)

message(STATUS "Detected pciedriver Version - ${pciedriver_VERSION}")
