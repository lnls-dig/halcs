/*
 * Copyright (C) 2017 LNLS (www.lnls.br)
 * Authors: Lucas Russo <lucas.russo@lnls.br>
 *          Janito Vaqueiro Ferreira Filho <janito.filho@gmail.com>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _REVISION_H_
#define _REVISION_H_

#include <stddef.h>

/* HALCS version macros for compile-time API detection */

#define HALCS_VERSION_MAJOR 2
#define HALCS_VERSION_MINOR 1
#define HALCS_VERSION_PATCH 0

#define HALCS_MAKE_VERSION(major, minor, patch) \
    ((major) * 10000 + (minor) * 100 + (patch))
#define HALCS_VERSION \
    HALCS_MAKE_VERSION(HALCS_VERSION_MAJOR, HALCS_VERSION_MINOR, \
            HALCS_VERSION_PATCH)

#ifdef __cplusplus
extern "C" {
#endif

/* Clone the build revision. Returns the cloned string is successfully or NULL
 * on error */
char *revision_clone_build_revision (void);
/* Copies the build revision into a pre-allocated buffer. Returns a negative number
 * on error and the number of copied bytes otherwise */
int revision_copy_build_revision (char *dest, size_t size);
/* Returns a const reference to build revision */
const char *revision_get_build_revision (void);

/* Clone the build date. Returns the cloned string is successfully or NULL
 * on error */
char *revision_clone_build_date (void);
/* Copies the build date into a pre-allocated buffer. Returns a negative number
 * on error and the number of copied bytes otherwise */
int revision_copy_build_date (char *dest, size_t size);
/* Returns a const reference to build version */
const char *revision_get_build_date (void);

/* Clone the build user name. Returns the cloned string is successfully or NULL
 * on error */
char *revision_clone_build_user_name (void);
/* Copies the build user name into a pre-allocated buffer. Returns a negative number
 * on error and the number of copied bytes otherwise */
int revision_copy_build_user_name (char *dest, size_t size);
/* Returns a const reference to build user name */
const char *revision_get_build_user_name (void);

/* Clone the build user email. Returns the cloned string is successfully or NULL
 * on error */
char *revision_clone_build_user_email (void);
/* Copies the build user email into a pre-allocated buffer. Returns a negative number
 * on error and the number of copied bytes otherwise */
int revision_copy_build_user_email (char *dest, size_t size);
/* Returns a const reference to build user email */
const char *revision_get_build_user_email (void);

/* Clone the build version. Returns the cloned string is successfully or NULL
 * on error */
char *revision_clone_build_version (void);
/* Copies the build version into a pre-allocated buffer. Returns a negative number
 * on error and the number of copied bytes otherwise */
int revision_copy_build_version (char *dest, size_t size);
/* Returns a const reference to build version */
const char *revision_get_build_version (void);

#ifdef __cplusplus
}
#endif

#endif
