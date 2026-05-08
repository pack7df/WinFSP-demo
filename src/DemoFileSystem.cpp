#include "DemoFileSystem.h"
#include <cwchar>
#include <cstring>

namespace DemoFileSystem {
    static FSP_FILE_SYSTEM_INTERFACE _interface = { 0 };

    // Helper: Fill FileInfo with default valid directory metadata
    void FillDefaultDirInfo(FSP_FSCTL_FILE_INFO* FileInfo) {
        memset(FileInfo, 0, sizeof(FSP_FSCTL_FILE_INFO));
        FileInfo->FileAttributes = FILE_ATTRIBUTE_DIRECTORY;
        FileInfo->IndexNumber = 1;
        FileInfo->CreationTime = 133485120000000000ULL; // Jan 2024
        FileInfo->LastAccessTime = FileInfo->CreationTime;
        FileInfo->LastWriteTime = FileInfo->CreationTime;
        FileInfo->ChangeTime = FileInfo->CreationTime;
    }

    // --- Volume Operations ---
    NTSTATUS OnGetVolumeInfo(FSP_FILE_SYSTEM *FileSystem, FSP_FSCTL_VOLUME_INFO* VolumeInfo) {
        memset(VolumeInfo, 0, sizeof(FSP_FSCTL_VOLUME_INFO));
        VolumeInfo->TotalSize = 100ULL * 1024 * 1024 * 1024;
        VolumeInfo->FreeSize = 50ULL * 1024 * 1024 * 1024;
        wcscpy_s(VolumeInfo->VolumeLabel, L"SaunaFS");
        VolumeInfo->VolumeLabelLength = (UINT16)(wcslen(VolumeInfo->VolumeLabel) * sizeof(WCHAR));
        return STATUS_SUCCESS;
    }

    NTSTATUS OnSetVolumeLabel(FSP_FILE_SYSTEM *FileSystem, PWSTR VolumeLabel, FSP_FSCTL_VOLUME_INFO *VolumeInfo) {
        return STATUS_ACCESS_DENIED;
    }

    // --- Security Operations ---
    NTSTATUS OnGetSecurityByName(FSP_FILE_SYSTEM *FileSystem, PWSTR FileName, PUINT32 FileAttributes, PSECURITY_DESCRIPTOR SecurityDescriptor, SIZE_T *PSecurityDescriptorSize) {
        if (FileAttributes) *FileAttributes = FILE_ATTRIBUTE_DIRECTORY;
        if (PSecurityDescriptorSize) *PSecurityDescriptorSize = 0;
        return STATUS_SUCCESS;
    }

    NTSTATUS OnGetSecurity(FSP_FILE_SYSTEM *FileSystem, PVOID FileContext, PSECURITY_DESCRIPTOR SecurityDescriptor, SIZE_T *PSecurityDescriptorSize) {
        if (PSecurityDescriptorSize) *PSecurityDescriptorSize = 0;
        return STATUS_SUCCESS;
    }

    NTSTATUS OnSetSecurity(FSP_FILE_SYSTEM *FileSystem, PVOID FileContext, SECURITY_INFORMATION SecurityInformation, PSECURITY_DESCRIPTOR SecurityDescriptor) {
        return STATUS_SUCCESS;
    }

    // --- File Operations ---
    NTSTATUS OnCreate(FSP_FILE_SYSTEM *FileSystem, PWSTR FileName, UINT32 CreateOptions, UINT32 GrantedAccess, UINT32 FileAttributes, PSECURITY_DESCRIPTOR SecurityDescriptor, UINT64 AllocationSize, PVOID *PFileContext, FSP_FSCTL_FILE_INFO *FileInfo) {
        return STATUS_ACCESS_DENIED;
    }

    NTSTATUS OnOpen(FSP_FILE_SYSTEM *FileSystem, PWSTR FileName, UINT32 CreateOptions, UINT32 GrantedAccess, PVOID *PFileContext, FSP_FSCTL_FILE_INFO *FileInfo) {
        if (wcscmp(FileName, L"\\") != 0) return STATUS_OBJECT_NAME_NOT_FOUND;
        FillDefaultDirInfo(FileInfo);
        *PFileContext = (PVOID)1;
        return STATUS_SUCCESS;
    }

    NTSTATUS OnOverwrite(FSP_FILE_SYSTEM *FileSystem, PVOID FileContext, UINT32 FileAttributes, BOOLEAN ReplaceFileAttributes, UINT64 AllocationSize, FSP_FSCTL_FILE_INFO *FileInfo) {
        return STATUS_ACCESS_DENIED;
    }

