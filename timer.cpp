#include <avr/io.h>
#include "timer.h"

//initialize Timer 1
void initTimer1()
{
    //initialize timer CTC mode
    TCCR1A &= ~(1 << WGM10);
    TCCR1A &= ~(1 << WGM11);
    TCCR1B |= (1 << WGM12);
    TCCR1B &= ~(1 << WGM13);

    //SET THE PRESCALAR TO 1
    TCCR1B &= ~((1 << CS12) | (1 << CS11));
    TCCR1B |= (1 << CS10);

    //OCR1A = (1us * 16MHZ)/ 1 = 16
    //This is the formula used to determine our top or ORC1A value

    //set output compare register to be the top value we count up to
    OCR1A = 16;
}

//this function creates a delay of 1 microsecond, multiplied by the parameter used to call function
void delayUs(unsigned int delay)
{
    unsigned int countDelay = 0;
    TCNT1 = 0;             //initializes counter to start at count 0
    TIFR1 |= (1 << OCF1A); //sets the output compare flag down by writing logic 1

    while (countDelay < delay)
    { //While the countdelay is less than delay, keep delaying
        if (TIFR1 & (1 << OCF1A))
        { //increment timer only when the timer is set ^^^^ which we do above
            countDelay++;
            TIFR1 |= (1 << OCF1A); // This restarts the timer to back to count at 0 before going back to the if statement above
        }
    }
}

//intialize Timer 0
void initTimer0()
{
    TCCR0A &= ~(1 << WGM00);
    TCCR0A |= (1 << WGM01);
    TCCR0B &= ~(1 << WGM02);

    //SET PRESCALAR TO 64
    TCCR0B &= ~(1 << CS02);
    TCCR0B |= (1 << CS00) | (1 << CS01);

    //COUNT = 1ms * 16MHZ)/ 64 = 250
    //This is the formula used to determine our top or ORC0A value

    //set output compare register to be the top value we count up to
    OCR0A = 250;
}

//this function creates a delay of 1 millisecond, multiplied by the parameter used to call function
void delayMs(unsigned int delay)
{
    unsigned int count = 0;
    TCNT0 = 0;             //initializes counter increment to start at 0
    TIFR0 |= (1 << OCF0A); //set the output compare flag down by writing logic 1

    while (count < delay)
    { //While the countdelay is less than delay, keep delaying
        if (TIFR0 & (1 << OCF0A))
        { //increment timer only when the timer is set ^^^^ which we do above
            count++;
            TIFR0 |= (1 << OCF0A); // This restarts the timer to back to count at 0 before going back to the if statement above
        }
    }
}
