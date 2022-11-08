//
// Created by Zachary Lineman on 11/7/22.
//

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

    window = glfwCreateWindow(width, height, "HUDL Emulator", NULL, NULL);

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

void Window::startWindowLoop() {
    glfwGetWindowSize(window, &width, &height);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // RENDER

        update();
        for(Sprite* sprite: sprites) {
            sprite->draw();
        }

        glDisable(GL_BLEND);

        // Swap front and back buffers
        glfwSwapBuffers(window);
        // Poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();
}

void Window::addSprite(Sprite *sprite) {
    sprite->updateScreenDimensions(width, height);

    sprite->camera = currentCamera;
    sprite->shader = defaultSpriteShader;
    sprites.push_back(sprite);
}