

#include <Arduino.h>
#include <avr/io.h>
#include <SPI.h>
#include "timer.h"
#include "lcd.h"
#include "pwm.h"
#include "led.h"

// We use the mfrc522 library instead of our own RFID library.
// Add it by going to PlatformIO library tab and searching mfrc522, add to project.
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
    // Initialize all components as we have done before.
    init();
    initTimer1();
    initTimer0();
    initLCD();
    initPWM();
    initLED();
    
    //LED starts in the blue state.
    turnBlue();
    moveCursor(0, 0); // moves the cursor to 0,0 position
    // Default state is locked and displaying the message Scan RFID.
    writeString(" Scan RFID");

    Serial.begin(9600);
    // Enables the SPI interface.
    SPI.begin();
 
    // RFID module initialization.
    mfrc522.PCD_Init();
    DDRC |= (1 << PORTC1);
    
    // Allow global interrupts.
    sei();
  
    while (1)
    {
        // Hardcoded values for the master fob/card
        int fobBytes[4] = {236, 51, 39, 35};
        int cardBytes[4] = {201, 168, 165, 185};

        // This if is only triggered the user presents a key/fob.
        if (mfrc522.PICC_IsNewCardPresent())
        {   
            PORTC |= (1 << PORTC1);
            //reads card/fob serial num.
            mfrc522.PICC_ReadCardSerial();
            // The UID needs to be iterated over 4 times because it contains 4 bytes. 
            for (int i = 0; i < mfrc522.uid.size; i++)
            {
                // If a match is found with the master card, then we turn the LED green,
                // unlock the door, and delay for 10 seconds. After ten seconds, the door locks again.
                if (fobBytes[i] == mfrc522.uid.uidByte[i] || cardBytes[i] == mfrc522.uid.uidByte[i])
                {
                    turnGreen();
                    eightBitCommandWithDelay(1, 4000);
                    moveCursor(0, 0); // moves the cursor to 0,0 position
                    writeString(" The Door Is:");
                    moveCursor(1, 0);
                    writeString(" Unlocked");
                    Turn(500);
                    delayMs(10000);
                    Turn(1000);
                }
                
                // If the presented key card is not valid, then the LED turns green and the LCD
                // displays an "Invalid Key Card" message followed by "Please Try Again" followed
                // by a "Scan RFID Again" message.
                else
                {
                    turnRed();
                    moveCursor(0, 0);
                    writeString(" Invalid Key Card");
                    moveCursor(1, 0);
                    writeString(" Please Try Again");   
                    delayMs(10000);
                    eightBitCommandWithDelay(1, 4000);//this is the clear display function for the LCD
                    moveCursor(0, 0); // moves the cursor to 0,0 position
                    writeString(" Scan RFID Again");
                    turnBlue();              
                    break;

                }
                    // Default state where LED is blue and LCD reads "The Door Is: Locked"
                    i = 0;
                    turnBlue();
                    eightBitCommandWithDelay(1, 4000);
                    moveCursor(0, 0); // moves the cursor to 0,0 position
                    writeString(" The Door Is:");
                    moveCursor(1, 0);
                    writeString(" Locked");
                    delayMs(20000);
                    eightBitCommandWithDelay(1, 4000);
                    moveCursor(0, 0); // moves the cursor to 0,0 position
                    writeString(" Scan RFID");
                    break;
            }
            // Similar to a stop data transfer until the next while(1).
            mfrc522.PICC_HaltA();
        }
    }
 }








