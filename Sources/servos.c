/*
 * servos.c
 *
 *  Created on: May 3, 2020
 *      Author: Roger Gili Coscojuela
 */

#include <MKL25Z4.h>
#include <servos.h>

#include <fontFile.h>


/**
 * grados should be an int ranged from 0 - 180
 */
void servoI(int grados){
	TPM0_BASE_PTR->CONTROLS[4].CnV = gradosToCnV(grados);
}

/**
 * grados should be an int ranged from 0 - 180
 */
void servoD(int grados){
	TPM0_BASE_PTR->CONTROLS[1].CnV =  gradosToCnV(grados);
}	

/**
 * grados should be an int ranged from 0 - 180
 */
void servoA(int grados){
	TPM0_BASE_PTR->CONTROLS[2].CnV = gradosToCnV(grados);
}
	
/** 
 * Convierte un valor de grados entre 0 180 a un valor adecuado para el valor de CnV.
 */
int gradosToCnV(int grados){
	
	//our servos goes from 9-85 values. General formula y=mx+n -> y = 0,422x+9
	return (int)((0.422*grados)+9); //TODO FORMULA CORRECTA
}
/**
 * 
 */
void servosInitialPosition(){
	servoA(SERVO_A_MAXALTURA);
	delayMs(300);
	servoI(140);
	servoD(120);
	delayMs(300);
}


void servosCenter(){
	//up
	servoA(SERVO_A_MAXALTURA);
	delayMs(300);
	//dar center
	servoI(90);
	servoD(90);
	delayMs(1000);
}
/*
 * Function used to visually check if the arms are correctly mounted. 
 * Goes up, center, far left, far right, down, up, far left, down, up.
 */
void servosCalibrate(){
	servosCenter();
	//far left
	servoI(150);
	servoD(130);
	delayMs(1000);
	//far right
	servoI(50);
	servoD(30);
	delayMs(1000);
	//down
	servoA_Bajar();
	delayMs(1000);
	//up
	servoA(SERVO_A_MAXALTURA);
	delayMs(1000);
	//far left
	servoI(140);
	servoD(120);
	delayMs(300);
	//down
	servoA_Bajar();
	//up
	servoA(SERVO_A_MAXALTURA);
}

void servoA_Bajar(){
	int i= SERVO_A_MAXALTURA;
	while(i!=SERVO_A_MINALTURA){
		TPM0_BASE_PTR->CONTROLS[2].CnV = gradosToCnV(i);
		i--;
		delayMs(30);
	}
	
}
void escribirNumero(int servoIz[], int servoDe[], int steps){
	int i;
	for (i=0; i<steps;i++){
		servoI(servoIz[i]);
		servoD(servoDe[i]);
		if(i==0){
			servoA_Bajar();
		}
		delayMs(2000);
	}
}


void borrar(){
	escribirNumero(CLEAR_WHITEBOARD_SERVO_I,CLEAR_WHITEBOARD_SERVO_D,sizeof(CLEAR_WHITEBOARD_SERVO_I)/sizeof(CLEAR_WHITEBOARD_SERVO_I[0]));
}
void escribirHora(int HH, int MM){
	switch (HH){
	case 0:
		//escribirNumero(HOURS_FIRST_DIGIT_0_SERVO_I,HOURS_FIRST_DIGIT_0_SERVO_D,sizeof(HOURS_FIRST_DIGIT_0_SERVO_I)/sizeof(HOURS_FIRST_DIGIT_0_SERVO_I[0]));
		//escribirNumero(HOURS_FIRST_DIGIT_0_SERVO_I,HOURS_FIRST_DIGIT_0_SERVO_D,sizeof(HOURS_FIRST_DIGIT_0_SERVO_I)/sizeof(HOURS_FIRST_DIGIT_0_SERVO_I[0]));
		break;
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
		break;
	default:
		break;
	}
	escribirNumero(SEPARATOR_SERVO_I,SEPARATOR_SERVO_D,sizeof(SEPARATOR_SERVO_I)/sizeof(SEPARATOR_SERVO_I[0]));
}

