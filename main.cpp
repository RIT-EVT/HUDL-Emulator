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


#include <stdio.h>
#include <libc.h>
#include "doom/doomgeneric/doomgeneric/doomgeneric.h"

#include "LCD Emulator/LCD.hpp"
#include "GUI/Window.hpp"
#include "GUI/Sprite.hpp"
#include "doom/imgui/imgui.h"
#include "doom/imgui/imgui_impl_glfw.h"
#include "doom/imgui/imgui_impl_opengl3.h"

LCD lcd = LCD();
Window mainWindow;

static int pixelCutoff = 50;
static int pixelSize = DOOMGENERIC_RESY / LCD::screenSizeY;
static float redWeight = 1;
static float blueWeight = 0.7;
static float greenWeight = 1.2;

/**
 * Runs the Dear Imgui process used for realtime variable editing while the game is playing.
 */
void imguiProcess() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Editor");

    ImGui::Text("Edit settings for the doom port using this UI.");

    ImGui::SliderInt("cut-off", &pixelCutoff, 0, 255);
    ImGui::SliderFloat("Blue Px Weight", &blueWeight, 0, 2);
    ImGui::SliderFloat("Green Px Weight", &greenWeight, 0, 2);
    ImGui::SliderFloat("Red Px Weight", &redWeight, 0, 2);

    if (ImGui::Button("Increase Pixel Size"))
        pixelSize ++;

    if (ImGui::Button("Decrease Pixel Size"))
        pixelSize --;

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::End();

    ImGui::Render();
}

uint8_t compressPixel(uint32_t pixel) {
    uint8_t red = pixel >> 16;
    uint8_t green = pixel >> 8;
    uint8_t blue = pixel >> 0;
    uint8_t transp = pixel >> 24;

    /*
     * Uncomment to enable color weighting. Could lead to better colors with trial and error.
     */
//    float dRed = (float) red * redWeight;
//    float dBlue = (float) blue * blueWeight;
//    float dGreen = (float) green * greenWeight;
//
//    blue = dBlue;
//    red = dRed;
//    green = dGreen;

    return (blue + green  + red) / 3;
}

void update() {
    int x = 0;
    int y = 0;

    for (int i = 1; i < DOOMGENERIC_RESY - 1; i++) {
        if (i % pixelSize == 0) {
            for (int j =  1; j < DOOMGENERIC_RESX - 1; j++) {
                if (j % pixelSize == 0) {
                    if (y < LCD::screenSizeY && x < LCD::screenSizeX) {
                        uint32_t currentFlatIndex = i * DOOMGENERIC_RESX + j;

                        /*
                         * Everything commented out here is an ~ algorithm ~ that averages the surrounding pixels to
                         * get a more accurate representation of the game. It is not an amazing algorithm by any standard,
                         * so it doesn't really do much. It is mostly here as a reminder that I need to actually write
                         * a useful algorithm for this.
                         */
//                        uint32_t pixel1Up = DG_ScreenBuffer[(currentFlatIndex - DOOMGENERIC_RESX)];
                        uint32_t centerPixel = DG_ScreenBuffer[currentFlatIndex];
//                        uint32_t pixel1Down = DG_ScreenBuffer[(currentFlatIndex + DOOMGENERIC_RESX)];

//                        uint32_t pixel1Left = DG_ScreenBuffer[(currentFlatIndex - 1)];
//                        uint32_t pixel1Right = DG_ScreenBuffer[(currentFlatIndex + 1)];
//
//                        uint8_t pixel1Value = compressPixel(pixel1Up);
                        uint8_t pixel2Value = compressPixel(centerPixel);
//                        uint8_t pixel3Value = compressPixel(pixel1Down);
//                        uint8_t pixel4Value = compressPixel(pixel1Left);
//                        uint8_t pixel5Value = compressPixel(pixel1Right);
//
//                        uint8_t combinedValue = (pixel1Value + pixel2Value + pixel3Value + pixel4Value + pixel5Value) / 5;

                        if (pixel2Value < pixelCutoff) {
                            lcd.setPixel(x, y, false);
                        } else {
                            lcd.setPixel(x, y, true);
                        }

                        x ++;
                    }
                }
            }
            y ++;
            x = 0;
        }
    }
}

/**
 * This majority of the code is wrapped in extern "C" to make it available to the DOOM source code.
 * If this is not wrapped in extern "C" the C++ compiler will alter the function names, making them
 * un-findable by C which searches off of the symbol name (function name).
 */
extern "C" {
/**
 * The Doomgeneric initializer. Setup the main window and IMGUI.
 * When moved to the actual HUDL, this is where all the HUDL main function code will be placed.
 */
void DG_Init() {
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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    const char* glsl_version = "#version 330";
    ImGui_ImplGlfw_InitForOpenGL(mainWindow.window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

/**
 * This is called everytime DOOM renders a new frame. The important function here is `update()` which
 * is what runs the downsizing and displays the frame buffer to the HUDL.
 */
void DG_DrawFrame() {
    update();
    lcd.updateDisplay();

    // Can be removed for actual HUDL.
    imguiProcess();
    mainWindow.process();
}

/**
 * Provides a sleep function for DOOM. This will need to be converted to an EVT-Core wait function.
 * @param ms the time to sleep in MS.
 */
void DG_SleepMs(uint32_t ms) {
    usleep (ms * 1000);
}

/**
 * Return the number of MS since the device booted.
 * @return the number of MS since boot.
 */
uint32_t DG_GetTicksMs() {
    struct timeval  tp;
    struct timezone tzp;

    gettimeofday(&tp, &tzp);

    return (tp.tv_sec * 1000) + (tp.tv_usec / 1000); /* return milliseconds */
}

/**
 * I honestly do not know, gonna have to look into this.
 * @param pressed
 * @param doomKey
 * @return
 */
int DG_GetKey(int* pressed, unsigned char* doomKey) {
    return 0;
}

/**
 * Sets the window title. Will not be used for the HUDL.
 * @param title the title to set,
 */
void DG_SetWindowTitle(const char * title) {
    glfwSetWindowTitle(mainWindow.window, title);
}
}
