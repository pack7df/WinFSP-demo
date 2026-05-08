# WinFsp C++ Demo Project

This is a minimal demonstration project for learning how to use **WinFsp** (Windows File System Proxy) with C++26.

## Prerequisites

1.  **WinFsp SDK**: Download and install from [winfsp.dev](https://winfsp.dev/).
2.  **CMake**: Version 3.20 or higher.
3.  **Visual Studio**: With "Desktop development with C++" workload (latest version recommended).

## Project Structure

-   `CMakeLists.txt`: Build configuration.
-   `src/main.cpp`: Entry point, initializes the WinFsp host and mounts the drive.
-   `src/DemoFileSystem.h/cpp`: Implementation of the WinFsp callbacks.

## How to Build

1.  Open a terminal in the project directory.
2.  Run the following commands:

```powershell
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

The executable will be located in `build/bin/Release/WinFspDemo.exe`.

## How to Run

You need an unused drive letter (e.g., `Z:`).

```powershell
.\bin\Release\WinFspDemo.exe Z:
```

Once mounted, you can open `Z:` in File Explorer. This demo currently only shows an empty root directory.

## Learning Path

This project implements the following core WinFsp callbacks:
-   `GetFileInfo`: Returns basic metadata about files/directories.
-   `Open`: Handles opening file or directory handles.
-   `ReadDirectory`: Returns the list of entries within a directory.
-   `Close`: Cleans up handles.

To expand this project, try implementing `ReadFile`, `WriteFile`, or adding real file entries to the `ReadDirectory` implementation.
