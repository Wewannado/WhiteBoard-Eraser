/*
 * This program provides an example of a UART capable of send and receive.
 */
#include <MKL25Z4.H>
#include <string.h>
#include <stdlib.h>
#include <common.h>
#include <uart.h>


void UART0_IRQHandler(void);
void setDate(void);
void programmingMode();

//HORA actual
int hh=00;
int mm=00;



int main (void) {
LED_init();
UART0_init();

while (1) {
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






