#ifndef PWM_H
#define PWM_H
#include <Arduino.h>
#include <avr/io.h>

void initPWM();
void Turn(int percent);

#endif
