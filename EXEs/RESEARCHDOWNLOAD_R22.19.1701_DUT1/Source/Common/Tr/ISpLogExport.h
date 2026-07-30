#if !defined(_ISPLOG_H_292D2AFD_F140_4758_8D67_05F4E2A8CB82_INCLUDED_)
#define _ISPLOG_H_292D2AFD_F140_4758_8D67_05F4E2A8CB82_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////////
/// Include files 
#include "..\Include\def\global_def.h"

//////////////////////////////////////////////////////////////////////////
/// Macro definition 

// should be times of 4 to avoid alignment problem 
#define MAX_MODULE_NAME_LEN   	( 12 )  

/*
/// Define the maximum length of LogFmtStr formatted string. 
/// The exceeded string will be truncated. 
#define MAX_FORMAT_STRING_LEN   (8192)  
*/

/// Log Level 
enum SPLOG_LEVEL
{
    SPLOGLV_NONE      = 0, /// No Logs      
    SPLOGLV_ERROR     = 1, /// Error      
    SPLOGLV_WARN      = 2, /// Warning       
    SPLOGLV_INFO      = 3, /// Information
    SPLOGLV_DATA      = 4, /// Exclusive, only output data
    SPLOGLV_VERBOSE   = 5, /// Everything
    MAX_LOG_LEVEL
};

enum LogExtraFlags
{
    NoExtraFlag    = 0,   /// 
    LOG_READ       = 1,   /// <--
    LOG_WRITE      = 2,   /// -->
    LOG_ASYNC_READ = 3,   /// <<-
    MaxExtraFlag
};

//
enum LogOptions
{
    LogToLocalFile      = 0x01000000,
    LogToServer         = 0x02000000,
    DisableLaunchServer = 0x10000000,
    LogModuleToFile     = 0x00000001,
    LogThreadToFile     = 0x00000002,  
    LogLevelToFile      = 0x00000004,
    LogTimeToFile       = 0x00000008
};  

enum LogProperties
{
    /// Options is combine listed 'LogOptions' by using the bitwise-OR (|) operator
    /// Example: value is LogToLocalFile|LogToServer|LogTimeToFile
    LogProp_Options     = 1, 

    /// Log filter level
    LogProp_Level,

    /// Number of backtrack traces 
    LogProp_BackTrack,

    // Specifies the cache size of local log file
    // If value is 0, no cache will be used that is, log will be flushed to file directly.
    LogProp_LocalFileCacheSize,

    /// Check whether log is opened or not
    LogProp_OpenState,

    /// Query the log file path, Only support query, setup is not supported.
    /// Example:
    ///     WCHAR szFilePath[MAX_PATH] = {0};
    ///     GetProperty(LogProp_LogFilePath, MAX_PATH, (LPVOID)szFilePath);
    LogProp_LogFilePath, 

    // iLogServer IP and port configuration
    LogProp_ServerIP    = 0x0010,
    LogProp_ServerPort,

    /// Maximum lines of buffer, if exceed, ... will be replaced as shown as follow:
    LogProp_MaxBufLines,   // default maximum line is 64K/16 = 4095 lines
    
    LogProp_PreciseTime, 

    MaxLogProperties
};

