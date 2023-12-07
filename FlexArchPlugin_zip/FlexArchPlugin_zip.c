#include <FlexArch/plugin.h>
#include <zip.h>
#include <string.h>
#include <stdlib.h>

struct dirlist_t
{
    zip_int64_t id;
    char *directory;
};
typedef struct dirlist_t dirlist;

struct archive_context_t
{
    zip_t* zip_handle;

    //libzip 
    dirlist* directories;
    uint64_t directories_count;
};
typedef struct archive_context_t *archive_context;

const char* Plugin_GetName()
{
    return u8"ZIP FlexArch plugin";
}

const char* Plugin_Description()
{
    return u8"Sample FlexArch plugin based on libzip from libzip.org";
}

const char* Plugin_ErrorCodeDescription(FlexArchResult error_code, uint16_t reserved)
{
    return u8"";
}

FlexArchResult Archive_Open(archive_context* archive, char* local_path)
{
    zip_t* zip;

    zip = zip_open(local_path, 0, NULL);
    if (!zip)
    {
        return FA_CORRUPTED_ARCHIVE;
    }

    *archive = calloc(1, sizeof(**archive));
    if (!*archive)
    {
        zip_close(zip);
        return FA_SYSTEM_ERROR;
    }

    (*archive)->zip_handle = zip;

    return FA_SUCCESS;
}

FlexArchResult Archive_Create(archive_context* archive, char* local_path, char* flags)
{
    return FA_NOT_IMPLEMENTID;
}

FlexArchResult Archive_Save(archive_context archive)
{
    return FA_SUCCESS; //there no special SAVE call for libzip, all work done in Archive_Close
}

FlexArchResult Archive_Close(archive_context archive)
{
    zip_close(archive->zip_handle);
    free(archive);

    return FA_SUCCESS;
}

FlexArchResult Archive_AddFileLocal(archive_context archive, archive_entry* archive_item, char* local_path)
{
    return FA_NOT_IMPLEMENTID;
}

FlexArchResult Archive_RemoveEntry(archive_context archive, archive_entry* archive_item)
{
    return FA_NOT_IMPLEMENTID;
}

FlexArchResult Archive_CreateDirectory(archive_context archive, archive_entry* archive_path)
{
    return FA_NOT_IMPLEMENTID;
}

FlexArchResult Archive_EnumerateEntries(archive_context archive, void *context, archive_enumerate_callback callback)
{
    zip_int64_t num_entries = zip_get_num_entries(archive->zip_handle, 0);
    zip_int64_t i;
    archive_entry entry;
    zip_stat_t stat;
    char curname[256];


    for (i=0; i< num_entries; ++i)
    {
        zip_stat_index(archive->zip_handle, i, ZIP_FL_ENC_UTF_8, &stat);

        entry.id = stat.index;
        entry.flags = 0;
        entry.parent = 0;
        entry.size = stat.size;
        entry.size_in_archive = stat.comp_size;
        entry.creation_date = 0;
        entry.modification_date = stat.mtime;

        strncpy(curname, stat.name, sizeof(curname));

        if (curname[strlen(curname) - 1] == '/')
        {
            curname[strlen(entry.super_name) - 1] = '\0';
            entry.flags |= FA_ENTRY_IS_DIRECTORY;
        }
        entry.super_name = curname;

        callback(archive, context , &entry, i == num_entries - 1 ? 1 : 0);
    }

    return FA_SUCCESS;
}

FlexArchResult Archive_ExtractFiles(archive_context archive, char* local_dir_path, archive_entry* archive_item, uint64_t archive_item_count)
{
    return FA_NOT_IMPLEMENTID;
}

FlexArchResult Archive_RegisterStatusCallback(status_callback callback)
{
    return FA_NOT_IMPLEMENTID;
}

FlexArchResult Archive_GetEntryInfo(archive_context archive, archive_entry* archive_item)
{
    return FA_NOT_IMPLEMENTID;
}
