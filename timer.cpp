#include <avr/io.h>
#include "timer.h"

// void initTimer4(){
//     //initialize timer CTC
//     TCCR4A &= ~(1 << WGM40);
//     TCCR4A &= ~( 1<< WGM41);
//     TCCR4B |= ( 1 << WGM42);
//     TCCR4B &= ~( 1 << WGM43);

//     //SET THE PRESCALAR TO 1
//     TCCR4B &= ~((1 << CS42) | (1 << CS41));
//     TCCR4B |= (1 << CS40);

//     //OCR3A = (1us * 16MHZ)/ 1 = 16
    

//     ////set output compare register
//     OCR4A = 16;
// }


// /* This delays the program an amount specified by unsigned int delay.
// * Use timer 0. Keep in mind that you need to choose your prescalar wisely
// * such that your timer is precise to 1 millisecond and can be used for
// * 100-2000 milliseconds
// */
// void delayUs(unsigned int delay){
//     unsigned int count = 0;
//        TCNT3 = 0; //initialize counter increment to start at 0
//        TIFR3 |= (1 << OCF3A); //set the output compare flag down by writing logic 1

//     while(count < delay){ //While the countdelay is less than delay, keep delaying 
//         if(TIFR3 & (1 << OCF3A)){ //increment timer only when the timer is set ^^^^ which we do above
//             count++;
//             TIFR3 |= (1 << OCF3A); // This restarts the timer to 0 before going back to the if statement above

//         }
//     }
// }





////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





void initTimer1(){
    //initialize timer CTC
    TCCR1A &= ~(1 << WGM10);
    TCCR1A &= ~( 1<< WGM11);
    TCCR1B |= ( 1 << WGM12);
    TCCR1B &= ~( 1 << WGM13);

    //SET THE PRESCALAR TO 1
    TCCR1B &= ~((1 << CS12) | (1 << CS11));
    TCCR1B |= (1 << CS10);

    //OCR1A = (1us * 16MHZ)/ 1 = 16
    

    ////set output compare register
    OCR1A = 16;
}

void delayUs(unsigned int delay){
    unsigned int countDelay = 0;
    TCNT1 = 0; //initializes counter to start at count 0
    TIFR1 |= (1 << OCF1A); //set the output compare flag down by writing logic 1 

    while(countDelay < delay){ //While the countdelay is less than delay, keep delaying 
        if(TIFR1 & (1 << OCF1A)){ //increment timer only when the timer is set ^^^^ which we do above
            countDelay++;
            TIFR1 |= (1 << OCF1A); // This restarts the timer to 0 before going back to the if statement above
        }
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



// void initTimer1(){
//     //initialize timer CTC
//     TCCR1A &= ~(1 << WGM10);
//     TCCR1A &= ~( 1<< WGM11);
//     TCCR1B |= ( 1 << WGM12);
//     TCCR1B &= ~( 1 << WGM13);

//     //SET THE PRESCALAR TO 1024
//     TCCR1B |= ((1 << CS12) | (1 << CS10));
//     TCCR1B &= ~(1 << CS11);

//     //OCR1A = (1sec * 16MHZ)/ 1024 = 16625


//     ////set output compare register
//     OCR1A = 16625;
// }

// void delayS(unsigned int delay){
//     unsigned int countDelay = 0;
//     TCNT1 = 0; //initializes counter to start at count 0
//     TIFR1 |= (1 << OCF1A); //set the output compare flag down by writing logic 1 

//     while(countDelay < delay){ //While the countdelay is less than delay, keep delaying 
//         if(TIFR1 & (1 << OCF1A)){ //increment timer only when the timer is set ^^^^ which we do above
//             countDelay++;
//             TIFR1 |= (1 << OCF1A); // This restarts the timer to 0 before going back to the if statement above
//         }
//     }
// }





////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






void initTimer0(){
    TCCR0A &= ~(1 << WGM00);
    TCCR0A |= (1 << WGM01);
    TCCR0B &= ~(1 << WGM02);

    //SET PRESCALAR TO 64
    TCCR0B &= ~(1 << CS02);
    TCCR0B |= (1 << CS00) | (1 << CS01);

    //COUNT = 1ms * 16MHZ)/ 64 = 250

    //SET OUTPUT COMPARE REGISTER
    OCR0A = 250;
}


/* This delays the program an amount specified by unsigned int delay.
* Use timer 0. Keep in mind that you need to choose your prescalar wisely
* such that your timer is precise to 1 millisecond and can be used for
* 100-2000 milliseconds
*/
void delayMs(unsigned int delay){
    unsigned int count = 0;
       TCNT0 = 0; //initialize counter increment to start at 0
       TIFR0 |= (1 << OCF0A); //set the output compare flag down by writing logic 1

    while(count < delay){ //While the countdelay is less than delay, keep delaying 
        if(TIFR0 & (1 << OCF0A)){ //increment timer only when the timer is set ^^^^ which we do above
            count++;
            TIFR0 |= (1 << OCF0A); // This restarts the timer to 0 before going back to the if statement above

        }
    }
}
