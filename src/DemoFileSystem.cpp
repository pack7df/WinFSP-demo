#include "DemoFileSystem.h"
#include <cwchar>
#include <cstring>
#include <cstdio>

namespace WinFspDemo {

    // -------------------------------------------------------
    // Interface initialization (lazy)
    // -------------------------------------------------------
    FSP_FILE_SYSTEM_INTERFACE DemoFileSystem::_interface = { 0 };

    DemoFileSystem::DemoFileSystem() {}

    FSP_FILE_SYSTEM_INTERFACE* DemoFileSystem::GetInterface() {
        if (_interface.Open != nullptr) return &_interface;

        _interface.GetVolumeInfo     = OnGetVolumeInfo;
        _interface.SetVolumeLabel    = OnSetVolumeLabel;
        _interface.GetSecurityByName = OnGetSecurityByName;
        _interface.Create            = OnCreate;
        _interface.Open              = OnOpen;
        _interface.Overwrite         = OnOverwrite;
        _interface.Cleanup           = OnCleanup;
        _interface.Close             = OnClose;
        _interface.Read              = OnRead;
        _interface.Write             = OnWrite;
        _interface.Flush             = OnFlush;
        _interface.GetFileInfo       = OnGetFileInfo;
        _interface.SetBasicInfo      = OnSetBasicInfo;
        _interface.SetFileSize       = OnSetFileSize;
        _interface.CanDelete         = OnCanDelete;
        _interface.Rename            = OnRename;
        _interface.GetSecurity       = OnGetSecurity;
        _interface.SetSecurity       = OnSetSecurity;
        _interface.ReadDirectory     = OnReadDirectory;

        return &_interface;
    }

    void DemoFileSystem::FillVolumeParams(FSP_FSCTL_VOLUME_PARAMS* Params) const {
        Params->Version                  = sizeof(FSP_FSCTL_VOLUME_PARAMS);
        Params->SectorSize               = 512;
        Params->SectorsPerAllocationUnit = 1;
        Params->MaxComponentLength       = 255;
        Params->FileInfoTimeout          = 1000;
        Params->VolumeSerialNumber       = 0x12345678;
        Params->CaseSensitiveSearch      = 0;
        Params->CasePreservedNames       = 1;
        Params->UnicodeOnDisk            = 1;
        Params->UmFileContextIsFullContext = 0;
        wcscpy_s(Params->FileSystemName, L"SAUNAFS");
    }

    // -------------------------------------------------------
    // Helper: instance pointer from WinFsp context
    // -------------------------------------------------------
    DemoFileSystem* DemoFileSystem::Instance(FSP_FILE_SYSTEM* Fs) {
        return static_cast<DemoFileSystem*>(Fs->UserContext);
    }

    // -------------------------------------------------------
    // Business Logic (Instance Methods)
    // -------------------------------------------------------
    void DemoFileSystem::FillDirInfo(FSP_FSCTL_FILE_INFO* FileInfo) const {
        memset(FileInfo, 0, sizeof(FSP_FSCTL_FILE_INFO));
        FileInfo->FileAttributes = FILE_ATTRIBUTE_DIRECTORY;
        FileInfo->IndexNumber    = 1;
        FileInfo->CreationTime   = 133485120000000000ULL;
        FileInfo->LastAccessTime = FileInfo->CreationTime;
        FileInfo->LastWriteTime  = FileInfo->CreationTime;
        FileInfo->ChangeTime     = FileInfo->CreationTime;
    }

    NTSTATUS DemoFileSystem::GetVolumeInfo(FSP_FSCTL_VOLUME_INFO* VolumeInfo) {
        memset(VolumeInfo, 0, sizeof(FSP_FSCTL_VOLUME_INFO));
        VolumeInfo->TotalSize = 100ULL * 1024 * 1024 * 1024;
        VolumeInfo->FreeSize  = 50ULL  * 1024 * 1024 * 1024;
        wcscpy_s(VolumeInfo->VolumeLabel, L"SaunaFS");
        VolumeInfo->VolumeLabelLength = (UINT16)(wcslen(VolumeInfo->VolumeLabel) * sizeof(WCHAR));
        return STATUS_SUCCESS;
    }

