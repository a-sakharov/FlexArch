#include <stdio.h>
#include <stdbool.h>
#include <Windows.h>
#include <getopt.h>
#include <FlexArch/plugin.h>
#include "common_code.h"


void PrintHelp();
void PrintVersion();
void ListPlugins();

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
        printf("%zd: %ls: %s\n", i+1, LoadedPlugins[i].module_name, LoadedPlugins[i].Plugin_GetName());
    }
}

int main(int argc, char** argv)
{
    CollectPlugins();

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
    char *archive = NULL;
    bool list = false;
    bool extract = false;
    bool add = false;

    while (1)
    {
        c = getopt_long(argc, argv, "vpa:ldc", long_options, NULL);
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

            case 'a':
            archive = optarg;
            break;

            case 'l':
            list = true;
            break;
            
            case 'd':
            extract = true;
            break;

            case 'c':
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

    if (!archive)
    {
        printf("Archive not specified!");
        return -1;
    }

    FreePlugins();

    return 0;
}