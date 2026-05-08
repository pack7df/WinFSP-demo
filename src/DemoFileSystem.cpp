#include "DemoFileSystem.h"
#include <cwchar>
#include <cstring>

namespace WinFspDemo {

    DemoFileSystem::DemoFileSystem() {
        memset(&_interface, 0, sizeof(_interface));
        
        // Initialize WinFsp interface with static bridge functions
        _interface.GetVolumeInfo = OnGetVolumeInfo;
        _interface.GetSecurityByName = OnGetSecurityByName;
        _interface.Open = OnOpen;
        _interface.GetFileInfo = OnGetFileInfo;
        _interface.ReadDirectory = OnReadDirectory;
        _interface.Cleanup = OnCleanup;
        _interface.Close = OnClose;
        _interface.Read = OnRead;
        _interface.Write = OnWrite;
        _interface.Flush = OnFlush;
    }

    DemoFileSystem::~DemoFileSystem() {}

    FSP_FILE_SYSTEM_INTERFACE* DemoFileSystem::GetInterface() {
        return &_interface;
    }

    void DemoFileSystem::FillVolumeParams(FSP_FSCTL_VOLUME_PARAMS* Params) {
        Params->Version = sizeof(FSP_FSCTL_VOLUME_PARAMS);
        Params->SectorSize = 512;
        Params->SectorsPerAllocationUnit = 1;
        Params->MaxComponentLength = 255;
        Params->FileInfoTimeout = 1000;
        Params->CaseSensitiveSearch = 0;
        Params->CasePreservedNames = 1;
        Params->UmFileContextIsFullContext = 0;
    }

    // --- Instance Methods (Core Logic) ---

    NTSTATUS DemoFileSystem::GetVolumeInfo(FSP_FILE_SYSTEM* FileSystem, FSP_FSCTL_VOLUME_INFO* VolumeInfo) {
        memset(VolumeInfo, 0, sizeof(FSP_FSCTL_VOLUME_INFO));
        VolumeInfo->TotalSize = 100ULL * 1024 * 1024 * 1024;
        VolumeInfo->FreeSize = 50ULL * 1024 * 1024 * 1024;
        wcscpy_s(VolumeInfo->VolumeLabel, L"WinFspDemo");
        VolumeInfo->VolumeLabelLength = (UINT16)(wcslen(VolumeInfo->VolumeLabel) * sizeof(WCHAR));
        return STATUS_SUCCESS;
    }

    NTSTATUS DemoFileSystem::Open(FSP_FILE_SYSTEM* FileSystem, PWSTR FileName, UINT32 CreateOptions, UINT32 GrantedAccess, PVOID* PFileContext, FSP_FSCTL_FILE_INFO* FileInfo) {
        // Only allow opening the root directory for now
        if (wcscmp(FileName, L"\\") != 0) return STATUS_OBJECT_NAME_NOT_FOUND;

        memset(FileInfo, 0, sizeof(FSP_FSCTL_FILE_INFO));
        FileInfo->FileAttributes = FILE_ATTRIBUTE_DIRECTORY;
        FileInfo->IndexNumber = 1;
        *PFileContext = (PVOID)1; // Dummy context for root
        return STATUS_SUCCESS;
    }

    NTSTATUS DemoFileSystem::GetFileInfo(FSP_FILE_SYSTEM* FileSystem, PVOID FileContext, FSP_FSCTL_FILE_INFO* FileInfo) {
        memset(FileInfo, 0, sizeof(FSP_FSCTL_FILE_INFO));
        FileInfo->FileAttributes = FILE_ATTRIBUTE_DIRECTORY;
        FileInfo->IndexNumber = 1;
        return STATUS_SUCCESS;
    }

    NTSTATUS DemoFileSystem::ReadDirectory(FSP_FILE_SYSTEM* FileSystem, PVOID FileContext, PWSTR Pattern, PWSTR Marker, PVOID Buffer, ULONG Length, PULONG PBytesTransferred) {
        if (PBytesTransferred) *PBytesTransferred = 0;
        return STATUS_SUCCESS;
    }

    // --- Static Wrappers (Bridge to Instance) ---

    NTSTATUS DemoFileSystem::OnGetVolumeInfo(FSP_FILE_SYSTEM* FileSystem, FSP_FSCTL_VOLUME_INFO* VolumeInfo) {
        return ((DemoFileSystem*)FileSystem->UserContext)->GetVolumeInfo(FileSystem, VolumeInfo);
    }

    NTSTATUS DemoFileSystem::OnGetSecurityByName(FSP_FILE_SYSTEM* FileSystem, PWSTR FileName, PUINT32 FileAttributes, PSECURITY_DESCRIPTOR SecurityDescriptor, SIZE_T* PSecurityDescriptorSize) {
        if (FileAttributes) *FileAttributes = FILE_ATTRIBUTE_DIRECTORY;
        if (PSecurityDescriptorSize) *PSecurityDescriptorSize = 0;
        return STATUS_SUCCESS;
    }

    NTSTATUS DemoFileSystem::OnOpen(FSP_FILE_SYSTEM* FileSystem, PWSTR FileName, UINT32 CreateOptions, UINT32 GrantedAccess, PVOID* PFileContext, FSP_FSCTL_FILE_INFO* FileInfo) {
        return ((DemoFileSystem*)FileSystem->UserContext)->Open(FileSystem, FileName, CreateOptions, GrantedAccess, PFileContext, FileInfo);
    }

    NTSTATUS DemoFileSystem::OnGetFileInfo(FSP_FILE_SYSTEM* FileSystem, PVOID FileContext, FSP_FSCTL_FILE_INFO* FileInfo) {
        return ((DemoFileSystem*)FileSystem->UserContext)->GetFileInfo(FileSystem, FileContext, FileInfo);
    }

    NTSTATUS DemoFileSystem::OnReadDirectory(FSP_FILE_SYSTEM* FileSystem, PVOID FileContext, PWSTR Pattern, PWSTR Marker, PVOID Buffer, ULONG Length, PULONG PBytesTransferred) {
        return ((DemoFileSystem*)FileSystem->UserContext)->ReadDirectory(FileSystem, FileContext, Pattern, Marker, Buffer, Length, PBytesTransferred);
    }

    // --- Mandatory Stubs ---
    VOID DemoFileSystem::OnCleanup(FSP_FILE_SYSTEM* FileSystem, PVOID FileContext, PWSTR FileName, ULONG Flags) {}
    VOID DemoFileSystem::OnClose(FSP_FILE_SYSTEM* FileSystem, PVOID FileContext) {}
    NTSTATUS DemoFileSystem::OnRead(FSP_FILE_SYSTEM* FileSystem, PVOID FileContext, PVOID Buffer, UINT64 Offset, ULONG Length, PULONG PBytesTransferred) {
        if (PBytesTransferred) *PBytesTransferred = 0;
        return STATUS_SUCCESS;
    }
    NTSTATUS DemoFileSystem::OnWrite(FSP_FILE_SYSTEM* FileSystem, PVOID FileContext, PVOID Buffer, UINT64 Offset, ULONG Length, BOOLEAN WriteToEndOfFile, BOOLEAN ConstrainedWrite, PULONG PBytesTransferred, FSP_FSCTL_FILE_INFO* FileInfo) {
        return STATUS_ACCESS_DENIED;
    }
    NTSTATUS DemoFileSystem::OnFlush(FSP_FILE_SYSTEM* FileSystem, PVOID FileContext, FSP_FSCTL_FILE_INFO* FileInfo) {
        return STATUS_SUCCESS;
    }
}
