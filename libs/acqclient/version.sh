#!/usr/bin/env sh
#
# This script extracts the version from the project header file
#

# This script is based on CZMQ version.sh script located at:
# https://github.com/zeromq/czmq/blob/master/version.sh

project=$1
appendix="_classes"

if [ ! -f include/$project$appendix.h ]; then
    echo "version.sh: error: could not open file include/$project$appendix.h" 1>&2
    exit 1
fi

MAJOR=`egrep '^#define .*_VERSION_MAJOR +[0-9]+$' include/$project$appendix.h`
MINOR=`egrep '^#define .*_VERSION_MINOR +[0-9]+$' include/$project$appendix.h`
PATCH=`egrep '^#define .*_VERSION_PATCH +[0-9]+$' include/$project$appendix.h`

if [ -z "$MAJOR" -o -z "$MINOR" -o -z "$PATCH" ]; then
    echo "version.sh: error: could not extract version from include/$project$appendix.h" 1>&2
    exit 1
fi

MAJOR=`echo $MAJOR | awk '{ print $3 }'`
MINOR=`echo $MINOR | awk '{ print $3 }'`
PATCH=`echo $PATCH | awk '{ print $3 }'`

echo $MAJOR.$MINOR.$PATCH | tr -d '\n'
