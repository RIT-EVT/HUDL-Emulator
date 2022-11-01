cmake_minimum_required(VERSION 3.23)
project(HudlEmulator)

set(CMAKE_CXX_STANDARD 14)
set(HEPHAESTUS_DIR ./libs/Hephaestus/)

add_executable(HudlEmulator main.cpp LCDEmulatorScene.cpp LCDEmulatorScene.hpp "LCD Emulator/LCD.cpp" "LCD Emulator/LCD.hpp" BitMapFont.hpp "Graphics Library/Graphics.cpp" "Graphics Library/Graphics.hpp")

add_subdirectory(${HEPHAESTUS_DIR})
target_link_libraries(HudlEmulator Hephaestus)
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
