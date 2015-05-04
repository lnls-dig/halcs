/*
 * Copyright (C) 2015 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _REVISION_
#define _REVISION_

extern const char *const build_revision;
extern const char *const build_date;
extern const char *const build_user_name;
extern const char *const build_user_email;

/* Clone the build revision. Returns the cloned string is successfully or NULL
 * on error */
char *revision_clone_build_revision (void);
/* Copies the build revision into a pre-allocated buffer. Returns a negative number
 * on error and the number of copied bytes otherwise */
int revision_copy_build_revision (char *dest, size_t size);

/* Clone the build date. Returns the cloned string is successfully or NULL
 * on error */
char *revision_clone_build_date (void);
/* Copies the build date into a pre-allocated buffer. Returns a negative number
 * on error and the number of copied bytes otherwise */
int revision_copy_build_date (char *dest, size_t size);

/* Clone the build user name. Returns the cloned string is successfully or NULL
 * on error */
char *revision_clone_build_user_name (void);
/* Copies the build user name into a pre-allocated buffer. Returns a negative number
 * on error and the number of copied bytes otherwise */
int revision_copy_build_user_name (char *dest, size_t size);

/* Clone the build user email. Returns the cloned string is successfully or NULL
 * on error */
char *revision_clone_build_user_email (void);
/* Copies the build user email into a pre-allocated buffer. Returns a negative number
 * on error and the number of copied bytes otherwise */
int revision_copy_build_user_email (char *dest, size_t size);

#endif
