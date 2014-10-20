#!/bin/bash

EXPECTED_ARGS=4

if [ $# -ne $EXPECTED_ARGS ]
then
	echo "Usage: `basename $0` {broker endpoint} {number of samples} {channel number} {bpm number}"
	exit 1;
fi

broker_endp=$1
nsamples=$2
ch_number=$3
bpm_number=$4

DATAPLOT_FILE=".feedgnuplot_data"

acquire_samples () {
    if [ $# -ne 4 ]
    then
        echo "acquire_samples usage: {broker endpoint} {number of samples} {channel number} {bpm number}"
        exit 1;
    fi

    while :
    do
        ./acq -b $1 -s $2 -ch $3 -bpm $4 > ${DATAPLOT_FILE}.$4
        sleep 2
    done
}

plot_data () {
    tail -n $1 -f ${DATAPLOT_FILE}.$2 |
    feedgnuplot --lines \
        --stream \
        --domain \
        --title 'Data' \
        --ylabel 'Counts [arb. units]' \
        --xlabel 'Samples' \
        --legend 0 'A' \
        --legend 1 'B' \
        --legend 2 'C' \
        --legend 3 'D'
}

acquire_samples $broker_endp $nsamples $ch_number $bpm_number &
sleep 2
plot_data $nsamples $bpm_number

