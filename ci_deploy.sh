#!/usr/bin/env bash

# help debug
set -x

TOPDIR=$(pwd)

if [ "$DEPLOY" = "yes" ]; then
    mkdir dist
    export HALCS_DEPLOYMENT=dist/*

    # Build put our deploy targets into build/releases.
    # Pack each target in a single tar.gz
    cd build
    mv *.deb ${TOPDIR}/dist
    mv *.rpm ${TOPDIR}/dist
    cd -

    # Generate hash sums
    cd dist
    md5sum *.deb *.rpm > MD5SUMS
    sha1sum *.deb *.rpm > SHA1SUMS
    cd -
fi

set +x
