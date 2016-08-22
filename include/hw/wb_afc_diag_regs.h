#ifndef _WB_AFC_DIAG_REGS_H_
#define _WB_AFC_DIAG_REGS_H_

#include <inttypes.h>

#if defined( __GNUC__)
#define PACKED __attribute__ ((packed))
#else
#error "Unsupported compiler?"
#endif

#ifndef __WBGEN2_MACROS_DEFINED__
#define __WBGEN2_MACROS_DEFINED__
#define WBGEN2_GEN_MASK(offset, size) (((1<<(size))-1) << (offset))
#define WBGEN2_GEN_WRITE(value, offset, size) (((value) & ((1<<(size))-1)) << (offset))
#define WBGEN2_GEN_READ(reg, offset, size) (((reg) >> (offset)) & ((1<<(size))-1))
#define WBGEN2_SIGN_EXTEND(value, bits) (((value) & (1<<bits) ? ~((1<<(bits))-1): 0 ) | (value))
#endif


/* definitions for register: AFC ID Part 4 */

/* definitions for register: AFC ID Part 3 */

/* definitions for register: AFC ID Part 2 */

/* definitions for register: AFC ID Part 1 */

/* definitions for register: AFC Geographical Address ID */

/* definitions for field: AFC Card Slot in reg: AFC Geographical Address ID */
#define HALCS_AFC_DIAG_GEO_ID_CARD_SLOT_MASK                   WBGEN2_GEN_MASK(0, 16)
#define HALCS_AFC_DIAG_GEO_ID_CARD_SLOT_SHIFT                  0
#define HALCS_AFC_DIAG_GEO_ID_CARD_SLOT_W(value)               WBGEN2_GEN_WRITE(value, 0, 16)
#define HALCS_AFC_DIAG_GEO_ID_CARD_SLOT_R(reg)                 WBGEN2_GEN_READ(reg, 0, 16)

/* definitions for field: IPMI I2C address in reg: AFC Geographical Address ID */
#define HALCS_AFC_DIAG_GEO_ID_IPMI_ADDR_MASK                   WBGEN2_GEN_MASK(16, 16)
#define HALCS_AFC_DIAG_GEO_ID_IPMI_ADDR_SHIFT                  16
#define HALCS_AFC_DIAG_GEO_ID_IPMI_ADDR_W(value)               WBGEN2_GEN_WRITE(value, 16, 16)
#define HALCS_AFC_DIAG_GEO_ID_IPMI_ADDR_R(reg)                 WBGEN2_GEN_READ(reg, 16, 16)

/* definitions for register: AFC Sensor Data Valid */

/* definitions for register: AFC Sensor Data Start */

/* definitions for register: AFC Sensor Data End */

/* definitions for register: AFC FMC Slot Status */

/* [0x0]: AFC ID Part 4 */
#define HALCS_AFC_DIAG_REG_ID_PART4           0x00000000
/* [0x4]: AFC ID Part 3 */
#define HALCS_AFC_DIAG_REG_ID_PART3           0x00000004
/* [0x8]: AFC ID Part 2 */
#define HALCS_AFC_DIAG_REG_ID_PART2           0x00000008
/* [0xc]: AFC ID Part 1 */
#define HALCS_AFC_DIAG_REG_ID_PART1           0x0000000c
/* [0x10]: AFC Geographical Address ID */
#define HALCS_AFC_DIAG_REG_GEO_ID             0x00000010
/* [0x14]: AFC Sensor Data Valid */
#define HALCS_AFC_DIAG_REG_DATA_VALID         0x00000014
/* [0x18]: AFC Sensor Data Start */
#define HALCS_AFC_DIAG_REG_SENS_DATA_START    0x00000018
/* [0x3f8]: AFC Sensor Data End */
#define HALCS_AFC_DIAG_REG_SENS_DATA_END      0x000003f8
/* [0x3fc]: AFC FMC Slot Status */
#define HALCS_AFC_DIAG_REG_FMC_SLOT_STATUS    0x000003fc

#endif
