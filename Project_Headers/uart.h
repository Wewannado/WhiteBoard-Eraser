/*
 * uart.h
 *
 *  Created on: May 16, 2020
 *      Author: Diddi
 */


#include <MKL25Z4.h>
#include <common.h>


void UART0_init(void);
void messageSend(char message[]);
