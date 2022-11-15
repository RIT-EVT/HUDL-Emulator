//
// Created by Zachary Lineman on 11/10/22.
//

#include "Graphics.hpp"

Graphics::Graphics(LCD &lcd) : lcd(lcd) {
    for(unsigned char & x : internalBitMap) {
        x = 0x00;
    }
}

void Graphics::updateDisplay() {
    if(fullGraphicsMode) {
        clearLCD();

        uint8_t* bitMap = internalBitMap;
        uint8_t i, j;
        uint8_t page = 0xB0;
        lcd.commandWrite(0xAE);           //Display OFF
        lcd.commandWrite(0x40);           //Display start address + 0x40
        for (i = 0; i < 8; i++) { //64 pixel display / 8 pixels per page = 8 pages
            lcd.commandWrite(page);       //send page address
            lcd.commandWrite(0x10);       //column address upper 4 bits + 0x10
            lcd.commandWrite(0x00);       //column address lower 4 bits + 0x00
            for (j = 0; j < 128; j++) {         //128 columns wide
                lcd.dataWrite(*bitMap);  //write pixels from bitmap
                bitMap++;                       // Advance the bitmap pointer by one. This means we can just grab the last one the next loop.
            }
            page++; //after 128 columns, go to next page
        }
        lcd.commandWrite(0xAF);
    }
}

void Graphics::clearLCD() {
    uint8_t i, j;
    uint8_t page = 0xB0;

    lcd.commandWrite(0xAE);         //Display OFF
    lcd.commandWrite(0x40);         //Display start address + 0x40
    for (i = 0; i < 8; i++) {            //64 pixel display / 8 pixels per page = 8 pages
        lcd.commandWrite(page);     //send page address
        lcd.commandWrite(0x10);     //column address upper 4 bits + 0x10
        lcd.commandWrite(0x00);     //column address lower 4 bits + 0x00
        for (j = 0; j < 128; j++) {      //128 columns wide
            lcd.dataWrite(0x00); //write clear pixels
        }
        page++;                         //after 128 columns, go to next page
    }
    lcd.commandWrite(0xAF);
}

void Graphics::displayBitMap(uint8_t* bitMap, uint8_t bitMapWidth, uint8_t bitMapHeight, uint8_t page, uint8_t column) {
    uint8_t i, j;
    uint8_t columnUpperAddress = column;
    columnUpperAddress >>= 4;

    uint8_t columnLowerAddress = column;
    columnLowerAddress <<= 4;
    columnLowerAddress >>= 4;

    uint8_t amountOfPages = bitMapHeight / 8;
    if (bitMapHeight < 8) amountOfPages = 1;

    if(fullGraphicsMode) {
        int oneDIndex = (page * LCD::screenSizeX) + column;

        for (i = 0; i < amountOfPages; i++) {
            for (j = 0; j < bitMapWidth; j++) {
                internalBitMap[oneDIndex] = bitMap[0];
                bitMap++;
                oneDIndex ++;
            }

            oneDIndex += LCD::screenSizeX;
        }
    } else {
        lcd.commandWrite(0xAE);           //Display OFF
        lcd.commandWrite(0x40);           //Display start address + 0x40
        for (i = 0; i < amountOfPages; i++) { //64 pixel display / 8 pixels per page = 8 pages
            lcd.commandWrite(0xB0 + page);       //send page address
            lcd.commandWrite(0x10 + columnUpperAddress);       //column address upper 4 bits + 0x10
            lcd.commandWrite(0x00 + columnLowerAddress);       //column address lower 4 bits + 0x00

            for (j = 0; j < bitMapWidth; j++) {         //128 columns wide
                lcd.dataWrite(*bitMap);  //write pixels from bitmap
                bitMap++;                       // Advance the bitmap pointer by one. This means we can just grab the last one the next loop.
            }

            page++; //after 128 columns, go to next page
        }
        lcd.commandWrite(0xAF);
    }
}

