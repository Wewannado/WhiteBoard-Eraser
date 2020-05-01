/**
 * Universitat Autonoma de Barcelona
 * MiP 2019 - 2020
 * NIUS
 * Roger Gili 1497893
 * Albert Romero 
 */

#include <MKL25Z4.h>



//module and preescaler for 50hz
static int Mod=625;
static int preescaler=6;


void clockConfig();
void servoA(int grados);
void servoB(int grados);
void servoC(int grados);
int gradosToCnV(int grados);
void tpm0Config();
void tpm2Config();
void pinConfig();


int main()
{
	clockConfig();
    tpm0Config();
    tpm2Config();
    pinConfig();
   
    while(1)
    {  
    	int i=0;
    	int grados;
    		//for (grados=8; grados<85; ){
			//	for (i=0;i<500000; i++){;}
			//	servoA(grados);
			//	grados=grados+1;
    		//}
    		//for (i=0;i<50000000; i++){;}
    		servoA(11);
    		for (i=0;i<10000000; i++){;}
    		servoA(83);
    		for (i=0;i<10000000; i++){;}
    		
    }
    return 0;
}

void clockConfig()
{
	MCG_C1 = MCG_C1_IREFS_MASK | MCG_C1_IRCLKEN_MASK; // INTERNAL CLOCK|MCGIRCLK ACTIVE(SET)
	MCG_C2 = MCG_C2_IRCS_MASK;                        // SELECT FAST INTERNAL REFERENCE CLOCK (1)
	SIM_SOPT2 |= SIM_SOPT2_TPMSRC(3);                 // MCGIRCLK IS SELECTED FOR TPM CLOCK
}
void pinConfig(){
	SIM_BASE_PTR->SCGC5 |= SIM_SCGC5_PORTA_MASK; //enable clock to portA
	SIM_BASE_PTR->SCGC5 |= SIM_SCGC5_PORTD_MASK; //enable clock to portA
	PORTA_BASE_PTR->PCR[1] = PORT_PCR_MUX(3);  //  especificar multiplexacio del TPM1_CH0
	PORTA_BASE_PTR->PCR[2] = PORT_PCR_MUX(3);  //  especificar multiplexacio del TPM1_CH0
	PORTD_BASE_PTR->PCR[4] = PORT_PCR_MUX(4);  //  especificar multiplexacio del TPM1_CH0
}

void tpm0Config()
{
	SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK; //  enable clock to TPM1 
	TPM0_SC  = 0;              // disable timer while configuring
	TPM0_SC |= TPM_SC_PS(preescaler);    // prescaler  
	TPM0_MOD = TPM_MOD_MOD(Mod);  //  modulo value 
	TPM0_SC |= TPM_SC_TOF_MASK; //  clear TOF 
	TPM0_BASE_PTR->CONTROLS[4].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK; // SELECT CHANNEL MODE
	TPM0_SC |= TPM_SC_CMOD(1);  //enable timer free-running mode 
}

void tpm2Config()
{
	SIM_SCGC6 |= SIM_SCGC6_TPM2_MASK; //  enable clock to TPM1 
	TPM2_SC  = 0;              // disable timer while configuring
	TPM2_SC |= TPM_SC_PS(preescaler);    // prescaler  
	TPM2_MOD = TPM_MOD_MOD(Mod);  //  modulo value 
	TPM2_SC |= TPM_SC_TOF_MASK; //  clear TOF 
	TPM2_BASE_PTR->CONTROLS[0].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK; // SELECT CHANNEL MODE
	TPM2_BASE_PTR->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK; // SELECT CHANNEL MODE
	TPM2_SC |= TPM_SC_CMOD(1);  //enable timer free-running mode 
}


/**
 * grados should be an int ranged from 0 - 180
 */
void servoA(int grados){
	TPM2_BASE_PTR->CONTROLS[0].CnV = gradosToCnV(grados);
}

/**
 * grados should be an int ranged from 0 - 180
 */
void servoB(int grados){
	TPM2_BASE_PTR->CONTROLS[1].CnV =  gradosToCnV(grados);
}	

/**
 * grados should be an int ranged from 0 - 180
 */
void servoC(int grados){
	TPM0_BASE_PTR->CONTROLS[4].CnV = gradosToCnV(grados);
}
	
/** 
 * Convierte un valor de grados entre 0 180 a un valor adecuado para el valor de CnV.
 */
int gradosToCnV(int grados){
	return (int)(grados); //TODO FORMULA CORRECTA
}

