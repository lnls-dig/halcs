/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include <czmq.h>
#include "debug_subsys.h"

char *dbg_lvl_str [DBG_LVL_NUM] = {
    [0] = DBG_LVL_TRACE_STR,
    [1] = DBG_LVL_INFO_STR,
    [2] = DBG_LVL_WARN_STR,
    [3] = DBG_LVL_ERR_STR,
    [4] = DBG_LVL_FATAL_STR
};

char *dbg_lvl_to_str (int dbg_lvl)
{
    /* Should be large enough for all possible debug levels */
    int size = 16;
    char *str = zmalloc (size);
    const char *dbg_str = dbg_lvl_str [DBG_LVL_DEGEN(dbg_lvl)-1];

    memcpy (str, dbg_str, strlen (dbg_str)+1);
    return str;
}
