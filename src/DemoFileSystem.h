#pragma once
#include <windows.h>
#include <winternl.h>

#ifndef PNTSTATUS
typedef NTSTATUS *PNTSTATUS;
#endif

#include <winfsp/winfsp.h>

namespace DemoFileSystem {
    // Volume Operations
    NTSTATUS OnGetVolumeInfo(FSP_FILE_SYSTEM *FileSystem, FSP_FSCTL_VOLUME_INFO* VolumeInfo);
    NTSTATUS OnSetVolumeLabel(FSP_FILE_SYSTEM *FileSystem, PWSTR VolumeLabel, FSP_FSCTL_VOLUME_INFO *VolumeInfo);
    
    // Security Operations
    NTSTATUS OnGetSecurityByName(FSP_FILE_SYSTEM *FileSystem, PWSTR FileName, PUINT32 FileAttributes, PSECURITY_DESCRIPTOR SecurityDescriptor, SIZE_T *PSecurityDescriptorSize);
    NTSTATUS OnGetSecurity(FSP_FILE_SYSTEM *FileSystem, PVOID FileContext, PSECURITY_DESCRIPTOR SecurityDescriptor, SIZE_T *PSecurityDescriptorSize);
    NTSTATUS OnSetSecurity(FSP_FILE_SYSTEM *FileSystem, PVOID FileContext, SECURITY_INFORMATION SecurityInformation, PSECURITY_DESCRIPTOR SecurityDescriptor);

    // File Operations
    NTSTATUS OnCreate(FSP_FILE_SYSTEM *FileSystem, PWSTR FileName, UINT32 CreateOptions, UINT32 GrantedAccess, UINT32 FileAttributes, PSECURITY_DESCRIPTOR SecurityDescriptor, UINT64 AllocationSize, PVOID *PFileContext, FSP_FSCTL_FILE_INFO *FileInfo);
    NTSTATUS OnOpen(FSP_FILE_SYSTEM *FileSystem, PWSTR FileName, UINT32 CreateOptions, UINT32 GrantedAccess, PVOID *PFileContext, FSP_FSCTL_FILE_INFO *FileInfo);
    NTSTATUS OnOverwrite(FSP_FILE_SYSTEM *FileSystem, PVOID FileContext, UINT32 FileAttributes, BOOLEAN ReplaceFileAttributes, UINT64 AllocationSize, FSP_FSCTL_FILE_INFO *FileInfo);
    VOID OnCleanup(FSP_FILE_SYSTEM *FileSystem, PVOID FileContext, PWSTR FileName, ULONG Flags);
    VOID OnClose(FSP_FILE_SYSTEM *FileSystem, PVOID FileContext);
    
    // I/O Operations
    NTSTATUS OnRead(FSP_FILE_SYSTEM *FileSystem, PVOID FileContext, PVOID Buffer, UINT64 Offset, ULONG Length, PULONG PBytesTransferred);
    NTSTATUS OnWrite(FSP_FILE_SYSTEM *FileSystem, PVOID FileContext, PVOID Buffer, UINT64 Offset, ULONG Length, BOOLEAN WriteToEndOfFile, BOOLEAN ConstrainedWrite, PULONG PBytesTransferred, FSP_FSCTL_FILE_INFO *FileInfo);
    NTSTATUS OnFlush(FSP_FILE_SYSTEM *FileSystem, PVOID FileContext, FSP_FSCTL_FILE_INFO *FileInfo);
    
    // Information Operations
    NTSTATUS OnGetFileInfo(FSP_FILE_SYSTEM *FileSystem, PVOID FileContext, FSP_FSCTL_FILE_INFO *FileInfo);
    NTSTATUS OnSetBasicInfo(FSP_FILE_SYSTEM *FileSystem, PVOID FileContext, UINT32 FileAttributes, UINT64 CreationTime, UINT64 LastAccessTime, UINT64 LastWriteTime, UINT64 ChangeTime, FSP_FSCTL_FILE_INFO *FileInfo);
    NTSTATUS OnSetFileSize(FSP_FILE_SYSTEM *FileSystem, PVOID FileContext, UINT64 NewSize, BOOLEAN SetAllocationSize, FSP_FSCTL_FILE_INFO *FileInfo);
    
    // Directory Operations
    NTSTATUS OnReadDirectory(FSP_FILE_SYSTEM *FileSystem, PVOID FileContext, PWSTR Pattern, PWSTR Marker, PVOID Buffer, ULONG Length, PULONG PBytesTransferred);
    
    // Namespace Operations
    NTSTATUS OnCanDelete(FSP_FILE_SYSTEM *FileSystem, PVOID FileContext, PWSTR FileName);
    NTSTATUS OnRename(FSP_FILE_SYSTEM *FileSystem, PVOID FileContext, PWSTR FileName, PWSTR NewFileName, BOOLEAN ReplaceIfExists);

    // Interface Provider
    FSP_FILE_SYSTEM_INTERFACE* GetFsInterface();
}
