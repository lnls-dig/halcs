#!/usr/bin/env bash

# help debug
set -x

if [ -z "$GRADLE" ]; then
    sonnar-scanner
else
    ./gradlew sonarqube
fi
