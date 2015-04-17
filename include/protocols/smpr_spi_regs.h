/*
* Copyright (C) 2014 LNLS (www.lnls.br)
* Author: Lucas Russo <lucas.russo@lnls.br>
*
* Released according to the GNU LGPL, version 3 or any later version.
*/

#ifndef _SMPR_SPI_REGS_H_
#define _SMPR_SPI_REGS_H_

#include "hw/wb_spi_regs.h"

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

/* Our flags are: SPI_PROTO_CHAR_LEN_MASK | SPI_PROTO_SS_MASK */
#define SMPR_PROTO_SPI_SS_FLAGS_W(value)            SPI_PROTO_SS_W(value)
#define SMPR_PROTO_SPI_SS_FLAGS_R(reg)              SPI_PROTO_SS_R(reg)

#define SMPR_PROTO_SPI_CHARLEN_FLAGS_SIZE           7
#define SMPR_PROTO_SPI_CHARLEN_FLAGS_SHIFT          SPI_PROTO_SS_SIZE
#define SMPR_PROTO_SPI_CHARLEN_FLAGS_MASK           WBGEN2_GEN_MASK(SMPR_PROTO_SPI_CHARLEN_FLAGS_SHIFT, \
                                                                    SMPR_PROTO_SPI_CHARLEN_FLAGS_SIZE)
#define SMPR_PROTO_SPI_CHARLEN_FLAGS_W(value)       WBGEN2_GEN_WRITE(value, SMPR_PROTO_SPI_CHARLEN_FLAGS_SHIFT, \
                                                        SMPR_PROTO_SPI_CHARLEN_FLAGS_SIZE)
#define SMPR_PROTO_SPI_CHARLEN_FLAGS_R(reg)         WBGEN2_GEN_READ(reg, SMPR_PROTO_SPI_CHARLEN_FLAGS_SHIFT, \
                                                        SMPR_PROTO_SPI_CHARLEN_FLAGS_SIZE)

#endif

