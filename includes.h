#ifndef __INCLUDES_H
#define __INCLUDES_H

// Define CPU Frequency
// This must be defined, if __delay_ms()
// or __delay_us() functions are used in the code
#define _XTAL_FREQ   4000000      // Hz

// Include files
#include <xc.h>
#include <stdint.h>
#include "eeprom.h"
#include "i2c.h"
#include "radio.h"
#include "buttons.h"

#define LED_PIN GP2
#define SAVE_TIMEOUT 5000

#define PWR_ON LED_PIN = 1
#define PWR_OFF LED_PIN = 0
#define VOUME_UP 1
#define VOUME_DOWN -1

bit needSave;
volatile unsigned int main_ticker = 0;


#endif