//////////////////////////////////////////////////////////////////////////
/// Callback definition
///
/// There are two kinds of trace
/// 1. String (nType == 0):
///
///          TIME                MODULE          LEVEL             STRING
/// [2016-06-07 10:55:57:464][TASK1       ][3 - Information] Creating PhoneCommand ...
///
/// lpszModue    : TASK1
/// lpszTime     : 2016-06-07 10:55:57:464
/// nLv          : SPLOG_LEVEL
/// nType        : 0, string
/// nFlag        : No used for string 
/// lpDataString : Creating PhoneCommand ...
/// lpConvString : [2016-06-07 10:55:57:464][TASK1       ][3 - Information] Creating PhoneCommand ...
///
/// 2. Buffer (nType == 1):
///
///          TIME                MODULE          LEVEL             STRING
/// [2016-05-29 23:00:43:752][DUT         ][4 - Data       ] <<- 171(0x000000ab) Bytes
///                                                          00000000h: 00 00 00 00 AB 00 00 00 0A 50 6C 61 74 66 6F 72 : .........Platfor
///                                                          00000010h: 6D 20 56 65 72 73 69 6F 6E 3A 20 4D 4F 43 4F 52 : m Version: MOCOR
///                                                          00000020h: 54 4D 5F 57 31 35 2E 37 2E 32 5F 44 65 62 75 67 : TM_W15.7.2_Debug
///  ... ... 
///                                                          000000a0h: 31 35 20 32 31 3A 33 39 3A 31 30                : 15 21:39:10   
///
/// lpszModue    : DUT
/// lpszTime     : 2016-05-29 23:00:43:752
/// nLv          : SPLOG_LEVEL
/// nType        : 1, buffer
/// nFlag        : 0: --> 1: <-- 2: <<-
/// lpDataString : <<- 171(0x000000ab) Bytes
///                00000000h: 00 00 00 00 AB 00 00 00 0A 50 6C 61 74 66 6F 72 : .........Platfor
///                00000010h: 6D 20 56 65 72 73 69 6F 6E 3A 20 4D 4F 43 4F 52 : m Version: MOCOR
///                00000020h: 54 4D 5F 57 31 35 2E 37 2E 32 5F 44 65 62 75 67 : TM_W15.7.2_Debug
///  ... ... 
///                000000a0h: 31 35 20 32 31 3A 33 39 3A 31 30                : 15 21:39:10   
/// lpConvString : [2016-05-29 23:00:43:752][DUT         ][4 - Data       ] <<- 171(0x000000ab) Bytes
///                                                                         00000000h: 00 00 00 00 AB 00 00 00 0A 50 6C 61 74 66 6F 72 : .........Platfor
///                                                                         00000010h: 6D 20 56 65 72 73 69 6F 6E 3A 20 4D 4F 43 4F 52 : m Version: MOCOR
///                                                                         00000020h: 54 4D 5F 57 31 35 2E 37 2E 32 5F 44 65 62 75 67 : TM_W15.7.2_Debug
///  ... ... 
///                                                                         000000a0h: 31 35 20 32 31 3A 33 39 3A 31 30                : 15 21:39:10   
typedef struct 
{
    LPCSTR   lpszModule;    
    LPCSTR     lpszTime;
    UINT32          nLv;
    UINT32        nType;
    UINT32        nFlag;
    LPCSTR lpDataString;  /// String without prefix such as "[Time][Module][Level][...]"
    LPCSTR lpConvString;  /// Full string including prefix.

} SPLOG_CALLBACK_DATA, *LPSPLOG_CALLBACK_DATA;
typedef const SPLOG_CALLBACK_DATA* LPCSPLOG_CALLBACK_DATA;

typedef void (CALLBACK* LPSPLOGCALLBACK)(LPCSPLOG_CALLBACK_DATA lpData, LPVOID pParam, UINT32 lParam);

///
typedef struct _tagSPLOGCALLBACK_PARAM
{
    LPSPLOGCALLBACK lpFn;
    LPVOID pParam;
    UINT32 lParam;
    
    _tagSPLOGCALLBACK_PARAM(void)
    {
        lpFn   = NULL;
        pParam = NULL;
        lParam = 0;
    };

} SPLOGCALLBACK_PARAM, *LPSPLOGCALLBACK_PARAM;
typedef const SPLOGCALLBACK_PARAM* LPCSPLOGCALLBACK_PARAM;

namespace splog
{
    /// For example: the local file name is "GPIB.Log"
    enum LocalLogOpenFlags 
    {        
        /// File path will be "AppPath:\Log\"GPIB_2013_12_16_12_39_50.Log"
        /// If the path of specified local file is absolute, this flag will be ignored. 
        modeTimeSuffix  =      0x0001,  

        /// File path will be "AppPath:\Log\"GPIB_2013_12_16.Log"
        /// If the path of specified local file is absolute, this flag will be ignored. 
        modeDateSuffix  =      0x0002,  

        /// Directs the constructor to create a new file. 
        /// If the file exists already, it is truncated to 0 length. 
        modeCreate      =      0x1000,  

        /// Combine this value with modeCreate. 
        /// If the file being created already exists, it is not truncated to 0 length.
        modeNoTruncate  =      0x2000,  
        
