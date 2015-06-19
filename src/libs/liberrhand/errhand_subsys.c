/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include <czmq.h>
#include "errhand_subsys.h"

const char *errhand_lvl_str [ERRHAND_LVL_NUM] = {
    [0] = ERRHAND_LVL_TRACE_STR,
    [1] = ERRHAND_LVL_INFO_STR,
    [2] = ERRHAND_LVL_WARN_STR,
    [3] = ERRHAND_LVL_ERR_STR,
    [4] = ERRHAND_LVL_FATAL_STR
};

char *errhand_lvl_to_str (int errhand_lvl)
{
    /* Should be large enough for all possible debug levels */
    int size = 16;
    char *str = zmalloc (size);
    const char *errhand_str = errhand_lvl_str [ERRHAND_LVL_DEGEN(errhand_lvl)-1];

    memcpy (str, errhand_str, strlen (errhand_str)+1);
    return str;
}

