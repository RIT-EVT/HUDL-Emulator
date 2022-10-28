//
// Created by Zachary Lineman on 10/24/22.
//

#include "LCD.hpp"
#include <iostream>

LCD::LCD() {

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

void LCD::commWrite(uint8_t data) {

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

void LCD::drivePixel(unsigned char page, unsigned char col_up, unsigned char col_low, unsigned char data) {
    this->commWrite(0x40);          //line to start writing on (0 -> 64) moves set bits with it DO NOT CHANGE
    this->commWrite(0xB0 + page);   //writes the page address (4 bits, 8 rows selected by values 0-7 )
    this->commWrite(0x10 + col_up); //writes the first 4 bits of the column select (out of 8 bits)
    this->commWrite(0x00 + col_low);//writes the second 4 bits of the column select (out)

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

void LCD::clearLCD(unsigned char* bitMap) {
    unsigned int i, j;
    unsigned char page = 0xB0;

    this->commWrite(0xAE);         //Display OFF
    this->commWrite(0x40);         //Display start address + 0x40
    for (i = 0; i < 8; i++) {            //64 pixel display / 8 pixels per page = 8 pages
        this->commWrite(page);     //send page address
        this->commWrite(0x10);     //column address upper 4 bits + 0x10
        this->commWrite(0x00);     //column address lower 4 bits + 0x00
        for (j = 0; j < 128; j++) {      //128 columns wide
            this->dataWrite(0x00); //write clear pixels
            bitMap++;                   // Advance the bitmap pointer by one. This means we can just grab the last one the next loop.
        }
        page++;                         //after 128 columns, go to next page
    }
    this->commWrite(0xAF);
}

void LCD::displayMap(unsigned char* bitMap) {
    unsigned int i, j;
    unsigned char page = 0xB0;
    this->commWrite(0xAE);           //Display OFF
    this->commWrite(0x40);           //Display start address + 0x40
    for (i = 0; i < 8; i++) { //64 pixel display / 8 pixels per page = 8 pages
        this->commWrite(page);       //send page address
        this->commWrite(0x10);       //column address upper 4 bits + 0x10
        this->commWrite(0x00);       //column address lower 4 bits + 0x00
        for (j = 0; j < 128; j++) {         //128 columns wide
            this->dataWrite(*bitMap);  //write pixels from bitmap
            bitMap++;                       // Advance the bitmap pointer by one. This means we can just grab the last one the next loop.
        }

        std::cout << "Progress Page" << std::endl;
        page++; //after 128 columns, go to next page
    }
    this->commWrite(0xAF);
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

    this->commWrite(0xAE);           //Display OFF
    this->commWrite(0x40);           //Display start address + 0x40
    for (i = 0; i < amountOfPages; i++) {                       //64 pixel display / 8 pixels per page = 8 pages
        this->commWrite(page);                            //send page address
        this->commWrite(0x10 + columnUpperAddress);       //column address upper 4 bits + 0x10
        this->commWrite(0x00 + columnLowerAddress);       //column address lower 4 bits + 0x00

        int columnCount = 128;
        if (i == amountOfPages - 1) columnCount = bitMapWidth;

        for (j = 0; j < columnCount; j++) {         //128 columns wide
            this->dataWrite(*bitMap);  //write pixels from bitmap
//            this->dataWrite(0xff);
            bitMap++;                       // Advance the bitmap pointer by one. This means we can just grab the last one the next loop.
        }

        page++; //after 128 columns, go to next page
    }
    this->commWrite(0xAF);
}

void LCD::initLCD() {

}

void LCD::writeText(string text, unsigned char page, unsigned char col_up, unsigned char col_low) {

}

//void LCD::writeText(string text, unsigned char page, unsigned char col_up, unsigned char col_low) {
//    unsigned int column = col_up;
//    column <<= 8;
//    column |= col_low;
//
//    for(char x : text) {
//        int fontIndex = (int) ((unsigned char) x) ;
//
//        unsigned char characterMap[8] = {
//                BitMapFont::font8x8_basic[fontIndex][0],
//                BitMapFont::font8x8_basic[fontIndex][1],
//                BitMapFont::font8x8_basic[fontIndex][2],
//                BitMapFont::font8x8_basic[fontIndex][3],
//                BitMapFont::font8x8_basic[fontIndex][4],
//                BitMapFont::font8x8_basic[fontIndex][5],
//                BitMapFont::font8x8_basic[fontIndex][6],
//                BitMapFont::font8x8_basic[fontIndex][7],
//        };
//
//        displayBitMap(characterMap, page, column);
//        column += 8;
//    }
//}
