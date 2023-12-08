#include "FlexArchInternals.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>



PluginFunctionsCollection* LoadedPlugins;
size_t LoadedPluginsCount;


#ifdef _WIN32
#define LOADADDR(x) plugin->x = (x)GetProcAddress(plugin->module, #x); if(!plugin->x) return false;
static bool FlexArch_LoadPlugin(PluginFunctionsCollection* plugin, wchar_t* path)
{
    plugin->module = LoadLibraryW(path);
    if (!plugin->module)
    {
        return false;
    }

    LOADADDR(Plugin_GetName);
    LOADADDR(Plugin_Description);
    LOADADDR(Plugin_ErrorCodeDescription);
    LOADADDR(Archive_Open);
    LOADADDR(Archive_Create);
    LOADADDR(Archive_Save);
    LOADADDR(Archive_Close);
    LOADADDR(Archive_AddFileLocal);
    LOADADDR(Archive_RemoveEntry);
    LOADADDR(Archive_CreateDirectory);
    LOADADDR(Archive_EnumerateEntries);
    LOADADDR(Archive_ExtractFiles);
    LOADADDR(Archive_RegisterStatusCallback);
    LOADADDR(Archive_GetEntryInfo);

    WideCharToMultiByte(CP_UTF8, 0, path, -1, plugin->module_name, sizeof(plugin->module_name), NULL, NULL);

    return true;
}
#undef LOADADDR

void FlexArch_CollectPlugins()
{
    PluginFunctionsCollection plugin;
    WIN32_FIND_DATAW found;
    HANDLE find_handle;
    void* temp;

    if (LoadedPlugins || LoadedPluginsCount)
    {
        return;
    }

    LoadedPlugins = NULL;
    LoadedPluginsCount = 0;

    find_handle = FindFirstFileW(L"FlexArchPlugin_*.dll", &found);
    if (find_handle == INVALID_HANDLE_VALUE)
    {
        return;
    }

    do
    {
        if (!FlexArch_LoadPlugin(&plugin, found.cFileName))
        {
            continue;
        }

        LoadedPluginsCount++;
        temp = realloc(LoadedPlugins, sizeof(PluginFunctionsCollection) * LoadedPluginsCount);
        if (!temp)
        {
            free(LoadedPlugins);
            LoadedPlugins = NULL;
            LoadedPluginsCount = 0;
            break;
        }
        LoadedPlugins = temp;

        memcpy(&LoadedPlugins[LoadedPluginsCount - 1], &plugin, sizeof(PluginFunctionsCollection));

    } while (FindNextFile(find_handle, &found));

    FindClose(find_handle);
}
#else

#define LOADADDR(x) plugin->x = (x)dlsym(plugin->module, #x); if(!plugin->x) {dlclose(plugin->module); return false;}
static bool FlexArch_LoadPlugin(PluginFunctionsCollection* plugin, char* path)
{
    char tempname[PATH_MAX];
    snprintf(tempname, sizeof(tempname), "./%s", path);

    plugin->module = dlopen(tempname, RTLD_LAZY);
    if (!plugin->module)
    {
        return false;
    }

    LOADADDR(Plugin_GetName);
    LOADADDR(Plugin_Description);
    LOADADDR(Plugin_ErrorCodeDescription);
    LOADADDR(Archive_Open);
    LOADADDR(Archive_Create);
    LOADADDR(Archive_Save);
    LOADADDR(Archive_Close);
    LOADADDR(Archive_AddFileLocal);
    LOADADDR(Archive_RemoveEntry);
    LOADADDR(Archive_CreateDirectory);
    LOADADDR(Archive_EnumerateEntries);
    LOADADDR(Archive_ExtractFiles);
    LOADADDR(Archive_RegisterStatusCallback);
    LOADADDR(Archive_GetEntryInfo);

    strncpy(plugin->module_name, path, sizeof(plugin->module_name));
}
#undef LOADADDR

static int filter_plugins(const struct dirent *diritem)
{
    if(diritem->d_type != DT_REG)
    {
        return 0;
    }

    if(strstr(diritem->d_name, "FlexArchPlugin_") != diritem->d_name)
    {
        return 0;
    }

    if(!strstr(diritem->d_name + strlen(diritem->d_name) - 3, ".so"))
    {
        return 0;
    }

    return 1;
}

