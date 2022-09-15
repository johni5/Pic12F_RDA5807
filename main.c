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
 * GP0 = Button 1
 * GP1 = Button 2
 * GP2 = LED 1
 * GP3 = 
 * GP4 = SCK
 * GP5 = SDA
 */

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
    button1.GP0 = 1;
    button2.GP1 = 1;
    TRISIO = 0b00000011;
    WPU = 0b00000011;
    nGPPU = 0;
    GPIO = 0x00; // Make all pins 0
}

void interrupt globalInterrupt() {
    if (T0IF) {
        buttonsTick();
        main_ticker++;
        T0IF = 0;
    }
}

void init(void) {
    InitCCT(); // Turn off ADC and comparator to make pins digital IOs
    InitI2C(); // Initialize i2c pins	
    InitButtons();
    // Start radio
    EEPROM_ReadRegisters(&memoryReg[0]);
    RDA_WriteRegister(REG02);
    RDA_WriteRegister(REG04);
    RDA_WriteRegister(REG05);
    currentVolume = reg05 -> ref16.VOLUME;
    reg03 -> ref16.TUNE = 1;
    RDA_WriteRegister(REG03);
}

void setNeedSave() {
    needSave = 1;
    main_ticker = 0;
}

void main() {
    init();
    PWR_ON;
    while (1) {
        checkButton(&button1);
        checkButton(&button2);

        if (bClick(&button2)) {
            RDA_addVolume(VOUME_UP);
            setNeedSave();
        }

        if (bClick(&button1)) {
            RDA_addVolume(VOUME_DOWN);
            setNeedSave();
        }

        if (bHold(&button2)) {
            RDA_Seek(RDA_SEEK_UP);
            setNeedSave();
        }

        if (bHold(&button1)) {
            RDA_Seek(RDA_SEEK_DOWN);
            setNeedSave();
        }

        if (needSave && main_ticker > SAVE_TIMEOUT) {
            RDA_ReadRegister3();
            EEPROM_WriteRegisters(&memoryReg[0]);
            needSave = 0;
        }

    }
}

