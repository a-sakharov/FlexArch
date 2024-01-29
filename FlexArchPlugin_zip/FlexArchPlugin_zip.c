#include <FlexArchBase/plugin.h>
#include <zip.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#if defined(_WIN32) && defined(_DEBUG)
#include <crtdbg.h>
#endif


//internal, hidden from plugin user stuff
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

    char *archive_path;
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

    archive->directories[archive->directories_count - 1].id = id + 1;//id=0 reserved for core directory
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
    archive_context arcontext = (archive_context)archive;

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
    entry->super_name = strdup(namestart + 1);
    if (!entry->super_name)
    {
        return FA_SYSTEM_ERROR;
    }

    uint64_t i;
    for (i=0; i< archive->directories_count; ++i)
    {
        if (!strcmp(dirpath, archive->directories[i].name))
        {
            free(dirpath);
            entry->parent = archive->directories[i].id;
            return FA_SUCCESS;
        }
    }

    free(entry->super_name);

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

FlexArchResult Archive_Open(archive_handle* archive, const char* local_path)
{
    archive_context arcontext;
    zip_t* zip;
    FlexArchResult r;

    zip = zip_open(local_path, 0, NULL);
    if (!zip)
    {
        return FA_CORRUPTED_ARCHIVE;
    }

    *archive = calloc(1, sizeof(*arcontext));
    if (!*archive)
    {
        zip_close(zip);
        return FA_SYSTEM_ERROR;
    }

    arcontext = (archive_context)*archive;

    arcontext->zip_handle = zip;

    r = EnumerateDirectories(arcontext);
    if (r != FA_SUCCESS)
    {
        free(arcontext);
        zip_close(zip);
        return r;
    }

    arcontext->archive_path = strdup(local_path);

    return FA_SUCCESS;
}

FlexArchResult Archive_Create(archive_handle* archive, char* local_path, char* flags)
{
    archive_context arcontext = (archive_context)archive;
    zip_t* zip;
    FlexArchResult r;

    zip = zip_open(local_path, ZIP_CREATE, NULL);
    if (!zip)
    {
        return FA_CORRUPTED_ARCHIVE;
    }

    *archive = calloc(1, sizeof(*arcontext));
    if (!*archive)
    {
        zip_close(zip);
        return FA_SYSTEM_ERROR;
    }

    arcontext = (archive_context)*archive;

    arcontext->zip_handle = zip;

    r = EnumerateDirectories(arcontext);
    if (r != FA_SUCCESS)
    {
        free(arcontext);
        zip_close(zip);
        return r;
    }

    zip_set_archive_flag(arcontext->zip_handle, ZIP_AFL_CREATE_OR_KEEP_FILE_FOR_EMPTY_ARCHIVE, 1);

    arcontext->archive_path = strdup(local_path);

    return FA_SUCCESS;
}

FlexArchResult Archive_Save(archive_handle archive)
{
    archive_context arcontext = (archive_context)archive;
    
    zip_close(arcontext->zip_handle);
    arcontext->zip_handle = zip_open(arcontext->archive_path, 0, NULL);

    return FA_SUCCESS;
}

FlexArchResult Archive_Close(archive_handle archive)
{
    archive_context arcontext = (archive_context)archive;

    uint64_t i;

    for (i = 0; i < arcontext->directories_count; ++i)
    {
        free(arcontext->directories[i].name);
    }

    free(arcontext->archive_path);
    free(arcontext->directories);
    zip_discard(arcontext->zip_handle);
    free(arcontext);

#if defined(_WIN32) && defined(_DEBUG)
    _CrtDumpMemoryLeaks();
#endif

    return FA_SUCCESS;
}

FlexArchResult Archive_AddEntry(archive_handle archive, archive_entry* archive_item, char* local_path)
{
    archive_context arcontext = (archive_context)archive;

    return FA_NOT_IMPLEMENTID;
}

FlexArchResult Archive_RemoveEntry(archive_handle archive, archive_entry* archive_item)
{
    archive_context arcontext = (archive_context)archive;

    return FA_NOT_IMPLEMENTID;
}
#if 0
FlexArchResult Archive_CreateDirectory(archive_handle archive, archive_entry* archive_path)
{
    archive_context arcontext = (archive_context)archive;

    return FA_NOT_IMPLEMENTID;
}
#endif
FlexArchResult Archive_EnumerateEntries(archive_handle archive, void *context, archive_enumerate_callback callback)
{
    archive_context arcontext = (archive_context)archive;

    zip_int64_t num_entries = zip_get_num_entries(arcontext->zip_handle, 0);
    zip_int64_t i;
    archive_entry entry;
    FlexArchResult r;

    for (i=0; i< num_entries; ++i)
    {
        entry.id = i + 1;

        r = Archive_GetEntryInfo(archive, &entry);
        if (r != FA_SUCCESS)
        {
            return r;
        }

        callback(archive, context , &entry, i == num_entries - 1 ? 1 : 0);

        free(entry.super_name);
    }

    return FA_SUCCESS;
}

FlexArchResult Archive_ExtractFiles(archive_handle archive, char* local_dir_path, archive_entry* archive_item, uint64_t archive_item_count)
{
    archive_context arcontext = (archive_context)archive;

    return FA_NOT_IMPLEMENTID;
}

FlexArchResult Archive_RegisterStatusCallback(status_callback callback)
{

    return FA_NOT_IMPLEMENTID;
}

FlexArchResult Archive_GetEntryInfo(archive_handle archive, archive_entry* archive_item)
{
    archive_context arcontext = (archive_context)archive;

    zip_stat_t stat;
    FlexArchResult r;

    if (zip_stat_index(arcontext->zip_handle, archive_item->id - 1, ZIP_FL_ENC_UTF_8, &stat))
    {
        return zip_get_error(arcontext->zip_handle)->zip_err | 0x80000000;
    }

    archive_item->flags = 0;
    archive_item->parent = 0;
    archive_item->size = stat.size;
    archive_item->size_in_archive = stat.comp_size;
    archive_item->creation_date = 0;
    archive_item->modification_date = stat.mtime;

    archive_item->super_name = strdup(stat.name);
    if (!archive_item->super_name)
    {
        return FA_SYSTEM_ERROR;
    }

    size_t len_name = strlen(archive_item->super_name);
    if (len_name > 1 && archive_item->super_name[len_name - 1] == '/')
    {
        archive_item->super_name[len_name - 1] = '\0';
        archive_item->flags |= FA_ENTRY_IS_DIRECTORY;
    }

    if (strchr(archive_item->super_name, '/'))
    {
        //get parent directory, cut directory path
        r = FixEntryDirectory(arcontext, archive_item);
        if (r != FA_SUCCESS)
        {
            free(archive_item->super_name);
            return r;
        }
    }

    return FA_SUCCESS;
}
