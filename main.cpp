/*
 * This is an emulator for the HUDL device on the RIT-EVT motorcycles.
 * It works by translating the write commands that you would normally send to the lcd
 * into instructions on where to draw on the virtual canvas. Any code written for the emulator
 * should work with very minimal changes on the actual displays. Currently, you must copy and paste between
 * the two LCD.cpp and LCD.hpp files to transfer your code.
 *
 * Created by Zachary Lineman, November 2022
 */


#include <iostream>
#include "GUI/Window.hpp"
#include "GUI/Sprite.hpp"

#include "LCD Emulator/Graphics.hpp"
#include "LCD Emulator/LCD.hpp"

LCD lcd = LCD();
Graphics graphics = Graphics(lcd);

void init() {
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);
}

void update() {
    const char* text =  R"( !"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~)";
    char* titles[9] = {
            "B Voltage", "Speed", "RPM",
            "Temp 1", "Temp 2", "Temp 3",
            "Status 1", "Pre Stat", "Torque",
    };
    graphics.setDefaultSections(titles);
    graphics.displaySectionHeaders();

    graphics.setTextForSection(0, "3.2 v");
    graphics.setTextForSection(1, "25 mph");
    graphics.setTextForSection(2, "3000");
    graphics.setTextForSection(3, "40 C");
    graphics.setTextForSection(4, "44 C");
    graphics.setTextForSection(5, "43 C");
    graphics.setTextForSection(6, "ON");
    graphics.setTextForSection(7, "Ready");
    graphics.setTextForSection(8, "100 NM");

//    graphics.writeText(text, 0 ,0, true);

    graphics.updateDisplay();
}

int main() {
    Window mainWindow = Window();
    mainWindow.update = update;

    init();
    const int spriteSize = 10;

    for(int x = 0; x < LCD::screenSizeX; x++) {
        for (int y = 0; y < LCD::screenSizeY; y++) {
            auto position = glm::vec3(x * spriteSize, y * spriteSize, 0.0);
            lcd.screen[x][y] = Sprite{"_", "./Images/White.png",
                                      Sprite::SamplingType::linear,
                                      position,
                                      glm::vec2(spriteSize,spriteSize)};
            lcd.screen[x][y].hidden = true;
            mainWindow.addSprite(&lcd.screen[x][y]);
        }
    }

    mainWindow.startWindowLoop();

    return 0;
}