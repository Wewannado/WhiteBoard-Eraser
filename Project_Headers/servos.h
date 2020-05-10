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

void clockConfig();
void pinConfig();

void tpm0Config();

void tpm2Config();


/**
 * grados should be an int ranged from 0 - 180
 */
void servoA(int grados);

/**
 * grados should be an int ranged from 0 - 180
 */
void servoB(int grados);	

/**
 * grados should be an int ranged from 0 - 180
 */
void servoC(int grados);
	
/** 
 * Convierte un valor de grados entre 0 180 a un valor adecuado para el valor de CnV.
 */
int gradosToCnV(int grados);
