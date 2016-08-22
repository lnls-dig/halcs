#!/bin/bash

EXPECTED_ARGS=5
DATAPLOT_FILE=".feedgnuplot_data"
DFLT_SLEEP_INTERV=2                 # in seconds

if [ $# -lt $EXPECTED_ARGS ]
then
	echo "Usage: `basename $0` {broker endpoint} {number of samples} {channel number} {board number} {halcs number} [sleep interval]"
	exit 1;
fi

broker_endp=$1
nsamples=$2
ch_number=$3
board_number=$4
halcs_number=$5

if [ -z "$6" ]
then
    sleep_interval=$DFLT_SLEEP_INTERV
else
    sleep_interval=$6
fi

acquire_samples () {
    if [ $# -lt 5 ]
    then
        echo "acquire_samples usage: {broker endpoint} {number of samples} {channel number} {board number} {halcs number} [sleep_interval]"
        exit 1;
    fi

    while :
    do
        ./acq -b $1 -s $2 -ch $3 -board $4 -halcs $5 > ${DATAPLOT_FILE}.$4.$5
        sleep $6
    done
}

plot_data () {
    tail -n $1 -f ${DATAPLOT_FILE}.$2.$3 2> /dev/null |
    feedgnuplot --lines \
        --stream $4 \
        --domain \
        --title 'Data' \
        --ylabel 'Counts [arb. units]' \
        --xlabel 'Samples' \
        --legend 0 'A' \
        --legend 1 'B' \
        --legend 2 'C' \
        --legend 3 'D'
}


acquire_samples $broker_endp $nsamples $ch_number $board_number $halcs_number $sleep_interval &
sleep 2
plot_data $nsamples $board_number $halcs_number $sleep_interval

