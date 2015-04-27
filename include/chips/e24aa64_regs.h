/*
* Copyright (C) 2014 LNLS (www.lnls.br)
* Author: Lucas Russo <lucas.russo@lnls.br>
*
* Released according to the GNU LGPL, version 3 or any later version.
*/

#ifndef _E24AA64_REGS_H_
#define _E24AA64_REGS_H_

/* Copied from wb_acq_core_regs.h */
#if defined( __GNUC__)
#define PACKED __attribute__ ((packed))
#else
#error "Unsupported compiler?"
#endif

/* Copied from wb_acq_core_regs.h */
#ifndef __WBGEN2_MACROS_DEFINED__
#define __WBGEN2_MACROS_DEFINED__
#define WBGEN2_GEN_MASK(offset, size) (((1<<(size))-1) << (offset))
#define WBGEN2_GEN_WRITE(value, offset, size) (((value) & ((1<<(size))-1)) << (offset))
#define WBGEN2_GEN_READ(reg, offset, size) (((reg) >> (offset)) & ((1<<(size))-1))
#define WBGEN2_SIGN_EXTEND(value, bits) (((value) & (1<<bits) ? ~((1<<(bits))-1): 0 ) | (value))
#endif

#define E24AA64_ADDR_TRANS_SIZE                             16         /* in bits */
#define E24AA64_DATA_TRANS_SIZE                             8          /* in bits */
#define E24AA64_BYTE_TRANS_SIZE                             (E24AA64_ADDR_TRANS_SIZE + \
                                                               E24AA64_DATA_TRANS_SIZE) /* in bits */

/* Max of 16 address bytes + 32 data bytes */
#define E24AA64_PAGE_DATA_BYTES_MAX                         32
#define E24AA64_PAGE_TRANS_SIZE_MAX                         (E24AA64_ADDR_TRANS_SIZE + \
                                                               E24AA64_PAGE_DATA_BYTES_MAX*E24AA64_DATA_TRANS_SIZE)

/* E24AA64 address write */
#define E24AA64_SWAP_16BIT_BYTES(value)                     (((value >> 8) & 0x000000FF) | \
                                                               ((value << 8) & 0x0000FF00))

#define E24AA64_ADDR_SIZE                                   16
#define E24AA64_ADDR_SHIFT                                  0
#define E24AA64_ADDR_MASK                                   WBGEN2_GEN_MASK(E24AA64_ADDR_SHIFT, \
                                                                                   E24AA64_ADDR_SIZE)
/* We need to swap values to be written to the 24AA            4 */
#define E24AA64_ADDR_W(value)                               WBGEN2_GEN_WRITE(E24AA64_SWAP_16BIT_BYTES(value), \
                                                                                   E24AA64_ADDR_SHIFT, \
                                                                                   E24AA64_ADDR_SIZE)
#define E24AA64_ADDR_R(reg)                                 E24AA64_SWAP_16BIT_BYTES( \
                                                                   WBGEN2_GEN_READ(reg, E24AA64_ADDR_SHIFT, \
                                                                   E24AA64_ADDR_SIZE))
#endif

