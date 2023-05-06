/*
 * LCD.cpp
 * This file declares the code for the LCD class. This is almost exactly the same as the version present in EVT-Core
 *
 * Created by Zachary Lineman on 10/24/22.
 */

#include "LCD.hpp"
#include <iostream>
#include <cstring>

LCD::LCD() {

}

void LCD::initLCD() {

}

void LCD::dataWrite(uint8_t data) {
    uint8_t page = lastPageAddress - emulationPageOffset;

    for(uint8_t x = 0; x < 8; x ++) {
        uint8_t bit = data >> x & 1;

        uint8_t yValue = 63 - ((page * 8) + x);

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

void LCD::driveColumn(uint8_t page, uint8_t colUp, uint8_t colLow, uint8_t data) {
    this->commandWrite(0xAE);         //Display OFF

    this->commandWrite(0x40);         //line to start writing on (0 -> 64) moves set bits with it DO NOT CHANGE
    this->commandWrite(0xB0 + page);  //writes the page address (4 bits, 8 rows selected by values 0-7 )
    this->commandWrite(0x10 + colUp); //writes the first 4 bits of the column select (out of 8 bits)
    this->commandWrite(0x00 + colLow);//writes the second 4 bits of the column select (out)

    this->dataWrite(data);

    this->commandWrite(0xAF);
    /*
     * writes 8 vertical bits based on value between 0-255 based on bits set ex: 01001100(0x4C) is
     * |WHITE|
     * |BLACK|
     * |WHITE|
     * |WHITE|
     * |BLACK|
     * |BLACK|
     * |WHITE|
     * |WHITE|
     */
}

void LCD::clearLCD() {
    uint8_t i, j;
    unsigned char page = 0xB0;

    this->commandWrite(0xAE);         //Display OFF
    this->commandWrite(0x40);         //Display start address + 0x40
    for (i = 0; i < 8; i++) {            //64 pixel display / 8 pixels per page = 8 pages
        this->commandWrite(page);     //send page address
        this->commandWrite(0x10);     //column address upper 4 bits + 0x10
        this->commandWrite(0x00);     //column address lower 4 bits + 0x00
        for (j = 0; j < 128; j++) {      //128 columns wide
            this->dataWrite(0x00); //write clear pixels
        }
        page++;                         //after 128 columns, go to next page
    }
    this->commandWrite(0xAF);
}

void LCD::clearArea(uint8_t width, uint8_t height, uint8_t page, uint8_t column){
    uint8_t i, j;
    uint8_t columnUpperAddress = (column & 0xf0) >> 4;
    uint8_t columnLowerAddress = (column & 0x0f);

    uint8_t amountOfPages = height / 8;
    if (height < 8) amountOfPages = 1;

    this->commandWrite(0xAE);           //Display OFF
    this->commandWrite(0x40);           //Display start address + 0x40
    for (i = 0; i < amountOfPages; i++) { //64 pixel display / 8 pixels per page = 8 pages
        this->commandWrite(0xB0 + page);       //send page address
        this->commandWrite(0x10 + columnUpperAddress);       //column address upper 4 bits + 0x10
        this->commandWrite(0x00 + columnLowerAddress);       //column address lower 4 bits + 0x00

        for (j = 0; j < width; j++) {
            this->dataWrite(0x00);
        }

        page++; //after 128 columns, go to next page
    }
    this->commandWrite(0xAF);
}

void LCD::displayMap(const uint8_t* bitMap) {
    uint8_t i, j;
    uint8_t page = 0xB0;
    this->commandWrite(0xAE);           //Display OFF
    this->commandWrite(0x40);           //Display start address + 0x40
    for (i = 0; i < 8; i++) { //64 pixel display / 8 pixels per page = 8 pages
        this->commandWrite(page);       //send page address
        this->commandWrite(0x10);       //column address upper 4 bits + 0x10
        this->commandWrite(0x00);       //column address lower 4 bits + 0x00
        for (j = 0; j < 128; j++) {         //128 columns wide
            this->dataWrite(*bitMap);  //write pixels from bitmap
            bitMap++;                       // Advance the bitmap pointer by one. This means we can just grab the last one the next loop.
        }
        page++; //after 128 columns, go to next page
    }
    this->commandWrite(0xAF);
}

void LCD::displayBitMap(uint8_t * bitMap, uint8_t bitMapWidth, uint8_t bitMapHeight, uint8_t page, uint8_t column) {
    uint8_t i, j;
    uint8_t columnUpperAddress = (column & 0xf0) >> 4;
    uint8_t columnLowerAddress = (column & 0x0f);

    uint8_t amountOfPages = bitMapHeight / 8;
    if (bitMapHeight < 8) amountOfPages = 1;

    this->commandWrite(0xAE);           //Display OFF
    this->commandWrite(0x40);           //Display start address + 0x40
    for (i = 0; i < amountOfPages; i++) { //64 pixel display / 8 pixels per page = 8 pages
        this->commandWrite(0xB0 + page);       //send page address
        this->commandWrite(0x10 + columnUpperAddress);       //column address upper 4 bits + 0x10
        this->commandWrite(0x00 + columnLowerAddress);       //column address lower 4 bits + 0x00

        for (j = 0; j < bitMapWidth; j++) {         //128 columns wide
            this->dataWrite(*bitMap);  //write pixels from bitmap
            bitMap++;                       // Advance the bitmap pointer by one. This means we can just grab the last one the next loop.
        }

        page++; //after 128 columns, go to next page
    }
    this->commandWrite(0xAF);

}

void LCD::writeText(const char *text, uint8_t page, uint8_t column, bool wrapText) {
    for(uint8_t x = 0; x < strlen(text); x ++) {
        uint8_t fontIndex = text[x];

        unsigned char characterMap[4] = {
                BitmapFont::font4x6[fontIndex][0],
                BitmapFont::font4x6[fontIndex][1],
                BitmapFont::font4x6[fontIndex][2],
                BitmapFont::font4x6[fontIndex][3],
        };

        if (column >= screenSizeX) return;

        displayBitMap(characterMap, 4, 8, page, column);
        column += 4;

        if (wrapText && column >= screenSizeX) {
            page ++;
            column = 0;
        }
    }
}

void LCD::setDefaultSections(char* newSectionTitles[9]) {
    for (uint8_t x = 0; x < 9; x++) {
        this->sectionTitles[x] = newSectionTitles[x];
    }
}

void LCD::displaySectionHeaders() {
    uint8_t page = 0;
    uint8_t column = 0;
    uint8_t rowCounter = 0;

    uint8_t sectionWidth = screenSizeX / sectionsPerRow;

    for (auto title : sectionTitles) {
        uint8_t length = strlen(title) * 4;
        uint8_t padding = (sectionWidth - length) / 2;

        column += padding;

        writeText(title, page, column, false);

        column += length;
        column += padding;

        rowCounter ++;
        if(rowCounter >= sectionsPerRow) {
            rowCounter = 0;
            column = 0;
            page += 3;
        }

        if (column >= screenSizeX) {
            column = 0;
        }
    }
}

void LCD::setTextForSection(uint8_t section, const char* text) {
    uint8_t sectionWidth = screenSizeX / sectionsPerRow;
    uint8_t adjustedSection = section + 1;
    uint8_t sectionRow = section / sectionsPerRow;
    uint8_t sectionPage = (sectionRow * 3) + 1;
    uint8_t sectionColumn = (adjustedSection - (sectionRow * sectionsPerRow) - 1) * sectionWidth;

    uint8_t length = strlen(text) * 4;
    uint8_t padding = (sectionWidth - length) / 2;

    sectionColumn += padding;

    writeText(text, sectionPage, sectionColumn, false);
}

void LCD::setPixel(uint8_t x, uint8_t y, bool on) {
    if (x < LCD::screenSizeX && x >= 0 && y < LCD::screenSizeY && y >= 0)  {
        int page = y / 8;
        int yLeft = y % 8;
        int oneDIndex = (page * LCD::screenSizeX) + x;

        uint8_t bit = (internalBitMap[oneDIndex] >> yLeft) & 1U;
        if(bit == 0 && on) {
            internalBitMap[oneDIndex] ^= 1UL << yLeft; // Toggle a bit yLeft in the byte
        } else if (bit == 1 && !on) {
            internalBitMap[oneDIndex] ^= 1UL << yLeft; // Toggle a bit yLeft in the byte
        }
    }
}

void LCD::updateDisplay() {
    clearLCD();
    uint8_t *bitMap = internalBitMap;
    uint8_t page = 0xB0;

    commandWrite(0x40);//Display start address + 0x40
    for (uint8_t i = 0; i < 8; i++) { //64 pixel display / 8 pixels per page = 8 pages
        commandWrite(page);       //send page address
        commandWrite(0x10);       //column address upper 4 bits + 0x10
        commandWrite(0x00);       //column address lower 4 bits + 0x00
        for (uint8_t j = 0; j < 128; j++) {         //128 columns wide
            dataWrite(*bitMap);  //write pixels from bitmap
            bitMap++;                       // Advance the bitmap pointer by one. This means we can just grab the last one the next loop.
        }
        page++; //after 128 columns, go to next page
    }
    commandWrite(DISPLAYON);
}