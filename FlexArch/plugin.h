#ifndef FLEX_ARCH_PLUGIN_H
#define FLEX_ARCH_PLUGIN_H

#include <wchar.h>
#include <stdint.h>
#include <time.h>

#ifdef __GNUC__
#define FLEXARCH_CALL_TYPE //__attribute__((__cdecl__))
#define FLEXARCH_EXPORT __attribute__((dllexport))
#elif _MSC_VER
#define FLEXARCH_CALL_TYPE _cdecl
#define FLEXARCH_EXPORT __declspec(dllexport)
#endif

#ifdef FLEXARCH_INTERNAL
#define FLEXARCH_PLUGIN_API(x)          uint32_t FLEXARCH_CALL_TYPE x
#define FLEXARCH_PLUGIN_API_STRING(x)   const char * FLEXARCH_CALL_TYPE x
#else
#define FLEXARCH_PLUGIN_API(x)          FLEXARCH_EXPORT uint32_t (FLEXARCH_CALL_TYPE *x)
#define FLEXARCH_PLUGIN_API_STRING(x)   FLEXARCH_EXPORT const char * (FLEXARCH_CALL_TYPE *x)
#endif

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
* 
*/

/*
* Programming rules:
* 1. All strings in UTF-8
* 2. Any path should use normal slash. That one: /
* 3. All return values - error codes. Some of them defined in FA_PREDESINED_ERRORS, other could be implementation-defined and describer through Plugin_ErrorCodeDescription
* 4. Every entry inside archive should have unique number. It is not important how those numbers assigned, but those numbers should be unique and constant. Those numbers could only be changed after Archive_Save.
* 5. 
*/

struct archive_entry_t
{
    uint64_t id;                /**< unique id inside archive*/
    char name[128];             /**< utf-8 name*/
    char *super_name;           /**< null if not used, pointer to string if `name` size is not enought*/
    uint64_t flags;             /**< defined in ARCHIVE_ENTRY_FLAGS*/
    uint64_t parent;            /**< 0 if top-level, othervise id of parent entry (parent should have directory flag)*/
    uint64_t size;              /**< uncompressed size*/
    uint64_t size_in_archive;   /**< compressed size*/
    time_t creation_date;       /**< 0 if unavailable. Always in UTC+0.*/
    time_t modification_date;   /**< 0 if unavailable. Always in UTC+0.*/


    void* reserved[64];/**<i am big fan of ms approach. should be zero. */
};

typedef struct archive_entry_t archive_entry;

enum ARCHIVE_ENTRY_FLAGS
{
    FA_ENTRY_IS_DIRECTORY       = 1 << 0, //1 if directory, 0 if file
    FA_ENTRY_IS_LINK            = 1 << 1, //1 if link, 0 if not
    FA_ENTRY_RAW_SIZE_UNKNOWN   = 1 << 2, //1 if size unknown, 0 if known
    FA_ENTRY_ARC_SIZE_UNKNOWN   = 1 << 3, //1 if size_in_archive unknown, 0 if known

};

enum FA_PREDESINED_ERRORS
{
    FA_SUCCESS = 0, //no error
    FA_SYSTEM_ERROR = 1, //look at errno or GetLastError
    FA_CORRUPTED_ARCHIVE = 2, //invalid archive
    FA_NOT_IMPLEMENTID = 3,
    FA_INVALID_PARAM = 4,

    //codes up to 0x7FFFFFFF reserved, from 0x80000000 to 0xFFFFFFFF are implementation defined (see rule 5)
    
};

enum FA_METADATA
{
    FA_INFO = 0,
    FA_COMMENT = 1,

    //codes up to 0x7FFFFFFF reserved, from 0x80000000 to 0xFFFFFFFF are implementation defined (see rule 5)
};

/**
* Passed to all archive functions to be used as memory pointer, as pointer-size value holder or something else (implementation-specific for plugins)
*/
typedef void* archive_handle;

/**
* Described in Archive_ExtractFiles
*/
typedef void (FLEXARCH_CALL_TYPE *archive_enumerate_callback)(archive_handle archive, archive_entry* entry, uint8_t last_item);

/**
* Described in Archive_RegisterStatusCallback
*/
typedef void (FLEXARCH_CALL_TYPE *status_callback)(archive_handle archive, uint64_t min_item, uint64_t max_item, uint64_t current_item, double current_item_percentage, char* hint);

/**
* \brief Returns user-readable plugin name
* \return User-readable plugin name
*/ 
FLEXARCH_PLUGIN_API_STRING(Plugin_GetName)();

/**
* \brief Returns user-readable plugin description
* \return User-readable plugin description
*/ 
FLEXARCH_PLUGIN_API_STRING(Plugin_Description)();

/**
* \brief TBD
*/
FLEXARCH_PLUGIN_API_STRING(Plugin_GetFilenameFilters)();

/**
* \brief Return error description by user-defined error code (see rule 5)
* \param error_code error code to get description for
* \param reserved do not use, set to 0
* \return error description
*/
FLEXARCH_PLUGIN_API_STRING(Plugin_ErrorCodeDescription)(uint32_t error_code, uint16_t reserved);

