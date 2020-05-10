
#include <MKL25Z4.H>

void UART0_init(void);
void delayMs(int n);
void LED_init(void);
void LED_set(char value);
int i=0;
int main (void) {
	
    UART0_init();
    LED_init();
    while (1) {   
    }
}

/* UART0 interrupt handler */
void UART0_IRQHandler(void) 
{
    char c;
    c = UART0_BASE_PTR->D;           /* read the char received */
    LED_set(c);             /* and use it to set LEDs */
    NVIC_ICPR |= 0x00001000; //clear the interruption flag.
}

/* initialize UART0 to receive at 57600 Baud */
void UART0_init(void) 
    {
    SIM_BASE_PTR->SCGC4 |= 0x0400;    /* enable clock for UART0 */
    SIM_BASE_PTR->SOPT2 |= 0x04000000;   /* use FLL output for UART Baud rate generator */
    UART0_BASE_PTR->C2 = 0;          /* turn off UART0 while changing configurations */
    UART0_BASE_PTR->BDH = 0x00;
    UART0_BASE_PTR->BDL = 0x17;      /* 115200 Baud, nope, 57600*/
    UART0_BASE_PTR->C4 = 0x0F;       /* Over Sampling Ratio 16 */
    UART0_BASE_PTR->C1 = 0x00;       /* 8-bit data */
    //UART0_BASE_PTR->C2 = 0x24;       /* enable receive and receive interrupt*/
    UART0_BASE_PTR->C2 |= UART0_C2_RE_MASK; /* receive enable */
    UART0_BASE_PTR->C2 |= UART0_C2_RIE_MASK; /* receive interrupt enable */
    NVIC_ICPR |= 0x00001000; /* clear interruption */
    NVIC_ISER |= 0x00001000; /* enable INT12 (bit 12 of ISER[0]) */
    SIM_BASE_PTR->SCGC5 |= 0x0200;    /* enable clock for PORTA */
    PORTA_BASE_PTR->PCR[1] = 0x0200; /* make PTA1 UART0_Rx pin */
}

/* initialize all three LEDs on the FRDM board */
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

/* turn on or off the LEDs according to bit 2-0 of the value */
void LED_set(char value)
{
	if(value=='c'){
      	GPIOB_PTOR |= 0x80000;    /* switch green LED */
	}
}
