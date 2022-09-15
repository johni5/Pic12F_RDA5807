#include "includes.h"

void EEPROM_WriteByte(uint8_t addr, uint8_t data8) {
    uint8_t status;
    while (WR);
    EEADR = addr;
    EEDATA = data8;
    WREN = 1;
    status = GIE;
    GIE = 0;
    EECON2 = 0x55;
    EECON2 = 0xAA;
    WR = 1;
    GIE = status;
    WREN = 0;
}

uint8_t EEPROM_ReadByte(uint8_t addr) {
    while (RD || WR);
    EEADR = addr;
    RD = 1;
    return EEDATA;
}

void EEPROM_WriteRegisters(uint8_t *addr) {
    uint8_t i;
    for (i = 0; i < 8; i++) {
        uint8_t status;
        while (WR);
        EEADR = i;
        EEDATA = *addr;
        WREN = 1;
        status = GIE;
        GIE = 0;
        EECON2 = 0x55;
        EECON2 = 0xAA;
        WR = 1;
        GIE = status;
        WREN = 0;
        addr++;
    }
}

void EEPROM_ReadRegisters(uint8_t *addr) {
    uint8_t i;
    for (i = 0; i < 8; i++) {
        while (RD || WR);
        EEADR = i;
        RD = 1;
        *addr = EEDATA;
        addr++;
    }
}