    // Returns success for all files to satisfy initial OS checks
    NTSTATUS DemoFileSystem::GetSecurityByName(PWSTR FileName, PUINT32 FileAttributes, PSECURITY_DESCRIPTOR Sd, SIZE_T* SdSize) {
        if (FileAttributes) *FileAttributes = FILE_ATTRIBUTE_DIRECTORY;
        if (SdSize)         *SdSize = 0;
        return STATUS_SUCCESS;
    }

    NTSTATUS DemoFileSystem::Create(PWSTR FileName, UINT32 CreateOptions, UINT32 GrantedAccess, UINT32 FileAttributes, PSECURITY_DESCRIPTOR Sd, UINT64 AllocationSize, PVOID* PFileContext, FSP_FSCTL_FILE_INFO* FileInfo) {
        return STATUS_ACCESS_DENIED;
    }

    NTSTATUS DemoFileSystem::Open(PWSTR FileName, UINT32 CreateOptions, UINT32 GrantedAccess, PVOID* PFileContext, FSP_FSCTL_FILE_INFO* FileInfo) {
        if (wcscmp(FileName, L"\\") != 0) return STATUS_OBJECT_NAME_NOT_FOUND;
        FillDirInfo(FileInfo);
        *PFileContext = (PVOID)1;
        return STATUS_SUCCESS;
    }

    NTSTATUS DemoFileSystem::GetFileInfo(PVOID FileContext, FSP_FSCTL_FILE_INFO* FileInfo) {
        FillDirInfo(FileInfo);
        return STATUS_SUCCESS;
    }

    NTSTATUS DemoFileSystem::ReadDirectory(PVOID FileContext, PWSTR Pattern, PWSTR Marker, PVOID Buffer, ULONG Length, PULONG PBytesTransferred) {
        // Signaling EOF with nullptr is mandatory for WinFsp to complete the IRP properly
        FspFileSystemAddDirInfo(nullptr, Buffer, Length, PBytesTransferred);
        return STATUS_SUCCESS;
    }

    // -------------------------------------------------------
    // Static Callbacks (WinFsp Bridge -> Instance)
    // -------------------------------------------------------
    NTSTATUS DemoFileSystem::OnGetVolumeInfo(FSP_FILE_SYSTEM* Fs, FSP_FSCTL_VOLUME_INFO* Info) {
        printf("[FS] OnGetVolumeInfo\n"); fflush(stdout);
        return Instance(Fs)->GetVolumeInfo(Info);
    }

    NTSTATUS DemoFileSystem::OnSetVolumeLabel(FSP_FILE_SYSTEM* Fs, PWSTR VolumeLabel, FSP_FSCTL_VOLUME_INFO* Info) {
        return STATUS_ACCESS_DENIED;
    }

    NTSTATUS DemoFileSystem::OnGetSecurityByName(FSP_FILE_SYSTEM* Fs, PWSTR FileName, PUINT32 FileAttributes, PSECURITY_DESCRIPTOR Sd, SIZE_T* SdSize) {
        printf("[FS] OnGetSecurityByName: %ls\n", FileName ? FileName : L"NULL"); fflush(stdout);
        return Instance(Fs)->GetSecurityByName(FileName, FileAttributes, Sd, SdSize);
    }

    NTSTATUS DemoFileSystem::OnCreate(FSP_FILE_SYSTEM* Fs, PWSTR FileName, UINT32 CreateOptions, UINT32 GrantedAccess, UINT32 FileAttributes, PSECURITY_DESCRIPTOR Sd, UINT64 AllocationSize, PVOID* PFileContext, FSP_FSCTL_FILE_INFO* FileInfo) {
        printf("[FS] OnCreate: %ls\n", FileName ? FileName : L"NULL"); fflush(stdout);
        return Instance(Fs)->Create(FileName, CreateOptions, GrantedAccess, FileAttributes, Sd, AllocationSize, PFileContext, FileInfo);
    }

    NTSTATUS DemoFileSystem::OnOpen(FSP_FILE_SYSTEM* Fs, PWSTR FileName, UINT32 CreateOptions, UINT32 GrantedAccess, PVOID* PFileContext, FSP_FSCTL_FILE_INFO* FileInfo) {
        printf("[FS] OnOpen: %ls\n", FileName ? FileName : L"NULL"); fflush(stdout);
        return Instance(Fs)->Open(FileName, CreateOptions, GrantedAccess, PFileContext, FileInfo);
    }

