#include "includes.h"

void RDA_PowerDown() {
    reg02->ref16.SEEK = 0;
    reg02->ref16.ENABLE = 0;
    RDA_WriteRegister(REG02);
}

void RDA_SeekUp() {
    reg02->ref16.SEEK = 1;
    reg02->ref16.SKMODE = RDA_SEEK_WRAP;
    reg02->ref16.SEEKUP = RDA_SEEK_UP;
    RDA_WriteRegister(REG02);
}

void RDA_Seek(uint8_t mode) {
    reg02->ref16.SEEK = 1;
    reg02->ref16.SKMODE = RDA_SEEK_WRAP;
    reg02->ref16.SEEKUP = mode;
    RDA_WriteRegister(REG02);
    do {
        PWR_OFF;
        __delay_ms(250);
        PWR_ON;
        __delay_ms(250);
        RDA_ReadStatusRegister();
    } while(reg0a -> ref16.STC == 0);
}

void RDA_addVolume(char step) {
    if ((currentVolume + step) & 0xF0)return;
    currentVolume += step;
    reg05->ref16.VOLUME = currentVolume;
    RDA_WriteRegister(REG05);
}

void RDA_ReadRegister3(void) {
    I2C_Start();
    while (I2C_Write_Byte(I2C_ADDR_DIRECT_ACCESS) == 1) {
        I2C_Start();
    }

    I2C_Write_Byte(REG03);
    I2C_ReStart();
    I2C_Write_Byte(I2C_ADDR_DIRECT_ACCESS + 1);
    __delay_us(250);
    reg03->ref8.highByte = I2C_Read_Byte();
    I2C_Send_ACK();
    reg03->ref8.lowByte = I2C_Read_Byte();
    I2C_Send_NACK();

    Set_SDA_Low;
    __delay_us(HalfBitDelay);
    Set_SDA_High;
    __delay_us(HalfBitDelay);
}

void RDA_SetRegister(uint8_t reg, uint8_t highByte, uint8_t lowByte) {
    I2C_Start();
    while (I2C_Write_Byte(I2C_ADDR_DIRECT_ACCESS) == 1) {
        I2C_Start();
    }
    I2C_Write_Byte(reg);
    I2C_Write_Byte(highByte);
    I2C_Write_Byte(lowByte);
    I2C_Stop();
    __delay_ms(5);
}

void RDA_WriteRegister(uint8_t reg) {
    // reg - 2,3,4,5
    // mem - 0,1,2,3
    uint8_t *addr = &memoryReg[reg - 2];
    I2C_Start();
    while (I2C_Write_Byte(I2C_ADDR_DIRECT_ACCESS) == 1) {
        I2C_Start();
    }
    I2C_Write_Byte(reg);
    I2C_Write_Byte(*(addr + 1)); // high byte
    I2C_Write_Byte(*addr); // low byte
    I2C_Stop();
    __delay_ms(5);
}

void RDA_SetRegisters(uint8_t *addr) {
    uint8_t reg;
    for (reg = 2; reg < 5; reg++) {
        I2C_Start();
        while (I2C_Write_Byte(I2C_ADDR_DIRECT_ACCESS) == 1) {
            I2C_Start();
        }

        I2C_Write_Byte(reg);
        I2C_Write_Byte(*(addr + 1));
        I2C_Write_Byte(*addr);
        I2C_Stop();
        __delay_ms(5);
        addr++;
    }
}

void RDA_ReadStatusRegister() {
    I2C_Start();
    while (I2C_Write_Byte(I2C_ADDR_FULL_ACCESS + 1) == 1) {
        I2C_Start();
    }
    __delay_us(250);
    reg0a->ref8.highByte = I2C_Read_Byte();
    I2C_Send_ACK();
    reg0a->ref8.lowByte = I2C_Read_Byte();
    I2C_Send_NACK();

    Set_SDA_Low;
    __delay_us(HalfBitDelay);
    Set_SDA_High;
    __delay_us(HalfBitDelay);
}



