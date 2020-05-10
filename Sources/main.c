/**
 * Universitat Autonoma de Barcelona
 * MiP 2019 - 2020
 * NIUS
 * Roger Gili 1497893
 * Albert Romero 
 */

#include <MKL25Z4.h>
#include "servos.h"



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
    		for (grados=8; grados<85; ){
				for (i=0;i<500000; i++){;}
				servoA(grados);
				servoB(grados);
				servoC(grados);
				grados=grados+1;
    		}
    		//for (i=0;i<50000000; i++){;}
    		//servoA(11);
    		//for (i=0;i<10000000; i++){;}
    		//servoA(83);
    		//for (i=0;i<10000000; i++){;}
    		
    }
    return 0;
}



