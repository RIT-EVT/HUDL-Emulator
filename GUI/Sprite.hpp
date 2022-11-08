/*
    Sprite.hpp
    Zachary lineman
    11/3/21
    
    =================
    Sprite Header Class
    Represents a sprite that is displayed on the screen. Sprites have their own shader, position, dimensions, rotation and texture.
    These all effect how the sprite is displayed.
    =================
*/

#ifndef OLYMPUS_SPRITE_HPP
#define OLYMPUS_SPRITE_HPP

// GLM Math
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>
#include <string>
#include <GL/glew.h>

#include "Shader.hpp"

class Sprite {
public:
    enum SamplingType { nearest, linear, nearestMipMapNearest, linearMipMapNearest, nearestMipmapLinear, linearMipmapLinear};

    Sprite(std::string id, std::string texturePath, SamplingType samplingType, glm::vec3 position = {300.0f, 300.0f, 0.0f}, glm::vec2 dimensions = {50.0f, 50.0f}, glm::vec3 rotation = {0.0f, 0.0f, 0.0f});
    Sprite();

    std::string id;

    bool immovable = false;

    /*
     * Sprite Position
     */
    glm::vec3 position { };

    /*
     * Sprite Rotation
     */
    glm::vec3 rotation { };

    /*
     * Sprite Size
     */
    glm::vec2 dimensions { };

    /*
     * Sprite Tint Color
     */
    glm::vec4 tintColor { 1.0, 1.0, 1.0, 1.0};

    /*
     * Register Sprite
     */
    void registerSprite();

    /*
     * Drawing Functions
     */
    void draw();

    void setTexture(const std::string& texturePath);
    SamplingType samplingType = linear;

    void updateScreenDimensions(int width, int height);

    Shader shader;

    bool hidden = false;
private:
    glm::mat4 projection { };
    glm::vec2 screenSize { };

    void createTexture(const std::string& texturePath, SamplingType samplingType);
    void createVirtualBufferObject();

    GLuint VBO, VAO, EBO, textureAtlas;

    static GLint translateSamplingType(SamplingType samplingType);
};

#endif //OLYMPUS_SPRITE_HPP
