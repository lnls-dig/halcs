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

/* EPICS mapping definitions */
extern const board_epics_map_t board_epics_map[NUM_MAX_HALCSS+1];

#ifdef __cplusplus
}
#endif

#endif
