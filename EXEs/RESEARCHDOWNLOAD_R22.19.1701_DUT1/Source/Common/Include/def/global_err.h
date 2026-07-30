/************************************************************************
*                                                                       *
*   global_err.h -  global error definitions                            *
*                                                                       *
*   COPYRIGHT (C) 2019 UNISOC TECHNOLOGIES INC.                         *
*                                                                       *
************************************************************************/
#if !defined(AFX_GLOBAL_ERR_H__DE321B2F_78A9_42A3_8420_A9680F528D01__INCLUDED_)
#define AFX_GLOBAL_ERR_H__DE321B2F_78A9_42A3_8420_A9680F528D01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//
#include "global_def.h"


//
//  Error code definition
//
typedef uint32  SPRESULT;


//
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---------------+---------------+-------------------------------+
//  |       R       |       M       |                E              |
//  +---------------+---------------+-------------------------------+
//
//  where
//
//
//      R - reserved
//
//      M - Modules
//
//      E - Error codes.
//

//
// Success codes
//
#define SP_OK                       ((SPRESULT)0x00000000L)

/*
//
// Generic test for success on any status value (non-negative numbers indicate success).
//
#define SPSUCC(res)                 ((SPRESULT)(res) >= 0)

//
// and the inverse
//
#define SPFAIL(res)                 ((SPRESULT)(res) < 0 )
*/
//
// Return the code
//
#define getResultCode(res)          ((res) & 0xFFFF)

//
//  Return the module
//
#define getResultModule(res)        (((res) >> 16) & 0xFF)

//
//  Return the severity
//
#define getResultSeverity(res)      (((res) >> 30) & 0x3)

//
// Create an SPRESULT value from component pieces
//
#define makeResultCode(mod,code) \
        ((SPRESULT) ( ((uint32)(mod&0xFF) << 16) | ((uint32)(code&0xFFFF))) )



///$$$
typedef enum
{
    /* Description ENU: FAIL*/
    /* Description CHS: 错误*/
    /* Solution ENU: */
    /* Solution CHS: */
    SP_E_BASE       =      0x0000,
    
    /* Description ENU: FAIL*/
    /* Description CHS: 错误*/
    /* Solution ENU: */
    /* Solution CHS: */
    SP_E_FAIL,
    
    /* Description ENU: Exception happen*/
    /* Description CHS: 异常错误*/
    /* Solution ENU: */
    /* Solution CHS: */
    SP_E_SYSTEM_EXCEPTION,
    
    /* Description ENU: Invalid parameters*/
    /* Description CHS: 无效的参数*/
    /* Solution ENU: */
    /* Solution CHS: */
    SP_E_INVALID_PARAMETER,
    
    /* Description ENU: Buffer is too small*/
    /* Description CHS: 输入缓存不足*/
    /* Solution ENU: */
    /* Solution CHS: */
    SP_E_BUFFER_TOO_SMALL,
    
    /* Description ENU: Invalid pointer*/
    /* Description CHS: 无效的指针*/
    /* Solution ENU: */
    /* Solution CHS: */
    SP_E_POINTER,
    
    /* Description ENU: Invalid handle*/
    /* Description CHS: 无效的句柄*/
    /* Solution ENU: */
    /* Solution CHS: */
    SP_E_HANDLE,
    
    /* Description ENU: Allocate memory fail*/
    /* Description CHS: 内存分配错误*/
    /* Solution ENU: */
    /* Solution CHS: */
    SP_E_ALLOC_MEMORY,
    
    /* Description ENU: Not supported*/
    /* Description CHS: 功能尚未支持*/
    /* Solution ENU: */
    /* Solution CHS: */
    SP_E_NOT_SUPPORTED,
    
    /* Description ENU: User abort*/
    /* Description CHS: 用户停止*/
    /* Solution ENU: */
    /* Solution CHS: */
    SP_E_USER_ABORT,
    
    /* Description ENU: TimeOut*/
    /* Description CHS: 测试超时*/
    /* Solution ENU: */
    /* Solution CHS: */
    SP_E_TIMEOUT,
    
    /* Description ENU: Create object fail*/
    /* Description CHS: 创建对象失败*/
    /* Solution ENU: */
    /* Solution CHS: */
    SP_E_CREATE_OBJECT,
    
    /* Description ENU: DUT is asserted*/
    /* Description CHS: 手机软件崩溃*/
    /* Solution ENU: */
    /* Solution CHS: */
    SP_E_DUT_ASSERT,
    
    /* Description ENU: File not exist*/
    /* Description CHS: 文件不存在*/
    /* Solution ENU: */
    /* Solution CHS: */
    SP_E_FILE_NOT_EXIST,
    
    /* Description ENU: Open file fail*/
    /* Description CHS: 打开文件失败*/
    /* Solution ENU: Please confirm: 1. Whether file exists or not? 2. If file has been opened by another program like EXCEL, please close it at first！*/
    /* Solution CHS: 请检查文件是否存在？ 如果文件被其他程序已打开，比如EXCEL，请先关闭*/
    SP_E_OPEN_FILE,
    
    /* Description ENU: Read/Write file fail*/
    /* Description CHS: 读或写文件失败*/
    /* Solution ENU: */
    /* Solution CHS: */
    SP_E_FILE_IO,
    
    /* Description ENU: Empty file*/
    /* Description CHS: 空文件*/
    /* Solution ENU: Please check the file */
    /* Solution CHS: */
    SP_E_EMPTY_FILE,
    
    /* Description ENU: LoadLibrary fail*/
    /* Description CHS: 加载动态库失败*/
    /* Solution ENU: Make sure the library(.dll) exists. */
    /* Solution CHS: 检查需要加载的动态库是否存在*/
    SP_E_LOAD_LIBRARY,
    
    /* Description ENU: Verify license certificate fail*/
    /* Description CHS: 许可文件验证失败*/
    /* Solution ENU: */
    /* Solution CHS: */
    SP_E_VERIFY_LICENSE,
    
    /* Description ENU: FAIL*/
    /* Description CHS: 错误*/
    /* Solution ENU: */
    /* Solution CHS: */
    SP_E_MAX_BASE_CODE = 0x100
    
} E_Common_E;
///&&&


