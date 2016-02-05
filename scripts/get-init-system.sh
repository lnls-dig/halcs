#!/usr/bin/env bash

# Based from the stackexchange answer
# http://unix.stackexchange.com/questions/18209/detect-init-system-using-the-shell

if [[ `/sbin/init --version` =~ upstart ]]; then
    echo "upstart";
elif [[ `systemctl` =~ -\.mount ]]; then
    echo "systemd";
elif [[ -f /etc/init.d/cron && ! -h /etc/init.d/cron ]]; then
    echo "sysv-init";
else
    echo "?";
fi
