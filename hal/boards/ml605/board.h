#ifndef _BOARD_H_
#define _BOARD_H_

#include <mem_layout/ml605/mem_layout.h>

/****************************/
/*    General Definitions  */
/****************************/
/* CPU Clock frequency in hertz */
#define SYS_CLOCK 100000000ULL

/* Baud rate of the builtin UART (does not apply to the VUART) */
#define UART_BAUDRATE 115200ULL

int board_init();
int board_update();

#endif