    NTSTATUS DemoFileSystem::OnOverwrite(FSP_FILE_SYSTEM* Fs, PVOID FileContext, UINT32 FileAttributes, BOOLEAN ReplaceFileAttributes, UINT64 AllocationSize, FSP_FSCTL_FILE_INFO* FileInfo) {
        return STATUS_ACCESS_DENIED;
    }

    VOID DemoFileSystem::OnCleanup(FSP_FILE_SYSTEM* Fs, PVOID FileContext, PWSTR FileName, ULONG Flags) {}
    VOID DemoFileSystem::OnClose(FSP_FILE_SYSTEM* Fs, PVOID FileContext) {}

    NTSTATUS DemoFileSystem::OnRead(FSP_FILE_SYSTEM* Fs, PVOID FileContext, PVOID Buffer, UINT64 Offset, ULONG Length, PULONG PBytesTransferred) {
        if (PBytesTransferred) *PBytesTransferred = 0;
        return STATUS_SUCCESS;
    }

    NTSTATUS DemoFileSystem::OnWrite(FSP_FILE_SYSTEM* Fs, PVOID FileContext, PVOID Buffer, UINT64 Offset, ULONG Length, BOOLEAN WriteToEndOfFile, BOOLEAN ConstrainedWrite, PULONG PBytesTransferred, FSP_FSCTL_FILE_INFO* FileInfo) {
        if (PBytesTransferred) *PBytesTransferred = Length;
        return STATUS_SUCCESS;
    }

    NTSTATUS DemoFileSystem::OnFlush(FSP_FILE_SYSTEM* Fs, PVOID FileContext, FSP_FSCTL_FILE_INFO* FileInfo) {
        return STATUS_SUCCESS;
    }

    NTSTATUS DemoFileSystem::OnGetFileInfo(FSP_FILE_SYSTEM* Fs, PVOID FileContext, FSP_FSCTL_FILE_INFO* FileInfo) {
        printf("[FS] OnGetFileInfo\n"); fflush(stdout);
        return Instance(Fs)->GetFileInfo(FileContext, FileInfo);
    }

    NTSTATUS DemoFileSystem::OnSetBasicInfo(FSP_FILE_SYSTEM* Fs, PVOID FileContext, UINT32 FileAttributes, UINT64 CreationTime, UINT64 LastAccessTime, UINT64 LastWriteTime, UINT64 ChangeTime, FSP_FSCTL_FILE_INFO* FileInfo) {
        return Instance(Fs)->GetFileInfo(FileContext, FileInfo);
    }

    NTSTATUS DemoFileSystem::OnSetFileSize(FSP_FILE_SYSTEM* Fs, PVOID FileContext, UINT64 NewSize, BOOLEAN SetAllocationSize, FSP_FSCTL_FILE_INFO* FileInfo) {
        return STATUS_SUCCESS;
    }

    NTSTATUS DemoFileSystem::OnCanDelete(FSP_FILE_SYSTEM* Fs, PVOID FileContext, PWSTR FileName) {
        return STATUS_SUCCESS;
    }

    NTSTATUS DemoFileSystem::OnRename(FSP_FILE_SYSTEM* Fs, PVOID FileContext, PWSTR FileName, PWSTR NewFileName, BOOLEAN ReplaceIfExists) {
        return STATUS_ACCESS_DENIED;
    }

    NTSTATUS DemoFileSystem::OnGetSecurity(FSP_FILE_SYSTEM* Fs, PVOID FileContext, PSECURITY_DESCRIPTOR Sd, SIZE_T* SdSize) {
        if (SdSize) *SdSize = 0;
        return STATUS_SUCCESS;
    }

    NTSTATUS DemoFileSystem::OnSetSecurity(FSP_FILE_SYSTEM* Fs, PVOID FileContext, SECURITY_INFORMATION SecurityInformation, PSECURITY_DESCRIPTOR Sd) {
        return STATUS_SUCCESS;
    }

    NTSTATUS DemoFileSystem::OnReadDirectory(FSP_FILE_SYSTEM* Fs, PVOID FileContext, PWSTR Pattern, PWSTR Marker, PVOID Buffer, ULONG Length, PULONG PBytesTransferred) {
        printf("[FS] OnReadDirectory\n"); fflush(stdout);
        return Instance(Fs)->ReadDirectory(FileContext, Pattern, Marker, Buffer, Length, PBytesTransferred);
    }

} // namespace WinFspDemo
