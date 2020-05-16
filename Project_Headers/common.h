/*
 * common.h
 *
 *  Created on: May 16, 2020
 *      Author: Diddi
 */

#include <MKL25Z4.h>
#include <string.h>

void LED_init(void);
void delayMs(int n);
void LED_set(char mode);
int ValidateTime(int hh , int mm);
