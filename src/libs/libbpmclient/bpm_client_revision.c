#include "bpm_client_revision.h"

const char *const build_revision = GIT_REVISION;
const char *const build_date = __DATE__ " " __TIME__;
const char *const build_user_name = GIT_USER_NAME;
const char *const build_user_email = GIT_USER_EMAIL;
