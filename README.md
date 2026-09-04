# GTA V Power Essential (Build 350)

A custom, high-performance native C++ proxy framework for Grand Theft Auto V (Build 350) designed to combine memory pool management, DirectX rendering, and custom script execution into a single unified binary (`dinput8.dll`).

## Build Instructions

1. Ensure you have CMake (3.20 or higher) and a C++17 compatible compiler (such as MSVC) installed.
2. Open a terminal in the root directory and execute the build commands:
   ```bash
   mkdir build
   cd build
   cmake ..
   cmake --build . --config Release
