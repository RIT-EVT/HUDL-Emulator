#include "Graphics.hpp"

Graphics::Graphics(LCD &lcd) : lcd(lcd) {
    for(unsigned char & x : internalBitMap) {
        x = 0x00;
    }
}

void Graphics::updateDisplay() {
    clearLCD();
    uint8_t *bitMap = internalBitMap;
    uint8_t i, j;
    uint8_t page = 0xB0;
    lcd.commandWrite(0xAE);//Display OFF
    lcd.commandWrite(0x40);//Display start address + 0x40
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

    int oneDIndex = (page * LCD::screenSizeX) + column;
    for (i = 0; i < amountOfPages; i++) {
        for (j = 0; j < bitMapWidth; j++) {
            internalBitMap[oneDIndex] = bitMap[0];
            bitMap++;
            oneDIndex++;
        }

        oneDIndex += LCD::screenSizeX;
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

    int oneDIndex = (page * LCD::screenSizeX) + column;
    int widthCounter = 0;
    int heightCounter = 0;
    for (uint8_t index = oneDIndex; index < 1024; index++) {
        if (height >= heightCounter) break;
        if (widthCounter < width) {
            widthCounter++;

            internalBitMap[oneDIndex] = 0x00;

            oneDIndex++;
        } else {
            heightCounter++;
            widthCounter = 0;
        }
    }
}

void Graphics::displayMap(uint8_t* bitMap) {
    int oneDIndex = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 128; j++) {
            internalBitMap[oneDIndex] = bitMap[0];
            bitMap++;
            oneDIndex++;
        }
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

void Graphics::setPixel(uint8_t x, uint8_t y, bool on) {
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

// Based on https://csustan.csustan.edu/~tom/Lecture-Notes/Graphics/Bresenham-Line/Bresenham-Line.pdf
void Graphics::drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
    int deltaX;
    int deltaY;
    int stepX;
    int stepY;
    deltaX = x1 - x0;
    deltaY = y1 - y0;

    if (deltaY < 0) {
        deltaY = -deltaY;
        stepY = -1;
    } else {
        stepY = 1;
    }

    if (deltaX < 0) {
        deltaX = -deltaX;
        stepX = -1;
    } else {
        stepX = 1;
    }

    // Shifting one to the left is the exact same as multiply by 2, just faster.
    deltaY <<= 1;
    deltaX <<= 1;

    // Set our starting pixel
    setPixel(x0, y0, true);

    if (deltaX > deltaY) {
        int fraction = deltaY - (deltaX >> 1);  // Divide Delta X by 2
        while (x0 != x1) {
            if (fraction >= 0) {
                y0 += stepY;
                fraction -= deltaX;
            }
            x0 += stepX;
            fraction += deltaY;

            setPixel(x0, y0, true);
        }
    } else {
        int fraction = deltaX - (deltaY >> 1); // Divide Delta Y by 2
        while (y0 != y1) {
            if (fraction >= 0) {
                x0 += stepX;
                fraction -= deltaY;
            }
            y0 += stepY;
            fraction += deltaX;

            setPixel(x0, y0, true);
        }
    }
}

void Graphics::drawVertices(Graphics::Vertex *vertexArray, uint8_t size) {
    for (uint8_t index = 0; index < size; index++) {
        Vertex start = vertexArray[index];
        uint8_t endIndex = index + 1;

        if (endIndex >= size) endIndex = 0;

        Vertex end = vertexArray[endIndex];

        drawLine(start.x, start.y, end.x, end.y);
    }
}