void FlexArch_CollectPlugins()
{
    struct dirent **namelist;
    int n;
    PluginFunctionsCollection plugin;
    bool enumerating_ok = true;
    void* temp;

    n = scandir(".", &namelist, filter_plugins, alphasort);
    if (n == -1) 
    {
        return;
    }

    while (n--) 
    {
        if(enumerating_ok && FlexArch_LoadPlugin(&plugin, namelist[n]->d_name))
        {
            LoadedPluginsCount++;
            temp = realloc(LoadedPlugins, sizeof(PluginFunctionsCollection) * LoadedPluginsCount);
            if (!temp)
            {
                free(LoadedPlugins);
                LoadedPlugins = NULL;
                LoadedPluginsCount = 0;

                enumerating_ok = false;
            }
            else
            {
                LoadedPlugins = temp;
                memcpy(&LoadedPlugins[LoadedPluginsCount - 1], &plugin, sizeof(PluginFunctionsCollection));
            }
        }
        free(namelist[n]);
    }
    free(namelist);
}
#endif

void FlexArch_FreePlugins()
{
    size_t i;

    for(i=0; i<LoadedPluginsCount; ++i)
    {
#ifndef _WIN32
        dlclose(LoadedPlugins[i].module);
#endif
    }

    free(LoadedPlugins);

    LoadedPlugins = NULL;
    LoadedPluginsCount = 0;
}

bool FlexArch_TryOpenArchive(opened_archive *arch, char *path)
{
    size_t i;

    for (i = 0; i < LoadedPluginsCount; ++i)
    {
        if (LoadedPlugins[i].Archive_Open(&arch->handle, path) == FA_SUCCESS)
        {
            memcpy(&arch->used_plugin, &LoadedPlugins[i], sizeof(*LoadedPlugins));
            arch->have_unsaved_changes = false;
            return true;
        }
    }

    return false;
}

const char* FlexArch_GetErrorDescription(FlexArchResult error_code)
{
    return u8"placeholder_value";
}

void FlexArch_FormatSizeHumanly(char* str, size_t chars, uint64_t size)
{
    uint64_t sz;
    uint64_t subsz;
    char ltr;
    uint64_t m;

    if (size < 1024)//bytes
    {
        snprintf(str, chars, "%"PRIu64, size);
        return;
    }
    else if (size < 1024 * 1024)//Kbytes
    {
        m = 1024;
        ltr = 'K';
    }
    else if (size < 1024 * 1024 * 1024)//Mbytes
    {
        m = 1024 * 1024;
        ltr = 'M';
    }
    else //Gbytes
    {
        m = 1024 * 1024 * 1024;
        ltr = 'G';
    }

    sz = size / m;
    subsz = ((size % m) * 10) / m;

    if (subsz)
    {
        snprintf(str, chars, "%"PRIu64 ".%"PRIu64 "%c", sz, subsz, ltr);
    }
    else
    {
        snprintf(str, chars, "%"PRIu64 "%c", sz, ltr);
    }
}

char* FlexArch_CreateFullPath(opened_archive* arch, archive_entry* entry)
{
    char* result = NULL;
    void* temp;
    char* fname;
    FlexArchResult r;
    archive_entry local_entry;
    size_t len;

    result = strdup(entry->super_name ? entry->super_name : entry->name);

    local_entry.parent = entry->parent;

    while (local_entry.parent != 0)
    {
        local_entry.id = local_entry.parent;

        r = arch->used_plugin.Archive_GetEntryInfo(arch->handle, &local_entry);
        if (r != FA_SUCCESS)
        {
            free(result);
            return NULL;
        }

        fname = local_entry.super_name ? local_entry.super_name : local_entry.name;
        len = strlen(fname);

        temp = malloc(strlen(result) + 1 + len + 1);
        if (!temp)
        {
            if (local_entry.super_name)
            {
                free(local_entry.super_name);
            }
            free(result);
            return NULL;
        }

        memcpy(temp, fname, len);
        ((char*)temp)[len] = '/';
        memcpy((char*)temp + len + 1, result, strlen(result) + 1);
        free(result);
        result = temp;

        if (local_entry.super_name)
        {
            free(local_entry.super_name);
        }
    }

    return result;
}
