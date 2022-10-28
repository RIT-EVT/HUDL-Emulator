//
// Created by Zachary Lineman on 10/21/22.
//

#ifndef HUDLEMULATOR_LCDEMULATORSCENE_HPP
#define HUDLEMULATOR_LCDEMULATORSCENE_HPP
#include <Hephaestus/Hephaestus.hpp>
#include <Hephaestus/Camera/Camera.hpp>
#include <Hephaestus/Sprite/Sprite.hpp>
#include "LCD Emulator/LCD.hpp"

class LCDEmulatorScene {
    Hephaestus* engine;
    LCD lcd;
    const int spriteSize = 10;

    int interval = 0;

    Camera camera = Camera();

    void init();
    void destroy();
    void tick();
    void update();
    void render();
    void updateScreen(unsigned char*);

public:
    explicit LCDEmulatorScene(Hephaestus *_engine);
    uint8_t* bitMap;

    Scene scene = Scene();
};

#endif //HUDLEMULATOR_LCDEMULATORSCENE_HPP
