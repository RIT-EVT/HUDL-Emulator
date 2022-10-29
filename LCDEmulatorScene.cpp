/**
 * 
 *
 * Created on 10/21/22
 * @author Zachary Lineman
 */

//
// Created by Zachary Lineman on 10/21/22.
//

#include "LCDEmulatorScene.hpp"

LCDEmulatorScene::LCDEmulatorScene() {
    this->lcd = LCD();

    for(int x = 0; x < lcd.screenSizeX; x++) {
        for (int y = 0; y < lcd.screenSizeY; y++) {
            auto position = glm::vec3(x * spriteSize, y * spriteSize, 0.0);
            lcd.screen[x][y] = Sprite{to_string(generateID()), "./Images/Piece2.png",
                                      linear,
                                      position,
                                      glm::vec2(spriteSize,spriteSize)};
            lcd.screen[x][y].hidden = true;
            scene.addSprite(&lcd.screen[x][y]);
        }
    }

    scene.setPhysicsEnabled(false);
    scene.setCollisionsEnabled(false);

    scene.setInit([this] { init(); });
    scene.setDestroy([this] { destroy(); });
    scene.setTick([this] { tick(); });
    scene.setUpdate([this] { update(); });
    scene.setRender([this] { render(); });
}

void LCDEmulatorScene::init() {
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);

    scene.addCamera(&camera, true);
}

void LCDEmulatorScene::destroy() {

}

void LCDEmulatorScene::tick() {

}

void LCDEmulatorScene::update() {
    const char* text =  "Hello World!";
    lcd.writeText(text, 0, 0);
}

void LCDEmulatorScene::render() {

}