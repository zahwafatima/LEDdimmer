/*
 * File:   main.c
 * Author: zahwafatima
 *
 * Created on September 25, 2023, 3:46 PM
 */

// LINKED .h SOURCE FILES HERE
#include "xc.h"
#include <p24fxxxx.h>
#include <p24F16KA101.h>
#include <math.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "ADC.h"
#include "ChangeClk.h"
#include "TimeDelay.h"
#include "IOs.h"
#include "UART2.h"

// Code protection 
#pragma config BSS = OFF // Boot segment code protect disabled
#pragma config BWRP = OFF // Boot sengment flash write protection off
#pragma config GCP = OFF // general segment code protecion off
#pragma config GWRP = OFF

// CLOCK CONTROL 
#pragma config IESO = OFF    // 2 Speed Startup disabled
#pragma config FNOSC = FRC  // Start up CLK = 8 MHz
#pragma config FCKSM = CSECMD // Clock switching is enabled, clock monitor disabled
#pragma config SOSCSEL = SOSCLP // Secondary oscillator for Low Power Operation
#pragma config POSCFREQ = MS  //Primary Oscillator/External clk freq betwn 100kHz and 8 MHz. Options: LS, MS, HS
#pragma config OSCIOFNC = ON  //CLKO output disabled on pin 8, use as IO. 
#pragma config POSCMOD = NONE  // Primary oscillator mode is disabled

// WDT
#pragma config FWDTEN = OFF // WDT is off
#pragma config WINDIS = OFF // STANDARD WDT/. Applicable if WDT is on
#pragma config FWPSA = PR32 // WDT is selected uses prescaler of 32
#pragma config WDTPS = PS1 // WDT postscler is 1 if WDT selected

//MCLR/RA5 CONTROL
#pragma config MCLRE = ON // RA5 pin configured as input, MCLR reset on RA5 diabled

//BOR  - FPOR Register
#pragma config BORV = LPBOR // LPBOR value=2V is BOR enabled
#pragma config BOREN = BOR0 // BOR controlled using SBOREN bit
#pragma config PWRTEN = OFF // Powerup timer disabled
#pragma config I2C1SEL = PRI // Default location for SCL1/SDA1 pin

////JTAG FICD Register
#pragma config BKBUG = OFF // Background Debugger functions disabled
#pragma config ICS = PGx2 // PGC2 (pin2) & PGD2 (pin3) are used to connect PICKIT3 debugger

//// Deep Sleep RTCC WDT
#pragma config DSWDTEN = OFF // Deep Sleep WDT is disabled
#pragma config DSBOREN = OFF // Deep Sleep BOR is disabled
#pragma config RTCOSC = LPRC// RTCC uses LPRC 32kHz for clock
#pragma config DSWDTOSC = LPRC // DeepSleep WDT uses Lo Power RC clk
#pragma config DSWDTPS = DSWDTPS7 // DSWDT postscaler set to 32768 

//// MACROS
#define Nop() {__asm__ volatile ("nop");}
#define ClrWdt() {__asm__ volatile ("clrwdt");}
#define Sleep() {__asm__ volatile ("pwrsav #0");}   // set sleep mode
#define Idle() {__asm__ volatile ("pwrsav #1");}
#define dsen() {__asm__ volatile ("BSET DSCON, #15");}

#define OFF_STATE 0
#define ON_STATE 1
#define BLINK_STATE 2
#define LED_ON LATBbits.LATB8 = 1
#define LED_OFF LATBbits.LATB8 = 0
#define LED_TOGGLE LATBbits.LATB8 ^= 1

//// GLOBAL VARIABLES
volatile int CN30flag = 0;     // indicates change of notification 
volatile int CN0flag = 0; 

uint8_t state;
bool butt1Pressed = false;
bool butt2Pressed = false;

