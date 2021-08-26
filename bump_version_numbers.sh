#!/usr/bin/env bash

set -euo pipefail

usage () {
    echo "Usage:" >&2
    echo "  $1 -t ["major"|"minor"|"patch"]" >&2
    echo >&2
    echo "  Options:" >&2
    echo "    -t                  bump the specified version number" >&2
}

bump_version() {
    local version_type=$1
    local file=$2

    case ${version_type} in
        "major")
            sed -i -r 's/^#define (.*)_VERSION_MAJOR ([0-9]+)/echo "#define \1_VERSION_MAJOR $((\2+1))"/ge' ${file}
            sed -i -r 's/^#define (.*)_VERSION_MINOR ([0-9]+)/echo "#define \1_VERSION_MINOR 0"/ge' ${file}
            sed -i -r 's/^#define (.*)_VERSION_PATCH ([0-9]+)/echo "#define \1_VERSION_PATCH 0"/ge' ${file}
        ;;
        "minor")
            sed -i -r 's/^#define (.*)_VERSION_MINOR ([0-9]+)/echo "#define \1_VERSION_MINOR $((\2+1))"/ge' ${file}
            sed -i -r 's/^#define (.*)_VERSION_PATCH ([0-9]+)/echo "#define \1_VERSION_PATCH 0"/ge' ${file}
        ;;
        "patch")
            sed -i -r 's/^#define (.*)_VERSION_PATCH ([0-9]+)/echo "#define \1_VERSION_PATCH $((\2+1))"/ge' ${file}
        ;;
        \?)
          echo "bump_version(): error: invalid version_type: ${version_type}" >&2
          exit 1
          ;;
    esac
}

FILES="\
    include/revision.h \
    libs/acqclient/include/acq_client_classes.h \
    libs/bpmclient/include/bpm_client_classes.h \
    libs/convc/include/convc_classes.h \
    libs/disptable/include/disptable_classes.h \
    libs/errhand/include/errhand_classes.h \
    libs/hutils/include/hutils_classes.h \
    libs/llio/include/ll_io_classes.h \
    libs/sdbutils/include/sdbutils_classes.h \
    src/client/include/halcs_client_classes.h \
    examples/include/halcs_examples_classes.h \
"

version_type=
while getopts ":t:" opt; do
  case ${opt} in
    t) version_type="${OPTARG}" ;;
    \?)
      echo "Invalid option: -${OPTARG}" >&2
      usage $0
      exit 1
      ;;
    :)
      echo "Option -${OPTARG} requires an argument." >&2
      usage $0
      exit 1
      ;;
  esac
done

# if getopts did not process all input
if [ "${OPTIND}" -le "$#" ]; then
    echo "Invalid argument at index \"${OPTIND}\" does not have a corresponding option." >&2
    usage $0
    exit 1
fi

if [ "${version_type}" != "major" ] && [ "${version_type}" != "minor" ] && [ "${version_type}" != "patch" ]; then
    usage $0
    exit 1
fi

for file in ${FILES}; do
    bump_version ${version_type} ${file}
done
