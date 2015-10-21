/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 * Parts taken from lwIP debug system
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _ERRHAND_OPTS_
#define _ERRHAND_OPTS_

#ifdef __cplusplus
extern "C" {
#endif

/* Defaults to sensible values if DBG_MIN_LEVEL and DBG_SUBSYS_ON
 * are not specified previously */

/*
 * Define the debug level here.
 * Available options are, in decreasing order of verbosity:
 *  DBG_LVL_TRACE = super verbose
 *  DBG_LVL_INFO
 *  DBG_LVL_WARN
 *  DBG_LVL_ERR
 *  DBG_LVL_FATAL = only the critical messages are shown
 */

#ifndef ERRHAND_MIN_LEVEL

/* For compatibility */
#ifndef DBG_MIN_LEVEL
#warning "liberrhand: undefined DBG_MIN_LEVEL. Using DBG_LVL_TRACE."
#define DBG_MIN_LEVEL DBG_LVL_TRACE
#endif /* DBG_MIN_LEVEL */

#define ERRHAND_MIN_LEVEL DBG_MIN_LEVEL

#endif /* ERRHAND_MIN_LEVEL */

/* Define which subsystem will be traced. The DBG_SUBSYS_ON
 * macro must be OR'ed.
 * Available options are:
 *  DBG_DEV_MNGR
 *  DBG_DEV_IO
 *  DBG_LL_IO
 *  DBG_SM_IO
 *  DBG_MSG
 *  DBG_LIB_CLIENT
 *  DBG_SM_PR
 *  DBG_SM_CH
 */

#ifndef ERRHAND_SUBSYS_ON

#ifndef DBG_SUBSYS_ON
#warning "liberrhand: undefined DBG_SUBSYS_ON. Using library defaults."
#define DBG_SUBSYS_ON           (\
                                 DBG_DEV_MNGR    | \
                                 DBG_DEV_IO      | \
                                 DBG_SM_IO       | \
                                 DBG_LIB_CLIENT  | \
                                 DBG_SM_PR       | \
                                 DBG_SM_CH       | \
                                 DBG_LL_IO       | \
                                 DBG_HAL_UTILS)
/*                                 DBG_MSG         | \ */
#endif /* DBG_SUBSYS_ON */

#define ERRHAND_SUBSYS_ON DBG_SUBSYS_ON

#endif /* ERRHAND_SUBSYS_ON */

#ifdef __cplusplus
}
#endif

#endif
