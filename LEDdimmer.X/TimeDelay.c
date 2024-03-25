/*
 * File:   TimeDelay.c
 * Authors: zahwafatima
 *
 * Created on December 1, 2023, 4:00PM
*/


#include "xc.h"
#include "TimeDelay.h"
#include "IOs.h"
#include "ChangeClk.h"

void delay_ms(uint16_t time_ms) {
    // T2CON config
    T2CONbits.TSIDL = 0;            // operate in idle mode
    T2CONbits.T32 = 0;              // operate timer 2 as 16 bit timer
    T2CONbits.TCS = 0;              // use internal clock
    T2CONbits.TCKPS = 0x000;       // prescaler = 1
    
    // Timer 2 interrupt config
    IPC1bits.T2IP = 4;      // 7 is highest priority,1 is lowest
    IEC0bits.T2IE = 1;      // Enable Timer2 interrupt
    IFS0bits.T2IF = 0;      // clear Timer2 flag
    
    TMR2 = 0;
    PR2 = time_ms * 16;     //After PR2 simplification
    T2CONbits.TON = 1;      //start timer 2
    Idle(); 
    T2CONbits.TON = 0;      // stop timer 2
    return;
}

void delay_us(uint16_t time_ms) {
    // T2CON config
    T2CONbits.TSIDL = 0;            // operate in idle mode
    T2CONbits.T32 = 0;              // operate timer 2 as 16 bit timer
    T2CONbits.TCS = 0;              // use internal clock
    T2CONbits.TCKPS = 0x000;       // prescaler = 1
    
    // Timer 2 interrupt config
    IPC1bits.T2IP = 4;      // 7 is highest priority,1 is lowest
    IEC0bits.T2IE = 1;      // Enable Timer2 interrupt
    IFS0bits.T2IF = 0;      // clear Timer2 flag
    
    TMR2 = 0;
    PR2 = time_ms * 4;     //After PR2 simplification
    T2CONbits.TON = 1;      //start timer 2
    Idle(); 
    T2CONbits.TON = 0;      // stop timer 2
    return;
}

void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void)
{
     IFS0bits.T2IF=0; //Clear timer 2 interrupt flag
     T2CONbits.TON = 0;      // Stop timer
//     TMR2 = 0;               // Clear the timer2 count
//     TMR2flag = 1; // optional global variable created by user
    return;
}



