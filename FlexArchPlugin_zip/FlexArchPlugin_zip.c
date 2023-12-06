#include <FlexArch/plugin.h>
#include <zip.h>


const char* Plugin_GetName()
{
    return u8"ZIP FlexArch plugin";
}

const char* Plugin_Description()
{
    return u8"Sample FlexArch plugin based on libzip from libzip.org";
}

const char* Plugin_ErrorCodeDescription(uint32_t error_code, uint16_t reserved)
{
    return u8"";
}

uint32_t Archive_Open(archive_handle* archive, char* local_path)
{
    zip_fopen(NULL, local_path, 0);
    return FA_NOT_IMPLEMENTID;
}

uint32_t Archive_Create(archive_handle* archive, char* local_path, char* flags)
{
    return FA_NOT_IMPLEMENTID;
}

uint32_t Archive_Save(archive_handle archive)
{
    return FA_NOT_IMPLEMENTID;
}

uint32_t Archive_Close(archive_handle archive)
{
    return FA_NOT_IMPLEMENTID;
}

uint32_t Archive_AddFileLocal(archive_handle archive, archive_entry* archive_item, char* local_path)
{
    return FA_NOT_IMPLEMENTID;
}

uint32_t Archive_RemoveEntry(archive_handle archive, archive_entry* archive_item)
{
    return FA_NOT_IMPLEMENTID;
}

uint32_t Archive_CreateDirectory(archive_handle archive, archive_entry* archive_path)
{
    return FA_NOT_IMPLEMENTID;
}

uint32_t Archive_EnumerateEntries(archive_handle archive, archive_enumerate_callback callback)
{
    return FA_NOT_IMPLEMENTID;
}

uint32_t Archive_ExtractFiles(archive_handle archive, char* local_dir_path, archive_entry* archive_item, uint64_t archive_item_count)
{
    return FA_NOT_IMPLEMENTID;
}

uint32_t Archive_RegisterStatusCallback(status_callback callback)
{
    return FA_NOT_IMPLEMENTID;
}

uint32_t Archive_GetEntryInfo(archive_handle archive, archive_entry* archive_item)
{
    return FA_NOT_IMPLEMENTID;
}
