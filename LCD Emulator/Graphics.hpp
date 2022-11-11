//
// Created by Zachary Lineman on 11/10/22.
//

#ifndef HUDLEMULATOR_GRAPHICS_HPP
#define HUDLEMULATOR_GRAPHICS_HPP

#include "../GUI/Sprite.hpp"
#include "./LCD.hpp"

class Graphics {
    LCD& lcd;

    static const uint8_t numberOfSections = 9;
    static const uint8_t sectionsPerRow = 3;

    char* sectionTitles[9] = {
            "NULL", "NULL", "NULL",
            "NULL", "NULL", "NULL",
            "NULL", "NULL", "NULL",
    };
public:
    explicit Graphics(LCD& lcd);

    void writeText(const char* text, uint8_t page, uint8_t column, bool wrapText);
    void setDefaultSections(char* newSectionTitles[9]);
    void displaySectionHeaders();
    void setTextForSection(uint8_t section, const char* text);

    /**
     * Clears only a certain area on the screen
     */
    void clearArea(uint8_t width, uint8_t height, uint8_t page, uint8_t column);

    /**
     * Clears the LCD, changes are mirrored in the bitmap
     *
     * @param[in] bitMap Bitmap to be displayed
     */
    void clearLCD(const uint8_t* bitMap);


    /**
     * Displays the map for diagnostic purposes
     *
     * @param[in] bitMap Bitmap to be displayed
     */
    void displayMap(const uint8_t* bitMap);
    void displayBitMap(uint8_t * bitMap, uint8_t bitMapWidth, uint8_t bitMapHeight, uint8_t page, uint8_t column);
};


#endif //HUDLEMULATOR_GRAPHICS_HPP
