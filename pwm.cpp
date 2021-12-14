#include "pwm.h"

void initPWM() {

    DDRE |= (1 << DDE5);

    // MODE 15 ON PWM TIMER4
    TCCR3A |= (1 << WGM30) | (1 << WGM31);
    TCCR3B |= (1 << WGM33) | (1 << WGM32);
    
    // NON-INVERTING MODE WITH TCCR3A = 10
    TCCR3A &= ~(1 << COM3C0);
    TCCR3A |= (1 << COM3C1);
    OCR3A = 5000;
    OCR3C = 0;
    
    // PRESCALER 64
    TCCR3B |= (1 << CS30);
    TCCR3B |= (1 << CS31);
    TCCR3B &= ~(1 << CS32);
}

void Turn(int way){
    // Giving a parameter of 500 turns it to be flat (unlocked).
    if (way == 500) {
        OCR3C = OCR3A * 0.05;
    }
    // Giving a parameter of 1000 turns it to be perpendicular to the ground (locked).
    else if (way == 1000) {
        OCR3C = OCR3A * 0.1;       
    }
        
}
