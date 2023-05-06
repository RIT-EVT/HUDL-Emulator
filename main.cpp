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

#include "LCD Emulator/LCD.hpp"

#define BALL_SIZE 1
#define NUMBER_OF_BALLS 10
#define FRICTION 0.7f
#define BALL_FRICTION 0.1f
#define WALL_FRICTION 0.3f
#define REST_GRACE 0.2f

LCD lcd = LCD();

void init() {
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);
}

struct Ball {
    float x = 0;
    float y = 0;
    float velX = 0;
    float velY = 0;

    Ball() = default;

    void process() {
        x += velX;
        y += velY;
    }

    void applyGravity(float xGravity, float yGravity) {
        velY += yGravity * FRICTION;
        velX += xGravity * FRICTION;
    }

    void applyBounds() {
        if (x < 0) {
            x = 0;
            velX = -velX * WALL_FRICTION;
        }
        if (x > 128 - BALL_SIZE) {
            x = 128 - BALL_SIZE;
            velX = -velX * WALL_FRICTION;
        }
        if (y < 0) {
            y = 0;
            velY = -velY * WALL_FRICTION ;
        }
        if (y > 64 - BALL_SIZE) {
            y = 64 - BALL_SIZE;
            velY = -velY * WALL_FRICTION;
        }
        if (velX < REST_GRACE && velX > -REST_GRACE) {
            velX = 0;
        }
        if (velY < REST_GRACE && velY > -REST_GRACE) {
            velY = 0;
        }
    }

    void clear(LCD& lcd) const {
        for (int x1 = (int) x; x1 < (int) x + BALL_SIZE; x1 ++) {
            for (int y1 = (int) y; y1 < (int) y + BALL_SIZE; y1 ++) {
                lcd.setPixel(x1, y1, false);
            }
        }
    }

    void display(LCD& lcd) const {
        for (int x1 = (int) x; x1 < (int) x + BALL_SIZE; x1 ++) {
            for (int y1 = (int) y; y1 < (int) y + BALL_SIZE; y1 ++) {
                lcd.setPixel(x1, y1, true);
            }
        }
    }
};

Ball balls[NUMBER_OF_BALLS];
int16_t gravity = 1;

void update() {
    // Clear the last ball
    for(Ball& ball : balls) {
        ball.clear(lcd);

        ball.applyGravity(0.3, 0.3);
        ball.process();
    }

    for(int i = 0; i < NUMBER_OF_BALLS; i ++) {
        for(int j = i + 1; j < NUMBER_OF_BALLS; j ++) {
            if (balls[i].x == balls[j].x) {
                balls[j].x --;
                balls[j].velX = -balls[j].velX * BALL_FRICTION;
            }
            if (balls[j].y == balls[i].y) {
                balls[j].y --;
                balls[j].velY = -balls[j].velY * BALL_FRICTION;
            }
        }
    }

    for(Ball& ball : balls) {
        ball.applyBounds();
        ball.display(lcd);
    }

    lcd.updateDisplay();
}

int main() {
    Window mainWindow = Window();
    mainWindow.update = update;

    init();
    const int spriteSize = 10;

    for(int i = 0; i < NUMBER_OF_BALLS; i ++) {
        balls[i] = Ball();

        balls[i].x = static_cast<float >((rand() % static_cast<int>(127 + 1)));
        balls[i].y = static_cast<float>((rand() % static_cast<int>(64 + 1)));
    }

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