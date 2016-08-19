#!/usr/bin/env bash

set -u
set -e
set -a

TOP=$(pwd)

function usage() {
        echo "Usage: $0 [-o <Current project name> -n <New project name> -i <Ignore files/dirs>]"
}

function change_filenames () {
    C_NAME=$1
    set +u
    C_DIR=${TOP}/$3
    set -u
    IGNORE=$4

    cd ${C_DIR}
    N_NAME=$2
    FILES=$(find . -type f \
        -name "*${C_NAME}*" -not -path "*.git/*" -not -path "*foreign/*" -not -path "${IGNORE}")
    for file in ${FILES};
    do
        new_file=$(echo ${file} | sed -e "s/${C_NAME}/${N_NAME}/g")
        new_dir=${new_file%/*}
        mkdir -p ${new_dir}
        mv ${file} ${new_file}
    done

    DIRS=$(find . -type d \
        -name "*${C_NAME}*" -not -path "*.git/*" -not -path "*foreign/*" -not -path "${IGNORE}")
    for dir in ${DIRS};
    do
        new_dir=$(echo ${dir} | sed -e "s/${C_NAME}/${N_NAME}/g")
        mkdir -p ${new_dir}
        mv ${dir}/* ${new_dir} 2>/dev/null || true
    done
    cd ${TOP}
}

function change_file_insides () {
    C_NAME=$1
    N_NAME=$2
    C_DIR=${TOP}/$3
    IGNORE=$4

    cd ${C_DIR}
    grep -lR "${C_NAME}" \
        --exclude-dir=".git" \
        --exclude-dir="foreign" \
        --exclude="*.orig" \
        --exclude="*.o" \
        --exclude="*.cmd" \
        --exclude="*.swp" \
        --exclude="${IGNORE}" | \
        xargs --no-run-if-empty sed -i -e "s/${C_NAME}/${N_NAME}/g"
    cd ${TOP}
}

CURRENT_NAME=
NEW_NAME=
IGNORE_REGEX=

# Get command line options
while getopts ":o:n:i:" opt; do
    case $opt in
        o)
            CURRENT_NAME=$OPTARG
            ;;
        n)
            NEW_NAME=$OPTARG
            ;;
        i)
            IGNORE_REGEX=$OPTARG
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

# Rename project filenames
change_filenames "${CURRENT_NAME}" "${NEW_NAME}" "" "${IGNORE_REGEX}"

## Change project file insides
change_file_insides "${CURRENT_NAME}" "${NEW_NAME}" "" "${IGNORE_REGEX}"
