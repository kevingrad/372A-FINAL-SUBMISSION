

#include <Arduino.h>
#include <avr/io.h>
#include <SPI.h>
#include "timer.h"
#include "lcd.h"
#include "pwm.h"
#include "led.h"

/* Include the RFID library */
//make sure to go to libraries and add this manually
//search MFRC522
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
    // initTimer4();
    initTimer1();
    initTimer0();
    initLCD();
    initPWM();
    initLED();
    // initPWMTimer3();
    // change_frequency(50);

    turnBlue();
    moveCursor(0, 0); // moves the cursor to 0,0 position
    writeString(" Scan RFID");

    // put your setup code here, to run once:
    Serial.begin(9600);
    /* Enable the SPI interface */
    SPI.begin();
 
    /* Initialise the RFID reader */
    mfrc522.PCD_Init();


    DDRC |= (1 << PORTC1);
    
    sei();
  

    // Serial.println("RFID reading UID");

    while (1)
    {
        
        // Turn(500);
        // _delay_ms(2000);
        // Turn(1000);
        // _delay_ms(2000);

        //set values for the fob/card we're using
        int fobBytes[4] = {236, 51, 39, 35};
        int cardBytes[4] = {201, 168, 165, 185};

    
        if (mfrc522.PICC_IsNewCardPresent())
        {   
            PORTC |= (1 << PORTC1);
            //reads card/fob serial num.
            mfrc522.PICC_ReadCardSerial();
            // Serial.println("Card detected:");

            //serial num. is stored as list, so must be iterated and printed
            for (int i = 0; i < mfrc522.uid.size; i++)
            {
                //Serial.print(mfrc522.uid.uidByte[i]);
                //Serial.print(mfrc522.uid.uidByte[i], HEX);
                //Serial.print(RC522.serNum[i],HEX); //to print card detail in Hexa Decimal format

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
                    
                    // _delay_ms(2000); 
                    // Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
                    // Serial.print(mfrc522.uid.uidByte[i], DEC);
                    
                }
                
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
                    // Serial.print("Card Not Accepted");
                    
                    break;

                }

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

            // Serial.println();
            // Serial.println();
            mfrc522.PICC_HaltA();

            //Serial.print(sum);

            
        }

    
    }


 }








