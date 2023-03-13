/*
 * Window.hpp
 * This file is the implementations for the Window abstraction class.
 *
 * This code is derived from https://github.com/ActuallyZach/Hephaestus.
 *
 * Created by Zachary Lineman on 11/7/22.
 */

#include "Window.hpp"
Window::Window() {
    // Initialize GLFW library
    if (!glfwInit()) {
        exit(-9);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    window = glfwCreateWindow(width / 2, height / 2, "HUDL Emulator", NULL, NULL);

    if (!window) {
        glfwTerminate();
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;

    // Start GLEW extension handler
    glewInit();

    glViewport(0, 0, width, height);

    defaultSpriteShader.setup();
}

void Window::addSprite(Sprite *sprite) {
    sprite->updateScreenDimensions(width, height);

    sprite->shader = defaultSpriteShader;
    sprites.push_back(sprite);
}

void Window::process() {
    if(glfwWindowShouldClose(window)) {
        glfwTerminate();
    }

    glfwGetWindowSize(window, &width, &height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(Sprite* sprite: sprites) {
        sprite->draw();
    }

    // Swap front and back buffers
    glfwSwapBuffers(window);
    // Poll for and process events
    glfwPollEvents();
}