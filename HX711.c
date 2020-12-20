#include <xc.h>
#include "LCD.h"

//CONFIG bits for PIC18F87J11
#pragma config FOSC=HSPLL
#pragma config WDTEN=OFF
#pragma config XINST=OFF

void InitPins(void);
void ConfigInterrupts(void);
void ConfigPeriph(void);

void main(void) {
    OSCTUNEbits.PLLEN = 1;
    InitPins();
    ConfigPeriph();
    ConfigInterrupts();

    //Main loop
    while (1) {
        
    }
}

void InitPins(void) {
    LATD = 0; //Turn off all LED's
    TRISD = 0; //LED's are outputs
    TRISB = 0b00000001; //Set RB0 as input (this is the button)
    //Set the TX and RX pins to the correct direction
}

void ConfigInterrupts(void) {

    //Turn on all enabled interrupts
    INTCONbits.PEIE = 1; //turn on peripheral interrupts
    INTCONbits.GIE = 1; //Turn on interrupts
}

void ConfigPeriph(void) {

    //Configure peripherals here
    

}

void __interrupt(high_priority) HighIsr(void) {

}
