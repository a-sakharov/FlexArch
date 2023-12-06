#ifndef COMMON_CODE_H
#define COMMON_CODE_H

#include <FlexArch/plugin.h>
#include <stdbool.h>
#ifdef _WIN32
#include <Windows.h>
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

#ifdef _WIN32
    HMODULE module;
    wchar_t module_name[MAX_PATH];
#endif
};
typedef struct PluginFunctionsCollection_t PluginFunctionsCollection;

struct opened_archive_t
{
    archive_handle handle;

};
typedef struct opened_archive_t opened_archive;

extern PluginFunctionsCollection* LoadedPlugins;
extern size_t LoadedPluginsCount;

void CollectPlugins();
void FreePlugins();

#endif
