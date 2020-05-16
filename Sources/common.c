/*
 * common.c

 *
 *  Created on: May 16, 2020
 *      Author: Diddi
 */

#include <MKL25Z4.h>





/* Delay n milliseconds */
/* The CPU core clock is set to MCGFLLCLK at 41.94 MHz in SystemInit(). */
void delayMs(int n) {
int i;
int j;
	for(i = 0 ; i < n; i++){
		for (j = 0; j < 1750; j++) {}
	}
}

void LED_set(char mode)
{
	if(mode=='s'){
      	GPIOB_PTOR |= 0x80000;    /* switch green LED */
	}
	
}

void LED_init(void)
{
    SIM_BASE_PTR->SCGC5 |= 0x400;        /* enable clock to Port B */
    SIM_BASE_PTR->SCGC5 |= 0x1000;       /* enable clock to Port D */
    PORTB_BASE_PTR->PCR[18] = 0x100;     /* make PTB18 pin as GPIO */
    GPIOB_PDDR |= (1<<18);       /* make PTB18 as output pin */
    GPIOB_PSOR |= (1<<18);       /* turn off red LED */
    PORTB_BASE_PTR->PCR[19] = 0x100;     /* make PTB19 pin as GPIO */
    GPIOB_PDDR |= 0x80000;       /* make PTB19 as output pin */
    GPIOB_PSOR |= 0x80000;       /* turn off green LED */
    PORTD_BASE_PTR->PCR[1] = 0x100;      /* make PTD1 pin as GPIO */
    GPIOD_PDDR |= (1<<1);          /* make PTD1 as output pin */
    GPIOD_PSOR |= (1<<1);          /* turn off blue LED */
}


/*
 * Validates a Time given hours and minutes.
 * Only does basic check. Hour below 24 and time below 60
 */
int ValidateTime(int hh , int mm)
{
    int ret=1;
    if(hh>=24)   ret=0;
    if(mm>=60)   ret=0;
    return ret;
}
