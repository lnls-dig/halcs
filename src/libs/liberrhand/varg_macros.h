/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _VARG_MACROS_H_
#define _VARG_MACROS_H_

/********************** Detecting NULL passed to MACRO ************************/
/* based on from "Detect empty macro arguments" from Jens Gustedt, published
 * June 8, 2010 on https://gustedt.wordpress.com/2010/06/08/detect-empty-macro-arguments/
 */
#define _ARG16(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, ...) _15
#define HAS_COMMA(...) _ARG16(__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0)
/* following macro that just transforms the following parenthesis and contents into a comma */
#define _TRIGGER_PARENTHESIS_(...) ,

#define ISEMPTY(...)                                                    \
_ISEMPTY(                                                               \
          /* test if there is just one argument, eventually an empty    \
             one */                                                     \
          HAS_COMMA(__VA_ARGS__),                                       \
          /* test if _TRIGGER_PARENTHESIS_ together with the argument   \
             adds a comma */                                            \
          HAS_COMMA(_TRIGGER_PARENTHESIS_ __VA_ARGS__),                 \
          /* test if the argument together with a parenthesis           \
             adds a comma */                                            \
          HAS_COMMA(__VA_ARGS__ (/*empty*/)),                           \
          /* test if placing it between _TRIGGER_PARENTHESIS_ and the   \
             parenthesis adds a comma */                                \
          HAS_COMMA(_TRIGGER_PARENTHESIS_ __VA_ARGS__ (/*empty*/))      \
          )

#define PASTE5(_0, _1, _2, _3, _4) _0 ## _1 ## _2 ## _3 ## _4
#define _ISEMPTY(_0, _1, _2, _3) HAS_COMMA(PASTE5(_IS_EMPTY_CASE_, _0, _1, _2, _3))
#define _IS_EMPTY_CASE_0001 ,

/**************** MACROS to "eat" caracter if parameter is empty **************/
/* from user pfultz2, published in github repository Cloak
 * https://github.com/pfultz2/Cloak/wiki/C-Preprocessor-tricks,-tips,-and-idioms*/

/* Simple MACRO concatenation */
#define CAT(a, ...) PRIMITIVE_CAT(a  __VA_ARGS__)
#define PRIMITIVE_CAT(a, ...) a ## __VA_ARGS__

#define CAT4(a, b, c, ...) PRIMITIVE_CAT4(a, b, c, __VA_ARGS__)
#define PRIMITIVE_CAT4(a, b, c, ...) a ## b ## c ## __VA_ARGS__

#define CAT6(a, b, c, d, e, ...) PRIMITIVE_CAT6(a, b, c, d, e,  __VA_ARGS__)
#define PRIMITIVE_CAT6(a, b, c, d, e, ...) a ## b ## c ## d ## e ## __VA_ARGS__

#define CAT8(a, b, c, d, e, f, g, ...) PRIMITIVE_CAT8(a, b, c, d, e, f, g, __VA_ARGS__)
#define PRIMITIVE_CAT8(a, b, c, d, e, f, g, ...) a ## b ## c ## d ## e ## f ## g ## __VA_ARGS__

/* Stringify MACRO */
#define STRINGIFY(s) PRIMITIVE_STRINGIFY(s)
#define PRIMITIVE_STRINGIFY(s) #s

/* Simple IF macro that returns the parameter passed itself, when expanded
 * to IFF_1 and the other parameters if it's expanded to IFF_0.
 * This is the basis for a generalized IF MACRO */
#define IIF(c) PRIMITIVE_CAT(IIF_, c)
#define IIF_0(t, ...) __VA_ARGS__
#define IIF_1(t, ...) t

/* Complement of 0 or 1 */
#define COMPL(b) PRIMITIVE_CAT(COMPL_, b)
#define COMPL_0 1
#define COMPL_1 0

/* Core macros for detecting patterns */
#define CHECK_N(x, n, ...) n
#define CHECK(...) CHECK_N(__VA_ARGS__, 0,)
#define PROBE(x) x, 1,

/* NOT(x) always expands to 0, except when the parameter passed is 0 */
#define NOT(x) CHECK(PRIMITIVE_CAT(NOT_, x))
#define NOT_0 PROBE(~)

/* Generalized IF MACRO */
#define BOOL(x) COMPL(NOT(x))
#define IF(c) IIF(BOOL(c))

/* Finally, the conditonal expansion MACRO, that only expands if the condition
 * is true. Otherwise it eats the parameter */
#define EAT(...)
#define EXPAND(...) __VA_ARGS__
#define WHEN(c) IF(c)(EXPAND, EAT)
#define WHENNOT(c) IF(c)(EAT, EXPAND)

#endif

