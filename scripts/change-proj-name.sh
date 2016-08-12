#!/usr/bin/env bash

function usage() {
    echo "Usage: $0 [-o <Current project name> -n <New project name> -i <Current main app> -t <New main app>]"
}

CURRENT_MAIN_APP=
NEW_MAIN_APP=
CURRENT_NAME=
NEW_NAME=

# Get command line options
while getopts ":o:n:i:t:" opt; do
    case $opt in
        o)
            CURRENT_NAME=$OPTARG
            ;;
        n)
            NEW_NAME=$OPTARG
            ;;
        i)
            CURRENT_MAIN_APP=$OPTARG
            ;;
        t)
            NEW_MAIN_APP=$OPTARG
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


if [ -z "$CURRENT_MAIN_APP" ]; then
    echo "\"CURRENT_MAIN_APP\" variable unset."
    usage
    exit 1
fi

if [ -z "$NEW_MAIN_APP" ]; then
    echo "\"NEW_MAIN_APP\" variable unset."
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

# Change main EBPM application
find . -type f \
    -name "*${MAIN_APP}*" -not -path "*.git/*" -not -path "*foreign/*" -print0 | \
    xargs -0 rename "s/${MAIN_APP}/${NEW_MAIN_APP}/g"

# Change file insides
grep -lR "${CURRENT_MAIN_APP}" \
    --exclude-dir=".git" \
    --exclude-dir="foreign" \
    --exclude="*.orig" \
    --exclude="*.o" \
    --exclude="*.cmd" \
    --exclude="*.swp" | \
    xargs sed -i -e "s/${CURRENT_MAIN_APP}/${NEW_MAIN_APP}/g"