        /// Sets text mode.   
        /// Default file extension is *.log if no extension is specified.    
        typeText        =      0x4000,  

        /// 
        defaultFlags    =      typeText|modeCreate|modeDateSuffix,
        addDateFlags    =      typeText|modeCreate|modeDateSuffix|modeNoTruncate,
        addTimeFlags    =      typeText|modeCreate|modeTimeSuffix|modeNoTruncate
    };

}; /* end namespace splog*/

//////////////////////////////////////////////////////////////////////////
/// 
typedef struct _tagOpenArgs_T
{
    UINT32 nSize;                             /// Structure size
    CHAR   szModule[MAX_MODULE_NAME_LEN];     /// Module name

    /// Log level filter, ignore the logs which level is higher than nLogLevel.
    /// Example: nFileLevel = SPLOGLV_WARN;
    /// Traces which level is higher than SPLOGLV_WARN will not be logged.
    UINT32 nLogLevel;   

    ///
    /// Enable backtrack or not, ONLY available while nLogLevel = SPLOGLV_ERROR
    /// While SPLOGLV_ERROR trace, iSpLog will backtrack previous 20 traces to help debug.
    /// 
    BOOL   bBackTrack;

    struct  
    {
        /// Specifies log to local file or not
        BOOL   bLogToFile;  

        /// Local file path, support relative and absolute path
        /// Example:  L"GPIB.Log"  or "D:\GPIB.Log"
        /// If path is relative, actual file path will be "AppPath:\Log\GPIB.log"
        /// If path is absolute, modeTimeSuffix & modeDateSuffix of 'LocalLogOpenFlags' will be ignored.
        WCHAR  szLogFile[MAX_PATH];          
        
        /// Open flags, which is combine listed 'LocalLogOpenFlags' above by using the bitwise-OR (|) operator 
        /// Example: splog::typeText|splog::modeCreate|splog::modeTimeSuffix
        UINT32 nOpenFlags;        

        /// Specifies the maximum size in MBytes of local file. Value range is [1-4095] MBytes
        /// If file is too bigger than dwMaxLogFileSizeInMB, it will be partitioned into several part files.
        /// "_X" will be added to the original file name like "GPIB_1.Log" to name the new partitioned file.
        /// If value is 0 or INFINITE or bigger than 4095, the file size is no limited.
        UINT32 nMaxLogFileSizeInMB; 

    } Local;

    _tagOpenArgs_T(void)
    {
        ZeroMemory((void* )this, sizeof(*this));
        nSize = (UINT)sizeof(_tagOpenArgs_T);
        nLogLevel  = SPLOGLV_ERROR;
        bBackTrack = TRUE;
        Local.bLogToFile = TRUE;
        Local.nOpenFlags = splog::typeText|splog::modeCreate|splog::modeTimeSuffix;
        Local.nMaxLogFileSizeInMB = INFINITE;
    };

} OpenArgs_T, *LPOpenArgs_T;

///
#if defined (UNICODE) ||  defined (_UNICODE)  
    #define LogRawStr       LogRawStrW
    #define LogFmtStr       LogFmtStrW
#else
    #define LogRawStr       LogRawStrA
    #define LogFmtStr       LogFmtStrA
#endif /// !UNICODE || !_UNICODE

//////////////////////////////////////////////////////////////////////////
/// Export APIs
#ifdef  ISPLOG_EXPORT
#undef  ISPLOG_EXPORT
#endif
#ifdef __cplusplus
    #ifdef __ISPLOG
        #define ISPLOG_EXPORT extern "C" __declspec (dllexport)
    #else
        #define ISPLOG_EXPORT extern "C" __declspec (dllimport)
    #endif
#else
    #ifdef __ISPLOG
        #define ISPLOG_EXPORT __declspec (dllexport)
    #else
        #define ISPLOG_EXPORT __declspec (dllimport)
    #endif
#endif

#ifdef __cplusplus

//////////////////////////////////////////////////////////////////////////
/// 
#if _MSC_VER > 1000
class ISpLog  
{
public:
    virtual ~ISpLog(void)   {   } ;

    /// ---------------------------------------------------------------------------
    /// Trace Observer
    ///    
    virtual void    SetCallBack(const SPLOGCALLBACK_PARAM& cb) = 0;

