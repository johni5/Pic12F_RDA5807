#include "includes.h"

#ifndef __BUTTONS_H
#define __BUTTONS_H

#define BTN_ON(msk) !(GPIO & msk)
#define BTN_OFF(msk) GPIO & msk


typedef union {

    struct {
        uint8_t GP0 : 1; 
        uint8_t GP1 : 1; 
        uint8_t GP2 : 1; 
        uint8_t GP3 : 1; 
        uint8_t GP4 : 1; 
        uint8_t GP5 : 1; 
        uint8_t enter : 1; 
        uint8_t exit : 1; 
        uint8_t hold : 1;         
    };

    struct {
        uint8_t gpio : 6; 
        uint8_t : 3; 
    } mask;
    
} button;


volatile unsigned int b_ticker = 0;
volatile unsigned int b_ticker2 = 0;

button button1;
button button2;

void buttonsTick(void);
void checkButton(button *bs);

bit bClick(button *bs);
bit bHold(button *bs);
bit bPress(button *bs);
bit bLongClick(button *bs);

#endif