/*
 * Copyright (C) 2015 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "revision.h"
#include "hutils.h"

#define HALCS_VERSION_MAJOR_STR STRINGIFY(HALCS_VERSION_MAJOR)
#define HALCS_VERSION_MINOR_STR STRINGIFY(HALCS_VERSION_MINOR)
#define HALCS_VERSION_PATCH_STR STRINGIFY(HALCS_VERSION_PATCH)

static const char *build_revision = GIT_REVISION;
static const char *build_date = __DATE__ " " __TIME__;
static const char *build_user_name = GIT_USER_NAME;
static const char *build_user_email = GIT_USER_EMAIL;
static const char *build_version = HALCS_VERSION_MAJOR_STR"."HALCS_VERSION_MINOR_STR"."HALCS_VERSION_PATCH_STR;

/************************* Clone functions ***********************************/

char *revision_clone_build_revision (void)
{
    return hutils_clone_str (build_revision);
}

char *revision_clone_build_date (void)
{
    return hutils_clone_str (build_date);
}

char *revision_clone_build_user_name (void)
{
    return hutils_clone_str (build_user_name);
}

char *revision_clone_build_user_email (void)
{
    return hutils_clone_str (build_user_email);
}

char *revision_clone_build_version (void)
{
    return hutils_clone_str (build_version);
}

/************************* Get Const functions ********************************/

const char *revision_get_build_revision (void)
{
    return build_revision;
}

const char *revision_get_build_date (void)
{
    return build_date;
}

const char *revision_get_build_user_name (void)
{
    return build_user_name;
}

const char *revision_get_build_user_email (void)
{
    return build_user_email;
}

const char *revision_get_build_version (void)
{
    return build_version;
}

/*************************** Copy functions **********************************/

int revision_copy_build_revision (char *dest, size_t size)
{
    return hutils_copy_str (dest, build_revision, size);
}

int revision_copy_build_date (char *dest, size_t size)
{
    return hutils_copy_str (dest, build_date, size);
}

int revision_copy_build_user_name (char *dest, size_t size)
{
    return hutils_copy_str (dest, build_user_name, size);
}

int revision_copy_build_user_email (char *dest, size_t size)
{
    return hutils_copy_str (dest, build_user_email, size);
}

int revision_copy_build_version (char *dest, size_t size)
{
    return hutils_copy_str (dest, build_version, size);
}
