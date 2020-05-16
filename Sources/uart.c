/*
 * uart.c
 *
 *  Created on: May 16, 2020
 *      Author: Diddi
 */

#include <MKL25Z4.h>
#include <common.h>

/* initialize UART0 to transmit AND interrupt receive at 115200 Baud?? -- in fact, 57600 */
void UART0_init(void) {
    SIM_BASE_PTR->SCGC4 |= SIM_SCGC4_UART0_MASK;    /* enable clock for UART0 */
    SIM_BASE_PTR->SOPT2 |= 0x04000000;   /* use FLL output for UART Baud rate generator */
    UART0_BASE_PTR->C2 = 0;          /* turn off UART0 while changing configurations */
    UART0_BASE_PTR->BDH = 0x00;
    UART0_BASE_PTR->BDL = 0x17;      /* 115200 Baud */
    UART0_BASE_PTR->C4 = 0x0F;       /* Over Sampling Ratio 16 */
    UART0_BASE_PTR->C1 = 0x00;       /* 8-bit data */
    UART0_BASE_PTR->C2 |= UART0_C2_RE_MASK; /* receive enable */
    UART0_BASE_PTR->C2 |= UART0_C2_RIE_MASK; /* receive interrupt enable */
    UART0_BASE_PTR->C2 |= UART0_C2_TE_MASK; /* transmit enable */
    NVIC_ICPR |= 0x00001000; /* clear interruption */
    NVIC_ISER |= 0x00001000; /* enable INT12 (bit 12 of ISER[0]) */
    SIM_BASE_PTR->SCGC5 |= SIM_SCGC5_PORTA_MASK;    /* enable clock for PORTA */
    PORTA_BASE_PTR->PCR[2] = PORT_PCR_MUX(2); /* make PTA2 UART0_Tx pin */
    PORTA_BASE_PTR->PCR[1] = PORT_PCR_MUX(2); ; /* make PTA1 UART0_Rx pin */
}



/*
 * 
 */
void messageSend(char message[]){
	int i=0;
	for (i=0; i<=strlen(message);i++){
		while(!(UART0_BASE_PTR->S1 & UART_S1_TDRE_MASK)) {
		} /* wait for transmit buffer empty */
		UART0_BASE_PTR->D = message[i]; /* send a char */	
	}
	/*wait for the last byte to be transmited */
	while(!(UART0_BASE_PTR->S1 & UART_S1_TDRE_MASK)) {
			} 
}
