/*
 * common.c

 *
 *  Created on: May 16, 2020
 *      Author: Diddi
 */

#include <MKL25Z4.h>
#include <common.h> 

int TPM_50HZ_MOD=625;
int TPM_50HZ_PREESCALER=6;
int SERVO_A_MAXALTURA=50; //more height than this makes the structure hit the servo.
int SERVO_A_MINALTURA=33;


void clockConfig()
{
	MCG_C1 = MCG_C1_IREFS_MASK | MCG_C1_IRCLKEN_MASK; // INTERNAL CLOCK|MCGIRCLK ACTIVE(SET)
	MCG_C2 = MCG_C2_IRCS_MASK;                        // SELECT FAST INTERNAL REFERENCE CLOCK (1)
	SIM_SOPT2 |= SIM_SOPT2_TPMSRC(3);                 // MCGIRCLK IS SELECTED FOR TPM CLOCK
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK; //enable clock to portA
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK; //enable clock to portB
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK; //enable clock to portD
	SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK; //  enable clock to TPM0
	SIM_SCGC6 |= SIM_SCGC6_TPM1_MASK; //  enable clock to TPM1	 
}
void pinConfig(){

	PORTD_BASE_PTR->PCR[4] = PORT_PCR_MUX(4);  //  especificar multiplexacio del TPM0_CH4
	PORTA_BASE_PTR->PCR[4] = PORT_PCR_MUX(3);  //  especificar multiplexacio del TPM0_CH1
	PORTA_BASE_PTR->PCR[5] = PORT_PCR_MUX(3);  //  especificar multiplexacio del TPM0_CH2
	
	PORTB_BASE_PTR->PCR[18] = 0x100;     /* make PTB18 pin as GPIO */
	PORTB_BASE_PTR->PCR[19] = 0x100;     /* make PTB19 pin as GPIO */
	PORTD_BASE_PTR->PCR[1] = 0x100;      /* make PTD1 pin as GPIO */
	

}

void tpm0Config()
{
	TPM0_SC  = 0;              // disable timer while configuring
	TPM0_SC |= TPM_SC_PS(TPM_50HZ_PREESCALER);    // prescaler  
	TPM0_MOD = TPM_MOD_MOD(TPM_50HZ_MOD);  //  modulo value 
	TPM0_SC |= TPM_SC_TOF_MASK; //  clear TOF 
	TPM0_BASE_PTR->CONTROLS[4].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK; // SELECT CHANNEL MODE
	TPM0_BASE_PTR->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK; // SELECT CHANNEL MODE
	TPM0_BASE_PTR->CONTROLS[2].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK; // SELECT CHANNEL MODE
	TPM0_SC |= TPM_SC_CMOD(1);  //enable timer free-running mode 
}


void tpm1Config()
{
	TPM1_SC  = 0;             // disable timer while configuring
	TPM1_SC |= TPM_SC_PS(7);   // prescaler 
	TPM1_MOD = TPM_MOD_MOD(15625); // modulo value  
	TPM1_SC |= TPM_SC_TOF_MASK;              //  clear TOF 
	TPM1_SC |= TPM_SC_TOIE_MASK;              //  enable timeout interrupt 
    TPM1_SC |= TPM_SC_CMOD(1); //  enable timer free-running mode 
	NVIC_ICPR |= 0x00040000; //clear pending interrupt IRQ18 (bit 18 of ICPR)
	NVIC_ISER |= 0x00040000; //enable IRQ18 (bit 18 of ISER)
}
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
	if(mode=='p'){
      	GPIOB_PTOR |= 0x80000;    /* switch green LED */
	}
	if(mode=='m'){
		GPIOB_PTOR |= (1<<18);    /* switch red LED */
		}
	
}

void LED_init(void)
{
	GPIOB_PDDR |= (1<<18);       /* make PTB18 as output pin */
	GPIOB_PSOR |= (1<<18);       /* turn off red LED */
	GPIOB_PDDR |= 0x80000;       /* make PTB19 as output pin */
	GPIOB_PSOR |= 0x80000;       /* turn off green LED */
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



