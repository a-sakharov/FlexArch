#ifndef FLEX_ARCH_PLUGIN_H
#define FLEX_ARCH_PLUGIN_H

#include <wchar.h>

/*
* Чем может быть архив?
* - Один файл
* - Несколько файлов тесно связанных соглашением об именовании файлов
* - Несколько файлов никак на первый взгляд не связанных (надо продумать как это организовать)
* 
* Что должен уметь хороший архиватр?
* - Создать архив из файлов
* - Распаковать файлы из архива
* - Выдать список файлов в архиве
* - Сообщить об ошибке внятным текстом если возникла ошибка
* - Добавить файл к архиву
* - Удалить файл из архива
* - Заменить файл в архиве (удалить+добавить в одно действие)
* - Создавать в архиве папки
* - Добавить к архиву метаданные?
* - Иметь несколько возможных режимов создания архивов (уровень компрессии, способ архивации, ...)
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