/**
* \brief Opens existing archive
* \param archive Pointer-size value, where some data that will be used to idenify this archive in future should be stored
* \param local_path Local file system to archive
* \return error code
*/
FLEXARCH_PLUGIN_API(Archive_Open)(archive_handle *archive, char *local_path);
/**
* \brief Creating new archive
* \param archive Pointer-size value, where some data that will be used to idenify this archive in future should be stored
* \param local_path Local file system path where archive should be placed
* \param flags TBD
* \return error code
*/
FLEXARCH_PLUGIN_API(Archive_Create)(archive_handle *archive, char *local_path, char *flags);
/**
* \brief If anything cached - data should be saved to disk after this call
* \param archive Archive handle, created by Archive_Open or Archive_Create
* \return error code
*/
FLEXARCH_PLUGIN_API(Archive_Save)(archive_handle archive);
/**
* \brief Closing archive. All unsaved changes should be saved and file should be closed, after this call archive handle become invalid.
* \param archive Archive handle, created by Archive_Open or Archive_Create
* \return error code
*/
FLEXARCH_PLUGIN_API(Archive_Close)(archive_handle archive);


/**
* \brief Adding file from local FS to archive
* \param archive Archive handle, created by Archive_Open or Archive_Create
* \param local_path path of local file system file to add
* \param archive_item node in archive to be created. Fields `id` and `size_in_archive` is uninitialized before call, field `id` *should* be initialized after call
* \return error code
*/
FLEXARCH_PLUGIN_API(Archive_AddFileLocal)(archive_handle archive, archive_entry* archive_item, char* local_path);


/**
* \brief Removing file, directory, or directory tree from archive
* \param archive Archive handle, created by Archive_Open or Archive_Create
* \param archive_item path to file or directory. If directory not empty - entire tree should be deleted
* \return error code
*/
FLEXARCH_PLUGIN_API(Archive_RemoveEntry)(archive_handle archive, archive_entry* archive_item);
/**
* \brief
* \param archive Archive handle, created by Archive_Open or Archive_Create
* \param archive_path Fields `id` and `size_in_archive` is uninitialized before call, field `id` *should* be initialized after call
* \return error code
*/
FLEXARCH_PLUGIN_API(Archive_CreateDirectory)(archive_handle archive, archive_entry* archive_path);
/**
* \brief
* \param archive Archive handle, created by Archive_Open or Archive_Create
* \param metadata_index index of metadata, implementation-defined or FA_METADATA
* \param buffer_data metadata buffer
* \param buffer_size metadata buffer size
* \return error code
*/
FLEXARCH_PLUGIN_API(Archive_AddArchiveMetadata)(archive_handle archive, uint32_t metadata_index, uint8_t* buffer_data, uint64_t buffer_size);


/**
* \brief 
* \param archive Archive handle, created by Archive_Open or Archive_Create
* \param callback callback to be used for enumeration. archive should be always same as passed to Archive_EnumerateEntries, entry is current item node (super_name buffer can be freed after callback call) and should be NULL for empty archive, last item *should* be set to non-zero for last call and *shoud* be set to zero for any other call.
* \return error code
*/
FLEXARCH_PLUGIN_API(Archive_EnumerateEntries)(archive_handle archive, archive_enumerate_callback callback);

/**
* \brief
* \param archive Archive handle, created by Archive_Open or Archive_Create
* \param local_dir_path path to local directory where files should be extracted
* \param archive_item array of items to be extracted
* \param archive_item_count count of `archive_item` array
* \return error code
*/
FLEXARCH_PLUGIN_API(Archive_ExtractFiles)(archive_handle archive, char* local_dir_path, archive_entry* archive_item, uint64_t archive_item_count);

/**
* \brief
* \param archive Archive handle, created by Archive_Open or Archive_Create
* \param callback callback function that highly recommended to call while processing large amounts of data
* \return error code
*/
FLEXARCH_PLUGIN_API(Archive_RegisterStatusCallback)(status_callback callback);

/**
* \brief
* \param archive Archive handle, created by Archive_Open or Archive_Create
* \param id node id 
* \param archive_item pointer to archive_entry to be filled with info for node with passed id
* \param callback callback function that highly recommended to call while processing large amounts of data
* \return error code
*/
FLEXARCH_PLUGIN_API(Archive_GetNodeInfo)(archive_handle archive, uint64_t id, archive_entry* archive_item);

//-------------------------------------------------------
/*
* TODO:
* archive input files requirements
* stream-reading nad stream-writing for file buffers?
*/

/**
* \brief
* \param archive Archive handle, created by Archive_Open or Archive_Create
* \param archive_item node in archive
* \return error code
*/
FLEXARCH_PLUGIN_API(Archive_GetFileBuffer)(archive_handle archive, archive_entry *archive_item, int i);

/**
* \brief
* \param archive Archive handle, created by Archive_Open or Archive_Create
* \param archive_item node in archive to be created. Fields `id` and `size_in_archive` is uninitialized before call, field `id` *should* be initialized after call
* \param buffer_data byte array of file to store
* \param buffer_size byte array length
* \return error code
*/
FLEXARCH_PLUGIN_API(Archive_AddFileBuffer)(archive_handle archive, archive_entry* archive_item, uint8_t* buffer_data, uint64_t buffer_size);

#endif
