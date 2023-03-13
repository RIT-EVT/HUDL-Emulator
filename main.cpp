/*
 * This is an emulator for the HUDL device on the RIT-EVT motorcycles.
 * It works by translating the write commands that you would normally send to the lcd
 * into instructions on where to draw on the virtual canvas. Any code written for the emulator
 * should work with very minimal changes on the actual displays. Currently, you must copy and paste between
 * the two LCD.cpp and LCD.hpp files to transfer your code.
 *
 * Created by Zachary Lineman, November 2022
 */

/*
 * DOOM MODE
 *
 * This version of the HUDL code has a port of doom included within it.
 * To successfully launch the DOOM game you will need to add DOOM1.wad into the folder doom in the resources folder
 * We can not distribute the WAD file alongside the repo.
 */


#include <libc.h>
#include "doom/doomgeneric/doomgeneric/doomgeneric.h"

#include "LCD Emulator/LCD.hpp"
#include "GUI/Window.hpp"
#include "GUI/Sprite.hpp"
#include "LCD Emulator/Graphics.hpp"

LCD lcd = LCD();
Graphics graphics = Graphics(lcd);
Window mainWindow;
//GLFWwindow *doomWindow;

uint8_t compressPixel(uint32_t pixel) {
    uint8_t red = pixel >> 16;
    uint8_t green = pixel >> 8;
    uint8_t blue = pixel >> 0;
    uint8_t transp = pixel >> 24;

    return (blue + green + red) / 3;
}

void update() {
    int x = 0;
    int y = 0;

    int pixelSize = 3;
    for (int i = 1; i < DOOMGENERIC_RESY - 1; i++) {
        if (i % pixelSize == 0) {
            for (int j =  1; j < DOOMGENERIC_RESX - 1; j++) {
                if (j % pixelSize == 0) {
                    if (y < LCD::screenSizeY && x < LCD::screenSizeX) {
                        uint32_t currentFlatIndex = i * DOOMGENERIC_RESX + j;

                        uint32_t pixel1Up = DG_ScreenBuffer[(currentFlatIndex - DOOMGENERIC_RESX)];
                        uint32_t centerPixel = DG_ScreenBuffer[currentFlatIndex];
                        uint32_t pixel1Down = DG_ScreenBuffer[(currentFlatIndex + DOOMGENERIC_RESX)];

                        uint32_t pixel1Left = DG_ScreenBuffer[(currentFlatIndex - 1)];
                        uint32_t pixel1Right = DG_ScreenBuffer[(currentFlatIndex + 1)];

                        uint8_t pixel1Value = compressPixel(pixel1Up);
                        uint8_t pixel2Value = compressPixel(centerPixel);
                        uint8_t pixel3Value = compressPixel(pixel1Down);
                        uint8_t pixel4Value = compressPixel(pixel1Left);
                        uint8_t pixel5Value = compressPixel(pixel1Right);

                        uint8_t combinedValue = (pixel1Value + pixel2Value + pixel3Value + pixel4Value + pixel5Value) / 5;

                        if (combinedValue < 128) {
                            graphics.setPixel(x, y, false);
                        } else {
                            graphics.setPixel(x, y, true);
                        }
                    }
                    x ++;
                }
            }
            y ++;
            x = 0;
        }
    }
}

//void drawDoomWindow() {
//    if(glfwWindowShouldClose(doomWindow)) {
//        glfwTerminate();
//    }
//
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//    glDrawPixels(DOOMGENERIC_RESX, DOOMGENERIC_RESY, GL_BGRA, GL_UNSIGNED_BYTE, DG_ScreenBuffer);
//    glfwSwapBuffers(doomWindow);
//    glfwWaitEvents();
//}

extern "C" {
void DG_Init() {
//    if (!glfwInit()) {
//        exit(-9);
//    }
//
//    doomWindow = glfwCreateWindow(DOOMGENERIC_RESX / 2, DOOMGENERIC_RESY / 2, "Doom", NULL, NULL);
//    glfwMakeContextCurrent(doomWindow);
//    glewInit();

    mainWindow.update = &update;

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
}


void DG_DrawFrame() {
    update();
    graphics.updateDisplay();
    mainWindow.process();
//    drawDoomWindow();
}

void DG_SleepMs(uint32_t ms) {
    usleep (ms * 1000);
}

uint32_t DG_GetTicksMs() {
    struct timeval  tp;
    struct timezone tzp;

    gettimeofday(&tp, &tzp);

    return (tp.tv_sec * 1000) + (tp.tv_usec / 1000); /* return milliseconds */
}

int DG_GetKey(int* pressed, unsigned char* doomKey) {
    return 0;
}

void DG_SetWindowTitle(const char * title) {
//    glfwSetWindowTitle(doomWindow, title);
}
}
