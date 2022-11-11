/*
 * Shader.hpp
 * An abstraction over an OpenGL shader
 *
 * This code is derived from https://github.com/ActuallyZach/Hephaestus.
 *
 * Created by Zachary Lineman on 11/3/21.
 */

#ifndef OLYMPUS_SHADER_HPP
#define OLYMPUS_SHADER_HPP

// iO Streams
#include <iostream>
#include <fstream>
#include <sstream>

// OpenGL
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

class Shader {
public:
    Shader();
    Shader(string identifier);
    Shader(string identifier, string vertexPath, string fragmentPath);
    Shader(string identifier, const char * _vertexShader, const char * _fragmentShader);

    void setup();
    void use();

    void setFloat (const char *name, float value, bool useShader = false);
    void setInteger (const char *name, int value, bool useShader = false);
    void setVector2f (const char *name, float x, float y, bool useShader = false);
    void setVector2f (const char *name, const glm::vec2 &value, bool useShader = false);
    void setVector3f (const char *name, float x, float y, float z, bool useShader = false);
    void setVector3f (const char *name, const glm::vec3 &value, bool useShader = false);
    void setVector4f (const char *name, float x, float y, float z, float w, bool useShader = false);
    void setVector4f (const char *name, const glm::vec4 &value, bool useShader = false);
    void setMatrix4 (const char *name, const glm::mat4 &matrix, bool useShader = false);

private:
    const char *vertexShader;
    const char *fragmentShader;
    string identifier;
    GLuint shaderProgram;

    bool isSetup = false;

};

#endif //OLYMPUS_SHADER_HPP
