#include "DemoFileSystem.h"
#include <cwchar>
#include <cstring>

namespace SaunaFS {

    SaunaFileSystem::SaunaFileSystem() {
        memset(&_interface, 0, sizeof(_interface));
        
        // Configuración de la tabla de funciones (C -> C++ bridge)
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

    SaunaFileSystem::~SaunaFileSystem() {}

    FSP_FILE_SYSTEM_INTERFACE* SaunaFileSystem::GetInterface() {
        return &_interface;
    }

    void SaunaFileSystem::FillVolumeParams(FSP_FSCTL_VOLUME_PARAMS* Params) {
        Params->Version = sizeof(FSP_FSCTL_VOLUME_PARAMS);
        Params->SectorSize = 512;
        Params->SectorsPerAllocationUnit = 1;
        Params->MaxComponentLength = 255;
        Params->FileInfoTimeout = 1000;
        Params->CaseSensitiveSearch = 0;
        Params->CasePreservedNames = 1;
        Params->UmFileContextIsFullContext = 0;
    }

    // --- Métodos de Instancia (Lógica de Negocio) ---

    NTSTATUS SaunaFileSystem::GetVolumeInfo(FSP_FSCTL_VOLUME_INFO* VolumeInfo) {
        memset(VolumeInfo, 0, sizeof(FSP_FSCTL_VOLUME_INFO));
        VolumeInfo->TotalSize = 100ULL * 1024 * 1024 * 1024;
        VolumeInfo->FreeSize = 50ULL * 1024 * 1024 * 1024;
        wcscpy_s(VolumeInfo->VolumeLabel, L"SaunaFS");
        VolumeInfo->VolumeLabelLength = (UINT16)(wcslen(VolumeInfo->VolumeLabel) * sizeof(WCHAR));
        return STATUS_SUCCESS;
    }

    NTSTATUS SaunaFileSystem::Open(PWSTR FileName, UINT32 CreateOptions, UINT32 GrantedAccess, PVOID* PFileContext, FSP_FSCTL_FILE_INFO* FileInfo) {
        if (wcscmp(FileName, L"\\") != 0) return STATUS_OBJECT_NAME_NOT_FOUND;

        memset(FileInfo, 0, sizeof(FSP_FSCTL_FILE_INFO));
        FileInfo->FileAttributes = FILE_ATTRIBUTE_DIRECTORY;
        FileInfo->IndexNumber = 1;
        *PFileContext = (PVOID)1; // Contexto dummy para la raiz
        return STATUS_SUCCESS;
    }

    NTSTATUS SaunaFileSystem::GetFileInfo(PVOID FileContext, FSP_FSCTL_FILE_INFO* FileInfo) {
        memset(FileInfo, 0, sizeof(FSP_FSCTL_FILE_INFO));
        FileInfo->FileAttributes = FILE_ATTRIBUTE_DIRECTORY;
        FileInfo->IndexNumber = 1;
        return STATUS_SUCCESS;
    }

    NTSTATUS SaunaFileSystem::ReadDirectory(PVOID FileContext, PWSTR Pattern, PWSTR Marker, PVOID Buffer, ULONG Length, PULONG PBytesTransferred) {
        if (PBytesTransferred) *PBytesTransferred = 0;
        return STATUS_SUCCESS;
    }

    // --- Wrappers Estáticos (C-Style callbacks) ---

    NTSTATUS SaunaFileSystem::OnGetVolumeInfo(FSP_FILE_SYSTEM* FileSystem, FSP_FSCTL_VOLUME_INFO* VolumeInfo) {
        return ((SaunaFileSystem*)FileSystem->UserContext)->GetVolumeInfo(VolumeInfo);
    }

    NTSTATUS SaunaFileSystem::OnGetSecurityByName(FSP_FILE_SYSTEM* FileSystem, PWSTR FileName, PUINT32 FileAttributes, PSECURITY_DESCRIPTOR SecurityDescriptor, SIZE_T* PSecurityDescriptorSize) {
        if (FileAttributes) *FileAttributes = FILE_ATTRIBUTE_DIRECTORY;
        if (PSecurityDescriptorSize) *PSecurityDescriptorSize = 0;
        return STATUS_SUCCESS;
    }

    NTSTATUS SaunaFileSystem::OnOpen(FSP_FILE_SYSTEM* FileSystem, PWSTR FileName, UINT32 CreateOptions, UINT32 GrantedAccess, PVOID* PFileContext, FSP_FSCTL_FILE_INFO* FileInfo) {
        return ((SaunaFileSystem*)FileSystem->UserContext)->Open(FileName, CreateOptions, GrantedAccess, PFileContext, FileInfo);
    }

    NTSTATUS SaunaFileSystem::OnGetFileInfo(FSP_FILE_SYSTEM* FileSystem, PVOID FileContext, FSP_FSCTL_FILE_INFO* FileInfo) {
        return ((SaunaFileSystem*)FileSystem->UserContext)->GetFileInfo(FileContext, FileInfo);
    }

    NTSTATUS SaunaFileSystem::OnReadDirectory(FSP_FILE_SYSTEM* FileSystem, PVOID FileContext, PWSTR Pattern, PWSTR Marker, PVOID Buffer, ULONG Length, PULONG PBytesTransferred) {
        return ((SaunaFileSystem*)FileSystem->UserContext)->ReadDirectory(FileContext, Pattern, Marker, Buffer, Length, PBytesTransferred);
    }

    // Stubs
    VOID SaunaFileSystem::OnCleanup(FSP_FILE_SYSTEM* FileSystem, PVOID FileContext, PWSTR FileName, ULONG Flags) {}
    VOID SaunaFileSystem::OnClose(FSP_FILE_SYSTEM* FileSystem, PVOID FileContext) {}
    NTSTATUS SaunaFileSystem::OnRead(FSP_FILE_SYSTEM* FileSystem, PVOID FileContext, PVOID Buffer, UINT64 Offset, ULONG Length, PULONG PBytesTransferred) {
        if (PBytesTransferred) *PBytesTransferred = 0;
        return STATUS_SUCCESS;
    }
    NTSTATUS SaunaFileSystem::OnWrite(FSP_FILE_SYSTEM* FileSystem, PVOID FileContext, PVOID Buffer, UINT64 Offset, ULONG Length, BOOLEAN WriteToEndOfFile, BOOLEAN ConstrainedWrite, PULONG PBytesTransferred, FSP_FSCTL_FILE_INFO* FileInfo) {
        return STATUS_ACCESS_DENIED;
    }
    NTSTATUS SaunaFileSystem::OnFlush(FSP_FILE_SYSTEM* FileSystem, PVOID FileContext, FSP_FSCTL_FILE_INFO* FileInfo) {
        return STATUS_SUCCESS;
    }

}
