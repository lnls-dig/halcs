#!/usr/bin/env bash

# help debug
set -x

if [ -n "$CMAKE" ] && [ "$SONARQUBE" == "yes" ] ; then
    sonar-scanner
fi
