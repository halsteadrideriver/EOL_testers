#pragma once

#include "def\global_err.h"
#include "def\global_def.h"
#include "def\callback_def.h"

#define MAX_FLASH_REP_ID_LEN          16
#define MAX_FLASH_LIST_ID_LEN         720  //Up to 20 partitions can be supported,the size of each partition is 72 bytes
#define MAX_FLASH_LEN                 256

enum FLASH_OPERATION_TYPE_ENUM
{
    FLASH_OP_WRITE_FDL2,
    FLASH_OP_WRITE_DLEND
};

typedef struct _tagFLASH_READ_FILE
{
    DWORD dwFileSize;                           // File size
    DWORD dwFileOffset;                         // File offset
    DWORD dwToatlSize;                          // File Toatl size
    WCHAR szFileId[MAX_FLASH_REP_ID_LEN];       // File ID
    STRUCT_INITIALIZE( _tagFLASH_READ_FILE );
} FLASH_READ_FILE, * LPFLASH_READ_FILE;
typedef const FLASH_READ_FILE* LPCFLASH_READ_FILE;

typedef struct _tagFLASH_WRITE_FILE
{
    DWORD dwFileSize;                           // File size
    DWORD dwFileOffset;                         // File offset
    WCHAR szFileId[MAX_FLASH_REP_ID_LEN];       // File ID
    UINT8 u8FileValue[MAX_FLASH_LEN];           // File value
    STRUCT_INITIALIZE( _tagFLASH_WRITE_FILE );
} FLASH_WRITE_FILE, * LPFLASH_WRITE_FILE;
typedef const FLASH_WRITE_FILE* LPCFLASH_WRITE_FILE;

typedef struct _tagFLASH_WRITE_FILE_VALUE
{
    PDWORD pFileSize;                           // File size
    PDWORD pFileOffset;                         // File offset
    LPCWCH pFileId;                             // File ID
    PUINT8 pFileValue;
    DWORD dwFileValueSize;
    STRUCT_INITIALIZE( _tagFLASH_WRITE_FILE_VALUE );
} FLASH_WRITE_FILE_VALUE, * LPFLASH_WRITE_FILE_VALUE;
typedef const FLASH_WRITE_FILE_VALUE* LPCFLASH_WRITE_FILE_VALUE;

typedef struct _tagFLASH_ERASE_FILE
{
    WCHAR szFileId[MAX_FLASH_LIST_ID_LEN];      // File ID
    STRUCT_INITIALIZE( _tagFLASH_ERASE_FILE );
} FLASH_ERASE_FILE, * LPFLASH_ERASE_FILE;
typedef const FLASH_ERASE_FILE* LPCFLASH_ERASE_FILE;

typedef struct _tagFLASH_OPERATION_SWITCH
{
    uint8 u8ReadFlashAfterFDL2;
    uint8 u8EraseFlashAfterFDL2;
    uint8 u8WriteFlashAfterFDL2;
    uint8 u8WriteFlashAfterDlEnd;
    uint8 u8Reserved0;
    uint8 u8Reserved1;
    
    STRUCT_INITIALIZE( _tagFLASH_OPERATION_SWITCH );
} FLASH_OPERATION_SWITCH, * LPFLASH_OPERATION_SWITCH;
typedef const FLASH_OPERATION_SWITCH* LPCFLASH_OPERATION_SWITCH;

class IFlashOperation
{
public:
    virtual void Release( void ) = 0;
    virtual void GetErrorMsg( LPTSTR lpErrMsg, DWORD dwCount ) = 0;
    virtual SPRESULT SetProperty( INT nProperty, INT nFlags, LPCVOID lpValue ) = 0;
    virtual SPRESULT GetProperty( INT nProperty, INT nFlags, LPVOID lpValue ) = 0;
    
    virtual SPRESULT EnableOperation( DL_TYPE_ENUM eType, LPFLASH_OPERATION_SWITCH lpOperation ) = 0;
    virtual SPRESULT CheckReadBuffer( LPCSTR lpReadBuffer, LPFLASH_READ_FILE lpReadFile ) = 0;
    virtual SPRESULT PrepareReadFlashAfterFDL2( LPFLASH_READ_FILE lpReadFile, INT& nCount ) = 0;
    virtual SPRESULT PrepareEraseFlashAfterFDL2( LPFLASH_ERASE_FILE lpEraseFile, INT& nCount ) = 0;
    virtual SPRESULT PrepareWriteFlashAfterFDL2( LPFLASH_WRITE_FILE lpWriteFile, INT& nCount ) = 0;
    virtual SPRESULT PrepareWriteFlashAfterDlEnd( LPFLASH_WRITE_FILE lpWriteFile, INT& nCount ) = 0;
};

SP_EXPORT IFlashOperation* SP_API CreateFlashOperationObject( LPVOID pLogUtil );

