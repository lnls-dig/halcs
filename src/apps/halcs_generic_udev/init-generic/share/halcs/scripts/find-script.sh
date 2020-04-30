#!/usr/bin/env bash

SCRIPT_NAME="$1"

# Generate HALCS instances. FIXME: improve script location
LOCATION_GUESSES=()
LOCATION_GUESSES+=("/usr/local/share")
LOCATION_GUESSES+=("/usr/share")
LOCATION_GUESSES+=("/usr/local/bin")
LOCATION_GUESSES+=("/usr/bin")
LOCATION_GUESSES+=("/bin")

# Guess common location for the script
for loc in "${LOCATION_GUESSES[@]}"; do
    script_rel=$(find ${loc} -iname "${SCRIPT_NAME}" | head -n 1)
    if [ -z "${script_rel}" ]; then
        continue;
    fi

    script=$(realpath ${script_rel})
    if [ ! -z "${script}" ]; then
        break;
    fi
done

# The last resort if to try the PATH
if [ -z "${script}" ]; then
    script=$(realpath $(which "${SCRIPT_NAME}" | head -n 1))
    if [ -z "${script}" ]; then
        echo "Could not find script: ${SCRIPT_NAME}" >&2
        exit 3
    fi
fi

echo ${script}
