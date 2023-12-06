#include "FlexArchInternals.h"

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

    wcscpy_s(plugin->module_name, sizeof(plugin->module_name) / sizeof(*plugin->module_name), path);

    return true;
}
#undef LOADADDR

PluginFunctionsCollection* LoadedPlugins;
size_t LoadedPluginsCount;

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

void FlexArch_FreePlugins()
{
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
