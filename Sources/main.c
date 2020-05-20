/**
 * Universitat Autònoma de Barcelona
 * MiP 2019 - 2020
 * NIUS
 * Roger Gili 1497893
 * Albert Romero 1425095
 */

#include <MKL25Z4.h>
#include "servos.h"
#include "uart.h"
#include "common.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ctime>

void UART0_IRQHandler(void);
void setDate(void);
void manualMode(void);
void programmingMode();
void actualitzaHora();

//HORA actual
struct tm dateinfo;
struct tm *dateinfoPointer=&dateinfo;

//boolea que indica si es necesari una actulitzacio de la hora que esta escrita a la pisarra
int repaintNeeded=0;

//cada quants SEGONS s'actualitza la dateinfo
int refreshTimmerSeconds=60;

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
	//borrarPizarra();
	//test
	while (1) {
		if(repaintNeeded){
			actualitzaHora();
		}
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
	LED_toogle('m');
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
	LED_toogle('m'); /* switch green LED off end of programming mode*/
}
void setDate(void) {
	messageSend("Si us plau, escriu la dateinfo actual en format HH:MM\r\n");
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
					"Data no valida. \r\nSi us plau, escriu la dateinfo actual en format HH:MM\r\n ");
		} else {
			messageSend("La nova data es:");
			messageSend(stringHora);
			messageSend(":");
			messageSend(stringMinuts);
			messageSend("\r\n");
			dateinfo.tm_hour = newHH;
			dateinfo.tm_min = newMM;
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
		servoA_Bajar();
		delayMs(2000);
		servoA(SERVO_A_MAXALTURA);
		
	}
}


void FTM1_IRQHandler(void) 
{
	LED_toogle('m');
	//messageSend("Interrupcio del timmer\r\n");
	segonsPassats++;
	if(segonsPassats>=refreshTimmerSeconds){
		repaintNeeded=1;
		segonsPassats=0;
	}
	TPM1_SC |= TPM_SC_TOF_MASK;   // clear TOF DONE
}


void actualitzaHora(){
	LED_toogle('t');
	int addMinutes=0;
	int addHours=0;
	if((dateinfo.tm_sec+refreshTimmerSeconds)<60){
		dateinfo.tm_sec=dateinfo.tm_sec+refreshTimmerSeconds;
	}
	else{
		dateinfo.tm_sec=(dateinfo.tm_sec+refreshTimmerSeconds)%60;
		addMinutes=(int)((dateinfo.tm_sec+refreshTimmerSeconds)/60);
	}
	if((dateinfo.tm_min+addMinutes)<60){
		dateinfo.tm_min=dateinfo.tm_min+addMinutes;
	}
	else{
		dateinfo.tm_min=(dateinfo.tm_min+addMinutes)%60;
		addHours=(int)((dateinfo.tm_min+addMinutes)/60);
	}
	if((dateinfo.tm_hour+addHours)<24){
			dateinfo.tm_hour=dateinfo.tm_hour+addHours;
	}
	else{
		dateinfo.tm_hour=(dateinfo.tm_hour+addHours)%24;
		}
	
	messageSend("Actualitzant a la nova hora:");
	//char buffer[8];
	//strftime(buffer,sizeof(buffer),"%H:%M:%S",dateinfoPointer);
	//messageSend(buffer);
	messageSend("\r\n");
	
	
	//mover los servos para actualizar la hora.
	borrarPizarra();
	//escribir la nueva hora
	escribirHoraEnPizarra(dateinfo.tm_hour, dateinfo.tm_min);
	//clears the repaint flag
	repaintNeeded=0;
	LED_toogle('t');
}



