#include <winfsp/winfsp.h>
#include "DemoFileSystem.h"
#include <iostream>

using namespace WinFspDemo;

int wmain(int argc, wchar_t* argv[]) {
    if (argc < 2) {
        std::wcout << L"Usage: WinFspDemo.exe <MountPoint>" << std::endl;
        return 1;
    }

    PWSTR MountPoint = argv[1];
    FSP_FILE_SYSTEM* FileSystem;
    NTSTATUS Result;
    
    // Instantiate the generic demo filesystem logic
    DemoFileSystem demoFs;

    FSP_FSCTL_VOLUME_PARAMS VolumeParams;
    memset(&VolumeParams, 0, sizeof(VolumeParams));
    demoFs.FillVolumeParams(&VolumeParams);

    // Create the WinFsp file system object
    Result = FspFileSystemCreate(
        (PWSTR)L"WinFsp.Disk",
        &VolumeParams,
        demoFs.GetInterface(),
        &FileSystem);

    if (!NT_SUCCESS(Result)) {
        std::cerr << "Failed to create file system: " << std::hex << (unsigned int)Result << std::endl;
        return (int)Result;
    }

    // Inject the class instance into the FileSystem UserContext
    FileSystem->UserContext = &demoFs;

    Result = FspFileSystemSetMountPoint(FileSystem, MountPoint);
    if (!NT_SUCCESS(Result)) {
        std::cerr << "Failed to set mount point: " << std::hex << (unsigned int)Result << std::endl;
        FspFileSystemDelete(FileSystem);
        return (int)Result;
    }

    Result = FspFileSystemStartDispatcher(FileSystem, 0);
    if (!NT_SUCCESS(Result)) {
        std::cerr << "Failed to start dispatcher: " << std::hex << (unsigned int)Result << std::endl;
        FspFileSystemDelete(FileSystem);
        return (int)Result;
    }

    std::wcout << L"Demo FS Mounted! Check " << MountPoint << std::endl;
    
    Sleep(INFINITE);

    FspFileSystemStopDispatcher(FileSystem);
    FspFileSystemDelete(FileSystem);

    return 0;
}
