#pragma once
#include <windows.h>
#include <winternl.h>

#ifndef PNTSTATUS
typedef NTSTATUS* PNTSTATUS;
#endif

#include <winfsp/winfsp.h>

namespace WinFspDemo {

    /**
     * DemoFileSystem class encapsulating the WinFsp callback logic.
     */
    class DemoFileSystem {
    public:
        DemoFileSystem();
        ~DemoFileSystem() = default;

        /**
         * Returns the static WinFsp interface structure.
         */
        static FSP_FILE_SYSTEM_INTERFACE* GetInterface();

        /**
         * Fills the volume parameters required by WinFsp.
         */
        void FillVolumeParams(FSP_FSCTL_VOLUME_PARAMS* Params) const;

    private:
        // -------------------------------------------------------
        // Business Logic (Instance Methods)
        // -------------------------------------------------------
        void FillDirInfo(FSP_FSCTL_FILE_INFO* FileInfo) const;

        NTSTATUS GetVolumeInfo(FSP_FSCTL_VOLUME_INFO* VolumeInfo);
        NTSTATUS GetSecurityByName(PWSTR FileName, PUINT32 FileAttributes, PSECURITY_DESCRIPTOR Sd, SIZE_T* SdSize);
        NTSTATUS Create(PWSTR FileName, UINT32 CreateOptions, UINT32 GrantedAccess, UINT32 FileAttributes, PSECURITY_DESCRIPTOR Sd, UINT64 AllocationSize, PVOID* PFileContext, FSP_FSCTL_FILE_INFO* FileInfo);
        NTSTATUS Open(PWSTR FileName, UINT32 CreateOptions, UINT32 GrantedAccess, PVOID* PFileContext, FSP_FSCTL_FILE_INFO* FileInfo);
        NTSTATUS GetFileInfo(PVOID FileContext, FSP_FSCTL_FILE_INFO* FileInfo);
        NTSTATUS ReadDirectory(PVOID FileContext, PWSTR Pattern, PWSTR Marker, PVOID Buffer, ULONG Length, PULONG PBytesTransferred);

        // -------------------------------------------------------
        // Static Bridge -> Instance (WinFsp Callbacks)
        // -------------------------------------------------------
        static DemoFileSystem* Instance(FSP_FILE_SYSTEM* Fs);

        static NTSTATUS OnGetVolumeInfo(FSP_FILE_SYSTEM* Fs, FSP_FSCTL_VOLUME_INFO* Info);
        static NTSTATUS OnSetVolumeLabel(FSP_FILE_SYSTEM* Fs, PWSTR VolumeLabel, FSP_FSCTL_VOLUME_INFO* Info);
        static NTSTATUS OnGetSecurityByName(FSP_FILE_SYSTEM* Fs, PWSTR FileName, PUINT32 FileAttributes, PSECURITY_DESCRIPTOR Sd, SIZE_T* SdSize);
        static NTSTATUS OnCreate(FSP_FILE_SYSTEM* Fs, PWSTR FileName, UINT32 CreateOptions, UINT32 GrantedAccess, UINT32 FileAttributes, PSECURITY_DESCRIPTOR Sd, UINT64 AllocationSize, PVOID* PFileContext, FSP_FSCTL_FILE_INFO* FileInfo);
        static NTSTATUS OnOpen(FSP_FILE_SYSTEM* Fs, PWSTR FileName, UINT32 CreateOptions, UINT32 GrantedAccess, PVOID* PFileContext, FSP_FSCTL_FILE_INFO* FileInfo);
        static NTSTATUS OnOverwrite(FSP_FILE_SYSTEM* Fs, PVOID FileContext, UINT32 FileAttributes, BOOLEAN ReplaceFileAttributes, UINT64 AllocationSize, FSP_FSCTL_FILE_INFO* FileInfo);
        static VOID     OnCleanup(FSP_FILE_SYSTEM* Fs, PVOID FileContext, PWSTR FileName, ULONG Flags);
        static VOID     OnClose(FSP_FILE_SYSTEM* Fs, PVOID FileContext);
        static NTSTATUS OnRead(FSP_FILE_SYSTEM* Fs, PVOID FileContext, PVOID Buffer, UINT64 Offset, ULONG Length, PULONG PBytesTransferred);
        static NTSTATUS OnWrite(FSP_FILE_SYSTEM* Fs, PVOID FileContext, PVOID Buffer, UINT64 Offset, ULONG Length, BOOLEAN WriteToEndOfFile, BOOLEAN ConstrainedWrite, PULONG PBytesTransferred, FSP_FSCTL_FILE_INFO* FileInfo);
        static NTSTATUS OnFlush(FSP_FILE_SYSTEM* Fs, PVOID FileContext, FSP_FSCTL_FILE_INFO* FileInfo);
        static NTSTATUS OnGetFileInfo(FSP_FILE_SYSTEM* Fs, PVOID FileContext, FSP_FSCTL_FILE_INFO* FileInfo);
        static NTSTATUS OnSetBasicInfo(FSP_FILE_SYSTEM* Fs, PVOID FileContext, UINT32 FileAttributes, UINT64 CreationTime, UINT64 LastAccessTime, UINT64 LastWriteTime, UINT64 ChangeTime, FSP_FSCTL_FILE_INFO* FileInfo);
        static NTSTATUS OnSetFileSize(FSP_FILE_SYSTEM* Fs, PVOID FileContext, UINT64 NewSize, BOOLEAN SetAllocationSize, FSP_FSCTL_FILE_INFO* FileInfo);
        static NTSTATUS OnCanDelete(FSP_FILE_SYSTEM* Fs, PVOID FileContext, PWSTR FileName);
        static NTSTATUS OnRename(FSP_FILE_SYSTEM* Fs, PVOID FileContext, PWSTR FileName, PWSTR NewFileName, BOOLEAN ReplaceIfExists);
        static NTSTATUS OnGetSecurity(FSP_FILE_SYSTEM* Fs, PVOID FileContext, PSECURITY_DESCRIPTOR Sd, SIZE_T* SdSize);
        static NTSTATUS OnSetSecurity(FSP_FILE_SYSTEM* Fs, PVOID FileContext, SECURITY_INFORMATION SecurityInformation, PSECURITY_DESCRIPTOR Sd);
        static NTSTATUS OnReadDirectory(FSP_FILE_SYSTEM* Fs, PVOID FileContext, PWSTR Pattern, PWSTR Marker, PVOID Buffer, ULONG Length, PULONG PBytesTransferred);

        static FSP_FILE_SYSTEM_INTERFACE _interface;
    };

} // namespace WinFspDemo
