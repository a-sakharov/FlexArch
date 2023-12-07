#ifndef FLEXARCHINTERNALS_H
#define FLEXARCHINTERNALS_H

#include <FlexArch/plugin.h>
#include <stdbool.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <linux/limits.h>
#define MAX_PATH PATH_MAX
#endif

struct PluginFunctionsCollection_t
{
    Plugin_GetName                  Plugin_GetName;
    Plugin_Description              Plugin_Description;
    Plugin_ErrorCodeDescription     Plugin_ErrorCodeDescription;

    Archive_Open                    Archive_Open;
    Archive_Create                  Archive_Create;
    Archive_Save                    Archive_Save;
    Archive_Close                   Archive_Close;
    Archive_AddFileLocal            Archive_AddFileLocal;
    Archive_RemoveEntry             Archive_RemoveEntry;
    Archive_CreateDirectory         Archive_CreateDirectory;
    Archive_EnumerateEntries        Archive_EnumerateEntries;
    Archive_ExtractFiles            Archive_ExtractFiles;
    Archive_RegisterStatusCallback  Archive_RegisterStatusCallback;
    Archive_GetEntryInfo            Archive_GetEntryInfo;

    char module_name[MAX_PATH];

#ifdef _WIN32
    HMODULE module;
#else
    void *module;    
#endif
};
typedef struct PluginFunctionsCollection_t PluginFunctionsCollection;

struct opened_archive_t
{
    archive_handle handle;
    PluginFunctionsCollection used_plugin;
    bool have_unsaved_changes;
};
typedef struct opened_archive_t opened_archive;

extern PluginFunctionsCollection* LoadedPlugins;
extern size_t LoadedPluginsCount;

void FlexArch_CollectPlugins();
void FlexArch_FreePlugins();
bool FlexArch_TryOpenArchive(opened_archive* arch, char* path);
const char* FlexArch_GetErrorDescription(FlexArchResult error_code);
void FlexArch_FormatSizeHumanly(char* str, size_t chars, uint64_t size);

#endif
