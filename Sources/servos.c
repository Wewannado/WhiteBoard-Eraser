/*
 * servos.c
 *
 *  Created on: May 3, 2020
 *      Author: Roger Gili Coscojuela
 */

#include <MKL25Z4.h>
#include <servos.h>
#include <common.h>




void clockConfig()
{
	MCG_C1 = MCG_C1_IREFS_MASK | MCG_C1_IRCLKEN_MASK; // INTERNAL CLOCK|MCGIRCLK ACTIVE(SET)
	MCG_C2 = MCG_C2_IRCS_MASK;                        // SELECT FAST INTERNAL REFERENCE CLOCK (1)
	SIM_SOPT2 |= SIM_SOPT2_TPMSRC(3);                 // MCGIRCLK IS SELECTED FOR TPM CLOCK
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK; //enable clock to portA
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK; //enable clock to portD
	SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK; //  enable clock to TPM0 
	 
}
void pinConfig(){

	PORTD_BASE_PTR->PCR[4] = PORT_PCR_MUX(4);  //  especificar multiplexacio del TPM0_CH4
	PORTA_BASE_PTR->PCR[4] = PORT_PCR_MUX(3);  //  especificar multiplexacio del TPM0_CH1
	PORTA_BASE_PTR->PCR[5] = PORT_PCR_MUX(3);  //  especificar multiplexacio del TPM0_CH2
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


/**
 * grados should be an int ranged from 0 - 180
 */
void servoI(int grados){
	TPM0_BASE_PTR->CONTROLS[4].CnV = gradosToCnV(grados);
}

/**
 * grados should be an int ranged from 0 - 180
 */
void servoD(int grados){
	TPM0_BASE_PTR->CONTROLS[1].CnV =  gradosToCnV(grados);
}	

/**
 * grados should be an int ranged from 0 - 180
 */
void servoA(int grados){
	TPM0_BASE_PTR->CONTROLS[2].CnV = gradosToCnV(grados);
}
	
/** 
 * Convierte un valor de grados entre 0 180 a un valor adecuado para el valor de CnV.
 */
int gradosToCnV(int grados){
	
	//our servos goes from 9-85 values. General formula y=mx+n -> y = 0,422x+9
	return (int)((0.422*grados)+9); //TODO FORMULA CORRECTA
}
/**
 * 
 */
void servosInitialPosition(){
	servoA(SERVO_A_MAXALTURA);
	delayMs(1000);
	servoI(90);
	servoD(90);
	delayMs(1000);
}

void servoA_Bajar(){
	int i= SERVO_A_MAXALTURA;
	while(i!=SERVO_A_MINALTURA){
		TPM0_BASE_PTR->CONTROLS[2].CnV = gradosToCnV(i);
		i--;
		delayMs(30);
	}
	
}

