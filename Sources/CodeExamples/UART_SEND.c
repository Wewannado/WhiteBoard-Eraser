
#include <MKL25Z4.H>

void UART0_init(void);
void delayMs(int n);
void messageSend(char message[]);

int main (void) {
UART0_init();
while (1) {
char missatgeEmetre[100]= "He creat un monstre??\r\n";
messageSend(missatgeEmetre);
delayMs(1000); /* leave a gap between messages */
char nouMissatge[100]= "IT'S ALIVE!!!!!\r\n";
messageSend(nouMissatge);
delayMs(10000); /* leave a gap between messages */
}
}
/* initialize UART0 to transmit at 115200 Baud?? -- in fact, 57600 */
void UART0_init(void) {
	SIM_BASE_PTR->SCGC4 |= SIM_SCGC4_UART0_MASK; /* enable clock for UART0 */
	SIM_BASE_PTR->SOPT2 |= 0x04000000; /* use FLL output for UART Baud rate generator */
	UART0_BASE_PTR->C2 = 0; /* turn off UART0 while changing configurations */
	UART0_BASE_PTR->BDH = 0x00;
	UART0_BASE_PTR->BDL = 0x17; /* 115200 Baud */
	UART0_BASE_PTR->C4 = 0x0F; /* Over Sampling Ratio 16 */
	UART0_BASE_PTR->C1 = 0x00; /* 8-bit data */
	UART0_BASE_PTR->C2 = 0x08; /* enable transmit */
	SIM_BASE_PTR->SCGC5 |= SIM_SCGC5_PORTA_MASK; /* enable clock for PORTA */
	PORTA_BASE_PTR->PCR[2] = PORT_PCR_MUX(2); /* make PTA2 UART0_Tx pin */
}
/* Delay n milliseconds */
/* The CPU core clock is set to MCGFLLCLK at 41.94 MHz in SystemInit(). */
void delayMs(int n) {
int i;
int j;
for(i = 0 ; i < n; i++)
for (j = 0; j < 7000; j++) {}
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

