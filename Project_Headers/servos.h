/*
 * servos.c
 *
 *  Created on: May 3, 2020
 *      Author: Roger Gili Coscojuela
 */

#include <MKL25Z4.h>
#include <common.h>

//module and preescaler for 50hz



void servosInitialPosition();
void servosCalibrate();
void servosCenter();
void escribirHoraEnPizarra(int HH, int MM);
void borrarPizarra();


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
