/*
 * LED WRISTWATCH
 * File:   main.c
 * Author: alexchitic
 *
 * Created on September 18, 2026, 2:08 AM
 * 
 * PINS FOR SEVEN SEGMENT
 * PORTD = LED ANODES G-A
 * PORTE = DIGIT, RE0 &RE1
 * 
 */


#include <xc.h>
#include <stdbool.h>

#define _XTAL_FREQ 1000000 

//variables to keep hours and minutes
int hour = 12;
int minutes = 20;
int num = 0;

//button flag
volatile bool buttonPressed = false;

//common cathode display number values
int number[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

void showDisplay(){
    
    //first digit
    LATD = number[num % 10];
    LATEbits.LATE0 = 1;
    LATEbits.LATE1 = 0;
    
    
    //second digit
    LATD = number[(num / 10) % 10];
    LATEbits.LATE0 = 0;
    LATEbits.LATE1 = 1;
    
}

void buttonPress(){
     
    buttonPressed = true;
            
}

void __interrupt() ISR(void){
    
    //interrupt for display
    if(INTCONbits.TMR0IF == 1){
        
        showDisplay();
        INTCONbits.TMR0IF = 0;
        TMR0L = 0x06;
    
    }
    
    //interrupt for button
    if(INTCONbits.INT0IF == 1){
        buttonPress();
        INTCONbits.INT0IF = 0;
    }
    
}

void main(void) {
    
    TRISD = 0x00; //set segments as output
    TRISE = 0x00; // set digits as output
    TRISBbits.TRISB0 = 1; //set button as input
    ADCON1 = 0x0F; //set rb0 as digital input and turn off adc due to pin being multiplexed
    
    // set interrupts to enable
    INTCON = 0b11110000;
    INTCON2 = 0b00000000;
    
    //main loop
    while(1){
        
        if(buttonPressed == true){
            
            //set timer 0 for display multiplexing
            T0CON = 0b11010000;
            TMR0L = 0X06; //1ms display
            
            //show hour and minutes in the span of 1 second
            num = hour;
            __delay_ms(200);
            num = minutes;
            __delay_ms(200);
            
            //reset the flag to turn off display
            buttonPressed = false;
        }
        
        else{
            //turn display off
            T0CON = 0b01010000;
            LATD = 0x00;
            LATE = 0xFF;
        }
    }
    
    return;
}