int main(void) {
    // Change Clock
    NewClk(8); // 8 for 8 MHz; 500 for 500 kHz; 32 for 32 kHz
     
    // Initialize IOs for low-power wake-up
    AD1PCFG = 0xFFFF; // Turn all analog pins as digital
    IOinit();  // enables IO and CN interrupts on Push buttons

    while (1) {
        switch(state) {
            case OFF_STATE:
                if (CN30flag && !CN0flag && !butt1Pressed){ //PB1 is pressed 1st time 
                    butt1Pressed = true; //PB1 has been pressed once
                    state = ON_STATE;
                } else if (CN0flag && !CN30flag && !butt2Pressed) { //PB2 is pressed 1st time
                    butt2Pressed = true; //PB2 has been pressed once
                    state = BLINK_STATE;
                } else {
                    LED_OFF;
                    Idle(); // program in low-power mode until new updates on the display
                }
                break;

            case ON_STATE:
                while (state == ON_STATE) {
                    if ((CN30flag && butt1Pressed) || (CN0flag && butt2Pressed)) {
                        butt1Pressed = false;
                        butt2Pressed = false;
                        state = OFF_STATE;
                    } else if (CN0flag && !CN30flag && !butt2Pressed) {
                        butt2Pressed = true;
                        state = BLINK_STATE;
                    } else {
                        uint16_t ADCnum = do_ADC();
                        float dutyCycle = ADCnum * (100.0/1023.0);
                        int pulsePeriod = 10000;

                        if (dutyCycle > 0 && dutyCycle < 95){
                            LED_ON;
                            delay_us((unsigned int)((pulsePeriod * (dutyCycle/100.0))));
                            LED_OFF;
                            delay_us((unsigned int)((pulsePeriod * (2000 - (dutyCycle/100.0)))));
                            if ((CN30flag && butt1Pressed) || (CN0flag && butt2Pressed)) {
                                butt1Pressed = false;
                                butt2Pressed = false;
                                state = OFF_STATE;
                            } else if (CN0flag && !CN30flag && !butt2Pressed) {
                                butt2Pressed = true;
                                state = BLINK_STATE;
                            }
                        } else if (dutyCycle <= 0){
                            LED_OFF;
                            if ((CN30flag && butt1Pressed) || (CN0flag && butt2Pressed)) {
                                butt1Pressed = false;
                                butt2Pressed = false;
                                state = OFF_STATE;
                            } else if (CN0flag && !CN30flag && !butt2Pressed) {
                                butt2Pressed = true;
                                state = BLINK_STATE;
                            }
                        } else if (dutyCycle >= 95){
                            LED_ON;
                            if ((CN30flag && butt1Pressed) || (CN0flag && butt2Pressed)) {
                                butt1Pressed = false;
                                butt2Pressed = false;
                                state = OFF_STATE;
                            } else if (CN0flag && !CN30flag && !butt2Pressed) {
                                butt2Pressed = true;
                                state = BLINK_STATE;
                            }
                        }
                        Disp2Dec((uint16_t)dutyCycle);
                        XmitUART2('\n',1);
                    }
                }
//                break;
                
            case BLINK_STATE:
                while (state == BLINK_STATE) {
                    if ( (CN30flag && butt1Pressed) || (CN0flag && butt2Pressed) ){ //if PB1 pressed second time or PB2 pressed second time
                        butt1Pressed = false; //reset the PB1 boolean
                        butt2Pressed = false; //reset the PB2 boolean
                        LED_OFF;
                        state = OFF_STATE; //go to off mode
                    }
                    else if (CN30flag && !CN0flag && !butt1Pressed){ //if PB1 pressed 
                        butt1Pressed = true; //PB1 has been pressed once 
                        LED_OFF;
                        state = ON_STATE; //go to on mode
                    } else {
//                        NewClk(8);
//                        Disp2String("\r BLINKING MODE");
//                        NewClk(32);
                        
                        LED_TOGGLE;
//                        NewClk(8);
                        Disp2Dec((uint16_t)100.0);
                        XmitUART2('\n',1);
                        NewClk(32);
                        delay_ms(400);
                        NewClk(8);
                        if ( (CN30flag && butt1Pressed) || (CN0flag && butt2Pressed) ){ //if PB1 pressed second time or PB2 pressed second time
                            butt1Pressed = false; //reset the PB1 boolean
                            butt2Pressed = false; //reset the PB2 boolean
                            LED_OFF;
                            state = OFF_STATE; //go to off mode
                        }
                        else if (CN30flag && !CN0flag && !butt1Pressed){ //if PB1 pressed 
                            butt1Pressed = true; //PB1 has been pressed once 
                            LED_OFF;
                            state = ON_STATE; //go to on mode
                        }
                        
                        LED_TOGGLE;
//                        NewClk(8);
                        Disp2Dec((uint16_t)100.0);
                        XmitUART2('\n',1);
                        NewClk(32);
                        delay_ms(400);
                        NewClk(8);
                        if ( (CN30flag && butt1Pressed) || (CN0flag && butt2Pressed) ){ //if PB1 pressed second time or PB2 pressed second time
                            butt1Pressed = false; //reset the PB1 boolean
                            butt2Pressed = false; //reset the PB2 boolean
                            LED_OFF;
                            state = OFF_STATE; //go to off mode
                        }
                        else if (CN30flag && !CN0flag && !butt1Pressed){ //if PB1 pressed 
                            butt1Pressed = true; //PB1 has been pressed once 
                            LED_OFF;
                            state = ON_STATE; //go to on mode
                        }
                    }
                }
                break;  
        }    
    }
    return 0;
}

void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void){
    if(IFS1bits.CNIF == 1) {
        if (PORTAbits.RA2 == 0) {           // While PB1 is pressed
            CN30flag = 1;
            CN0flag = 0; 
        } else if(PORTAbits.RA4 == 0) {       // While PB2 is pressed
            CN30flag = 0;
            CN0flag = 1; 
        } else {                              // No PBs pressed
            CN30flag = 0;
            CN0flag = 0; 
        }
    }
    // Clear Interrupt flag
    IFS1bits.CNIF = 0;
    Nop();
}  



