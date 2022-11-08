#include <iostream>
#include "GUI/Window.hpp"
#include "GUI/Sprite.hpp"

#include "LCD Emulator/LCD.hpp"

//Hephaestus engine = Hephaestus("EVT HUDL Display", 1280, 640);
LCD lcd = LCD();
Camera camera = Camera();

void init() {
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);
}

void update() {
    const char* text =  "Hello World!";
    char* titles[9] = {
            "Voltage", "Battery %", "MPH",
            "NULL", "NULL", "NULL",
            "NULL", "NULL", "NULL",
    };
    lcd.setDefaultSections(titles);

    lcd.displaySectionHeaders();
    lcd.setTextForSection(0, "3.2 v");
    lcd.setTextForSection(1, "85%");
}

int main() {
    Window mainWindow = Window();
    mainWindow.currentCamera = &camera;
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