//
// Module based error
// Example:
// GPIB module error code definition
// typedef enum
// {
//      E_GPIB_BASE_ERROR = MODULE_BASE_ERROR(MODULE_GPIB),
//
//      E_GPIB_OPEN_FAIL,
//      E_GPIB_SEND_FAIL,
//      E_GPIB_MAX_ERROR
//
//  } E_GPIB_ERROR_CODE;

//

#define MODULE_BASE_ERROR(mod)          (makeResultCode((mod), SP_E_MAX_BASE_CODE))


typedef enum
{
    SP_E_AUTH_DUT_FAIL = 0x201,
    SP_E_AUTH_SURVER_FAIL = 0x202,
    SP_E_AUTH_SURVER_SIZE_IS_ZEOR = 0x203,
    SP_E_AUTH_SECURE_KEY_ERROR = 0x204,
    SP_E_AUTH_PRIVITE_KEY_SIZE_ERROR = 0x205,
    SP_E_AUTH_AES_KEY_SIZE_ERROR = 0x206,
} E_Authentication_E;

/// Check the error code, if failure happen, return the error directly
/// Example:
///     CHKRESULT(SendCmd("*RST"));

#define CHKRESULT(statement)    {                                       \
                                    SPRESULT __sprslt = (statement);    \
                                    if (SP_OK != __sprslt)              \
                                    {                                   \
                                        return __sprslt;                \
                                    }                                   \
                                }                                       \

#define CHKRESULTBRK(statement)    {                                       \
                                    SPRESULT __sprslt = (statement);    \
                                    if (SP_OK != __sprslt)              \
                                    {                                   \
                                        break;                \
                                    }                                   \
                                }                                       \

#define CHKBOOL(statement)     {                                        \
                                    BOOL __sprslt = (statement);        \
                                    if (!__sprslt)                      \
                                    {                                   \
                                        return __sprslt;                \
                                    }                                   \
                               }

#endif /* AFX_GLOBAL_ERR_H__DE321B2F_78A9_42A3_8420_A9680F528D01__INCLUDED_ */
