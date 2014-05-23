/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 * Parts taken from lwIP debug system
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _HAL_OPTS_
#define _HAL_OPTS_

/*
 * Define the debug level here.
 * Available options are, in decreasing order of verbosity:
 *  DBG_LVL_TRACE = super verbose
 *  DBG_LVL_INFO
 *  DBG_LVL_WARN
 *  DBG_LVL_ERR
 *  DBG_LVL_FATAL = only the critical messages are shown
 */

#define DBG_MIN_LEVEL DBG_LVL_TRACE

/* Define which subsystem will be traced. The DBG_SUBSYS_ON
 * macro must be OR'ed.
 * Available options are:
 *  DBG_DEV_MNGR
 *  DBG_DEV_IO
 *  DBG_LL_IO
 *  DBG_SM_IO
 *  DBG_MSG
 *  DBG_HAL_UTILS
 */

#define DBG_SUBSYS_ON           (\
                                 DBG_DEV_MNGR    | \
                                 DBG_DEV_IO      | \
                                 DBG_SM_IO       | \
                                 DBG_HAL_UTILS)
/*                                 DBG_LL_IO       | \ */
/*                                 DBG_MSG         | \ */

#endif
