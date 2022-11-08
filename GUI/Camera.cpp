/*
    Camera.cpp
    Zachary lineman
    11/10/21
    
    =================
    DESCRIPTION
    =================
*/
//typedef void (*Function)();

#include "Camera.hpp"

Camera::Camera() {

}

glm::mat4 Camera::getView() {
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, position);

    return view;
}