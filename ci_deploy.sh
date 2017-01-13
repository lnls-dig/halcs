#!/usr/bin/env bash

if [ -n "$GRADLE" ]; then
    md5sum build/release/*.rpm > build/release/MD5SUMS
    sha1sum build/release/*.rpm > build/release/SHA1SUMS
fi
