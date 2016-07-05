#!/usr/bin/env bash

function usage() {
    echo "Usage: $0 [-s <service_name>]"
}

SERVICE=

# Get command line options
while getopts ":s:" opt; do
    case $opt in
        s)
            SERVICE=$OPTARG
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

if [ -z "$SERVICE" ]; then
    echo "\"SERVICE\" variable unset."
    usage
    exit 1
fi

INIT_SYSTEM=$(./get-init-system.sh)

# Enable service depending on which init system we are

case ${INIT_SYSTEM} in
    "systemd")
        echo "Enabling systemd service "${SERVICE}
        ;;
    "upstart")
        echo "Enabling upstart service "${SERVICE}
        ;;
    "sysv-init")
        echo "Enabling sysv-init service "${SERVICE}
        ;;
    "?")
        echo "Invalid init system detected." >&2
        exit 1
        ;;
esac
