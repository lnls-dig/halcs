#!/usr/bin/env bash

if [ "$DEPLOY" = "yes" ]; then
    md5sum build/release/*.rpm > build/release/MD5SUMS
    sha1sum build/release/*.rpm > build/release/SHA1SUMS
fi
