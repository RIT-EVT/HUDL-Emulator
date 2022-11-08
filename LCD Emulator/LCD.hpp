//
// Created by Zachary Lineman on 10/24/22.
//

#ifndef HUDLEMULATOR_LCD_HPP
#define HUDLEMULATOR_LCD_HPP

#include "BitMapFont.hpp"
#include "../GUI/Sprite.hpp"

/*
* This class represents the structure to command a GLCD with
* a ST7565 controller.
*/
class LCD {
public:
    static const int screenSizeX = 128;
    static const int screenSizeY = 64;
    /// bit map to display the LCD's current state
    Sprite screen[screenSizeX][screenSizeY];

    /*
    * Constructor for the LCD class
    *
    * @param[in]
    *
    */
    LCD();

    /**
     * Constructor the LCD class
     *
     * @param[in] regSelect Register select pin
     * @param[in] reset Reset pin
     * @param[in] spi SPI class for communication
     * @param[in] bitMap bitmap to display to the LCD
     */
//    LCD(EVT::core::IO::GPIO& regSelect, EVT::core::IO::GPIO& reset, EVT::core::IO::SPI& spi);
    /**
     *
     * @param[in] page Page address to write to
     * @param[in] colUp Bits to write to the page address
     * @param[in] colLow Bits to write to the column select
     */
    void driveColumn(uint8_t page, uint8_t colUp, uint8_t colLow, uint8_t data);

    /**
     * Clears the LCD, changes are mirrored in the bitmap
     *
     * @param[in] bitMap Bitmap to be displayed
     */
    void clearLCD(const uint8_t* bitMap);

    void clearArea(uint8_t width, uint8_t height, uint8_t page, uint8_t column);

    /**
     * Displays the map for diagnostic purposes
     *
     * @param[in] bitMap Bitmap to be displayed
     */
    void displayMap(const uint8_t* bitMap);

    void displayBitMap(uint8_t * bitMap, uint8_t bitMapWidth, uint8_t bitMapHeight, uint8_t page, uint8_t column);

    void writeText(const char* text, uint8_t page, uint8_t column);
    void setDefaultSections(char* newSectionTitles[9]);
    void displaySectionHeaders();
    void setTextForSection(uint8_t section, const char* text);

    /// initializes the LCD for operation (must be called to use the LCD)
    void initLCD();
private:
    static const uint8_t numberOfSections = 9;
    static const uint8_t sectionsPerRow = 3;

    /// function to write data to the LCD
    void dataWrite(uint8_t data);

    /// writes commands to the LCD
    void commandWrite(uint8_t data);


    // Emulation Values
    uint8_t emulationPageOffset = 176;
    bool isWriting = false;
    bool writePageAddress = false;
    bool writeColumnUpperAddress = false;
    bool writeColumnLowerAddress = false;

    uint8_t lastPageAddress = 0x00;
    uint8_t lastColumnAddress = 0x00;

    char* sectionTitles[9] = {
            "NULL", "NULL", "NULL",
            "NULL", "NULL", "NULL",
            "NULL", "NULL", "NULL",
    };
};

#endif //HUDLEMULATOR_LCD_HPP
