#include <MKL25Z4.h>
#include "derivative.h" /* include peripheral declarations */


#define RED 18
#define RED_SHIFT (1<<RED)
static int Mod=625;
static int preescaler=6;

void clockConfig();
void inicializarPines();
void inicializarTPM();
void disableClocks();
void enableClocks();
void cosas();

void test();

int main(void) {

	void clockConfig();
	void inicializarPines();
	void inicializarTPM();

	
	//activamos el clock al port B para pruebas con el pin rojo
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
	PORTB_PCR18 = PORT_PCR_MUX(1);
	GPIOB_PSOR |= RED_SHIFT; 
	GPIOB_PDDR |= RED_SHIFT ;
	
	
	
	for (;;) {
		test(1000000, 100);
		GPIOB_PTOR |= RED_SHIFT;
		test(1000000, 80);
		GPIOB_PTOR |= RED_SHIFT;
		test(1000000, 60);
		GPIOB_PTOR |= RED_SHIFT;
		test(1000000, 40);
		GPIOB_PTOR |= RED_SHIFT;
		test(1000000, 20);
		GPIOB_PTOR |= RED_SHIFT;
		test(1000000, 10);
		GPIOB_PTOR |= RED_SHIFT;
		test(1000000, 5);
		GPIOB_PTOR |= RED_SHIFT;
		test(1000000, 0);
		GPIOB_PTOR |= RED_SHIFT;
	}
	return 0;
}

void clockConfig() {
	// set clock to 2MHz
	MCG_C1 = MCG_C1_IREFS_MASK | MCG_C1_IRCLKEN_MASK; // INTERNAL CLOCK|MCGIRCLK ACTIVE(SET)
	MCG_C2 = MCG_C2_IRCS_MASK;       // SELECT FAST INTERNAL REFERENCE CLOCK (1)
	SIM_SOPT2 |= SIM_SOPT2_TPMSRC(3);      // MCGIRCLK IS SELECTED FOR TPM CLOCK
}

void inicializarPines() {
	//activamos el clock al port D y A
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;
	
	
	
	
	//activamos el pin PTD4 al TPM0_CH4
	PORTD_PCR4 = PORT_PCR_MUX(4);
	//activamos PTA1 y PTA2 al TPM2_CH0 y TPM2_CH1 repectivamente
	PORTA_PCR1 = PORT_PCR_MUX(3);
	PORTA_PCR2 = PORT_PCR_MUX(3);
}

void cosas(){
	MCG_BASE_PTR ->C1 = MCG_C1_IREFS_MASK | MCG_C1_IRCLKEN_MASK; //  INTERNAL CLOCK|MCGIRCLK ACTIVE(SET)
	MCG_BASE_PTR ->C2 = MCG_C2_IRCS_MASK; // SELECT FAST INTERNAL REFERENCE CLOCK (1)
	SIM_BASE_PTR ->SCGC6 |= SIM_SCGC6_TPM0_MASK;  // ENABLE TPM2 CLOCK GATE
	SIM_BASE_PTR ->SCGC6 |= SIM_SCGC6_TPM2_MASK;  // ENABLE TPM2 CLOCK GATE
	SIM_BASE_PTR ->SOPT2 |= SIM_SOPT2_TPMSRC(3); // MCGIRCLK IS SELECTED FOR TPM CLOCK

	TPM0_BASE_PTR ->SC |= TPM_SC_PS(6);  // TODO DONE
	TPM0_BASE_PTR ->SC |= TPM_SC_CMOD(1);  // COUNTER INC. ON EVERY CLOCK
	TPM2_BASE_PTR ->SC |= TPM_SC_PS(6);  // TODO DONE
	TPM2_BASE_PTR ->SC |= TPM_SC_CMOD(1);  // COUNTER INC. ON EVERY CLOCK


}
void inicializarTPM() { //iniciamos los timers TPM0 y 2, que son los que estan conectados con los pines PTA1 PTA2
	SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK;
	SIM_SCGC6 |= SIM_SCGC6_TPM2_MASK;
	
	disableClocks();

	TPM0_SC |= TPM_SC_PS(preescaler); //  prescaler 
	TPM0_MOD = TPM_MOD_MOD(Mod);  //  modulo value 
	TPM0_SC |= TPM_SC_TOF_MASK;
	TPM0_BASE_PTR->CONTROLS[4].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
	TPM2_SC |= TPM_SC_PS(preescaler); //  prescaler 6 para 50HZ
	TPM2_MOD = TPM_MOD_MOD(Mod);  //  modulo value 625 para 50 HZ
	TPM2_SC |= TPM_SC_TOF_MASK;
	TPM2_BASE_PTR ->CONTROLS[0].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK; // SELECT CHANNEL MODE
	TPM2_BASE_PTR ->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK; // SELECT CHANNEL MODE
	
	enableClocks();
}

void disableClocks(){   //error? quizas deberia ser TPM0_SC |= TPM_SC_CMOD(0);
	//TPM0_SC = 0;             // disable timer while configuring
	TPM0_SC |= TPM_SC_CMOD(0);
	//TPM2_SC = 0;
	TPM2_SC |= TPM_SC_CMOD(0);
}

void enableClocks(){
	TPM0_SC |= TPM_SC_TOF_MASK;
	TPM2_SC |= TPM_SC_TOF_MASK;
	TPM0_SC |= TPM_SC_CMOD(1); 
	TPM2_SC |= TPM_SC_CMOD(1);
}

void test(int time, int duty){
		
		int count=0;
		int i=0;
		for (i=0;i<time; i++)
		{
			count++;
		}
		//TPM0_BASE_PTR->CONTROLS[4].CnV = duty;  // TODO especificar duty cycle
		TPM2_BASE_PTR->CONTROLS[0].CnV = duty;  // TODO especificar duty cycle
		//TPM2_BASE_PTR->CONTROLS[1].CnV = duty;  // TODO especificar duty cycle
		
}
