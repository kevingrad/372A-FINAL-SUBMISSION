#include <avr/io.h>
#include <util/delay.h>
#include "led.h"

void initLED(){ //LED order red ground green blue
    // PIN HEADS 34 AND 35 and 36 
       DDRC |= (1 << PORTC1) | (1 << PORTC2) | (1 << PORTC3);
       //PORTC |= (1 << PORTC2) | (1 << PORTC3);
       
}
void turnRed(){ //invalid fob RED
    PORTC |= (1 << PORTC2); //pin head 35 
    PORTC &= ~(1 << PORTC3);
    PORTC &= ~(1 << PORTC1);
}

void turnGreen(){ //valid fob, door unlocked GREEN
    PORTC |= (1 << PORTC3); //pin head 34 
    PORTC &= ~(1 << PORTC2);
    PORTC &= ~(1 << PORTC1);
}

void turnBlue(){ //door locked, scan fob BLUE
    PORTC |= (1 << PORTC1); //pin head 36 
    PORTC &= ~(1 << PORTC2);
    PORTC &= ~(1 << PORTC3);
}
