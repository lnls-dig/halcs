/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include <czmq.h>
#include "acq_chan_gen_defs.h"
#include "board.h"
#include "ddr3_map.h"
#include "ddr3_map_structs.h"

#if defined __BOARD_AFCV3__ && !defined __BOARD_AFCV3_1__

const acq_buf_t __acq_buf[NUM_ACQ_CORE_SMIOS][END_CHAN_ID] = {
    /*** Acquisition Core 0 Channel Parameters ***/
    {
        {
            .id = ADC_CHAN_ID,
            .start_addr = DDR3_ADC0_START_ADDR,
            .end_addr = DDR3_ADC0_END_ADDR
        },
        {
            .id = ADCSWAP_CHAN_ID,
            .start_addr = DDR3_ADCSWAP0_START_ADDR,
            .end_addr = DDR3_ADCSWAP0_END_ADDR
        },
        {
            .id = MIXIQ_CHAN_ID,
            .start_addr = DDR3_MIXIQ0_START_ADDR,
            .end_addr = DDR3_MIXIQ0_END_ADDR
        },
        {
            .id = DUMMY0_CHAN_ID,
            .start_addr = DDR3_DUMMY00_START_ADDR,
            .end_addr = DDR3_DUMMY00_END_ADDR
        },
        {
            .id = TBTDECIMIQ_CHAN_ID,
            .start_addr = DDR3_TBTDECIMIQ0_START_ADDR,
            .end_addr = DDR3_TBTDECIMIQ0_END_ADDR
        },
        {
            .id = DUMMY1_CHAN_ID,
            .start_addr = DDR3_DUMMY10_START_ADDR,
            .end_addr = DDR3_DUMMY10_END_ADDR
        },
        {
            .id = TBTAMP_CHAN_ID,
            .start_addr = DDR3_TBTAMP0_START_ADDR,
            .end_addr = DDR3_TBTAMP0_END_ADDR
        },
        {
            .id = TBTPHA_CHAN_ID,
            .start_addr = DDR3_TBTPHA0_START_ADDR,
            .end_addr = DDR3_TBTPHA0_END_ADDR
        },
        {
            .id = TBTPOS_CHAN_ID,
            .start_addr = DDR3_TBTPOS0_START_ADDR,
            .end_addr = DDR3_TBTPOS0_END_ADDR
        },
        {
            .id = FOFBDECIMIQ_CHAN_ID,
            .start_addr = DDR3_FOFBDECIMIQ0_START_ADDR,
            .end_addr = DDR3_FOFBDECIMIQ0_END_ADDR
        },
        {
            .id = DUMMY2_CHAN_ID,
            .start_addr = DDR3_DUMMY20_START_ADDR,
            .end_addr = DDR3_DUMMY20_END_ADDR
        },
        {
            .id = FOFBAMP_CHAN_ID,
            .start_addr = DDR3_FOFBAMP0_START_ADDR,
            .end_addr = DDR3_FOFBAMP0_END_ADDR
        },
        {
            .id = FOFBPHA_CHAN_ID,
            .start_addr = DDR3_FOFBPHA0_START_ADDR,
            .end_addr = DDR3_FOFBPHA0_END_ADDR
        },
        {
            .id = FOFBPOS_CHAN_ID,
            .start_addr = DDR3_FOFBPOS0_START_ADDR,
            .end_addr = DDR3_FOFBPOS0_END_ADDR
        },
        {
            .id = MONITAMP_CHAN_ID,
            .start_addr = DDR3_MONITAMP0_START_ADDR,
            .end_addr = DDR3_MONITAMP0_END_ADDR
        },
        {
            .id = MONITPOS_CHAN_ID,
            .start_addr = DDR3_MONITPOS0_START_ADDR,
            .end_addr = DDR3_MONITPOS0_END_ADDR
        },
        {
            .id = MONIT1POS_CHAN_ID,
            .start_addr = DDR3_MONIT1POS0_START_ADDR,
            .end_addr = DDR3_MONIT1POS0_END_ADDR
        }
    },
    /*** Acquisition Core 1 Channel Parameters ***/
    {
        {
            .id = ADC_CHAN_ID,
            .start_addr = DDR3_ADC1_START_ADDR,
            .end_addr = DDR3_ADC1_END_ADDR
        },
        {
            .id = ADCSWAP_CHAN_ID,
            .start_addr = DDR3_ADCSWAP1_START_ADDR,
            .end_addr = DDR3_ADCSWAP1_END_ADDR
        },
        {
            .id = MIXIQ_CHAN_ID,
            .start_addr = DDR3_MIXIQ1_START_ADDR,
            .end_addr = DDR3_MIXIQ1_END_ADDR
        },
        {
            .id = DUMMY0_CHAN_ID,
            .start_addr = DDR3_DUMMY01_START_ADDR,
            .end_addr = DDR3_DUMMY01_END_ADDR
        },
        {
            .id = TBTDECIMIQ_CHAN_ID,
            .start_addr = DDR3_TBTDECIMIQ1_START_ADDR,
            .end_addr = DDR3_TBTDECIMIQ1_END_ADDR
        },
        {
            .id = DUMMY1_CHAN_ID,
            .start_addr = DDR3_DUMMY11_START_ADDR,
            .end_addr = DDR3_DUMMY11_END_ADDR
        },
        {
            .id = TBTAMP_CHAN_ID,
            .start_addr = DDR3_TBTAMP1_START_ADDR,
            .end_addr = DDR3_TBTAMP1_END_ADDR
        },
        {
            .id = TBTPHA_CHAN_ID,
            .start_addr = DDR3_TBTPHA1_START_ADDR,
            .end_addr = DDR3_TBTPHA1_END_ADDR
        },
        {
            .id = TBTPOS_CHAN_ID,
            .start_addr = DDR3_TBTPOS1_START_ADDR,
            .end_addr = DDR3_TBTPOS1_END_ADDR
        },
        {
            .id = FOFBDECIMIQ_CHAN_ID,
            .start_addr = DDR3_FOFBDECIMIQ1_START_ADDR,
            .end_addr = DDR3_FOFBDECIMIQ1_END_ADDR
        },
        {
            .id = DUMMY2_CHAN_ID,
            .start_addr = DDR3_DUMMY21_START_ADDR,
            .end_addr = DDR3_DUMMY21_END_ADDR
        },
        {
            .id = FOFBAMP_CHAN_ID,
            .start_addr = DDR3_FOFBAMP1_START_ADDR,
            .end_addr = DDR3_FOFBAMP1_END_ADDR
        },
        {
            .id = FOFBPHA_CHAN_ID,
            .start_addr = DDR3_FOFBPHA1_START_ADDR,
            .end_addr = DDR3_FOFBPHA1_END_ADDR
        },
        {
            .id = FOFBPOS_CHAN_ID,
            .start_addr = DDR3_FOFBPOS1_START_ADDR,
            .end_addr = DDR3_FOFBPOS1_END_ADDR
        },
        {
            .id = MONITAMP_CHAN_ID,
            .start_addr = DDR3_MONITAMP1_START_ADDR,
            .end_addr = DDR3_MONITAMP1_END_ADDR
        },
        {
            .id = MONITPOS_CHAN_ID,
            .start_addr = DDR3_MONITPOS1_START_ADDR,
            .end_addr = DDR3_MONITPOS1_END_ADDR
        },
        {
            .id = MONIT1POS_CHAN_ID,
            .start_addr = DDR3_MONIT1POS1_START_ADDR,
            .end_addr = DDR3_MONIT1POS1_END_ADDR
        },
    },
    /*** Acquisition Core 2 Channel Parameters ***/
    {
        {
            .id = ADC_CHAN_ID,
            .start_addr = DDR3_ADC2_START_ADDR,
            .end_addr = DDR3_ADC2_END_ADDR
        },
        {
            .id = ADCSWAP_CHAN_ID,
            .start_addr = DDR3_ADCSWAP2_START_ADDR,
            .end_addr = DDR3_ADCSWAP2_END_ADDR
        },
        {
            .id = MIXIQ_CHAN_ID,
            .start_addr = DDR3_MIXIQ2_START_ADDR,
            .end_addr = DDR3_MIXIQ2_END_ADDR
        },
        {
            .id = DUMMY0_CHAN_ID,
            .start_addr = DDR3_DUMMY02_START_ADDR,
            .end_addr = DDR3_DUMMY02_END_ADDR
        },
        {
            .id = TBTDECIMIQ_CHAN_ID,
            .start_addr = DDR3_TBTDECIMIQ2_START_ADDR,
            .end_addr = DDR3_TBTDECIMIQ2_END_ADDR
        },
        {
            .id = DUMMY1_CHAN_ID,
            .start_addr = DDR3_DUMMY12_START_ADDR,
            .end_addr = DDR3_DUMMY12_END_ADDR
        },
        {
            .id = TBTAMP_CHAN_ID,
            .start_addr = DDR3_TBTAMP2_START_ADDR,
            .end_addr = DDR3_TBTAMP2_END_ADDR
        },
        {
            .id = TBTPHA_CHAN_ID,
            .start_addr = DDR3_TBTPHA2_START_ADDR,
            .end_addr = DDR3_TBTPHA2_END_ADDR
        },
        {
            .id = TBTPOS_CHAN_ID,
            .start_addr = DDR3_TBTPOS2_START_ADDR,
            .end_addr = DDR3_TBTPOS2_END_ADDR
        },
        {
            .id = FOFBDECIMIQ_CHAN_ID,
            .start_addr = DDR3_FOFBDECIMIQ2_START_ADDR,
            .end_addr = DDR3_FOFBDECIMIQ2_END_ADDR
        },
        {
            .id = DUMMY2_CHAN_ID,
            .start_addr = DDR3_DUMMY22_START_ADDR,
            .end_addr = DDR3_DUMMY22_END_ADDR
        },
        {
            .id = FOFBAMP_CHAN_ID,
            .start_addr = DDR3_FOFBAMP2_START_ADDR,
            .end_addr = DDR3_FOFBAMP2_END_ADDR
        },
        {
            .id = FOFBPHA_CHAN_ID,
            .start_addr = DDR3_FOFBPHA2_START_ADDR,
            .end_addr = DDR3_FOFBPHA2_END_ADDR
        },
        {
            .id = FOFBPOS_CHAN_ID,
            .start_addr = DDR3_FOFBPOS2_START_ADDR,
            .end_addr = DDR3_FOFBPOS2_END_ADDR
        },
        {
            .id = MONITAMP_CHAN_ID,
            .start_addr = DDR3_MONITAMP2_START_ADDR,
            .end_addr = DDR3_MONITAMP2_END_ADDR
        },
        {
            .id = MONITPOS_CHAN_ID,
            .start_addr = DDR3_MONITPOS2_START_ADDR,
            .end_addr = DDR3_MONITPOS2_END_ADDR
        },
        {
            .id = MONIT1POS_CHAN_ID,
            .start_addr = DDR3_MONIT1POS2_START_ADDR,
            .end_addr = DDR3_MONIT1POS2_END_ADDR
        },
    },
    /*** Acquisition Core 3 Channel Parameters ***/
    {
        {
            .id = ADC_CHAN_ID,
            .start_addr = DDR3_ADC3_START_ADDR,
            .end_addr = DDR3_ADC3_END_ADDR
        },
        {
            .id = ADCSWAP_CHAN_ID,
            .start_addr = DDR3_ADCSWAP3_START_ADDR,
            .end_addr = DDR3_ADCSWAP3_END_ADDR
        },
        {
            .id = MIXIQ_CHAN_ID,
            .start_addr = DDR3_MIXIQ3_START_ADDR,
            .end_addr = DDR3_MIXIQ3_END_ADDR
        },
        {
            .id = DUMMY0_CHAN_ID,
            .start_addr = DDR3_DUMMY03_START_ADDR,
            .end_addr = DDR3_DUMMY03_END_ADDR
        },
        {
            .id = TBTDECIMIQ_CHAN_ID,
            .start_addr = DDR3_TBTDECIMIQ3_START_ADDR,
            .end_addr = DDR3_TBTDECIMIQ3_END_ADDR
        },
        {
            .id = DUMMY1_CHAN_ID,
            .start_addr = DDR3_DUMMY13_START_ADDR,
            .end_addr = DDR3_DUMMY13_END_ADDR
        },
        {
            .id = TBTAMP_CHAN_ID,
            .start_addr = DDR3_TBTAMP3_START_ADDR,
            .end_addr = DDR3_TBTAMP3_END_ADDR
        },
        {
            .id = TBTPHA_CHAN_ID,
            .start_addr = DDR3_TBTPHA3_START_ADDR,
            .end_addr = DDR3_TBTPHA3_END_ADDR
        },
        {
            .id = TBTPOS_CHAN_ID,
            .start_addr = DDR3_TBTPOS3_START_ADDR,
            .end_addr = DDR3_TBTPOS3_END_ADDR
        },
        {
            .id = FOFBDECIMIQ_CHAN_ID,
            .start_addr = DDR3_FOFBDECIMIQ3_START_ADDR,
            .end_addr = DDR3_FOFBDECIMIQ3_END_ADDR
        },
        {
            .id = DUMMY2_CHAN_ID,
            .start_addr = DDR3_DUMMY23_START_ADDR,
            .end_addr = DDR3_DUMMY23_END_ADDR
        },
        {
            .id = FOFBAMP_CHAN_ID,
            .start_addr = DDR3_FOFBAMP3_START_ADDR,
            .end_addr = DDR3_FOFBAMP3_END_ADDR
        },
        {
            .id = FOFBPHA_CHAN_ID,
            .start_addr = DDR3_FOFBPHA3_START_ADDR,
            .end_addr = DDR3_FOFBPHA3_END_ADDR
        },
        {
            .id = FOFBPOS_CHAN_ID,
            .start_addr = DDR3_FOFBPOS3_START_ADDR,
            .end_addr = DDR3_FOFBPOS3_END_ADDR
        },
        {
            .id = MONITAMP_CHAN_ID,
            .start_addr = DDR3_MONITAMP3_START_ADDR,
            .end_addr = DDR3_MONITAMP3_END_ADDR
        },
        {
            .id = MONITPOS_CHAN_ID,
            .start_addr = DDR3_MONITPOS3_START_ADDR,
            .end_addr = DDR3_MONITPOS3_END_ADDR
        },
        {
            .id = MONIT1POS_CHAN_ID,
            .start_addr = DDR3_MONIT1POS3_START_ADDR,
            .end_addr = DDR3_MONIT1POS3_END_ADDR
        },
    },
};

#endif
