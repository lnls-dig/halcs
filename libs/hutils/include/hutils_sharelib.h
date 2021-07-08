/*
 * Copyright (C) 2021 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

/*
 * Based on EPICS Base project:
 * include/shareLib.h
 */

#ifndef _HUTILS_SHARELIB_H_
#define _HUTILS_SHARELIB_H_

#ifdef __cplusplus
extern "C" {
#endif

#undef HUTILS_SHARE_EXTERN
#undef HUTILS_SHARE_CLASS
#undef HUTILS_SHARE_FUNC

#if defined(_WIN32) || defined(__CYGWIN__)

#   if defined(HUTILS_EXPORTS)
#       define HUTILS_SHARE_EXTERN __declspec(dllexport) extern
#       define HUTILS_SHARE_CLASS  __declspec(dllexport)
#       define HUTILS_SHARE_FUNC   __declspec(dllexport)
#   else
#       define HUTILS_SHARE_EXTERN extern
#       define HUTILS_SHARE_CLASS
#       define HUTILS_SHARE_FUNC
#   endif

#elif __GNUC__ >= 4
/*
 * See http://gcc.gnu.org/wiki/Visibility
 * For these to work, gcc must be given the flag
 *     -fvisibility=hidden
 * and g++ the flags
 *     -fvisibility=hidden -fvisibility-inlines-hidden
 */

#   define HUTILS_SHARE_EXTERN __attribute__ ((visibility("default"))) extern
#   define HUTILS_SHARE_CLASS __attribute__ ((visibility("default")))
#   define HUTILS_SHARE_FUNC __attribute__ ((visibility("default")))

#else

/* else => no import/export specifiers */

#   define HUTILS_SHARE_EXTERN extern
#   define HUTILS_SHARE_CLASS
#   define HUTILS_SHARE_FUNC

#endif

#ifdef __cplusplus
}
#endif

#endif
