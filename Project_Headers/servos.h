/**
 * Universitat Autònoma de Barcelona
 * MiP 2019 - 2020
 * PROJECTE WHITEBOARD CLOCK
 * NIUS
 * Roger Gili 1497893
 * Albert Romero 1425095
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
