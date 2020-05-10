/*
 * This program provides an example of a UART capable of send and receive.
 */
#include <MKL25Z4.H>
#include <string.h>
#include <stdlib.h>


void UART0_init(void);
void UART0_IRQHandler(void);
void setDate(void);

void delayMs(int n);
void messageSend(char message[]);
void LED_init(void);
void programmingMode();
int ValidateTime(int hh , int mm);

//HORA actual
int hh=00;
int mm=00;



int main (void) {
LED_init();
UART0_init();

while (1) {
}
}




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


/* Delay n milliseconds */
/* The CPU core clock is set to MCGFLLCLK at 41.94 MHz in SystemInit(). */
void delayMs(int n) {
int i;
int j;
	for(i = 0 ; i < n; i++){
		for (j = 0; j < 7000; j++) {}
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


void LED_set(char mode)
{
	if(mode=='s'){
      	GPIOB_PTOR |= 0x80000;    /* switch green LED */
	}
	
}

