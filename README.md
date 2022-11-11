# HUDL-Emulator
An emulator for the EVT-HUDL device. Very rudimentary and does not actually emulate the full device. Just enough functionality for the emulator to display data.

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