#ifndef FLEX_ARCH_PLUGIN_H
#define FLEX_ARCH_PLUGIN_H

#include <wchar.h>


struct FlexArchPlugin_InfoStruct_t
{
    const wchar_t* (*GetPluginName)();
    const wchar_t* (*GetPluginDescription)();
    const wchar_t* (*GetFilenameFilters)();
    const wchar_t* (*GetCapabilities)();

    //
    //get archive contents list
    //extract archive contents (fully or partially?)
    //   
    //create archive
    //add file to archive
    //remove file from archive
    //
    //test is file a compatible archive
};
typedef struct FlexArchPlugin_InfoStruct_t FlexArchPlugin_InfoStruct;

void (*FlexArchPlugin_GetInfoStruct)(FlexArchPlugin_InfoStruct *dst);

#endif
