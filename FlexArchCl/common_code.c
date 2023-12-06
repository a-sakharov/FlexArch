#include "common_code.h"

#define LOADADDR(x) plugin->x = (x)GetProcAddress(plugin->module, #x); if(!plugin->x) return false;
bool LoadPlugin(PluginFunctionsCollection* plugin, wchar_t* path)
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

void CollectPlugins()
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
        if (!LoadPlugin(&plugin, found.cFileName))
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

void FreePlugins()
{
    free(LoadedPlugins);

    LoadedPlugins = NULL;
    LoadedPluginsCount = 0;
}

int TryOpenArchive()
{

}