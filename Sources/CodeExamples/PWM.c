#include <MKL25Z4.h>

int direccion=1;
int brithgness=0;
static int Mod=625;
static int preescaler=6;


void clockConfig();
void changeBrightness(int brightness);
void tpm1Config();
void pinConfig();


int main()
{
	clockConfig();
    tpm1Config(0);
    pinConfig();
   
    while(1)
    {  
    	int i=0;
    		for (i=0;i<50000; i++){;}
    		changeBrightness(brithgness);
    		if(direccion){
    			brithgness++;
    		}
    		else{
    			brithgness--;
    		}
    		if(brithgness==100){
    			direccion=0;
    		}
    		else if(brithgness==0){
    			direccion=1;
    		}
    }
    return 0;
}

void clockConfig()
{
	MCG_C1 = MCG_C1_IREFS_MASK | MCG_C1_IRCLKEN_MASK; // INTERNAL CLOCK|MCGIRCLK ACTIVE(SET)
	MCG_C2 = MCG_C2_IRCS_MASK;                        // SELECT FAST INTERNAL REFERENCE CLOCK (1)
	SIM_SOPT2 |= SIM_SOPT2_TPMSRC(3);                 // MCGIRCLK IS SELECTED FOR TPM CLOCK
	SIM_BASE_PTR->SCGC5 |= SIM_SCGC5_PORTA_MASK; //enable clock
}
void pinConfig(){
	PORTA_BASE_PTR->PCR[12] = PORT_PCR_MUX(3);  //  especificar multiplexacio del TPM1_CH0
}

void tpm1Config()
{
	SIM_SCGC6 |= SIM_SCGC6_TPM1_MASK; //  enable clock to TPM1 
	TPM1_SC  = 0;              // disable timer while configuring
	TPM1_SC |= TPM_SC_PS(preescaler);    // prescaler  
	TPM1_MOD = TPM_MOD_MOD(Mod);  //  modulo value 
	TPM1_SC |= TPM_SC_TOF_MASK; //  clear TOF 
	TPM1_BASE_PTR->CONTROLS[0].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK; // SELECT CHANNEL MODE
	TPM1_SC |= TPM_SC_CMOD(1);  //enable timer free-running mode 
}
/**
 * brightness should be an int ranged from 0 - 100
 */
void changeBrightness(int brightness){
	TPM1_BASE_PTR->CONTROLS[0].CnV = (int)(Mod*brightness/100);
}
