/*
* Copyright (C) 2014 LNLS (www.lnls.br)
* Author: Lucas Russo <lucas.russo@lnls.br>
*
* Released according to the GNU LGPL, version 3 or any later version.
*/

#ifndef _SMPR_I2C_REGS_H_
#define _SMPR_I2C_REGS_H_

#include "hw/wb_i2c_regs.h"

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

/* Our flags are: I2C_PROTO_REP_START | I2C_PROTO_TRANS_SIZE | I2C_PROTO_ADDR */

#define SMPR_PROTO_I2C_ADDR_FLAGS_SIZE                  7          /* I2C Address are 7 bits (no 10-bit support) */
#define SMPR_PROTO_I2C_ADDR_FLAGS_SHIFT                 0
#define SMPR_PROTO_I2C_ADDR_FLAGS_MASK                  WBGEN2_GEN_MASK(SMPR_PROTO_I2C_ADDR_FLAGS_SHIFT, \
                                                                        SMPR_PROTO_I2C_ADDR_FLAGS_SIZE)
#define SMPR_PROTO_I2C_ADDR_FLAGS_W(value)              WBGEN2_GEN_WRITE(value, SMPR_PROTO_I2C_ADDR_FLAGS_SHIFT, \
                                                                        SMPR_PROTO_I2C_ADDR_FLAGS_SIZE)
#define SMPR_PROTO_I2C_ADDR_FLAGS_R(reg)                WBGEN2_GEN_READ(reg, SMPR_PROTO_I2C_ADDR_FLAGS_SHIFT, \
                                                                        SMPR_PROTO_I2C_ADDR_FLAGS_SIZE)

#define SMPR_PROTO_I2C_TRANS_SIZE_FLAGS_SIZE            9
#define SMPR_PROTO_I2C_TRANS_SIZE_FLAGS_SHIFT           SMPR_PROTO_I2C_ADDR_FLAGS_SIZE
#define SMPR_PROTO_I2C_TRANS_SIZE_FLAGS_MASK            WBGEN2_GEN_MASK(SMPR_PROTO_I2C_TRANS_SIZE_FLAGS_SHIFT, \
                                                                        SMPR_PROTO_I2C_TRANS_SIZE_FLAGS_SIZE)
#define SMPR_PROTO_I2C_TRANS_SIZE_FLAGS_W(value)        WBGEN2_GEN_WRITE(value, SMPR_PROTO_I2C_TRANS_SIZE_FLAGS_SHIFT, \
                                                                        SMPR_PROTO_I2C_TRANS_SIZE_FLAGS_SIZE)
#define SMPR_PROTO_I2C_TRANS_SIZE_FLAGS_R(reg)          WBGEN2_GEN_READ(reg, SMPR_PROTO_I2C_TRANS_SIZE_FLAGS_SHIFT, \
                                                                        SMPR_PROTO_I2C_TRANS_SIZE_FLAGS_SIZE)
#define SMPR_PROTO_I2C_TRANS_SIZE_FLAGS_MAX             (SMPR_PROTO_I2C_TRANS_SIZE_FLAGS_MASK >> SMPR_PROTO_I2C_TRANS_SIZE_FLAGS_SHIFT)

#define SMPR_PROTO_I2C_REP_START_SHIFT                  (SMPR_PROTO_I2C_TRANS_SIZE_FLAGS_SIZE + \
                                                            SMPR_PROTO_I2C_TRANS_SIZE_FLAGS_SHIFT)
#define SMPR_PROTO_I2C_REP_START_SIZE                   1
#define SMPR_PROTO_I2C_REP_START                        WBGEN2_GEN_MASK(SMPR_PROTO_I2C_REP_START_SHIFT,\
                                                                    SMPR_PROTO_I2C_REP_START_SIZE)

#endif

