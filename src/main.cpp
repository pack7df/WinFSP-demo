#include <winfsp/winfsp.h>
#include "DemoFileSystem.h"
#include <cstdio>

using namespace WinFspDemo;

/**
 * Main entry point for the WinFsp Demo application.
 * Mounts a virtual disk using the DemoFileSystem implementation.
 */
int wmain(int argc, wchar_t* argv[]) {
    if (argc < 2) {
        printf("Usage: WinFspDemo.exe <MountPoint>\n");
        return 1;
    }

    PWSTR MountPoint = argv[1];
    printf("--- WinFsp Demo (Class-based) ---\n");
    printf("MountPoint: %ls\n", MountPoint);
    fflush(stdout);

    DemoFileSystem demoFs;

    // Configure volume parameters
    FSP_FSCTL_VOLUME_PARAMS VolumeParams;
    memset(&VolumeParams, 0, sizeof(VolumeParams));
    demoFs.FillVolumeParams(&VolumeParams);

    // Create the file system object
    FSP_FILE_SYSTEM* FileSystem = nullptr;
    NTSTATUS Result = FspFileSystemCreate(
        (PWSTR)L"WinFsp.Disk",
        &VolumeParams,
        DemoFileSystem::GetInterface(),
        &FileSystem);

    if (!NT_SUCCESS(Result)) {
        printf("Error FspFileSystemCreate: 0x%08X\n", (unsigned int)Result);
        return 1;
    }

    // Associate our C++ class instance with the WinFsp file system object
    FileSystem->UserContext = &demoFs;
    printf("FileSystem created successfully\n");

    // Set the mount point (e.g., Z:)
    Result = FspFileSystemSetMountPoint(FileSystem, MountPoint);
    if (!NT_SUCCESS(Result)) {
        printf("Error SetMountPoint: 0x%08X\n", (unsigned int)Result);
        FspFileSystemDelete(FileSystem);
        return 1;
    }
    printf("MountPoint set successfully\n");

    // Start the FSD (File System Dispatcher)
    Result = FspFileSystemStartDispatcher(FileSystem, 0);
    if (!NT_SUCCESS(Result)) {
        printf("Error StartDispatcher: 0x%08X\n", (unsigned int)Result);
        FspFileSystemRemoveMountPoint(FileSystem);
        FspFileSystemDelete(FileSystem);
        return 1;
    }
    printf("Dispatcher started. File system ready at %ls\n", MountPoint);
    printf("Press Ctrl+C to unmount.\n");
    fflush(stdout);

    // Keep the process alive while the file system is mounted
    Sleep(INFINITE);

    // Cleanup
    FspFileSystemStopDispatcher(FileSystem);
    FspFileSystemRemoveMountPoint(FileSystem);
    FspFileSystemDelete(FileSystem);

    return 0;
}
