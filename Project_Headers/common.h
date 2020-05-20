/**
 * Universitat Autònoma de Barcelona
 * MiP 2019 - 2020
 * PROJECTE WHITEBOARD CLOCK
 * NIUS
 * Roger Gili 1497893
 * Albert Romero 1425095
 */


#include <MKL25Z4.h>
#include <string.h>

extern int TPM_50HZ_MOD;
extern int TPM_50HZ_PREESCALER;
extern int SERVO_A_MAXALTURA; //more height than this makes the structure hit the servo.
extern int SERVO_A_MINALTURA;

void clockConfig();
void pinConfig();
void tpm0Config();
void tpm1Config();
void LED_init(void);
void delayMs(int n);
void LED_toogle(char mode);
void LED_shutoff(char mode);
int ValidateTime(int hh , int mm);
