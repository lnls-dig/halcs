#!/usr/bin/env bash

function usage() {
    echo "Usage: $0 [-o <Current project name> -n <New project name]"
}

CURRENT_NAME=
NEW_NAME=

# Get command line options
while getopts ":o:n:" opt; do
    case $opt in
        o)
            CURRENT_NAME=$OPTARG
            ;;
        n)
            NEW_NAME=$OPTARG
            ;;
        \?)
            echo "Invalid option: -$OPTARG" >&2
            usage
            exit 1
            ;;
        :)
            echo "Option -$OPTARG requires an argument." >&2
            usage
            exit 1
            ;;
    esac
done

if [ -z "$CURRENT_NAME" ]; then
    echo "\"CURRENT_NAME\" variable unset."
    usage
    exit 1
fi

if [ -z "$NEW_NAME" ]; then
    echo "\"NEW_NAME\" variable unset."
    usage
    exit 1
fi

# Rename filenames
find . -type f \
    -name "*${CURRENT_NAME}*" -not -path "*.git/*" -not -path "*foreign/*" -print0 | \
    xargs -0 rename "s/${CURRENT_NAME}/${NEW_NAME}/g"

# Change file insides
grep -lR "${CURRENT_NAME}" \
    --exclude-dir=".git" \
    --exclude-dir="foreign" \
    --exclude="*.orig" \
    --exclude="*.o" \
    --exclude="*.cmd" \
    --exclude="*.swp" | \
    xargs sed -i -e "s/${CURRENT_NAME}/${NEW_NAME}/g"
