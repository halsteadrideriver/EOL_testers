
#if !defined(AFX_CALLBACK_DEF_H__3C71714E_9156_4801_B01D_E4079A65C17A__INCLUDED_)
#define AFX_CALLBACK_DEF_H__3C71714E_9156_4801_B01D_E4079A65C17A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "global_err.h"


/// Callback data type
typedef enum : uint16
{
    CALLBACK_LOAD_PACKET_PROGRESS = 0,   //That corresponds to the previous message: BM_DOWNLOAD_PROCESS
    CALLBACK_PACKET_INFO,                //That corresponds to the previous message:
    CALLBACK_DL_STEP_DESCRIPTION,        //That corresponds to the previous message: AddStepDescription()
    CALLBACK_DL_IMAGE_BEGIN,             //That corresponds to the previous message: BM_FILE_BEGIN
    CALLBACK_DL_END,                     //That corresponds to the previous message: BM_END
    
    CALLBACK_STRING_INFO,                //Reference STRING_INFO_TYPE_E
    CALLBACK_STEP_INFO,                  //Reference STEP_INFO_TYPE_E
    CALLBACK_PROGRESS_INFO,              //Reference PROGRESS_INFO_TYPE_E
    
    MAX_CALLBACK_DATA
} CALLBACKDATA_TYPE;

typedef struct
{
    CALLBACKDATA_TYPE eType;
    /*
        eType                     |            lpData
    CALLBACK_LOAD_PACKET_PROGRESS    Pointer to structure "CALLBACKDATA_LOAD_PACKET_PROGRESS"
    CALLBACK_PACKET_INFO             Pointer to structure "CALLBACKDATA_PACKET_INFO"
    CALLBACK_DL_STEP_DESCRIPTION     Pointer to structure "CALLBACKDATA_DL_STEP_DESCRIPTION"
    CALLBACK_DL_IMAGE_BEGIN          Pointer to structure "CALLBACKDATA_DL_IMAGE_SIZE"
    CALLBACK_DL_END                  Pointer to structure "CALLBACKDATA_DL_RESULT"
    
    CALLBACK_STRING_INFO             Pointer to structure "CALLBACKDATA_STRING_INFO"
    CALLBACK_STEP_INFO               Pointer to structure "CALLBACKDATA_STEP_INFO"
    CALLBACK_PROGRESS_INFO           Pointer to structure "CALLBACKDATA_PROGRESS_INFO"
    
    */
    LPCVOID lpData;
    
} SPCALLBACK_DATA, *LPSPCALLBACK_DATA;
typedef const SPCALLBACK_DATA* LPCSPCALLBACK_DATA;

/// Action callback parameter definition
typedef BOOL ( CALLBACK* LPSPDATACALLBACK )( LPCSPCALLBACK_DATA lpcbData, LPCVOID pParam );
typedef struct _tagSPCALLBACK_PARAM
{
    LPSPDATACALLBACK pFunc;
    LPCVOID pParam;
    
    STRUCT_INITIALIZE( _tagSPCALLBACK_PARAM );
} SPCALLBACK_PARAM, *LPSPCALLBACK_PARAM;
typedef const SPCALLBACK_PARAM* LPCSPCALLBACK_PARAM;


typedef enum
{
    CBK_PROG_BEGIN = 0,
    CBK_PROG_PROCEED = 1,
    CBK_PROG_END = 2,
} LOAD_PACKET_PROGRESS_E;

/*
SPCALLBACK_DATA:: eType = CALLBACK_LOAD_PACKET_PROGRESS;
SPCALLBACK_DATA:: lpData = CALLBACKDATA_LOAD_PACKET_PROGRESS;
*/
typedef struct
{
    LOAD_PACKET_PROGRESS_E    eType;
    UINT32                    u32Progress;
    
} CALLBACKDATA_LOAD_PACKET_PROGRESS, * LPCALLBACKDATA_LOAD_PACKET_PROGRESS;
typedef const CALLBACKDATA_LOAD_PACKET_PROGRESS* LPCCALLBACKDATA_LOAD_PACKET_PROGRESS;

/*
SPCALLBACK_DATA:: eType = CALLBACK_PACKET_INFO;
SPCALLBACK_DATA::lpData = CALLBACKDATA_PACKET_INFO;
*/
typedef struct
{
    LPCWSTR lpName;
    LPCWSTR lpVersion;
    LPCWSTR lpLoadTime;
    uint64* pu64Size;
    
} CALLBACKDATA_PACKET_INFO, * LPCALLBACKDATA_PACKET_INFO;
typedef const CALLBACKDATA_PACKET_INFO* LPCCALLBACKDATA_PACKET_INFO;

/*
SPCALLBACK_DATA:: eType = CALLBACK_DL_STEP_DESCRIPTION;
SPCALLBACK_DATA:: lpData = CALLBACKDATA_DL_STEP_DESCRIPTION;
*/
typedef struct
{
    LPCWSTR lpFileID;
    LPCWSTR lpFileType;
} CALLBACKDATA_DL_STEP_DESCRIPTION, *LPCALLBACKDATA_DL_STEP_DESCRIPTION;
typedef const CALLBACKDATA_DL_STEP_DESCRIPTION* LPCCALLBACKDATA_DL_STEP_DESCRIPTION;

/*
SPCALLBACK_DATA:: eType = CALLBACK_DL_IMAGE_BEGIN;
SPCALLBACK_DATA:: lpData = CALLBACKDATA_DL_IMAGE_SIZE;
*/
typedef struct
{
    uint32  u32Port;
    uint64* pu64Size;
} CALLBACKDATA_DL_IMAGE_SIZE, * LPCALLBACKDATA_DL_IMAGE_SIZE;
typedef const CALLBACKDATA_DL_IMAGE_SIZE* LPCCALLBACKDATA_DL_IMAGE_SIZE;

