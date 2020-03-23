#!/usr/bin/env bash

# help debug
set -x

COMPILE_MODES=()
COMPILE_MODES+=("")
COMPILE_MODES+=("Debug")
COMPILE_MODES+=("DebugDevelopment")
COMPILE_MODES+=("Development")

BOARDS=()
BOARDS+=("afcv3")
BOARDS+=("afcv3_1")
BOARDS+=("ml605")

TOPDIR=$(pwd)

if [ "$DEPLOY" = "yes" ]; then
    mkdir dist
    export HALCS_DEPLOYMENT=dist/*

    # Build put our deploy targets into build/releases.
    # Pack each target in a single tar.gz
    cd build/release
    for board in "${BOARDS[@]}"
    do
        for mode in "${COMPILE_MODES[@]}"
        do
            tar cvzf ${board}${mode}.tar.gz ${board}${mode}-* || exit 1
            mv ${board}${mode}.tar.gz ${TOPDIR}/dist
        done
    done
    cd -

    # Generate hash sums
    cd dist
    md5sum *.rpm > MD5SUMS
    sha1sum *.rpm > SHA1SUMS
    cd -
fi

set +x
