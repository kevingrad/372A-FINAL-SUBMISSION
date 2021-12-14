//Kevin Grady, Cameron Sexton, Kathryn Hoang, Sheehab Zaman
//Final Project
//ECE372A
#include <Arduino.h>
#include <avr/io.h>
#include <SPI.h>
#include "timer.h"
#include "lcd.h"
#include "pwm.h"
#include "led.h"

/* Include the RFID library */
//make sure to go to libraries and add this manually
//search MFRC522 in PlatformIO libraries, and add the library that says MFRC522 by Miguel André Balboa
#include <mfrc522.h>

/* Define the DIO used for the SDA (SS) and RST (reset) pins. */
//Pin 53
#define SDA_DIO 53
//Pin 49
#define RESET_DIO 49
/* Create an instance of the RFID library */
MFRC522 mfrc522(SDA_DIO, RESET_DIO);

int main()
{
    init();
    initTimer1();
    initTimer0();
    initLCD();
    initPWM();
    initLED();

    // this is the inital state of the project
    // so the LED light starts with blue to indicate lock
    // the cursor is set to it's orgiginal position and the
    // LCD indicates to the user to scan their RFID

    turnBlue();
    moveCursor(0, 0); // moves the cursor to 0,0 position
    writeString(" Scan RFID");

    // put your setup code here, to run once:
    Serial.begin(9600);
    /* Enable the SPI interface */
    SPI.begin();

    /* Initialise the RFID reader */
    mfrc522.PCD_Init();

    //sets the port C1 to output
    DDRC |= (1 << PORTC1);

    sei();

    while (1)
    {

        //set bytes we defined for the fob/card we're using
        //if a card or fob does not match this specific order, the RFID will signal an invalid card and start the corresponding proccesses
        int fobBytes[4] = {236, 51, 39, 35};
        int cardBytes[4] = {201, 168, 165, 185};

        //checks if there is a card/fob being scanned near RFID
        if (mfrc522.PICC_IsNewCardPresent())
        {
            PORTC |= (1 << PORTC1);
            //reads card/fob serial num.
            mfrc522.PICC_ReadCardSerial();

            //serial num. is stored as list, so must be iterated and printed
            for (int i = 0; i < mfrc522.uid.size; i++)
            {

                if (fobBytes[i] == mfrc522.uid.uidByte[i] || cardBytes[i] == mfrc522.uid.uidByte[i]) // checks if the bytes of the RFID card or fob scanned match the master
                {
                    turnGreen(); //this turns on the green LED to indicate correct RFID and signal the door is unclocked
                    eightBitCommandWithDelay(1, 4000);
                    moveCursor(0, 0); // moves the cursor to 0,0 position
                    writeString(" The Door Is:");
                    moveCursor(1, 0); //moves cursor to bottom row
                    writeString(" Unlocked");
                    Turn(500);      //calls turn function, the 500 indicated to change the duty cycle to turn to the unlocked position
                    delayMs(10000); //delays 10 seconds
                    Turn(1000);     //calls turn function, the 1000 indicated to change the duty cycle to turn to the unlocked position
                }

                else // if the scanned RFID card or fob bytes do not match the master
                {
                    turnRed(); //turns on red LED light to signify to the user the card scanned is not a valid one
                    moveCursor(0, 0);
                    writeString(" Invalid Key Card");
                    moveCursor(1, 0);
                    writeString(" Please Try Again");
                    delayMs(10000);                    //delays 10 sec, this also gives the user enough time to try and scan their card again
                    eightBitCommandWithDelay(1, 4000); //this is the clear display function for the LCD, this is used by using the predesing functions within the LCD
                    moveCursor(0, 0);                  // moves the cursor to 0,0 position
                    writeString(" Scan RFID Again");
                    turnBlue(); //goes back to locked indication light which is blue

                    break; //this is used to stop the program from running 4 times because of the for loop if a valid card is scanned
                }

                // the follow code resets whatever process happened before,
                // whether it was the incorrect or correct card scanned, back
                // to the initial state we want in this case to indicate a locked door
                // and to ask the user to scan their card or fob

                i = 0;
                turnBlue();
                eightBitCommandWithDelay(1, 4000);
                moveCursor(0, 0); // moves the cursor to 0,0 position
                writeString(" The Door Is:");
                moveCursor(1, 0);
                writeString(" Locked");
                delayMs(20000);                    //will stay saying locked with the light on to let the user know and have time to see that the door is in fact now locked.
                eightBitCommandWithDelay(1, 4000); //clears display
                moveCursor(0, 0);                  // moves the cursor to 0,0 position
                writeString(" Scan RFID");
                break;
            }

            mfrc522.PICC_HaltA(); //used to stop RFID, which prevents it from trying to scan a card too many times
        }
    }
}
