# HUDL-Emulator 🤯 DOOM MODE 🤯
An emulator for the EVT-HUDL device. Very rudimentary and does not actually emulate the full device. Just enough functionality for the emulator to display data.

## What is 🤯 DOOM MODE 🤯???
🤯 DOOM MODE 🤯 is a port of DOOM 1 (1993) to the HUDL display. This source port uses the [doomgeneric](https://github.com/ozkl/doomgeneric) port for its internal game. The majority of work for porting is found in main.cpp, and consists of getting the game actually displaying on the HUDL.

### To get 🤯 DOOM MODE 🤯 running
To get 🤯 DOOM MODE 🤯 running, you will need to add `-iwad ./doom/DOOM1.wad` as your program arguments. This tells the core doom game where the WAD file is that it should play off of. You will need to supply your own WAD file to get the emulator up and running.

#### Changes to Doomgeneric
Some changes are needed to Doomgeneric to allow it to interoperate with C++. The changes are all in one file, for ease this file has been provided below, just replace `doom/doomgeneric/doomgeneric/doomgeneric.h` (that's a lot of doomgeneric) with this file.
```c
#ifndef DOOM_GENERIC
#define DOOM_GENERIC

#include <stdlib.h>
#include <stdint.h>

#define DOOMGENERIC_RESX 320
#define DOOMGENERIC_RESY 200

extern uint32_t* DG_ScreenBuffer;

#ifdef __cplusplus
extern "C" {
    void DG_Init();
    void DG_DrawFrame();
    void DG_SleepMs(uint32_t ms);
    uint32_t DG_GetTicksMs();
    int DG_GetKey(int* pressed, unsigned char* key);
    void DG_SetWindowTitle(const char * title);
};
#else
void DG_Init();
void DG_DrawFrame();
void DG_SleepMs(uint32_t ms);
uint32_t DG_GetTicksMs();
int DG_GetKey(int* pressed, unsigned char* key);
void DG_SetWindowTitle(const char * title);
#endif
#endif //DOOM_GENERIC
```

## Install Dependencies
To install and run the emulator, you need a few libraries.

1. [GLFW3](#installing-glfw3)
2. [GLEW](#installing-glew)
3. [GLM](#installing-glm)

### Installing [GLFW3](https://www.glfw.org/)
#### MacOS
On macOS you can use the brew package manager to install GLFW.

```
brew install glfw
```

#### Linux
On linux you can use apt-get to install GLFW

```
sudo apt-get install libglfw3
sudo apt-get install libglfw3-dev
```

#### Windows
1. Download the latest windows binary from https://www.glfw.org/download.html and unzip the package.

2. Follow the information in the README to install GLFW.

### Installing [GLEW](https://glew.sourceforge.net/)
#### MacOS
On macOS you can use the brew package manager to install GLEW.

```
brew install glew
```

#### Linux
On linux you can use apt-get to install GLEW

```
sudo apt -y install libglew-dev
```


#### Windows
To install GLEW on windows install the instructions on the [GLEW Website](https://glew.sourceforge.net/install.html)


### Installing [GLM](https://github.com/g-truc/glm)
#### MacOS
On macOS you can use the brew package manager to install GLM.

```
brew install glm
```

#### Linux
On Linux you can use apt-get to install GLM.
```
sudo apt-get install libglm-dev
```

#### Windows
Download the latest release from the GLM github, https://github.com/g-truc/glm. Follow the `manual.md` file to install GLM.

If you need help, you can check out: https://stackoverflow.com/a/17912620/14886210