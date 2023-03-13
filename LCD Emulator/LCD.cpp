/*
 * LCD.cpp
 * This file declares the code for the LCD class. This is almost exactly the same as the version present in EVT-Core
 *
 * Created by Zachary Lineman on 10/24/22.
 */

#include "LCD.hpp"
#include <iostream>
#include <cstring>

LCD::LCD() { }

void LCD::initLCD() { }

void LCD::dataWrite(uint8_t data) {
    uint8_t page = lastPageAddress - emulationPageOffset;

    for(unsigned int x = 0; x < 8; x ++) {
        unsigned int bit = data >> x & 1;

        unsigned int yValue = 63 - ((page * 8) + x);

        if (bit == 0) {
            screen[lastColumnAddress][yValue].hidden = true;
        } else {
            screen[lastColumnAddress][yValue].hidden = false;
        }
    }
    lastColumnAddress ++;
}

void LCD::commandWrite(uint8_t data) {

    if(data == 0x40) {
        lastPageAddress = 0x00;
        lastColumnAddress = 0x00;
        isWriting = true;
        writePageAddress = true;
    } else if (data == 0xAF) {
        lastPageAddress = 0x00;
        lastColumnAddress = 0x00;
        isWriting = false;
        writePageAddress = false;
        writeColumnUpperAddress = false;
        writeColumnLowerAddress = false;
    } else {
        if (isWriting) {
            if (writePageAddress) {
                lastPageAddress = data;

                writePageAddress = false;
                writeColumnUpperAddress = true;
            } else if (writeColumnUpperAddress) {
                lastColumnAddress = data;
                writeColumnUpperAddress = false;
                writeColumnLowerAddress = true;
            } else if (writeColumnLowerAddress) {
                lastColumnAddress <<= 4;

                uint8_t lowerColumnAddress = data;
                lowerColumnAddress <<= 4;
                lowerColumnAddress >>= 4;
                lastColumnAddress |= lowerColumnAddress;
                writeColumnLowerAddress = false;
                writePageAddress = true;
            }
        }
    }
}