SMB Level Workshop 2 (WS2)
==========================

Another installment in Super Monkey Ball hacking - SMB Level Workshop 2 is a stage editor for SMB 1 and 2.

WS2 is very early in development for now, and is not at all useable as a level editor. It may not be ready for a while, with me needing to learn and apply modern OpenGL and Qt 5. In the meanwhile, I refer you to the [original SMB Level Workshop](https://github.com/CraftedCart/SMBLevelWorkshop).

## Why? Wasn't the original SMB Level Workshop good enough?

The original SMB Level Workshop has been quite the learning experience for me, in what is good to do and especially what not to do; the what not to do part however comes from experience maintaining it. The original one has become quite the pain to maintain with little seperation between the UI and the logic of the editor, fiddly at best and buggy at worst UI code, outdated, slow OpenGL, and issues with LWJGL 2.9. I decided that it would be better to move on and start afresh.

## Building

### Dependencies

- Qt 5.9
    - Qt Core
    - Qt Widgets
- OpenGL
- GLEW
- GLM
- Assimp

### Building

#### With Ninja

```shell
mkdir build
cd build
cmake -G "Ninja" -DCMAKE_BUILD_TYPE="RelWithDebInfo" ..
ninja
```

#### With Make

```shell
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE="RelWithDebInfo" ..
make
```

#### With Visual Studio

I'm not familiar with Visual Studio. Apparently it has CMake support now - You could try opening this CMake project in Visual Studio.

Alternatively, the following should generate a Visual Studio project for Visual Studio 2010. Replace the 10 with your version of Visual Studio.
```shell
mkdir build
cd build
cmake -G "Visual Studio 10" ..
```

