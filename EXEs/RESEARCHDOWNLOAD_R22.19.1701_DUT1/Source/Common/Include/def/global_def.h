/************************************************************************
*                                                                       *
*   global_def.h -  global definitions                                  *
*                                                                       *
*   COPYRIGHT (C) 2019 UNISOC TECHNOLOGIES INC.                         *
*                                                                       *
************************************************************************/
#if !defined(AFX_GLOBAL_DEF_H__3C71714E_9156_4801_B01D_E4079A65C17A__INCLUDED_)
#define AFX_GLOBAL_DEF_H__3C71714E_9156_4801_B01D_E4079A65C17A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>

#ifdef __cplusplus
#ifdef UNILIBFRAMEWORK_EXPORTS
#define SP_EXPORT extern "C" __declspec (dllexport)
#else
#define SP_EXPORT extern "C" __declspec (dllimport)
#endif
#else
#ifdef UNILIBFRAMEWORK_EXPORTS
#define SP_EXPORT __declspec (dllexport)
#else
#define SP_EXPORT __declspec (dllimport)
#endif
#endif
//////////////////////////////////////////////////////////////////////////
///
typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint32;
typedef unsigned __int64    uint64;

typedef char                int8;
typedef short               int16;
typedef int                 int32;
typedef __int64             int64;

//#define Internal_Debug
///
/// UNICDOE string
///
#include <string>
#if defined (UNICODE) ||  defined (_UNICODE)
typedef std::wstring    string_t;
#else
typedef std::string     string_t;
#endif

//////////////////////////////////////////////////////////////////////////
///
/// Handle
///
typedef void* SP_HANDLE;
#define INVALID_NPI_HANDLE      (NULL)

///
/// Calling convention, if VARARG functions, replaced with __cdecl.
///
#ifdef  SP_API
#undef  SP_API
#endif
#define SP_API  //__stdcall



#ifdef __cplusplus
#define STRUCT_INITIALIZE(_tag)         \
        _tag(void)  { \
        ZeroMemory(this, sizeof(*this));    \
    }
#else
#define STRUCT_INITIALIZE(_tag)
#endif

///
#define SAFE_RELEASE_OBJECT(_pObj)  \
    {  \
        if (NULL != (_pObj)) { \
            (_pObj)->Release(); \
            (_pObj) = NULL; \
        } \
    } \

///
/// Magic word
///
const DWORD MAGIC_SP14 = 0x53503134; // "SP14"

///
/// IP: xxx.xxx.xxx.xxx
#define MAX_IP_ADDR_LEN             ( 16 )

///
/// Calculate array size.
///
#define ARRAY_SIZE(a)               ( sizeof((a))/sizeof((a)[0]) )


///
/// Limit check
///
#define IN_RANGE(low, x, high)      ( (x) >= (low) && (x) <= (high) )


///
/// Integer to string
/// Example:
///      NUM2STR(Hello, __LINE__)   ==> Hello129
///
#define NUM2STR_2(x,y)      x##y
#define NUM2STR(x,y)        NUM2STR_2(x,y)

#define TOSTR(str)          #str
#define TOWSTR(str)        L#str

#define CopySize(x)         (sizeof(x) - 1)


#ifdef __cplusplus
#define CLASS_UNCOPYABLE(class_name) \
    private: \
    class_name##(const class_name##&); \
    class_name##& operator=(const class_name##&);
#endif
///
/// Timeout, unit: ms
///
#define TIMEOUT_1S      ( 1000)
#define TIMEOUT_2S      ( 2000)
#define TIMEOUT_3S      ( 3000)
#define TIMEOUT_5S      ( 5000)
#define TIMEOUT_10S     (10000)
#define TIMEOUT_20S     (20000)
#define TIMEOUT_30S     (30000)
#define TIMEOUT_60S     (60000)
#define TIMEOUT_100S    (100000)
#define TIMEOUT_180S    (180000)

///
#define INVALID_POSITIVE_DOUBLE_VALUE       (+999999999999.00)
#define INVALID_NEGATIVE_DOUBLE_VALUE       (-999999999999.00)

#define INVALID_POSITIVE_INTEGER_VALUE      (+99999999)
#define INVALID_NEGATIVE_INTEGER_VALUE      (-99999999)

#define ROUNDTOINTEGER(_x)                  ( ( ((_x) > 0) ? (int((_x) + 0.5)) : (int((_x) - 0.5)) ) )
#define IS_EQUAL(d1,d2)                     (((d1)-(d2))>-0.000001 && ((d1)-(d2))<0.000001)
#define IS_BIT_SET(mask, bit)               ((mask) & (bit))

enum
{
    FAIL = 0,
    PASS = 1
};

// General param definition
typedef struct  _tagDOUBLE_LIMIT
{
    double low;
    double upp;
    void Init()
    {
        low = 0;
        upp = 0;
    }
    
    _tagDOUBLE_LIMIT()
    {
        Init();
    }
} DOUBLE_LIMIT;

typedef struct  _tagINTEGER_LIMIT
{
    int low;
    int upp;
    void Init()
    {
        low = 0;
        upp = 0;
    }
    _tagINTEGER_LIMIT()
    {
        Init();
    }
} INTEGER_LIMIT;

typedef enum DL_TYPE_ENUM : INT
{
    INVALID_DL_TYPE = -1,
    RESEARCH_TYPE,
    FACTORY_TYPE,
    UPGRADE_TYPE,
    MAX_DL_TYPE
} DL_TYPE_ENUM_INT;

typedef struct
{
    LPCWSTR      name;
    DL_TYPE_ENUM type;
} UNISOC_DLTYPE;

#if _MSC_VER > 1000
static UNISOC_DLTYPE UNISOC_DLTYPE_ARR[] =
{
    {L"ResearchDownload",       RESEARCH_TYPE},
    {L"FactoryDownload",        FACTORY_TYPE},
    {L"UpgradeDownload",        UPGRADE_TYPE}
};
#endif
//
#endif /* AFX_GLOBAL_DEF_H__3C71714E_9156_4801_B01D_E4079A65C17A__INCLUDED_ */