void Graphics::clearArea(uint8_t width, uint8_t height, uint8_t page, uint8_t column){
    uint8_t i, j;
    uint8_t columnUpperAddress = column;
    columnUpperAddress >>= 4;

    uint8_t columnLowerAddress = column;
    columnLowerAddress <<= 4;
    columnLowerAddress >>= 4;

    uint8_t amountOfPages = height / 8;
    if (height < 8) amountOfPages = 1;

    if (!fullGraphicsMode) {
        lcd.commandWrite(0xAE);           //Display OFF
        lcd.commandWrite(0x40);           //Display start address + 0x40
        for (i = 0; i < amountOfPages; i++) { //64 pixel display / 8 pixels per page = 8 pages
            lcd.commandWrite(0xB0 + page);       //send page address
            lcd.commandWrite(0x10 + columnUpperAddress);       //column address upper 4 bits + 0x10
            lcd.commandWrite(0x00 + columnLowerAddress);       //column address lower 4 bits + 0x00

            for (j = 0; j < width; j++) {
                lcd.dataWrite(0x00);
            }

            page++; //after 128 columns, go to next page
        }
        lcd.commandWrite(0xAF);
    } else {
        int oneDIndex = (page * LCD::screenSizeX) + column;

        int widthCounter = 0;
        int heightCounter = 0;
        for(uint8_t index = oneDIndex; index < 1024; index ++) {
            if(height >= heightCounter) break;
            if (widthCounter < width) {
                widthCounter ++;

                internalBitMap[oneDIndex] = 0x00;

                oneDIndex ++;
            } else {
                heightCounter ++;
                widthCounter = 0;
            }
        }
    }
}

void Graphics::displayMap(uint8_t* bitMap) {
    if(fullGraphicsMode) {
        int oneDIndex = 0;

        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 128; j++) {
                internalBitMap[oneDIndex] = bitMap[0];
                bitMap++;
                oneDIndex ++;
            }
        }
    } else {
        uint8_t i, j;
        uint8_t page = 0xB0;
        lcd.commandWrite(0xAE);           //Display OFF
        lcd.commandWrite(0x40);           //Display start address + 0x40
        for (i = 0; i < 8; i++) { //64 pixel display / 8 pixels per page = 8 pages
            lcd.commandWrite(page);       //send page address
            lcd.commandWrite(0x10);       //column address upper 4 bits + 0x10
            lcd.commandWrite(0x00);       //column address lower 4 bits + 0x00
            for (j = 0; j < 128; j++) {         //128 columns wide
                lcd.dataWrite(*bitMap);  //write pixels from bitmap
                bitMap++;                       // Advance the bitmap pointer by one. This means we can just grab the last one the next loop.
            }
            page++; //after 128 columns, go to next page
        }
        lcd.commandWrite(0xAF);
    }
}

 void Graphics::writeText(const char *text, uint8_t page, uint8_t column, bool wrapText) {
     for(uint8_t x = 0; x < strlen(text); x ++) {
         uint8_t fontIndex = (int) ((unsigned char) text[x]);

         unsigned char characterMap[4] = {
                 BitmapFont::font4x6[fontIndex][0],
                 BitmapFont::font4x6[fontIndex][1],
                 BitmapFont::font4x6[fontIndex][2],
                 BitmapFont::font4x6[fontIndex][3],
         };

         if (column >= LCD::screenSizeX) return;

         displayBitMap(characterMap, 4, 8, page, column);
         column += 4;

         if (wrapText && column >= LCD::screenSizeX) {
             page++;
             std::cout << (int) page << std::endl;
             column = 0;
         }
     }
}

void Graphics::setDefaultSections(char* newSectionTitles[9]) {
    for (uint8_t x = 0; x < 9; x++) {
        this->sectionTitles[x] = newSectionTitles[x];
    }
}

void Graphics::displaySectionHeaders() {
    uint8_t page = 0;
    uint8_t column = 0;
    uint8_t rowCounter = 0;

    uint8_t sectionWidth = LCD::screenSizeX / sectionsPerRow;

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

        if (column >= LCD::screenSizeX) {
            column = 0;
        }
    }
}

void Graphics::setTextForSection(uint8_t section, const char* text) {
    uint8_t sectionWidth = LCD::screenSizeX / sectionsPerRow;
    uint8_t adjustedSection = section + 1;
    uint8_t sectionRow = section / sectionsPerRow;
    uint8_t sectionPage = (sectionRow * 3) + 1;
    uint8_t sectionColumn = (adjustedSection - (sectionRow * sectionsPerRow) - 1) * sectionWidth;

    uint8_t length = strlen(text) * 4;
    uint8_t padding = (sectionWidth - length) / 2;

    sectionColumn += padding;

    writeText(text, sectionPage, sectionColumn, false);
}

void Graphics::drawLine(uint8_t startX, uint8_t startY, uint8_t endX, uint8_t endY) {
    double slope = (endY - endX) / (endX / startX);

}

