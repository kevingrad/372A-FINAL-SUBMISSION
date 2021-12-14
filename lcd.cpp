#include "lcd.h"
#include "timer.h"
#include <util/delay.h>
#include <avr/io.h>

/*
 * Initializes all pins related to the LCD to be outputs
 */
void initLCDPins(){
    // pins on lcd
    DDRA |= (1 << DDA0) | (1 << DDA1) | (1 << DDA2) | (1 << DDA3);
    //enable pin on LCD
    DDRB |= (1 << DDB4);
    //RS pin on LCD
    DDRB |= (1 << DDB6);
}

/* 1. Assert bits to pins connected to DB[7:4] on the LCD screen
 *      a. data is an unsigned char which has 8 bits. Therefore, you
 *         need assign the bottom 4 bits of "data" to the appropriate bits in
 *         PORTA
 *  2. This is a "command" signal, meaning RS should be Low
 *  3. Assert high on enable pin, delay, and asset low on enable pin
 *  4. delay the provided number in MICROseconds.
 */
void fourBitCommandWithDelay(unsigned char data, unsigned int delay){
    PORTB &= ~(1 << PORTB6); // SET RS TO LOW
    PORTA = (PORTA & 0xF0) | (data & 0x0F); // SET DATA TO THE BOTTOM FOUR BITS
    PORTB |= (1 << PORTB4); // SET ENABLE PIN TO HIGH
    delayUs(1); // DELAY
    PORTB &= ~(1 << PORTB4); // SET ENABLE PIN TO LOW
    delayUs(delay); // DELAY TO ALLOW FOR EXECUTION
}



/* Similar to fourBitCommandWithDelay except that now all eight bits of command are
 * sent.
 * 1. Assert bits to pins connected to DB[7:4] on the LCD screen
 *       a. unlike fourBitCommandWithDelay, you need to send the TOP four bits of
 *          "command" first. These should be assigned to appropriate bits in PORTA
 * 2. This is a command signal, meaning RS should be low
 * 3. Assert high on enable pin, delay, and asset low on enable pin
 * 4. Now set the lower four bits of command to appropriate bits in PORTA
 * 5. Assert high on enable pin, delay, and asset low on enable pin
 * 6. delay the provided number in MICROseconds.
 */
void eightBitCommandWithDelay(unsigned char command, unsigned int delay){
    PORTB &= ~(1 << PORTB6); // SET RS TO LOW
    PORTA = (PORTA & 0xF0) | ((command >> 4)); // SET COMMAND TO THE TOP FOUR BITS
    PORTB |= (1 << PORTB4); // SET ENABLE PIN TO HIGH
    delayUs(1); // DELAY
    PORTB &= ~(1<< PORTB4); // SET ENABLE PIN BACK TO LOW
    PORTA = (PORTA & 0xF0) | (command & 0x0F); //SET COMMAND AS BOTTOM FOUR BITS
    PORTB |= (1 << PORTB4);// SET ENABLE TO HIGH
    delayUs(1); // DELAY 1MS
    PORTB &= ~(1 << PORTB4); // SET ENABLE PIN BACK TO LOW 
    delayUs(delay); //switch back to DELAY
}



/* Similar to eightBitCommandWithDelay except that now RS should be high
 * 1. Assert bits to pins connected to DB[7:4] on the LCD screen
 * 2. This is a "data" signal, meaning RS should be high
 * 3. Assert high on enable pin, delay, and asset low on enable pin
 * 4. Now set the lower four bits of character to appropriate bits in PORTA
 * 5. Assert high on enable pin, delay, and asset low on enable pin
 * 6. delay is always 46 MICROseconds for a character write
 */
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
/*
 * Writes a provided string such as "Hello!" to the LCD screen. You should
 * remember that a c string always ends with the '\0' character and
 * that this should just call writeCharacter multiple times.
 */
void writeString(const char *string) {
    while(*string != '\0') {
        writeCharacter(*string);
        string++;
    }
}

/*
 * This moves the LCD cursor to a specific place on the screen.
 * This can be done using the eightBitCommandWithDelay with correct arguments
 */
void moveCursor(unsigned char x, unsigned char y){
	unsigned char cursor = (0x80 | (x << 6) | y);
  //0X80 WILL SET DB7 TO 1 IN ORDER TO WRITE
  //X IS LEFT SHIFTED 6 TO SET THE SECOND ROW
  // FIRST ROW X = 0, SECOND ROW X = 1
  //Y ARE THE COLUMNS 
    eightBitCommandWithDelay(cursor, 0);
}


/* This is the procedure outline on the LCD datasheet page 4 out of 9.
 * This should be the last function you write as it largely depends on all other
 * functions working.
 */
void initLCDProcedure(){
  //Delay 15 milliseconds
    delayMs(15); 
  // Write 0b0011 to DB[7:4] and delay 4100 microseconds
  fourBitCommandWithDelay(3, 4100);
  // Write 0b0011 to DB[7:4] and delay 100 microseconds
    fourBitCommandWithDelay(3, 100);
  // The data sheet does not make this clear, but at this point you are issuing
  // commands in two sets of four bits. You must delay after each command
  // (which is the second set of four bits) an amount specified on page 3 of
  // the data sheet.
  // write 0b0011 to DB[7:4] and 100us delay
    eightBitCommandWithDelay(3, 100);
  // write 0b0010 to DB[7:4] and 100us delay.
    eightBitCommandWithDelay(2, 100);
  // Function set in the command table with 53us delay
    eightBitCommandWithDelay(40, 53);
  // Display off in the command table with 53us delay
    eightBitCommandWithDelay(8, 53);
  // Clear display in the command table. Remember the delay is longer!!!
    eightBitCommandWithDelay(1, 4000);
  // Entry Mode Set in the command table.
    eightBitCommandWithDelay(6, 53);
  // Display ON/OFF Control in the command table. (Yes, this is not specified),
  // in the data sheet, but you have to do it to get this to work. Yay datasheets!)
    eightBitCommandWithDelay(15, 53);
}



/* Initializes Tri-state for LCD pins and calls initialization procedure.
* This function is made so that it's possible to test initLCDPins separately
* from initLCDProcedure which will likely be necessary.
*/
void initLCD(){
  initLCDPins();
  initLCDProcedure();
}
