#!/usr/bin/env bash

VALID_BOARDS_STR="Valid values are: \"ml605\", \"afcv3\" or \"afcv3_1\"."
VALID_APPS_STR="Valid values are: \"halcsd\"."
VALID_WITH_EXAMPLES_STR="Valid values are: \"yes\" or \"no\"."
VALID_WITH_SYSTEM_INTEGRATION_STR="Valid values are: \"yes\" or \"no\"."
VALID_WITH_DRIVER_STR="Valid values are: \"yes\" or \"no\"."
VALID_WITH_HALCS_STR="Valid values are: \"yes\" or \"no\"."
VALID_COMPACT_INSTALL_STR="Valid values are: \"yes\" or \"no\"."

function usage() {
    echo "Usage: $0 [-b <board>] [-a <applications>] [-e <with examples = yes/no>]"
    echo "    [-l <with library linking = yes/no>] [-d <with driver = yes/no>]"
    echo "    [-f <with HALCS = yes/no>] [-c <compact install = yes/no>] [-x <extra flags>]"
}

#######################################
# All of our Makefile options
#######################################

BOARD=
APPS=
WITH_EXAMPLES="no"
WITH_SYSTEM_INTEGRATION="yes"
WITH_DRIVER="no"
WITH_HALCS="yes"
COMPACT_INSTALL="no"
EXTRA_FLAGS=

# Get command line options
while getopts ":b:a:e:l:x:d:f:c:" opt; do
    case $opt in
        b)
            BOARD=$OPTARG
            ;;
        a)
            APPS=$OPTARG
            ;;
        e)
            WITH_EXAMPLES=$OPTARG
            ;;
        l)
            WITH_SYSTEM_INTEGRATION=$OPTARG
            ;;
        x)
            EXTRA_FLAGS=$OPTARG
            ;;
        d)
            WITH_DRIVER=$OPTARG
            ;;
        f)
            WITH_HALCS=$OPTARG
            ;;
        c)
            COMPACT_INSTALL=$OPTARG
            ;;
        \?)
            echo "Invalid option: -$OPTARG" >&2
            usage
            exit 1
            ;;
        :)
            echo "Option -$OPTARG requires an argument." >&2
            usage
            exit 1
            ;;
    esac
done

if [ -z "$BOARD"  ]; then
    echo "\"board\" variable unset."
    usage
    exit 1
fi

if [ "$BOARD" != "afcv3" ] && [ "$BOARD" != "afcv3_1" ] && [ "$BOARD" != "ml605" ]; then
    echo "Unsupported board. "$VALID_BOARDS_STR
    exit 1
fi

if [ -z "$APPS"  ]; then
    echo "\"applications\" variable unset."
    usage
    exit 1
fi

if [ -z "$WITH_EXAMPLES"  ]; then
    echo "\"examples\" variable unset."
    usage
    exit 1
fi

if [ "$WITH_EXAMPLES" != "yes" ] && [ "$WITH_EXAMPLES" != "no" ]; then
    echo "Unsupported examples. "$VALID_WITH_EXAMPLES_STR
    exit 1
fi

if [ "$WITH_DRIVER" != "yes" ] && [ "$WITH_DRIVER" != "no" ]; then
    echo "Unsupported driver option. "$VALID_WITH_DRIVER_STR
    exit 1
fi

if [ "$WITH_HALCS" != "yes" ] && [ "$WITH_HALCS" != "no" ]; then
    echo "Unsupported driver option. "$VALID_WITH_HALCS_STR
    exit 1
fi

if [ "$COMPACT_INSTALL" != "yes" ] && [ "$COMPACT_INSTALL" != "no" ]; then
    echo "Unsupported compact install option. "$VALID_COMPACT_INSTALL_STR
    exit 1
fi

if [ "$COMPACT_INSTALL" = "yes" ]; then
    TASKS=":apps:${APPS}:${BOARD}InstallRpm"
else
    TASKS=":apps:${APPS}:${BOARD}DevelopmentInstallRpm"
fi

if [ "$WITH_EXAMPLES" = "yes" ]; then
    TASKS="$TASKS :examples:assemble"
fi

if [ "$WITH_SYSTEM_INTEGRATION" = "no" ]; then
    echo "Currently unable to install without system integration."
    exit 1
fi

if [ "$WITH_DRIVER" = "yes" ]; then
    make ${EXTRA_FLAGS} pcie_driver pcie_driver_install || exit 1
fi

# Install HALCS only if requested
if [ "$WITH_HALCS" = "yes" ]; then
    for lib in $(find libs -maxdepth 1 -mindepth 1 -type d); do
        LIB="$(echo $lib | sed -e 's|libs/||')"

        if [ "$COMPACT_INSTALL" = "yes" ]; then
            TASKS="$TASKS :libs:${LIB}:${BOARD}InstallRpm"
        else
            TASKS="$TASKS :libs:${LIB}:${BOARD}DevelopmentInstallRpm"
        fi
    done

    ./gradlew $TASKS
fi

if [ "$WITH_SYSTEM_INTEGRATION" = "yes" ]; then
    ldconfig
fi
