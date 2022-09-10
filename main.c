// CONFIG
#pragma config FOSC = INTRCIO   // Oscillator Selection bits (INTOSC oscillator: I/O function on GP4/OSC2/CLKOUT pin, I/O function on GP5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-Up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = OFF      // GP3/MCLR pin function select (GP3/MCLR pin function is digital I/O, MCLR internally tied to VDD)
#pragma config BOREN = ON       // Brown-out Detect Enable bit (BOD enabled)
#pragma config CP = OFF         // Code Protection bit (Program Memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)

#include "includes.h"

// LOW_BYTE - HIGH_BYTE
__EEPROM_DATA(
        0x01, 0xF2, // REG02
        0x80, 0x34, // REG03 // 002E - autoradio
        0x00, 0x04, // REG04
        0x87, 0x08 // REG05
        );

/* Pin configuration
 * 
 * GP0 = LED 2
 * GP1 = LED 1
 * GP2 = Button 1
 * GP3 = Button 2
 * GP4 = SDA pin for i2c
 * GP5 = SCK pin for i2c
 */

#define PWR_ON GP1 = 1
#define PWR_OFF GP1 = 0

void InitCCT(void) {
#ifdef _12F675
    ANSEL = 0x00; // Set ports as digital I/O, not analog input
    ADCON0 = 0x00; // Shut off the A/D Converter
#endif     
    CMCON = 0x07; // Shut off the Comparator
    VRCON = 0x00; // Shut off the Voltage Reference
}

void InitButtons(void) {
    T0CS = 0;
    PSA = 0;

    // Timer divider: ticker step = 1ms
    PS0 = 1;
    PS1 = 0;
    PS2 = 0;

    GIE = 1; // enable global interrupts
    T0IE = 1; // enable timer0 interrupts
    TMR0 = 0; // reset timer
    PEIE = 0;
    INTE = 0;
    button1.GP2 = 1;
    button2.GP5 = 1;
    TRISIO = 0b00100100;
    WPU = 0b00100100;
    nGPPU = 0;
    IOCB2 = 1;
    GPIO = 0x00; // Make all pins 0
}

void interrupt globalInterrupt() {
    if (T0IF) {
        buttonsTick();
        T0IF = 0;
    }
    if (GPIF) {
        GPIE = 0;
        GPIF = 0;
    }
}

void init(void) {
    InitCCT(); // Turn off ADC and comparator to make pins digital IOs
    InitI2C(); // Initialize i2c pins	
    InitButtons();
}

void stopRadio() {
    RDA_ReadRegister3();
    EEPROM_WriteRegisters(&memoryReg[0]);
    RDA_PowerDown();
    PWR_OFF;
}

void startRadio() {
    EEPROM_ReadRegisters(&memoryReg[0]);

    RDA_WriteRegister(REG02);
    RDA_WriteRegister(REG04);
    RDA_WriteRegister(REG05);
    currentVolume = reg05 -> ref16.VOLUME;

    reg03 -> ref16.TUNE = 1;
    RDA_WriteRegister(REG03);
}

void goSleep(void) {
go_sleep:
    GPIO = 0;
    GPIE = 1;
    GPIF = 0;
    SLEEP();
    // zzz...
    init();
    // Long click to wakeup
    checkButton(&button1);
    while (bPress(&button1)) {
        checkButton(&button1);
        if (bHold(&button1))PWR_ON;
    }
    if (!bLongClick(&button1)) goto go_sleep;
    //----------------------
}


// Main function

void main() {
    init();
    goSleep();

    // zzz...

    startRadio();
    while (1) {
        checkButton(&button1);
        checkButton(&button2);

        if (bClick(&button2)) {
            RDA_addVolume(1);
        }

        if (bClick(&button1)) {
            RDA_addVolume(-1);
        }

        if (bHold(&button2)) {
            RDA_SeekUp();
            while (bPress(&button2))
                checkButton(&button2);
        }

        if (bHold(&button1)) {
            stopRadio();
            while (bPress(&button1))
                checkButton(&button1);
            PWR_ON;
            goSleep();
            // zzz...
            startRadio();
        }
    }
}