/*
SPCALLBACK_DATA:: eType = CALLBACK_DL_END;
SPCALLBACK_DATA:: lpData = CALLBACKDATA_DL_RESULT;
*/
typedef struct
{
    uint32  u32Port;
    uint32  u32Stage;
    uint32  u32ErrCode;
    LPCWSTR lpErrMsg;
} CALLBACKDATA_DL_END, * LPCALLBACKDATA_DL_END;
typedef const CALLBACKDATA_DL_END* LPCCALLBACKDATA_DL_END;

typedef enum : uint32
{
    CBK_STRING_POWER_MANAGE = 0,    // That corresponds to the previous message : WM_POWER_MANAGE
    CBK_STRING_WARN_MSG,            // That corresponds to the previous message : WM_WARN_MESSAGEBOX
    CBK_STRING_ERROR_MSG,           // That corresponds to the previous message :
    CBK_STRING_IMEI,                // That corresponds to the previous message : SetIMEI
    CBK_STRING_MCP,                 // That corresponds to the previous message : SetMcpType
    CBK_STRING_SN1,                 // That corresponds to the previous message :
    CBK_STRING_SN2,                 // That corresponds to the previous message :
    CBK_STRING_USB_INSERT,
    CBK_STRING_USB_REMOVE,
    CBK_STRING_CHIPID,
    CBK_STRING_WARN_ERASE = 0x1000,
    
} STRING_INFO_TYPE_E;

/*
SPCALLBACK_DATA:: eType = CALLBACK_STRING_INFO;
SPCALLBACK_DATA:: lpData = CALLBACKDATA_STRING_INFO;
*/
typedef struct
{
    uint32  u32Port;
    STRING_INFO_TYPE_E  eType;
    LPCWSTR lpInfo;
} CALLBACKDATA_STRING_INFO, *LPCALLBACKDATA_STRING_INFO;
typedef const CALLBACKDATA_STRING_INFO* LPCCALLBACKDATA_STRING_INFO;

typedef enum : uint16
{
    CBK_STEP_CHECK_BAUDRATE = 0,  //That corresponds to the previous message: BM_CHANGE_BAUD
    CBK_STEP_CONNECT,             //That corresponds to the previous message: BM_CONNECT
    CBK_STEP_ERASE_FLASH,         //That corresponds to the previous message: BM_ERASE_FLASH
    CBK_STEP_READ_FLASH,          //That corresponds to the previous message: BM_READ_FLASH
    CBK_STEP_RESET,               //That corresponds to the previous message: BM_RESET
    CBK_STEP_SET_FIRST_MODE,      //That corresponds to the previous message: BM_SET_FIRST_MODE
    CBK_STEP_READ_CHIPTYPE,       //That corresponds to the previous message: BM_SET_FIRST_MODE
    CBK_STEP_READ_NVITEM,         //That corresponds to the previous message: BM_READ_NVITEM
    CBK_STEP_UN_TRANSCODE,        //That corresponds to the previous message: BM_UN_TRANSCODE
    CBK_STEP_REQUIRE_SN,          //That corresponds to the previous message: WM_REQUIRE_SN
    CBK_STEP_DL_BEGIN,            //That corresponds to the previous message: BM_BEGIN
    CBK_STEP_DL_IMAGE,            //That corresponds to the previous message: BM_DOWNLOAD
    CBK_STEP_MUTI_SOFTWARE,       //That corresponds to the previous message:
    CBK_STEP_PRELOAD,             //That corresponds to the previous message:  
    CBK_STEP_STOP_AUTODLOADER,   //That corresponds to the previous message:
    CBK_STEP_DL_IMAGE_END         //That corresponds to the previous message:
    
} STEP_INFO_TYPE_E;

/*
SPCALLBACK_DATA:: eType = CALLBACK_STEP_INFO;
SPCALLBACK_DATA:: lpData = CALLBACKDATA_STEP_INFO;
*/
typedef struct
{
    uint32  u32Port;
    STEP_INFO_TYPE_E  eType;
} CALLBACKDATA_STEP_INFO, * LPCALLBACKDATA_STEP_INFO;
typedef const CALLBACKDATA_STEP_INFO* LPCCALLBACKDATA_STEP_INFO;

typedef enum : uint16
{
    CBK_PROGRESS_DL_IMAGE = 0,    // That corresponds to the previous message : BM_DOWNLOAD_PROCESS
    CBK_PROGRESS_READ_FLASH,      // That corresponds to the previous message : BM_READ_FLASH_PROCESS
    
} PROGRESS_INFO_TYPE_E;

/*
SPCALLBACK_DATA:: eType = CALLBACK_PROGRESS_INFO;
SPCALLBACK_DATA:: lpData = CALLBACKDATA_PROGRESS_INFO;
*/
typedef struct
{
    uint32  u32Port;
    PROGRESS_INFO_TYPE_E  eType;
    uint32  u32Progress;
} CALLBACKDATA_PROGRESS_INFO, * LPCALLBACKDATA_PROGRESS_INFO;
typedef const CALLBACKDATA_PROGRESS_INFO* LPCCALLBACKDATA_PROGRESS_INFO;

#endif /* AFX_CALLBACK_DEF_H__3C71714E_9156_4801_B01D_E4079A65C17A__INCLUDED_ */