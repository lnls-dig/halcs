#!/usr/bin/env bash

# help debug
set -x

if [ -n "$CMAKE" ]; then
    sonar-scanner
fi
