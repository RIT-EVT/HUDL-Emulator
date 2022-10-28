//
// Created by Zachary Lineman on 10/24/22.
//

#ifndef HUDLEMULATOR_LCD_HPP
#define HUDLEMULATOR_LCD_HPP

#include <Hephaestus/Sprite/Sprite.hpp>
#include "../BitMapFont.hpp"

/*
* This class represents the structure to command a GLCD with
* a ST7565 controller.
*/
class LCD {
public:
    /// bit map to display the LCD's current state
    static const int screenSizeX = 128;
    static const int screenSizeY = 64;
    Sprite screen[screenSizeX][screenSizeY];

    /*
    * Constructor for the LCD class
    *
    * @param[in]
    *
    */
    LCD();

    /// function to write data to the LCD
    void dataWrite(unsigned char data);

    /// writes commands to the LCD
    void commWrite(unsigned char data);

    /// drives a particular pixel on the LCD
    void drivePixel(unsigned char page, unsigned char col_up, unsigned char col_low, unsigned char data);

    /// clears the LCD, changes are mirrored in the bitmap
    void clearLCD(unsigned char* bitMap);

    /// displays the map for diagnostic purposes
    void displayMap(unsigned char* bitMap);

    void writeText(string text, unsigned char page, unsigned char col_up, unsigned char col_low);

    void displayBitMap(uint8_t * bitMap, uint8_t bitMapWidth, uint8_t bitMapHeight, uint8_t page, uint8_t column);

    /// initializes the LCD for operation (must be called to use the LCD)
    void initLCD();
private:
    uint8_t emulationPageOffset = 176;
    bool isWriting = false;
    bool writePageAddress = false;
    bool writeColumnUpperAddress = false;
    bool writeColumnLowerAddress = false;

    uint8_t lastPageAddress = 0x00;
    uint8_t lastColumnAddress = 0x00;
};

#endif //HUDLEMULATOR_LCD_HPP
