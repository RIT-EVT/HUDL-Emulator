//
// Created by Zachary Lineman on 10/31/22.
//

#ifndef HUDLEMULATOR_GRAPHICS_HPP
#define HUDLEMULATOR_GRAPHICS_HPP


#include <cstdint>
#include "../LCD Emulator/LCD.hpp"

class Graphics {
    static const uint8_t numberOfSections = 9;
    static const uint8_t sectionsPerRow = 3;

    LCD *lcd = nullptr;
    vector<const char*> sectionTitles = {
            "NULL", "NULL", "NULL",
            "NULL", "NULL", "NULL",
            "NULL", "NULL", "NULL",
    };
public:
    explicit Graphics(LCD* lcd);

    void writeText(const char* text, uint8_t page, uint8_t column);
    void setDefaultSections(const char* newSectionTitles[9]);
    void displaySectionHeaders();
    void setTextForSection(uint8_t x, const char* text);
};


#endif //HUDLEMULATOR_GRAPHICS_HPP
