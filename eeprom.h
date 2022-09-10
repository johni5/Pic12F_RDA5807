#ifndef __EEPROM_H
#define __EEPROM_H

//void EEPROM_WriteByte(uint8_t addr, uint8_t data8);
uint8_t EEPROM_ReadByte(uint8_t addr);
void EEPROM_ReadRegisters(uint8_t *addr);
void EEPROM_WriteByte(uint8_t addr, uint8_t *data8);
void EEPROM_WriteRegisters(uint8_t *addr);

#endif