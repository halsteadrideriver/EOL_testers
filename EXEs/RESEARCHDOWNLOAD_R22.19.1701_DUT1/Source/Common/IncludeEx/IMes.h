#pragma once

#include "def\global_err.h"
#include "def\global_def.h"
#include "def\callback_def.h"

typedef struct _tagDLMES_RESULT
{
    INT    nResult;
    LPCSTR lpszTime;
    LPCSTR lpszSN;
    LPCSTR lpszToolName;
    LPCSTR lpszToolVer;
    LPCSTR lpszError;
    
    STRUCT_INITIALIZE( _tagDLMES_RESULT );
} DLMES_RESULT, * LPDLMES_RESULT;
typedef const DLMES_RESULT* LPCDLMES_RESULT;

class IMes
{
public:
    virtual void Release( void ) = 0;
    virtual BOOL Startup() = 0;
    virtual void Clearup() = 0;
    virtual void MES_GetLastError( LPSTR pszErrMsg, DWORD dwSize ) = 0;
    virtual SPRESULT MES_Connect( ) = 0;
    virtual SPRESULT MES_Disconnect( ) = 0;
    virtual SPRESULT MES_CheckFlow( LPCSTR lpszSN ) = 0;
    virtual SPRESULT MES_SnInput( LPCSTR lpszSN ) = 0;
    virtual SPRESULT MES_SendTestResult( LPCDLMES_RESULT lpResult ) = 0;
    virtual SPRESULT MES_CheckCRC( LPCTSTR lpPacPath ) = 0;
};

SP_EXPORT IMes* SP_API CreateMesObject( LPVOID pLogUtil );

