/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
*/

#ifndef _SM_IO_FMC250M_4CH_CORE_H_
#define _SM_IO_FMC250M_4CH_CORE_H_

#define SMIO_AD9510_HANDLER(smio_handler) ((smch_ad9510_t *) smio_handler->smch_ad9510)
#define SMIO_SI57X_HANDLER(smio_handler) ((smch_si57x_t *) smio_handler->smch_si571)
#define SMIO_ISLA216P_HANDLER0(smio_handler) ((smch_isla216p_t *) smio_handler->smch_isla216p_adc0)
#define SMIO_ISLA216P_HANDLER1(smio_handler) ((smch_isla216p_t *) smio_handler->smch_isla216p_adc1)
#define SMIO_ISLA216P_HANDLER2(smio_handler) ((smch_isla216p_t *) smio_handler->smch_isla216p_adc2)
#define SMIO_ISLA216P_HANDLER3(smio_handler) ((smch_isla216p_t *) smio_handler->smch_isla216p_adc3)

/* The follosing codes were generated via the following command:
 * > echo FMC250M_4CH_ACTIVE | md5sum | cut -c 1-8
 * > cb04db4d
 * > echo FMC250M_4CH_ACTIVE | md5sum | cut -c 1-8
 * > 59da56ae
 */
#define FMC250M_4CH_ACTIVE_MD5              0x955393fc
#define FMC250M_4CH_PASSIVE_MD5             0xf9556611

/* Start writing on EEPROM address 0x0 */
#define FMC250M_4CH_EEPROM_START_ADDR       0x0

typedef enum {
    TYPE_FMC250M_4CH_UNDEF=0xFF,
    TYPE_FMC250M_4CH_ACTIVE=1,
    TYPE_FMC250M_4CH_PASSIVE
} fmc250m_4ch_type_e;

typedef struct {
    fmc250m_4ch_type_e type;                /* FMC250M_4CH type */
#if 0
    smch_amc7823_t *smch_amc7823;           /* AMC7823 chip handler */
#endif
    smch_isla216p_t *smch_isla216p_adc0;    /* ISLA216P 0 chip handler */
    smch_isla216p_t *smch_isla216p_adc1;    /* ISLA216P 1 chip handler */
    smch_isla216p_t *smch_isla216p_adc2;    /* ISLA216P 2 chip handler */
    smch_isla216p_t *smch_isla216p_adc3;    /* ISLA216P 3 chip handler */
    smch_ad9510_t *smch_ad9510;             /* AD9510 chip handler */
    smch_si57x_t *smch_si571;               /* SI571 chip handler */
    smch_24aa64_t *smch_24aa64;             /* 24AA64 chip handler */
    smch_pca9547_t *smch_pca9547;           /* FPGA I2C Switch */
} smio_fmc250m_4ch_t;

/* FPGA FMC250M delay definitions */

#define DLY_TYPE_DATA                       0x01
#define DLY_TYPE_CLK                        0x02
#define DLY_TYPE_ALL                        (DLY_TYPE_DATA | DLY_TYPE_CLK)

typedef enum {
    DLY_INIT_NO = 0,
    DLY_INIT_YES,
    DLY_INIT_END                            /* Placeholder for end of struct */
} fmc250m_4ch_delay_init_e;

/* Holds the delay value for each channel */

typedef struct {
    fmc250m_4ch_delay_init_e init;
    uint32_t value;
} delay_lines_t;

/***************** Our methods *****************/

/* Creates a new instance of the smio realization */
smio_fmc250m_4ch_t * smio_fmc250m_4ch_new (smio_t *parent);
/* Destroys the smio realization */
smio_err_e smio_fmc250m_4ch_destroy (smio_fmc250m_4ch_t **self_p);

#endif
