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

void UART0_IRQHandler(void);
void setDate(void);
void programmingMode();
void testA();
//HORA actual
int hh=00;
int mm=00;


int main()
{
	clockConfig();
	tpm0Config();
	pinConfig();
	LED_init();
	UART0_init();
	
	servosInitialPosition();
	

	
	delayMs(50);
	borrar();
	while(1){
		
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


void testA(){
	delayMs(3000);
			int i= 90;
			for (i=90; i>30; i--){
				servoI(i);
				servoD(i);
				delayMs(40);
			}
}

void UART0_IRQHandler(void) 
{
    unsigned char c;
    c = UART0_BASE_PTR->D;           /* read the char received */
    switch(c)
    {
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
void programmingMode(){
	GPIOB_PCOR |= 0x80000;    /* switch green LED Programming mode ON */
	char missatgeExplicacio[]="Prem 't' per programar la hora actual\r\nPrem 'e' per sortir\r\n";
	//char missatgeEmetre[100]= "Entrant en mode de programacio\r\nSi us plau, escriu la hora actual en format HH:MM\r\n";
	messageSend("Entrant en mode de programacio\r\n\r\n");
	messageSend(missatgeExplicacio);
	unsigned char c;
	int exit=0;
	while(!exit){
		while(!(UART0_BASE_PTR->S1 & UART0_S1_RDRF_MASK));
	    c = UART0_BASE_PTR->D;           /* read the char received */
	    switch(c)
	    {
	    case 'e': //Exit
	    	exit=1;
	    	break;
	    case 't': //Programacion del tiempo
	    	setDate();
	    	exit=1;
	    	break;
	    default: //only shown if the user presses an unknow key
			messageSend("Opcio no reconeguda.\r\n");
			messageSend(missatgeExplicacio);
			break;
	    }
	}
	messageSend("Exit.\r\n");
	GPIOB_PSOR |= 0x80000;    /* switch green LED off end of programming mode*/
}
void setDate(void) 
{
	messageSend("Si us plau, escriu la hora actual en format HH:MM\r\n");
	char stringHora[2];
	char stringMinuts[2];
	int newHH;
	int newMM;
	while(!ValidateTime(newHH,newMM)){
	    /* Wait until character has been received */
	    while(!(UART0_BASE_PTR->S1 & UART0_S1_RDRF_MASK));
	    stringHora[0]=UART0_BASE_PTR->D;
	    while(!(UART0_BASE_PTR->S1 & UART0_S1_RDRF_MASK));
	    stringHora[1]=UART0_BASE_PTR->D;
	    newHH=atoi(stringHora);
	    
	    /*ignore the ":" */
	    while(!(UART0_BASE_PTR->S1 & UART0_S1_RDRF_MASK));
	    UART0_BASE_PTR->D;
	    
		/* Wait until character has been received */
		while(!(UART0_BASE_PTR->S1 & UART0_S1_RDRF_MASK));
		stringMinuts[0]=UART0_BASE_PTR->D;
		while(!(UART0_BASE_PTR->S1 & UART0_S1_RDRF_MASK));
		stringMinuts[1]=UART0_BASE_PTR->D;
		newMM=atoi(stringMinuts);
		
		if(!ValidateTime(newHH,newMM)){
			messageSend("Data no valida. \r\nSi us plau, escriu la hora actual en format HH:MM\r\n ");
		}
		else{
			messageSend("La nova data es:");
			messageSend(stringHora);
			messageSend(":");
			messageSend(stringMinuts);
			messageSend("\r\n");
			hh=newHH;
			mm=newMM;
		}
	}
	
}


