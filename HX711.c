#include <xc.h>
#include <pic18f87j11.h>
#include "LCD.h"

//CONFIG bits for PIC18F87J11
#pragma config FOSC=HSPLL
#pragma config WDTEN=OFF
#pragma config XINST=OFF


//PIC Board    HX711
//======================
//    RD6  <->  SCK
//    RD5  <->  DT
//    3.3V <->  VCC
//    GND  <->  GND
//
////////////////////////////////////


void InitPins(void);
void ConfigInterrupts(void);
void ConfigPeriph(void);

#define CLK LATDbits.LATD6
#define _XTAL_FREQ 32000000L

enum {
    A_128 = 1, B_32 = 2, A_64 = 3
};
void InitHX711(void);
long ReadHX711(char nextConversion);
char HX711Ready(void);

void main(void) {
    OSCTUNEbits.PLLEN = 1;
    InitPins();
    ConfigPeriph();
    ConfigInterrupts();
    LCDInit();
    lprintf(0, "HX711 Demo");
    InitHX711();
    //Main loop
    while (1) {
        __delay_ms(200);
        long data = ReadHX711(B_32);
        lprintf(1, "%ld", data);
        LATDbits.LATD0 ^= 1;
    }
}

void InitHX711(void) {
    TRISDbits.TRISD6 = 0;
    TRISDbits.TRISD5 = 1;
    CLK = 0;

}

long ReadHX711(char nextConversion) {
    long result = 0;
    while (PORTDbits.RD5 == 1); //Wait for ready
    //Read 24 bits
    for (char i = 0; i < 24; ++i) {
        CLK = 1;
        __delay_us(1);
        result <<= 1;
        result |= PORTDbits.RD5;
        CLK = 0;
        __delay_us(1);
    }
    //Generate extra clock pulses to determine next conversion
    for (char i = 0; i < nextConversion; ++i) {
        CLK = 1;
        __delay_us(1);
        CLK = 0;
        __delay_us(1);
    }
    if (result & 0x800000) {
        //extend sign if negative
        result |= 0xff000000;
    }
    return result;
}

char HX711Ready(void) {
    if (PORTDbits.RD5 == 0)
        return 1;
    else
        return 0;
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
