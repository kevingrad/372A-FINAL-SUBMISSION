#ifndef PWM_H
#define PWM_H
#include <Arduino.h>
#include <avr/io.h>

// Initializes our PWM using timer 3 for the SG90 servo movement.
void initPWM();

// Only two valid parameters, moves the SG90 servo to either the locked or unlocked position.
void Turn(int percent);

#endif
