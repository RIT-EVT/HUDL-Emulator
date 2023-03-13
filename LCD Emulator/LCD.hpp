#ifndef _EVT_LCD_
#define _EVT_LCD_

#include <cstdint>
#include "BitmapFont.hpp"
#include "../GUI/Sprite.hpp"

#define ADCSELECT 0xA0
#define DISPLAYOFF 0xAE
#define COMDIRSCAN 0xC8
#define LCDBIASET 0xA2
#define POWERCONTROLSET 0x2F
#define RESRATIOSET 0x26
#define ELECTRONICVOLCOMMAND 0x81
#define ELECTRONICVOLVALUE 0x11
#define DISPLAYON 0xAF
#define DISPLAYOFF 0xAE

#define MAX_SECTIONS 12
#define MAX_SECTION_HEIGHT 8
#define MAX_SECTION_PER_ROW 3

class LCD {
public:
    static const uint8_t screenSizeX = 128;
    static const uint8_t screenSizeY = 64;

    /// bit map to display the LCD's current state
    Sprite screen[screenSizeX][screenSizeY];

    /*
    * Constructor for the LCD class
    *
    * @param[in]
    *
    */
    LCD();

    /// initializes the LCD for operation (must be called to use the LCD)
    void initLCD();

    /// function to write data to the LCD
    void dataWrite(uint8_t data);

    /// writes commands to the LCD
    void commandWrite(uint8_t data);
private:
    // Emulation Values
    uint8_t emulationPageOffset = 176;
    bool isWriting = false;
    bool writePageAddress = false;
    bool writeColumnUpperAddress = false;
    bool writeColumnLowerAddress = false;

    uint8_t lastPageAddress = 0x00;
    uint8_t lastColumnAddress = 0x00;
};

#endif
