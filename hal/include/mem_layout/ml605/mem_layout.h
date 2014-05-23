
#ifndef _MEM_LAYOUT__H_
#define _MEM_LAYOUT__H_

/*********************** Static ML605 FPGA layout ***********************/

/* Should be autodiscovered by SDB */
#define FMC130M_BASE_ADDR 0x00310000

#define FPGA_CTRL_REGS (FMC130M_BASE_ADDR + 0x0000)
#define FPGA_SI571_I2C (FMC130M_BASE_ADDR + 0x0100)
#define FPGA_AD9510_SPI (FMC130M_BASE_ADDR + 0x0200)
#define FPGA_EEPROM_I2C (FMC130M_BASE_ADDR + 0x0300)
#define FPGA_LM75A_I2C (FMC130M_BASE_ADDR + 0x0400)

#define DSP_BASE_ADDR 0x00308000

#define DSP_CTRL_REGS (DSP_BASE_ADDR + 0x00000000)
#define DSP_BPM_SWAP (DSP_BASE_ADDR + 0x00000100)

#define WB_ACQ_BASE_ADDR 0x00330000

#endif

