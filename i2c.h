#ifndef __I2C_H
#define __I2C_H

// Define i2c pins
#define SDA			GP5				// Data pin for i2c
#define SCK			GP4				// Clock pin for i2c
#define SDA_DIR		TRISIO5			// Data pin direction
#define SCK_DIR		TRISIO4			// Clock pin direction


#define DataPinADCMask      ANS3
#define ClockPinADCMask     ANS3


#ifdef _12F675
#define DataPinADCMask_High		DataPinADCMask=1
#define ClockPinADCMask_High	ClockPinADCMask=1
#define DataPinADCMask_Low		DataPinADCMask=0
#define ClockPinADCMask_Low 	ClockPinADCMask=0
#else
#define DataPinADCMask_High
#define ClockPinADCMask_High
#define DataPinADCMask_Low
#define ClockPinADCMask_Low
#endif




// Define i2c speed
#define I2C_SPEED	10				// kbps 
#define HalfBitDelay 500/I2C_SPEED	// usec

// Define macros
#ifdef _12F675
#define Set_SDA_Low		SDA_DIR = 0
#define Set_SCK_Low		SCK_DIR = 0
#else
#define Set_SDA_Low		SDA_DIR = 0;SDA=0
#define Set_SCK_Low		SCK_DIR = 0;SCK=0
#endif

#define Set_SDA_High	SDA_DIR = 1
#define Set_SCK_High	SCK_DIR = 1

//Function Declarations
void InitI2C(void);
void I2C_Start(void);
void I2C_ReStart(void);
void I2C_Stop(void);
void I2C_Send_ACK(void);
void I2C_Send_NACK(void);
bit I2C_Write_Byte(unsigned char);
unsigned char I2C_Read_Byte(void);

#endif