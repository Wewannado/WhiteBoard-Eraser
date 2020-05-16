/*
 * servos.c
 *
 *  Created on: May 3, 2020
 *      Author: Roger Gili Coscojuela
 */

#include <MKL25Z4.h>


//module and preescaler for 50hz
static int TPM_50HZ_MOD=625;
static int TPM_50HZ_PREESCALER=6;
static int SERVO_A_MAXALTURA=50; //more height than this makes the structure hit the servo.
static int SERVO_A_MINALTURA=33;

void clockConfig();
void pinConfig();

void tpm0Config();

void tpm2Config();

void servosInitialPosition();
void escribirHora(int HH, int MM);


/**
 * grados should be an int ranged from 0 - 180
 */
void servoA(int grados);

/**
 * grados should be an int ranged from 0 - 180
 */
void servoI(int grados);	

/**
 * grados should be an int ranged from 0 - 180
 */
void servoD(int grados);
	
/** 
 * Convierte un valor de grados entre 0 180 a un valor adecuado para el valor de CnV.
 */
int gradosToCnV(int grados);

/**
 * 
 */
void servoA_Bajar();