    VOID OnCleanup(FSP_FILE_SYSTEM *FileSystem, PVOID FileContext, PWSTR FileName, ULONG Flags) {}

    VOID OnClose(FSP_FILE_SYSTEM *FileSystem, PVOID FileContext) {}

    // --- I/O Operations ---
    NTSTATUS OnRead(FSP_FILE_SYSTEM *FileSystem, PVOID FileContext, PVOID Buffer, UINT64 Offset, ULONG Length, PULONG PBytesTransferred) {
        if (PBytesTransferred) *PBytesTransferred = 0;
        return STATUS_SUCCESS;
    }

    NTSTATUS OnWrite(FSP_FILE_SYSTEM *FileSystem, PVOID FileContext, PVOID Buffer, UINT64 Offset, ULONG Length, BOOLEAN WriteToEndOfFile, BOOLEAN ConstrainedWrite, PULONG PBytesTransferred, FSP_FSCTL_FILE_INFO *FileInfo) {
        if (PBytesTransferred) *PBytesTransferred = Length;
        return STATUS_SUCCESS;
    }

    NTSTATUS OnFlush(FSP_FILE_SYSTEM *FileSystem, PVOID FileContext, FSP_FSCTL_FILE_INFO *FileInfo) {
        return STATUS_SUCCESS;
    }

    // --- Information Operations ---
    NTSTATUS OnGetFileInfo(FSP_FILE_SYSTEM *FileSystem, PVOID FileContext, FSP_FSCTL_FILE_INFO *FileInfo) {
        FillDefaultDirInfo(FileInfo);
        return STATUS_SUCCESS;
    }

    NTSTATUS OnSetBasicInfo(FSP_FILE_SYSTEM *FileSystem, PVOID FileContext, UINT32 FileAttributes, UINT64 CreationTime, UINT64 LastAccessTime, UINT64 LastWriteTime, UINT64 ChangeTime, FSP_FSCTL_FILE_INFO *FileInfo) {
        return STATUS_SUCCESS;
    }

    NTSTATUS OnSetFileSize(FSP_FILE_SYSTEM *FileSystem, PVOID FileContext, UINT64 NewSize, BOOLEAN SetAllocationSize, FSP_FSCTL_FILE_INFO *FileInfo) {
        return STATUS_SUCCESS;
    }

    // --- Directory Operations ---
    NTSTATUS OnReadDirectory(FSP_FILE_SYSTEM *FileSystem, PVOID FileContext, PWSTR Pattern, PWSTR Marker, PVOID Buffer, ULONG Length, PULONG PBytesTransferred) {
        if (PBytesTransferred) *PBytesTransferred = 0;
        return STATUS_SUCCESS;
    }

    // --- Namespace Operations ---
    NTSTATUS OnCanDelete(FSP_FILE_SYSTEM *FileSystem, PVOID FileContext, PWSTR FileName) {
        return STATUS_SUCCESS;
    }

    NTSTATUS OnRename(FSP_FILE_SYSTEM *FileSystem, PVOID FileContext, PWSTR FileName, PWSTR NewFileName, BOOLEAN ReplaceIfExists) {
        return STATUS_ACCESS_DENIED;
    }

    // --- Interface Provider ---
    FSP_FILE_SYSTEM_INTERFACE* GetFsInterface() {
        if (_interface.Open != nullptr) return &_interface;

        _interface.GetVolumeInfo = OnGetVolumeInfo;
        _interface.SetVolumeLabel = OnSetVolumeLabel;
        _interface.GetSecurityByName = OnGetSecurityByName;
        _interface.Create = OnCreate;
        _interface.Open = OnOpen;
        _interface.Overwrite = OnOverwrite;
        _interface.Cleanup = OnCleanup;
        _interface.Close = OnClose;
        _interface.Read = OnRead;
        _interface.Write = OnWrite;
        _interface.Flush = OnFlush;
        _interface.GetFileInfo = OnGetFileInfo;
        _interface.SetBasicInfo = OnSetBasicInfo;
        _interface.SetFileSize = OnSetFileSize;
        _interface.CanDelete = OnCanDelete;
        _interface.Rename = OnRename;
        _interface.GetSecurity = OnGetSecurity;
        _interface.SetSecurity = OnSetSecurity;
        _interface.ReadDirectory = OnReadDirectory;

        return &_interface;
    }
}
