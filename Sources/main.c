/**
 * Universitat Autonoma de Barcelona
 * MiP 2019 - 2020
 * NIUS
 * Roger Gili 1497893
 * Albert Romero 
 */

#include <MKL25Z4.h>
#include "servos.h"
#include "uart.h"
#include "common.h"
#include <stdlib.h>
#include <stdio.h>

void UART0_IRQHandler(void);
void setDate(void);
void manualMode(void);
void programmingMode();
void actualitzaHora();

//HORA actual
int hh = 00;
int mm = 00;

//cada quants MINUTS s'actualitza la hora
int refreshTimmerMinutes=1;

//timer variable
int segonsPassats=0;

int main() {
	clockConfig();
	tpm0Config();
	tpm1Config();
	pinConfig();
	LED_init();
	UART0_init();

	servosCenter();
	servosInitialPosition();
	//servosCalibrate();

	delayMs(50);
	//borrar();
	//servosInitialPosition();
	while (1) {

		//escribirHora(0,0);
		//		
		//		int i=0;
		//		servoA(SERVO_A_MAXALTURA);
		//		for (i=0;i<5000000; i++){;}
		//		servoA(SERVO_A_MINALTURA);
		//		for (i=0;i<5000000; i++){;}
		//    while(1)
		//    {  
		//    	int i=0;
		//    	int grados;
		//    		for (grados=8; grados<85; ){
		//				for (i=0;i<500000; i++){;}
		//				servoA(grados);
		//				servoI(grados);
		//				servoD(grados);
		//				grados=grados+1;
		//    		}
		//for (i=0;i<50000000; i++){;}
		//servoA(11);
		//for (i=0;i<10000000; i++){;}
		//servoA(83);
		//for (i=0;i<10000000; i++){;}

	}
	return 0;
}


void UART0_IRQHandler(void) {
	unsigned char c;
	c = UART0_BASE_PTR ->D; /* read the char received */
	switch (c) {
	case 'p': //Mode de programació
		programmingMode();
		NVIC_ICPR |= 0x00001000; //clear the interruption flag.
		break;
	default: //only shown if the user presses an unknow key

		messageSend("Premeu 'p' per entrar en el mode de programacio.\r\n");
		NVIC_ICPR |= 0x00001000; //clear the interruption flag.
		break;
	}
	//LED_set(c);             /* and use it to set LEDs */

}
void programmingMode() {
	//GPIOB_PCOR |= 0x80000; /* switch green LED Programming mode ON */
	LED_set('m');
	char missatgeExplicacio[] =
			"Prem 't' per programar la hora actual\r\nPrem 'd' per moure els servos manualment\r\nPrem 'e' per sortir\r\n";
	//char missatgeEmetre[100]= "Entrant en mode de programacio\r\nSi us plau, escriu la hora actual en format HH:MM\r\n";
	messageSend("Entrant en mode de programacio\r\n\r\n");
	messageSend(missatgeExplicacio);
	unsigned char c;
	int exit = 0;
	while (!exit) {
		while (!(UART0_BASE_PTR ->S1 & UART0_S1_RDRF_MASK))
			;
		c = UART0_BASE_PTR ->D; /* read the char received */
		switch (c) {
		case 'e': //Exit
			exit = 1;
			break;
		case 't': //Programacion del tiempo
			setDate();
			exit = 1;
			break;
		case 'd': //Moviment manual dels servos
			manualMode();
			exit = 1;
			break;
		default: //only shown if the user presses an unknow key
			messageSend("Opcio no reconeguda.\r\n");
			messageSend(missatgeExplicacio);
			break;
		}
	}
	messageSend("Exit.\r\n");
	LED_set('m'); /* switch green LED off end of programming mode*/
}
void setDate(void) {
	messageSend("Si us plau, escriu la hora actual en format HH:MM\r\n");
	char stringHora[2];
	char stringMinuts[2];
	int newHH;
	int newMM;
	while (!ValidateTime(newHH, newMM)) {
		/* Wait until character has been received */
		while (!(UART0_BASE_PTR ->S1 & UART0_S1_RDRF_MASK))
			;
		stringHora[0] = UART0_BASE_PTR ->D;
		while (!(UART0_BASE_PTR ->S1 & UART0_S1_RDRF_MASK))
			;
		stringHora[1] = UART0_BASE_PTR ->D;
		newHH = atoi(stringHora);

		/*ignore the ":" */
		while (!(UART0_BASE_PTR ->S1 & UART0_S1_RDRF_MASK))
			;
		UART0_BASE_PTR ->D;

		/* Wait until character has been received */
		while (!(UART0_BASE_PTR ->S1 & UART0_S1_RDRF_MASK))
			;
		stringMinuts[0] = UART0_BASE_PTR ->D;
		while (!(UART0_BASE_PTR ->S1 & UART0_S1_RDRF_MASK))
			;
		stringMinuts[1] = UART0_BASE_PTR ->D;
		newMM = atoi(stringMinuts);

		if (!ValidateTime(newHH, newMM)) {
			messageSend(
					"Data no valida. \r\nSi us plau, escriu la hora actual en format HH:MM\r\n ");
		} else {
			messageSend("La nova data es:");
			messageSend(stringHora);
			messageSend(":");
			messageSend(stringMinuts);
			messageSend("\r\n");
			hh = newHH;
			mm = newMM;
		}
	}
}
void manualMode(void) {
	while (1) {
		messageSend(
				"Posicio del servo esquerra? Format xxx. Exemple: per a 90 graus escriure 090. Escriu 'e' per sortir\r\n");
		char stringI[3];
		int i = 0;
		for (i = 0; i < 3; i++) {
			while (!(UART0_BASE_PTR ->S1 & UART0_S1_RDRF_MASK))
				;
			stringI[i] = UART0_BASE_PTR ->D;
			if (stringI[i] == 'e')
				return;
		}
		messageSend(
				"Posicio del servo dret? Format xxx. Exemple: per a 90 graus escriure 090. Escriu 'e' per sortir\r\n");
		char stringD[3];
		for (i = 0; i < 3; i++) {
			while (!(UART0_BASE_PTR ->S1 & UART0_S1_RDRF_MASK))
				;
			stringD[i] = UART0_BASE_PTR ->D;
			if (stringD[i] == 'e')
				return;
		}
		messageSend("Movent servo esquerra a: ");
					messageSend(stringI);
					messageSend(" movent servo dret a: ");
					messageSend(stringD);
					messageSend("\r\n");
		servoI(atoi(stringI));
		servoD(atoi(stringD));
		
	}
}


void FTM1_IRQHandler(void) 
{
	LED_set('m');
	//messageSend("Interrupcio del timmer\r\n");
	segonsPassats++;
	if(segonsPassats==(refreshTimmerMinutes*60)){
		actualitzaHora();
		segonsPassats=0;
	}
	TPM1_SC |= TPM_SC_TOF_MASK;   // clear TOF DONE
}


void actualitzaHora(){
	mm=mm+refreshTimmerMinutes;
	if(mm>=60){
		mm=mm%60;
		hh++;
	}
	if(hh==24){
		hh=0;
	}
	char stringHH[2];
	char stringMM[2];
	sprintf(stringHH, "%d", hh);
	sprintf(stringMM, "%d", mm);
	messageSend("Actualitzant a la nova hora:");
	messageSend(stringHH);
	messageSend(":");
	messageSend(stringMM);
	messageSend("\r\n");
}



