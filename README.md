# WinFsp Virtual Filesystem Demo

A professional C++ demonstration of a virtual filesystem using the **WinFsp** (Windows File System Proxy) framework. This project serves as a template for building custom user-mode filesystems on Windows.

## Features
- **OOP Architecture**: Fully encapsulated logic using the `DemoFileSystem` class.
- **WinFsp Integration**: Robust implementation of mandatory callbacks (Open, ReadDirectory, GetFileInfo, etc.).
- **Automatic Mounting**: Easy-to-use CLI to mount virtual drives to any available letter.
- **SOLID Principles**: Clean, modular, and maintainable codebase.

## Prerequisites
- [WinFsp SDK](https://github.com/winfsp/winfsp) installed on your system.
- Visual Studio 2022 with C++ and CMake support.

## Getting Started
1. Clone the repository.
2. Open the project folder in Visual Studio.
3. Build the project using the `x64-Debug` configuration.
4. Run the application passing a drive letter as an argument:
   ```powershell
   .\WinFspDemo.exe Z:
   ```

## Development
Refer to [git-workflow.md](./git-workflow.md) for branching strategy and commit standards. All development follows the rules defined in [STYLEGUIDE.md](./STYLEGUIDE.md).

## License
This project is for demonstration purposes.
