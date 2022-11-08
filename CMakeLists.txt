cmake_minimum_required(VERSION 3.23)
project(HudlEmulator)

set(CMAKE_CXX_STANDARD 14)
set(HEPHAESTUS_DIR ./libs/Hephaestus/)

add_executable(HudlEmulator main.cpp "LCD Emulator/LCD.cpp" "LCD Emulator/LCD.hpp" "LCD Emulator/BitMapFont.hpp" GUI/Window.cpp GUI/Window.hpp GUI/Shader.cpp GUI/Shader.hpp GUI/Sprite.cpp GUI/Sprite.hpp GUI/stb_image.hpp)

# Handle Dependencies
if(UNIX AND NOT APPLE)
    find_package(OpenGL REQUIRED)

    target_link_libraries(${PROJECT_NAME}
            PUBLIC OpenGL
            )
endif()

find_package(glm REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

target_link_libraries(${PROJECT_NAME}
        PUBLIC glfw
        PUBLIC GLEW::GLEW
        PUBLIC glm::glm
        )
#
### Link to the EVT-core library
#add_subdirectory(${EVT_CORE_DIR} ./libs/EVT-Core)
#
#include(CMakeDependentOption)
#include(${EVT_CORE_DIR}/cmake/evt-core_compiler.cmake)
#include(${EVT_CORE_DIR}/cmake/evt-core_install.cmake)
#
## Link the EVT-core library
#target_link_libraries(HudlEmulator EVT-Core)
#
add_custom_command(TARGET HudlEmulator PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Resources $<TARGET_FILE_DIR:HudlEmulator>)
