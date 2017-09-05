SMB Level Workshop 2 (WS2)
==========================

Another installment in Super Monkey Ball hacking - SMB Level Workshop 2 is a stage editor for SMB 1 and 2.

WS2 is very early in development for now, and is not at all useable as a level editor. It may not be ready for a while, with me needing to learn and apply modern OpenGL and Qt 5. In the meanwhile, I refer you to the [original SMB Level Workshop](https://github.com/CraftedCart/SMBLevelWorkshop).

## Why? Wasn't the original SMB Level Workshop good enough?

The original SMB Level Workshop has been quite the learning experience for me, in what is good to do and especially what not to do; the what not to do part however comes from experience maintaining it. The original one has become quite the pain to maintain with little seperation between the UI and the logic of the editor, fiddly at best and buggy at worst UI code, outdated, slow OpenGL, and issues with LWJGL 2.9. I decided that it would be better to move on and start afresh.

## Dependencies

- Qt 5.9
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

### With Make, MSYS Makefiles

Replace the 4 in `-j 4` with however many threads you want Make to use

```shell
mkdir build
cd build
cmake -G "MSYS Makefiles" -DCMAKE_BUILD_TYPE="RelWithDebInfo" ..
make -j 4
```

You will also need to copy over a lot of DLLs into the same directory as smblevelworkshop2.exe if they aren't in your PATH, or if you want to redistribute this.
You can find the DLLs in C:\msys64\mingw64\bin (Assuming you installed MSYS2 to the default location, and you want x86_64 DLLs).

- platforms\qwindowsd.dll (Debug builds only)
- platforms\qwindows.dll (Non-debug builds only)
- glew32.dll
- libassimp.dll
- libBulletCollision.dll
- libBulletDynamics.dll
- libbz2-1.dll
- libfreetype-6.dll
- libgcc_s_seh-1.dll
- libglib-2.0-0.dll
- libgraphite2.dll
- libharfbuzz-0.dll
- libiconv-2.dll
- libicudtd58.dll (Debug builds only)
- libicudt58.dll (Non-debug builds only)
- libicuind58.dll (Debug builds only)
- libicuin58.dll (Non-debug builds only)
- libicuucd58.dll (Debug builds only)
- libicuuc58.dll (Non-debug builds only)
- libintl-8.dll
- libLinearMath.dll
- libminizip-1.dll
- libpcre-1.dll
- libpng16-16.dll
- libstdc++-6.dll
- libwinpthread-1.dll
- Qt5Cored.dll (Debug builds only)
- Qt5Core.dll (Non-debug builds only)
- Qt5Guid.dll (Debug builds only)
- Qt5Gui.dll (Non-debug builds only)
- Qt5Widgetsd.dll (Debug builds only)
- Qt5Widgets.dll (Non-debug builds only)
- zlib1.dll

### With Visual Studio

The following should generate a Visual Studio project for Visual Studio 2010. Replace the 10 with your version of Visual Studio.  
I generally recommend against using MSVC - have fun with dependencies ;)

```shell
mkdir build
cd build
cmake -G "Visual Studio 10" ..
```

