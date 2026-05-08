#include <winfsp/winfsp.h>
#include "DemoFileSystem.h"
#include <iostream>

int wmain(int argc, wchar_t* argv[]) {
    if (argc < 2) {
        std::wcout << L"Usage: WinFspDemo.exe <MountPoint>" << std::endl;
        return 1;
    }

    PWSTR MountPoint = argv[1];
    FSP_FILE_SYSTEM* FileSystem;
    NTSTATUS Result;

    FSP_FSCTL_VOLUME_PARAMS VolumeParams;
    memset(&VolumeParams, 0, sizeof(VolumeParams));

    VolumeParams.Version = sizeof(FSP_FSCTL_VOLUME_PARAMS);
    VolumeParams.SectorSize = 512;
    VolumeParams.SectorsPerAllocationUnit = 1;
    VolumeParams.MaxComponentLength = 255;
    VolumeParams.FileInfoTimeout = 1000;
    VolumeParams.CaseSensitiveSearch = 0;
    VolumeParams.CasePreservedNames = 1;
    VolumeParams.UmFileContextIsFullContext = 0;

    // Usamos el nombre del dispositivo estandar de WinFsp para discos
    Result = FspFileSystemCreate(
        (PWSTR)L"WinFsp.Disk",
        &VolumeParams,
        DemoFileSystem::GetFsInterface(),
        &FileSystem);

    if (!NT_SUCCESS(Result)) {
        std::cerr << "Failed to create file system: " << std::hex << (unsigned int)Result << std::endl;
        return (int)Result;
    }

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

    std::wcout << L"Mounted! Check " << MountPoint << L" and breakpoints." << std::endl;
    
    Sleep(INFINITE);

    FspFileSystemStopDispatcher(FileSystem);
    FspFileSystemDelete(FileSystem);

    return 0;
}
