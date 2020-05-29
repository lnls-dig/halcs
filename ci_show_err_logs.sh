#!/usr/bin/env bash

# help debug
set -x
set -e
set -o

if [ -n "$CPACK" ]; then
    # Show logs if present
    if [ -d build/_CPack_Packages/Linux ]; then
        CPACK_LOGS=$(find build/_CPack_Packages/Linux -iname "*.log")
        for log in "${CPACK_LOGS}"; do
            if [ -f "${log}" ]; then
                echo "Showing log ${log}:"
                echo "------------------------------------------------------"
                cat ${log}
                echo "------------------------------------------------------"
            fi
        done
    fi
fi
