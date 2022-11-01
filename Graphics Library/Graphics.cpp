//
// Created by Zachary Lineman on 10/31/22.
//

#include "Graphics.hpp"
#include "../BitMapFont.hpp"
//Graphics::Graphics() { }
Graphics::Graphics(LCD *lcd) {
    this->lcd = lcd;
}

void Graphics::writeText(const char *text, uint8_t page, uint8_t column) {
    uint8_t textWidth = 0;

    for(int x = 0; x < strlen(text); x ++) {
        int fontIndex = (int) ((unsigned char) text[x]);

        unsigned char characterMap[4] = {
                BitMapFont::font4x6[fontIndex][0],
                BitMapFont::font4x6[fontIndex][1],
                BitMapFont::font4x6[fontIndex][2],
                BitMapFont::font4x6[fontIndex][3],
        };

        if (column >= lcd->screenSizeX) return;

        lcd->displayBitMap(characterMap, 4, 8, page, column);
        column += 4;
        textWidth += 4;
    }
}

void Graphics::setDefaultSections(const char* newSectionTitles[9]) {
    for (uint8_t x = 0; x < 9; x++) {
        this->sectionTitles[x] = newSectionTitles[x];
    }
}

void Graphics::displaySectionHeaders() {
    int page = 0;
    int column = 0;
    int rowCounter = 0;

    const int sectionWidth = LCD::screenSizeX / sectionsPerRow;

    for (uint8_t sectionNumber = 0; sectionNumber < numberOfSections; sectionNumber++) {
        const char* title = sectionTitles.at(sectionNumber);
        uint8_t length = strlen(title) * 4;
        uint8_t padding = (sectionWidth - length) / 2;

        column += padding;

        writeText(title, page, column);

        column += length;
        column += padding;

        rowCounter ++;
        if(rowCounter >= sectionsPerRow) {
            rowCounter = 0;
            column = 0;
            page += 2;
        }

        if (column >= lcd->screenSizeX) {
            column = 0;
        }
    }
}

void Graphics::setTextForSection(uint8_t section, const char* text) {
    const int sectionWidth = lcd->screenSizeX / sectionsPerRow;
    int sectionRow = section / sectionsPerRow;
    int sectionPage = (sectionRow * 3) + 1;
    int sectionColumn = (section - (sectionRow * sectionsPerRow) - 1) * sectionWidth;

    uint8_t length = strlen(text) * 4;
    uint8_t padding = (sectionWidth - length) / 2;

    sectionColumn += padding;

    writeText(text, sectionPage, sectionColumn);
}