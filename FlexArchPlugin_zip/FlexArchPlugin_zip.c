#include <FlexArch/plugin.h>
#include <zip.h>
#include <string.h>
#include <stdlib.h>


//internal, hidden fron plugin user stuff
struct dirinfo_t
{
    zip_int64_t id;
    char *name;
};
typedef struct dirinfo_t dirinfo;

struct archive_context_t
{
    zip_t* zip_handle;

    //due to libzip and zip itself specific structure we need to separate file file names and folder names
    dirinfo* directories;
    uint64_t directories_count;
};
typedef struct archive_context_t *archive_context;

static FlexArchResult PushDirectory(archive_context archive, const char *name, zip_int64_t id)
{
    void* temp;
    size_t len;

    len = strlen(name);
    if (len < 1)
    {
        return FA_SYSTEM_ERROR;//TODO: not an actual system error
    }

    temp = realloc(archive->directories, sizeof(*archive->directories) * (size_t)(archive->directories_count + 1));
    if (!temp)
    {
        return FA_SYSTEM_ERROR;
    }

    archive->directories_count++;
    archive->directories = temp;

    archive->directories[archive->directories_count - 1].id = id;
    archive->directories[archive->directories_count - 1].name = strdup(name);

    if (!archive->directories[archive->directories_count - 1].name)
    {
        return FA_SYSTEM_ERROR;
    }

    archive->directories[archive->directories_count - 1].name[len - 1] = '\0';

    return FA_SUCCESS;
}

static FlexArchResult EnumerateDirectories(archive_context archive)
{
    zip_int64_t num_entries = zip_get_num_entries(archive->zip_handle, 0);
    zip_int64_t i;
    zip_stat_t stat;
    FlexArchResult r;

    for (i = 0; i < num_entries; ++i)
    {
        zip_stat_index(archive->zip_handle, i, ZIP_FL_ENC_UTF_8, &stat);
        size_t len = strlen(stat.name);
        if (len < 1)
        {
            continue;
        }

        if (stat.name[len - 1] != '/')
        {
            continue;
        }

        //ok, this is directory. now we need path to directory as token path
        r = PushDirectory(archive, stat.name, stat.index);
        if (r != FA_SUCCESS)
        {
            free(archive->directories);
            archive->directories = NULL;
            archive->directories_count = 0;
            return r;
        }
    }

    return FA_SUCCESS;
}

static FlexArchResult FixEntryDirectory(archive_context archive, archive_entry* entry)
{
    char* namestart;
    char* dirpath;
    size_t len;

    len = strlen(entry->super_name);

    dirpath = entry->super_name;

    namestart = strrchr(entry->super_name, '/');
    if (!namestart || (entry->super_name + len) < namestart)
    {
        return FA_SYSTEM_ERROR; //TODO: is it so system? well, it should't ever occur...
    }

    *namestart = '\0';
    entry->super_name = namestart + 1;

    uint64_t i;
    for (i=0; i< archive->directories_count; ++i)
    {
        if (!strcmp(dirpath, archive->directories[i].name))
        {
            entry->parent = archive->directories[i].id;
            return FA_SUCCESS;
        }
    }

    return FA_SYSTEM_ERROR; //TODO: not an actual system error
}

//interface functions
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
    zip_error_t err;

    err.zip_err = error_code &= 0x7FFFFFFF;
    
    return zip_error_strerror(&err);
}

FlexArchResult Archive_Open(archive_context* archive, char* local_path)
{
    zip_t* zip;
    FlexArchResult r;

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

    r = EnumerateDirectories(*archive);
    if (r != FA_SUCCESS)
    {
        free(*archive);
        zip_close(zip);
        return r;
    }

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
    free(archive->directories);
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
    char *curname = NULL;
    FlexArchResult r;

    for (i=0; i< num_entries; ++i)
    {
        if (zip_stat_index(archive->zip_handle, i, ZIP_FL_ENC_UTF_8, &stat))
        {
            return zip_get_error(archive->zip_handle)->zip_err | 0x80000000;
        }

        entry.id = stat.index;
        entry.flags = 0;
        entry.parent = 0;
        entry.size = stat.size;
        entry.size_in_archive = stat.comp_size;
        entry.creation_date = 0;
        entry.modification_date = stat.mtime;

        curname = strdup(stat.name);
        if (!curname)
        {
            return FA_SYSTEM_ERROR;
        }

        size_t len_name = strlen(curname);
        if (len_name > 1 && curname[len_name - 1] == '/')
        {
            curname[strlen(curname) - 1] = '\0';
            entry.flags |= FA_ENTRY_IS_DIRECTORY;
        }
        entry.super_name = curname;

        if (strchr(entry.super_name, '/'))
        {
            //get parent directory, cut directory path
            r = FixEntryDirectory(archive , &entry);
            if (r != FA_SUCCESS)
            {
                free(curname);
                return r;
            }
        }

        callback(archive, context , &entry, i == num_entries - 1 ? 1 : 0);

        free(curname);
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