    /// ---------------------------------------------------------------------------
    /// Open local file or connect to iLogServer if needed
    /// @param lpArg - [IN] Open parameters specified by 'OpenArgs_T'
    /// @return TRUE = success; FALSE = fail
    virtual BOOL    Open(LPCVOID lpArg) = 0;

    /// ---------------------------------------------------------------------------
    /// Close 
    /// @param nOptions - [IN] Specifies option to close
    ///   1: Close immediately. The logs which haven't been dealt with will be lost.
    ///   0: Close until all of the logs have been dealt with.
    ///      This option may cause some delay for dealing with internal cached logs.
    /// @return TRUE = success; FALSE = fail
    virtual BOOL    Close(UINT32 nOptions = 0) = 0;

    /// ---------------------------------------------------------------------------
    /// Delete object 
    virtual void    Release(void) = 0;

    /// ---------------------------------------------------------------------------
    /// Log string 
    /// Example:  "[hDiagPhone = 0x9400048] Port = 26, Set Remove Event!"
    /// @param nLogLevel  - [IN] Log level 
    /// @param lpszString - [IN] String
    /// @return TRUE = success; FALSE = fail
    virtual BOOL    LogRawStrA(UINT32 nLogLevel, LPCSTR  lpszString, LPCSTR lpszModule = NULL) = 0;
    virtual BOOL    LogRawStrW(UINT32 nLogLevel, LPCWSTR lpszString, LPCSTR lpszModule = NULL) = 0;

    /// ---------------------------------------------------------------------------
    /// Log formatted string
    /// @param nLogLevel  - [IN] Log level 
    /// @param lpszFmt    - [IN] Format control
    /// @param argument   - [IN] Optional arguments
    /// @return TRUE = success; FALSE = fail
    virtual BOOL    LogFmtStrA(UINT32 nLogLevel, LPCSTR  lpszFmt, ...) = 0;
    virtual BOOL    LogFmtStrW(UINT32 nLogLevel, LPCWSTR lpszFmt, ...) = 0;

    virtual BOOL    LogFmtStrA(LPCSTR lpszModule, UINT32 nLogLevel, LPCSTR  lpszFmt, ...) = 0;
    virtual BOOL    LogFmtStrW(LPCSTR lpszModule, UINT32 nLogLevel, LPCWSTR lpszFmt, ...) = 0;
    /// ---------------------------------------------------------------------------
    /// Log buffer
    /// Example:  --> 114756(0x0001c044) Bytes
    ///           00000000h: 4D 5A 90 00 03 00 00 00 04 00 00 00 FF FF 00 00 ; MZ..............
    ///           00000010h: B8 00 00 00 00 00 00 00 40 00 00 00 00 00 00 00 ; ........@.......
    ///           00000020h: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ; ................
    /// @param nLogLevel      - [IN] Log level 
    /// @param lpData         - [IN] Data buffer
    /// @param dwDataSize     - [IN] Data size in bytes
    /// @param uFlag          - [IN] ExtralFlags 
    /// @param lpdwExpSize    - [IN] this size is that user want to Read/Write buffer size
    ///                              use this parameter user can see the different between real Read/Write size (dwBufSize) 
    ///                              and expired size (*pUserNeedSize)
    ///                              If this pointer is not null,log will like bellow:
    ///                              --> 114756(0x0001c044)/114757(0x0001c045)  Bytes
    /// @return TRUE = success; FALSE = fail
    virtual BOOL    LogBufData(
        UINT32  nLogLevel, 
        const void* lpData, 
        UINT32  nDataSize, 
        UINT32 nFlag = LOG_WRITE,
        const UINT32* lpnExpSize = NULL,
        LPCSTR lpszModule = NULL
        ) = 0;

    /// ---------------------------------------------------------------------------
    /// @param lAttr     - [IN    ] Properties which is specified by 'LogProperties'
    /// @param lFlags    - [IN    ] Extra flag
    /// @param lpValue   - [IN/OUT] Value 
    /// @return TRUE = success; FALSE = fail
    virtual BOOL    SetProperty(UINT32 nAttr, UINT32 nFlags, LPCVOID lpValue) = 0;
    virtual BOOL    GetProperty(UINT32 nAttr, UINT32 nFlags, LPVOID  lpValue) = 0; 
};

