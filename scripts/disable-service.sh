#!/usr/bin/env bash

function usage() {
    echo "Usage: $0 [-s <service_name>]"
}

SERVICE=

# Get command line options
while getopts ":s:" opt; do
    case $opt in
        d)
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

INIT_SYSTEM=$(./get-init-system.sh)

# Enable service depending on which init system we are

case ${INIT_SYSTEM} in
    "systemd")
        echo "Disabling systemd service "${SERVICE}
        systemctl disable ${SERVICE}
        ;;
    "upstart")
        echo "Disabling upstart service "${SERVICE}
        ;;
    "sysv-init")
        echo "Disabling sysv-init service "${SERVICE}
        ;;
    "?")
        echo "Invalid init system detected." >&2
        exit 1
        ;;
esac
