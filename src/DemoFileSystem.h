#pragma once
#include <windows.h>
#include <winternl.h>

#ifndef PNTSTATUS
typedef NTSTATUS *PNTSTATUS;
#endif

#include <winfsp/winfsp.h>

namespace WinFspDemo {

    class DemoFileSystem {
    public:
        DemoFileSystem();
        ~DemoFileSystem();

        // Get the WinFsp interface configured with our callbacks
        FSP_FILE_SYSTEM_INTERFACE* GetInterface();
        
        // Populate standard volume parameters for this demo
        void FillVolumeParams(FSP_FSCTL_VOLUME_PARAMS* Params);

    private:
        FSP_FILE_SYSTEM_INTERFACE _interface;

        // Instance methods (Business Logic)
        NTSTATUS GetVolumeInfo(FSP_FILE_SYSTEM* FileSystem, FSP_FSCTL_VOLUME_INFO* VolumeInfo);
        NTSTATUS Open(FSP_FILE_SYSTEM* FileSystem, PWSTR FileName, UINT32 CreateOptions, UINT32 GrantedAccess, PVOID* PFileContext, FSP_FSCTL_FILE_INFO* FileInfo);
        NTSTATUS GetFileInfo(FSP_FILE_SYSTEM* FileSystem, PVOID FileContext, FSP_FSCTL_FILE_INFO* FileInfo);
        NTSTATUS ReadDirectory(FSP_FILE_SYSTEM* FileSystem, PVOID FileContext, PWSTR Pattern, PWSTR Marker, PVOID Buffer, ULONG Length, PULONG PBytesTransferred);
        
        // Static Wrappers (C -> C++ Bridge)
        static NTSTATUS OnGetVolumeInfo(FSP_FILE_SYSTEM* FileSystem, FSP_FSCTL_VOLUME_INFO* VolumeInfo);
        static NTSTATUS OnGetSecurityByName(FSP_FILE_SYSTEM* FileSystem, PWSTR FileName, PUINT32 FileAttributes, PSECURITY_DESCRIPTOR SecurityDescriptor, SIZE_T* PSecurityDescriptorSize);
        static NTSTATUS OnOpen(FSP_FILE_SYSTEM* FileSystem, PWSTR FileName, UINT32 CreateOptions, UINT32 GrantedAccess, PVOID* PFileContext, FSP_FSCTL_FILE_INFO* FileInfo);
        static NTSTATUS OnGetFileInfo(FSP_FILE_SYSTEM* FileSystem, PVOID FileContext, FSP_FSCTL_FILE_INFO* FileInfo);
        static NTSTATUS OnReadDirectory(FSP_FILE_SYSTEM* FileSystem, PVOID FileContext, PWSTR Pattern, PWSTR Marker, PVOID Buffer, ULONG Length, PULONG PBytesTransferred);
        
        // Mandatory Stubs
        static VOID OnCleanup(FSP_FILE_SYSTEM* FileSystem, PVOID FileContext, PWSTR FileName, ULONG Flags);
        static VOID OnClose(FSP_FILE_SYSTEM* FileSystem, PVOID FileContext);
        static NTSTATUS OnRead(FSP_FILE_SYSTEM* FileSystem, PVOID FileContext, PVOID Buffer, UINT64 Offset, ULONG Length, PULONG PBytesTransferred);
        static NTSTATUS OnWrite(FSP_FILE_SYSTEM* FileSystem, PVOID FileContext, PVOID Buffer, UINT64 Offset, ULONG Length, BOOLEAN WriteToEndOfFile, BOOLEAN ConstrainedWrite, PULONG PBytesTransferred, FSP_FSCTL_FILE_INFO* FileInfo);
        static NTSTATUS OnFlush(FSP_FILE_SYSTEM* FileSystem, PVOID FileContext, FSP_FSCTL_FILE_INFO* FileInfo);
    };

}