//////////////////////////////////////////////////////////////////////////
/// C++ Interface
ISPLOG_EXPORT BOOL SP_API CreateISpLogObject(ISpLog **lppObject);

//////////////////////////////////////////////////////////////////////////

#include <TCHAR.h>
#include <vector>

class CFnLog sealed
{
public:
    CFnLog(ISpLog *lpTr, LPCTSTR lpszFnName, LPCTSTR lpszFmt = NULL, ...)
        : m_lpTr(lpTr)
        , m_sFnName(lpszFnName)
    {
        if (NULL != m_lpTr)
        {
            if (NULL == lpszFmt)
            {
                m_lpTr->LogFmtStr(SPLOGLV_VERBOSE, _T("Enter [%s]"), m_sFnName.c_str());
            }
            else
            {
                va_list  args = NULL;  
                va_start(args, lpszFmt);                          
                size_t nLength = _vsctprintf(lpszFmt, args) + 1;   
                std::vector<TCHAR> vBuff(nLength, _T('\0'));       
                if (_vsntprintf_s(&vBuff[0], vBuff.size(), nLength, lpszFmt, args) > 0) 
                {   
                    m_lpTr->LogFmtStr(SPLOGLV_VERBOSE, _T("Enter [%s] %s"), m_sFnName.c_str(), (LPCTSTR)&vBuff[0]);
                }
            }
        }
    }
   
    ~CFnLog(void)
    {
        if (NULL != m_lpTr)
        {
            m_lpTr->LogFmtStr(SPLOGLV_VERBOSE, _T("Leave [%s]"), m_sFnName.c_str());  
            m_lpTr = NULL;
        }
    }

private:
    ISpLog*     m_lpTr;
    string_t    m_sFnName;
};
#endif
#endif 


/// C/C#
ISPLOG_EXPORT SP_HANDLE SP_API SpLog_CreateObject(void);
ISPLOG_EXPORT void      SP_API SpLog_FreeObject(SP_HANDLE hSpLog);

ISPLOG_EXPORT void      SP_API SpLog_SetCallBack(SP_HANDLE hSpLog, LPCSPLOGCALLBACK_PARAM lpCallback);

ISPLOG_EXPORT BOOL      SP_API SpLog_Open (SP_HANDLE hSpLog, LPCVOID lpArg);
ISPLOG_EXPORT BOOL      SP_API SpLog_Close(SP_HANDLE hSpLog, UINT32 nOption);

ISPLOG_EXPORT BOOL      SP_API SpLog_LogRawStrA(SP_HANDLE hSpLog, UINT32 nLogLevel, LPCSTR  lpszString);
ISPLOG_EXPORT BOOL      SP_API SpLog_LogRawStrW(SP_HANDLE hSpLog, UINT32 nLogLevel, LPCWSTR lpszString);

ISPLOG_EXPORT BOOL     __cdecl SpLog_LogFmtStrA(SP_HANDLE hSpLog, UINT32 nLogLevel, LPCSTR  lpszFmt, ...);
ISPLOG_EXPORT BOOL     __cdecl SpLog_LogFmtStrW(SP_HANDLE hSpLog, UINT32 nLogLevel, LPCWSTR lpszFmt, ...);

ISPLOG_EXPORT BOOL      SP_API SpLog_LogBufData(SP_HANDLE hSpLog, 
                                                UINT32 nLogLevel, 
                                                const void* lpData, 
                                                UINT32 nDataSize, 
                                                UINT32 nFlag,
                                                const UINT32* lpnExpSize
                                                );

ISPLOG_EXPORT BOOL      SP_API SpLog_SetProperty(SP_HANDLE hSpLog, UINT32 nAttr, UINT32 nFlags, LPCVOID lpValue);
ISPLOG_EXPORT BOOL      SP_API SpLog_GetProperty(SP_HANDLE hSpLog, UINT32 nAttr, UINT32 nFlags, LPVOID  lpValue);


#endif /* _ISPLOG_H_292D2AFD_F140_4758_8D67_05F4E2A8CB82_INCLUDED_ */
