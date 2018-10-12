SMB Level Workshop 2 (WS2)
==========================

Linux/macOS builds: [![Travis CI build status](https://travis-ci.org/CraftedCart/smblevelworkshop2.svg?branch=master)](https://travis-ci.org/CraftedCart/smblevelworkshop2)  
Windows builds: [![Appveyor build status](https://ci.appveyor.com/api/projects/status/xgkxkfuh5r8tu9is/branch/master?svg=true)](https://ci.appveyor.com/project/CraftedCart/smblevelworkshop2/branch/master)  
Development downloads: [![Development downloads](https://api.bintray.com/packages/craftedcart/the-workshop/smblevelworkshop2-develop/images/download.svg)](https://bintray.com/craftedcart/the-workshop/smblevelworkshop2-develop/_latestVersion)

---

Another installment in Super Monkey Ball hacking - SMB Level Workshop 2 is a stage editor for SMB 1 and 2.

![SMB Level Workshop 2 Stage Editor screenshot](https://i.imgur.com/X0bNIdN.png)

## Discord

Join the Workshop Discord server over here: https://discord.gg/CEYjvDj

## Why? Wasn't the original SMB Level Workshop good enough?

The original SMB Level Workshop has been quite the learning experience for me, in what is good to do and especially what not to do; the what not to do part however comes from experience maintaining it. The original one has become quite the pain to maintain with little seperation between the UI and the logic of the editor, fiddly at best and buggy at worst UI code, outdated, slow OpenGL, and issues with LWJGL 2.9. I decided that it would be better to move on and start afresh.

## Dependencies

- Qt 5
    - Qt Core
    - Qt Widgets
- OpenGL
- GLEW
- GLM
- Assimp
- Bullet Physics

### Fetching dependencies

#### Windows MSYS2
I recommend using MSYS2 on a Windows system - it makes dependency management quick and easy  
If it's not installed already, grab the installer from http://www.msys2.org/  
Then run the following in a MinGW terminal  
Replace `x86_64` with `i686` if you want to compile for x86 instead of x86_64  

First of all, update everything. Doooo it!!!

```shell
pacman -Syu
```

Quit the terminal and run `pacman -Syu` again if `pacman` instructs you to.

Then proceed to install the required packages.

```shell
pacman -S mingw-w64-x86_64-cmake make mingw-w64-x86_64-gcc mingw-w64-x86_64-qt5 mingw-w64-x86_64-glew mingw-w64-x86_64-glm mingw-w64-x86_64-assimp mingw-w64-x86_64-bullet
```

If you need Git to clone the repo:

```shell
pacman -S git
```

Then proceed to the "Building: With Make, MSYS Makefiles" section

#### macOS Homebrew

If you don't have Xcode installed, fetch that from the App Store first  
I recommend using Homebrew on macOS to manage dependencies. Grab it from https://brew.sh/

```shell
brew install cmake qt5 glew glm assimp bullet
```

Git should come bundled with the Xcode installation.

Then proceed to the "Building: With Make, Unix Makefiles" section

#### Arch Linux (Or Arch based)

```shell
sudo pacman -S cmake qt5 glew glm assimp bullet
```

If you haven't installed `base-devel`, fetch that:

```shell
sudo pacman -S base-devel
```

If you need Git to clone the repo:

```shell
sudo pacman -S git
```

Then proceed to the "Building: With Make, Unix Makefiles" section

#### Many other Linux distributions

Many Linux distributions come with a package manager, such as Apt for Debian based distros (That includes all you Ubuntu folk). Have a look around your package manager for the dependencies listed above.  
If your distro does not happen to have a package manager, have fun with fetching and installing the packages yourself. ;)

## Building

### With Make, Unix Makefiles

Replace the 4 in `-j 4` with however many threads you want Make to use

```shell
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE="RelWithDebInfo" ..
make -j 4
```

#### macOS issues

While this works on my machine™, others have reported that on macOS, the build can fail in a few places.

If CMake can't find Qt5 configuration files, run the following command (Assuming you installed Qt5 with `brew`) then try again.

```shell
export CMAKE_PREFIX_PATH="/usr/local/opt/qt/lib/cmake"
```

If you get linker errors with `-lassimp`, well I have no idea what's going on. You can edit `ws2common/CMakeLists.txt` to include a full path to `libassimp.dylib`.

Replace

```cmake
if(APPLE) #For whatever reason ASSIMP_LIBRARIES does not use a full path on macOS
    link_directories(${ASSIMP_LINK_DIRS})
endif(APPLE)
```

with

```cmake
if(APPLE)
    set(ASSIMP_LIBRARIES /usr/local/lib/libassimp.dylib)
endif(APPLE)
```

The path there is the default place where Homebrew installs assimp.

### With Make, MSYS Makefiles

Replace the 4 in `-j 4` with however many threads you want Make to use

```shell
mkdir build
cd build
cmake -G "MSYS Makefiles" -DCMAKE_BUILD_TYPE="RelWithDebInfo" ..
make -j 4
```

### With Visual Studio

The following should generate a Visual Studio project for Visual Studio 2010. Replace the 10 with your version of Visual Studio.  
I generally recommend against using MSVC - have fun with dependencies ;)

```shell
mkdir build
cd build
cmake -G "Visual Studio 10" ..
```

