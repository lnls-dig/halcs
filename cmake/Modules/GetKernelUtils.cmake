########
# Find kernel version
#

find_program(UNAME_EXECUTABLE "uname")
if (${UNAME_EXECUTABLE} STREQUAL "UNAME_EXECUTABLE-NOTFOUND" AND
        UNAME_KERNEL_VERSION)
    message (FATAL_ERROR "Variable UNAME_KERNEL_VERSION \
        is defined, but we can't find uname program")
elseif(NOT ${UNAME_EXECUTABLE} STREQUAL "UNAME_EXECUTABLE-NOTFOUND" AND
        "${UNAME_KERNEL_VERSION}" STREQUAL "")
    message(STATUS "Uname program found: ${UNAME_EXECUTABLE}")
    # kernel version
    execute_process(COMMAND ${UNAME_EXECUTABLE}
        -r
        OUTPUT_VARIABLE UNAME_KERNEL_VERSION)
    string(REGEX REPLACE "[ \t\n]+" "" UNAME_KERNEL_VERSION
        "${UNAME_KERNEL_VERSION}")
endif()

if (NOT ${UNAME_EXECUTABLE} STREQUAL "UNAME_EXECUTABLE-NOTFOUND")
    set(WITH_UNAME_EXECUTABLE "ON")
    message(STATUS "Detected kernel version: ${UNAME_KERNEL_VERSION}")
else()
    set(WITH_UNAME_EXECUTABLE "OFF")
endif ()
