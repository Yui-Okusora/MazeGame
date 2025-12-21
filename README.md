# MazeGame


## Đồ án cuối kì CSLT
----------------------
## Team members:

- Đỗ Quang Dũng
- Dương Ngọc Khôi
- Hồ Đăng Thiên Phúc
- Lê Chí Bảo
- Phạm Nhật Đăng Khoa

Installation & Build
====================

Requirements
------------

- Windows 10 / 11
- CMake 3.20 or newer
- C++23 compatible compiler
  - MSVC (Visual Studio Build Tools)
- Git

Note: The Visual Studio IDE is NOT required. Only the MSVC compiler toolchain is needed.


Install Build Tools
-------------------

1. Download and install Visual Studio Build Tools:
   https://visualstudio.microsoft.com/downloads/

2. During installation, select:
   - MSVC v143 C++ build tools
   - Windows 10/11 SDK
   - CMake tools for Windows


Clone the Repository
--------------------

```
git clone https://github.com/Yui-Okusora/MazeGame.git
cd MazeGame
```


Build
-------------------

The project provides a one-command build script that configures and builds
The project uses MSVC and Ninja.

From the project root, simply run:

```
build.bat
```

This will:
- Automatically initialize the MSVC build environment
- Configure CMake with the Ninja generator
- Build the project in Release mode
- Copy the resources/ folder next to the executable


Run
---

After a successful build, the executable will be located at:

build\MazeGame.exe


Build Configuration
-------------------

The default build configuration is Release.

To change the configuration, edit the following line in build.bat:

set CONFIG=Release

Available options:
- Debug
- Release


Clean Build
-----------

To perform a clean build, delete the build directory:

```
rmdir /s /q build
```

Then run:

```
build.bat
```

Notes
-----

- The project uses C++23
- Optimizations are enabled automatically for Release builds
- No Visual Studio IDE is required
- All third-party libraries are built from source
