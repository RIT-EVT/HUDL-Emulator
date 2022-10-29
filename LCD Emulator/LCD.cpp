//
// Created by Zachary Lineman on 10/24/22.
//

#include "LCD.hpp"
#include <iostream>
#include <cstring>

LCD::LCD() {

}

void LCD::initLCD() {

}

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

void LCD::drivePixel(uint8_t page, uint8_t col_up, uint8_t col_low, uint8_t data) {
    this->commandWrite(0x40);          //line to start writing on (0 -> 64) moves set bits with it DO NOT CHANGE
    this->commandWrite(0xB0 + page);   //writes the page address (4 bits, 8 rows selected by values 0-7 )
    this->commandWrite(0x10 + col_up); //writes the first 4 bits of the column select (out of 8 bits)
    this->commandWrite(0x00 + col_low);//writes the second 4 bits of the column select (out)

    this->dataWrite(data);
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

void LCD::clearLCD(const uint8_t* bitMap) {
    unsigned int i, j;
    unsigned char page = 0xB0;

    this->commandWrite(0xAE);         //Display OFF
    this->commandWrite(0x40);         //Display start address + 0x40
    for (i = 0; i < 8; i++) {            //64 pixel display / 8 pixels per page = 8 pages
        this->commandWrite(page);     //send page address
        this->commandWrite(0x10);     //column address upper 4 bits + 0x10
        this->commandWrite(0x00);     //column address lower 4 bits + 0x00
        for (j = 0; j < 128; j++) {      //128 columns wide
            this->dataWrite(0x00); //write clear pixels
            bitMap++;                   // Advance the bitmap pointer by one. This means we can just grab the last one the next loop.
        }
        page++;                         //after 128 columns, go to next page
    }
    this->commandWrite(0xAF);
}

void LCD::displayMap(const uint8_t* bitMap) {
    unsigned int i, j;
    unsigned char page = 0xB0;
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
    unsigned int i, j;
    unsigned char columnUpperAddress = column;
    columnUpperAddress >>= 4;

    unsigned char columnLowerAddress = column;
    columnLowerAddress <<= 4;
    columnLowerAddress >>= 4;

    int amountOfPages = bitMapHeight / 8;
    if (bitMapHeight < 8) amountOfPages = 1;

    this->commandWrite(0xAE);           //Display OFF
    this->commandWrite(0x40);           //Display start address + 0x40
    for (i = 0; i < amountOfPages; i++) {                       //64 pixel display / 8 pixels per page = 8 pages
        this->commandWrite(0xB0 + page);                            //send page address
        this->commandWrite(0x10 + columnUpperAddress);       //column address upper 4 bits + 0x10
        this->commandWrite(0x00 + columnLowerAddress);       //column address lower 4 bits + 0x00

        for (j = 0; j < bitMapWidth; j++) {         //128 columns wide
            this->dataWrite(*bitMap);  //write pixels from bitmap
//            this->dataWrite(0xff);
            bitMap++;                       // Advance the bitmap pointer by one. This means we can just grab the last one the next loop.
        }

        page++; //after 128 columns, go to next page
    }
    this->commandWrite(0xAF);
}

void LCD::writeText(const char* text, uint8_t page, uint8_t column) {
    for(int x = 0; x < strlen(text); x ++) {
        int fontIndex = (int) ((unsigned char) text[x]);

        unsigned char characterMap[8] = {
                BitMapFont::font8x8_basic[fontIndex][0],
                BitMapFont::font8x8_basic[fontIndex][1],
                BitMapFont::font8x8_basic[fontIndex][2],
                BitMapFont::font8x8_basic[fontIndex][3],
                BitMapFont::font8x8_basic[fontIndex][4],
                BitMapFont::font8x8_basic[fontIndex][5],
                BitMapFont::font8x8_basic[fontIndex][6],
                BitMapFont::font8x8_basic[fontIndex][7],
        };

        displayBitMap(characterMap, 8, 8, page, column);
        column += 8;
    }
}

void LCD::clearArea(uint8_t width, uint8_t height, uint8_t page, uint8_t column) {
    unsigned int i, j;
    uint8_t columnUpperAddress = column;
    columnUpperAddress >>= 4;

    uint8_t columnLowerAddress = column;
    columnLowerAddress <<= 4;
    columnLowerAddress >>= 4;

    int amountOfPages = height / 8;
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


void LCD::addBoundingBox(uint8_t minX, uint8_t minX, uint8_t maxY) {
    lastBoundingBox ++;
    boundingBoxes[lastBoundingBox] = GraphicsBoundingBox()
}

void LCD::drawSquare(uint8_t width, uint8_t height, uint8_t x, uint8_t y) {
    for (int n = 0; n < y; n ++) {
        int index = (screenSizeX * n) + x;
        for (int i = 0; i < width; i ++) {
            internalBitMap[index] = 0xFF;
        }
    }
    addBoundingBox(x, )
}

void LCD::renderBoxes() {
    for(pair<uint8_t, uint8_t> box: boundingBoxes) {
        int minX = box.first;
        int maxY = box.second;

        for (int n = 0; n < y; n ++) {
            int index = (screenSizeX * n) + x;
            for (int i = 0; i < width; i ++) {
                internalBitMap[index] = 0xFF;
            }
        }
    }
}
