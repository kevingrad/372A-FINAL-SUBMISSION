#include "lcd.h"
#include "timer.h"
#include <util/delay.h>
#include <avr/io.h>

void initLCDPins(){
    // pins on lcd
    DDRA |= (1 << DDA0) | (1 << DDA1) | (1 << DDA2) | (1 << DDA3);
    //enable pin on LCD
    DDRB |= (1 << DDB4);
    //RS pin on LCD
    DDRB |= (1 << DDB6);
}


void fourBitCommandWithDelay(unsigned char data, unsigned int delay){
    PORTB &= ~(1 << PORTB6); // SET RS TO LOW
    PORTA = (PORTA & 0xF0) | (data & 0x0F); // SET DATA TO THE BOTTOM FOUR BITS
    PORTB |= (1 << PORTB4); // SET ENABLE PIN TO HIGH
    delayUs(1); // DELAY
    PORTB &= ~(1 << PORTB4); // SET ENABLE PIN TO LOW
    delayUs(delay); //THIS DLEAY ALLOWS FOR A USER TO DEFINE THE NECCASSARY TIME NEEDED TO COMPLETE THE ACTION
}




void eightBitCommandWithDelay(unsigned char command, unsigned int delay){
    PORTB &= ~(1 << PORTB6); // SETS RS TO LOW
    PORTA = (PORTA & 0xF0) | ((command >> 4)); // SETS COMMAND TO THE TOP FOUR BITS
    PORTB |= (1 << PORTB4); // SETS ENABLE PIN TO HIGH
    delayUs(1); // DELAY
    PORTB &= ~(1<< PORTB4); // SETS ENABLE PIN BACK TO LOW
    PORTA = (PORTA & 0xF0) | (command & 0x0F); //SETS COMMAND AS BOTTOM FOUR BITS
    PORTB |= (1 << PORTB4);// SETS ENABLE TO HIGH
    delayUs(1); // DELAY 1MS
    PORTB &= ~(1 << PORTB4); // SETS ENABLE PIN BACK TO LOW 
    delayUs(delay); //THIS DLEAY ALLOWS FOR A USER TO DEFINE THE NECCASSARY TIME NEEDED TO COMPLETE THE ACTION
}


void writeCharacter(unsigned char character){
    PORTB |= (1 << PORTB6); //SET RS TO HIGH
    PORTA = (PORTA & 0xF0) | ((character >> 4)); //CSET CHARACTER TO BOTTOM FOUR BITS 
    PORTB |= (1 << PORTB4); //SET ENABLE TO HIGH
    delayUs(1); //DELAY 1MS
    PORTB &= ~(1 << PORTB4); //SET ENABLE BACK TO 0


    PORTA = (PORTA & 0xF0) | (character & 0x0F); // SET CHARACTER TO LOWER FOUR BITS
    PORTB |= (1 << PORTB4); // ENABLE TO 1
    delayUs(1); // DELAY 1MS
    PORTB &= ~(1 << PORTB4); //SET ENABLE TO  0
    delayUs(46); // DEAL 46US FOR CHARACTER ///Switch back to DELAY
}

void writeString(const char *string) {
    while(*string != '\0') {
        writeCharacter(*string);
        string++;
    }
}


void moveCursor(unsigned char x, unsigned char y){
	unsigned char cursor = (0x80 | (x << 6) | y);
  //0X80 WILL SET DB7 TO 1 IN ORDER TO WRITE
  //X IS LEFT SHIFTED 6 TO SET THE SECOND ROW
  // FIRST ROW X = 0, SECOND ROW X = 1
  //Y ARE THE COLUMNS 
    eightBitCommandWithDelay(cursor, 0);
}



//below runs the series of commands needed to use the LCD in a 4 bit set up 
//these include the commands to turn the display on initialize the display clear the 
//display. It also includes all the minimum delays required to complete said actions which is given within the LCD's
//data sheet

void initLCDProcedure(){
  //Delay 15 milliseconds
    delayMs(15); 
  // Write 0b0011 to DB[7:4] and delay 4100 microseconds
  fourBitCommandWithDelay(3, 4100);
  // Write 0b0011 to DB[7:4] and delay 100 microseconds
    fourBitCommandWithDelay(3, 100);
  // write 0b0011 to DB[7:4] and 100us delay
    eightBitCommandWithDelay(3, 100);
  // write 0b0010 to DB[7:4] and 100us delay.
    eightBitCommandWithDelay(2, 100);
  // Function set in the command table with 53us delay
    eightBitCommandWithDelay(40, 53);
  // Display off in the command table with 53us delay
    eightBitCommandWithDelay(8, 53);
  // Clear display in the command table. 
    eightBitCommandWithDelay(1, 4000);
  // Entry Mode Set in the command table.
    eightBitCommandWithDelay(6, 53);
  // Display ON/OFF Control in the command table. 
    eightBitCommandWithDelay(15, 53);
}


void initLCD(){
  initLCDPins();
  initLCDProcedure();
}
