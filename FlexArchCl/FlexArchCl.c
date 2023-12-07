#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>
#include <ctype.h>
#include <inttypes.h>
#include <FlexArch/plugin.h>
#include "FlexArchInternals.h"


void PrintHelp();
void PrintVersion();
void ListPlugins();
void archive_enumeration_receiver(archive_handle archive, void* context, archive_entry* entry, uint8_t last_item);

void PrintHelp()
{
    PrintVersion();

    printf("-v or --version to get version\n");
    printf("-p or --plugins to list loaded plugins\n");
    printf("-h or --help to get this help\n");
    printf("-f <name> or --archive <name> to specify archive to work with\n");
    printf("-l or --list to list files in archive\n");
    printf("-d or --extract to extract files from archive to current folder\n");
    printf("-a or --add to add files from current folder to archive\n");
}

void PrintVersion()
{
    printf("FlexArchCl. Alpha build %s %s\n", __DATE__, __TIME__);
}

void ListPlugins()
{
    size_t i;
    printf("Total %zd plugins loaded:\n", LoadedPluginsCount);
    for (i = 0; i < LoadedPluginsCount; ++i)
    {
        printf("%zd: %s: %s\n", i+1, LoadedPlugins[i].module_name, LoadedPlugins[i].Plugin_GetName());
    }
}

void ListArchiveFiles(opened_archive *archive)
{
    printf("ID\tSIZE\t\tCDATE\t\t\tMDATE\t\t\tNAME\n");
    archive->used_plugin.Archive_EnumerateEntries(archive->handle, NULL, archive_enumeration_receiver);
}

void archive_enumeration_receiver(archive_handle archive, void* context, archive_entry* entry, uint8_t last_item)
{
    struct tm ctime;
    struct tm mtime;

    gmtime_s(&ctime, &entry->creation_date);
    gmtime_s(&mtime, &entry->modification_date);

    char ctime_iso[64];
    char mtime_iso[64];

    strftime(ctime_iso, sizeof(ctime_iso), "%Y-%m-%d %H:%M:%S", &ctime);
    strftime(mtime_iso, sizeof(mtime_iso), "%Y-%m-%d %H:%M:%S", &mtime);

    char size[64];

    if (entry->flags & FA_ENTRY_IS_DIRECTORY)
    {
        snprintf(size, sizeof(size), "<DIR>");
    }
    else
    {
        //constrt size to kb, mb, gb?
        //add space every 3 digits?
        snprintf(size, sizeof(size), "%"PRIu64, entry->size);
    }

    printf("%"PRIu64 "\t" "%s\t\t" "%s\t" "%s\t" "%s\n", entry->id, size, ctime_iso, mtime_iso, entry->super_name ? entry->super_name : entry->name);
}

void ReportError(char *situation, FlexArchResult code, PluginFunctionsCollection *plugin)
{
    printf("While performing %s got error %ud (%s)", situation, code,
        code <= FA_PREDEFINED_ERRORS_MAX ? FlexArch_GetErrorDescription(code) : plugin->Plugin_ErrorCodeDescription(code, 0));
}

int main(int argc, char** argv)
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    FlexArch_CollectPlugins();

    int c;
    static struct option long_options[] = 
    {
        { "version", no_argument,       0, 'v' },
        { "plugins", no_argument,       0, 'p' },
        { "help",    no_argument,       0, 'h' },
        { "archive", required_argument, 0, 'f' },
        { "list",    no_argument,       0, 'l' },
        { "extract", no_argument,       0, 'd' },
        { "add",     no_argument,       0, 'a' },
        { 0,         0,                 0,  0 }
    };
    char *archive_path = NULL;
    bool list = false;
    bool extract = false;
    bool add = false;

    while (1)
    {
        c = getopt_long(argc, argv, "vphf:lda", long_options, NULL);
        if (c == -1)
        {
            break;
        }

        switch (c) 
        {
            case 'v':
            PrintVersion();
            break;

            case 'p':
            ListPlugins();
            break;

            case 'h':
            PrintHelp();
            break;

            case 'f':
            archive_path = optarg;
            break;

            case 'l':
            list = true;
            break;
            
            case 'd':
            extract = true;
            break;

            case 'a':
            add = true;
            break;

            default:
            if (isgraph(c))
            {
                printf("Error: Undefined option %c passed\n", c);
            }
            else
            {
                printf("Error: Undefined option passed\n");
            }
            return -1;
            break;
        }
    }

    if (!archive_path)
    {
        printf("Archive not specified!\n");
        return -1;
    }

    opened_archive a;

    if (!FlexArch_TryOpenArchive(&a, archive_path))
    {
        printf("Could not open archive %s\n", archive_path);
        return -1;
    }

    if (list)
    {
        ListArchiveFiles(&a);
    }

    if (add)
    {

    }

    if (extract)
    {

    }

    a.used_plugin.Archive_Save(a.handle);
    a.used_plugin.Archive_Close(a.handle);

    FlexArch_FreePlugins();

    return 0;
}
