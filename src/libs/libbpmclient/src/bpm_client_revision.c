/*
 * Copyright (C) 2015 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include "bpm_client.h"
/* Private headers */
#include "errhand.h"

#define BPM_CLIENT_VERSION_MAJOR_STR STRINGIFY(BPM_CLIENT_VERSION_MAJOR)
#define BPM_CLIENT_VERSION_MINOR_STR STRINGIFY(BPM_CLIENT_VERSION_MINOR)
#define BPM_CLIENT_VERSION_PATCH_STR STRINGIFY(BPM_CLIENT_VERSION_PATCH)

const char *build_revision = GIT_REVISION;
const char *build_date = __DATE__ " " __TIME__;
