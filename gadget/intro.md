## Gadget Intro

This file includes serveral testing patterns created with glut

### Contents
0 - Before starting
1 - Full Screen Grid : grid.cpp

### Before starting

#### Prepare the environment
All the gadgets are based on cplusplus. Make sure to install `freeglut` and `g++` on your computer.

```sh
sudo apt-get install g++ build-essential libgl1-mesa-dev freeglut3-dev
```
#### Complie the source code

You need to add `-lGL -lGLU -lglut` command to link the library
The sample is listed below: 
```sh
g++ -o main main.cpp -lglut -lGL -lGLU
```

#### Run the code

Please google it!

### 1 - Full Screen Grid:
This gadget will create a pattern for distortion test on lens.

It is suitable for a FHD screen (1920 * 1080) on windows system. It creates a full screen grid. Each square is 20 pixel * 20 pixel.

There are stll some problems on Linux or on a non-FHD screen.

