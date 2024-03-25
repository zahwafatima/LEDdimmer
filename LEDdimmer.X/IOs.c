/*
 * File:   IOs.c
 * Authors: zahwafatima
 *
 * Created on December 1, 2023, 4:00PM
*/


#include "xc.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "IOs.h"
#include "TimeDelay.h"
#include "ChangeClk.h"
#include "UART2.h"


void IOinit() {
    TRISBbits.TRISB8 = 0;   // sets LED RB8-GPIO12 as output
    LATBbits.LATB8 = 0; // Make GPIO RB8 as a digital output
    
    TRISAbits.TRISA2 = 1;   // sets PB1 RA2-GPIO7 as input (1 sec interval)
    CNPU2bits.CN30PUE = 1;  // Enables pull up resistor on RA2/CN30
    CNEN2bits.CN30IE = 1;   // Enabling CN on RA2/CN30
    
    TRISAbits.TRISA4 = 1;   // sets PB2 RA4-GPIO10 as input (2 sec interval)
    CNPU1bits.CN0PUE = 1;   // Enables pull up resistor on RA4/CN0
    CNEN1bits.CN0IE = 1;    // Enabling CN on RA4/CN0
    
//    TRISBbits.TRISB4 = 1;   // sets PB3 RB4-GPIO9 as input (3 sec interval)
//    CNPU1bits.CN1PUE = 1;   // Enables pull up resistor on RB4/CN1
//    CNEN1bits.CN1IE = 1;    // Enabling CN on RB4/CN1 
    
    //Clock output on REFO pin - pin 18 RB15
    TRISBbits.TRISB15 = 0;  // Set RB15 as output for REFO
    REFOCONbits.ROEN = 1; // Ref oscillator is enabled
    REFOCONbits.ROSSLP = 0; // Ref oscillator is disabled in sleep
    REFOCONbits.ROSEL = 0; // Output base clk showing clock switching
    REFOCONbits.RODIV = 0b0000;
     
    //CN Interrupt settings
    IPC4bits.CNIP = 7; //7 is highest priority, 0 is disabled interrupt
    IFS1bits.CNIF = 0; // Clear interrupt flag
    IEC1bits.CNIE = 1; //Enable CN interrupts
    
    AD1PCFG = 0xFFFF;  // Turn all analog pins to digital
    TRISAbits.TRISA3 = 1;
    
    IPC3bits.AD1IP = 6;
    IFS0bits.AD1IF = 0;
    IEC0bits.AD1IE = 1;
    
    InitUART2();
    
    return;
}

