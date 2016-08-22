/*
 * Copyright (C) 2015 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _EPICS_MAPPING_H_
#define _EPICS_MAPPING_H_

#ifdef __cplusplus
extern "C" {
#endif

/* HALCS reverse mappping structure */
typedef struct {
    int dev_id;
    int smio_id;
} board_epics_map_t;

/* HALCS reverse mappping structure */
typedef struct {
    int halcs_id;
} board_epics_rev_map_t;

typedef struct {
    int telnet_port;
    int telnet_afe_port;
} board_epics_opts_t;


/* EPICS mapping definitions */
extern const board_epics_map_t board_epics_map[NUM_MAX_HALCSS+1];
extern const board_epics_rev_map_t board_epics_rev_map [NUM_MAX_SLOTS+1][NUM_MAX_HALCS_PER_SLOT];
extern const board_epics_opts_t board_epics_opts [NUM_MAX_SLOTS+1][NUM_MAX_HALCS_PER_SLOT];

#ifdef __cplusplus
}
#endif

#endif
