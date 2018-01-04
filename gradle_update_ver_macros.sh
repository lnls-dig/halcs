#!/usr/bin/env bash

if [ "$1" = "major" ] || [ "$1" = "minor" ] || [ "$1" = "patch" ]; then
    scope="$1"
else
    echo "USAGE: ./gradle_update_ver_macros.sh [major|minor|patch]" >&2
fi

./gradlew updateVersionMacros -Prelease.scope="$scope" -Prelease.stage=ci_release
