#include <xc.h>
#include <stdint.h>
#include "config.h"
#define _XTAL_FREQ 4000000

void PMW1SetDuty(uint16_t DC){
    //check the DC value to make sure it's within the 10-bit range
    if(DC < 1024){
    CCP1Y = DC & 1;
    CCP1X = DC & 2;
    CCPR1L = DC >> 2;// this is right shift which means remove the last 2 numbers from the right
    //for example: 10011 >> 2 = 100
    //there is also left shift which means add 2 zeros from the right
    //for example : 10011 << 2 = 1001100
    
    //we are doing right shift because CCPR1L is an 8 bit register and we want to remove the last 2 bits from the right
    //which are CCP1Y and CCP1X so that we can define only CCPR1L 
    }
}
void main(void) {
    //configure the CCP1 MODULE TO PWM MODE
    CCP1M2 = 1;
    CCP1M3 = 1;
    //configure RC2/CCP1 (PWM) AS OUTPUT
    TRISC2 = 0;
    //setting timer2 module prescaler value to 4 >>> 1:0
    T2CKPS0 = 1;
    T2CKPS1 = 0;
    //Value of the PR2 register determine the time of the period (T) so we also control the frequency from PR2/// T = 1 / F
    PR2 = 124;
    //turning on timer2 AND START CCP1 PWM
    TMR2ON = 1;
    //defining a 16 bit variable for the DutyCycle
    uint16_t DC = 0;
    while(1){
        DC = 0; //start with 0% DutyCycle
        //500 is the 100% intensity so we will increase it gradually each 2 millisecond
        while(DC < 500){
            PMW1SetDuty(DC);
            DC++;
            __delay_ms(2);
        }
        __delay_ms(100);
        //then decrease it gradually each 2 millisecond
        while(DC > 0){
            PMW1SetDuty(DC);
            DC--;
            __delay_ms(2);
        }
        __delay_ms(100);
    }
    return;
}