//
// Created by Zachary Lineman on 11/7/22.
//

#ifndef HUDLEMULATOR_WINDOW_HPP
#define HUDLEMULATOR_WINDOW_HPP

#include <iostream>

// OpenGL
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Sprite.hpp"

class Window {
private:
    /// The GLFW window that this class wraps.
    GLFWwindow *window;
    /// The pixel width of the window.
    int width { 1280 };
    /// The pixel height of the window.
    int height { 640 };

    vector<Sprite*> sprites;

    const char *defaultVertexShader = R""""(
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texturePosition;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    TexCoord = texturePosition;
}
)"""";

    const char *defaultFragmentShader = R""""(
#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, TexCoord);
}
)"""";
    Shader defaultSpriteShader = Shader("Scene Sprite Shader", defaultVertexShader, defaultFragmentShader);
public:
    Window();

    void addSprite(Sprite* sprite);

    /// Called to start the window loop. This will display the window onto screen.
    void startWindowLoop();

    std::function<void()> update{};
};


#endif //HUDLEMULATOR_WINDOW_HPP
