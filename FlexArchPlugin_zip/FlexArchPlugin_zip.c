#include <FlexArch/plugin.h>



const char* Plugin_GetName()
{

}

const char* Plugin_Description()
{

}

const char* Plugin_ErrorCodeDescription(uint32_t error_code, uint16_t reserved)
{

}

uint32_t Archive_Open(archive_handle* archive, char* local_path)
{

}

uint32_t Archive_Create(archive_handle* archive, char* local_path, char* flags)
{

}

uint32_t Archive_Save(archive_handle archive)
{

}

uint32_t Archive_Close(archive_handle archive)
{

}

uint32_t Archive_AddFileLocal(archive_handle archive, archive_entry* archive_item, char* local_path)
{

}

uint32_t Archive_RemoveEntry(archive_handle archive, archive_entry* archive_item)
{

}

uint32_t Archive_CreateDirectory(archive_handle archive, archive_entry* archive_path)
{

}

uint32_t Archive_EnumerateEntries(archive_handle archive, archive_enumerate_callback callback)
{

}

uint32_t Archive_ExtractFiles(archive_handle archive, char* local_dir_path, archive_entry* archive_item, uint64_t archive_item_count)
{

}


uint32_t Archive_RegisterStatusCallback(status_callback callback)
{

}

uint32_t Archive_GetEntryInfo(archive_handle archive, archive_entry* archive_item)
{

}
