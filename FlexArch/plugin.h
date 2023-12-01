#ifndef FLEX_ARCH_PLUGIN_H
#define FLEX_ARCH_PLUGIN_H

#include <wchar.h>

/*
* ��� ����� ���� �����?
* - ���� ����
* - ��������� ������ ����� ��������� ����������� �� ���������� ������
* - ��������� ������ ����� �� ������ ������ �� ��������� (���� ��������� ��� ��� ������������)
* 
* ��� ������ ����� ������� ��������?
* - ������� ����� �� ������
* - ����������� ����� �� ������
* - ������ ������ ������ � ������
* - �������� �� ������ ������� ������� ���� �������� ������
* - �������� ���� � ������
* - ������� ���� �� ������
* - �������� ���� � ������ (�������+�������� � ���� ��������)
* - ��������� � ������ �����
* - �������� � ������ ����������?
* - ����� ��������� ��������� ������� �������� ������� (������� ����������, ������ ���������, ...)
* */

struct FlexArchPlugin_InfoStruct_t
{
    const wchar_t* (*GetPluginName)();
    const wchar_t* (*GetPluginDescription)();
    const wchar_t* (*GetFilenameFilters)();
    const wchar_t* (*GetCapabilities)();

    void* placeholder_keep_zeroed[128]; //for future expansions
};
typedef struct FlexArchPlugin_InfoStruct_t FlexArchPlugin_InfoStruct;

void (*FlexArchPlugin_GetInfoStruct)(FlexArchPlugin_InfoStruct *dst);

#endif
