#pragma once
#include "Tr.h"
#include "IMes.h"
#include "MESDriver.h"
#include "CSimpleLock.h"

class CUnisocMesHelper : public CTr
{
public:
    CUnisocMesHelper( ISpLog* pLog );
    virtual ~CUnisocMesHelper( void );
    
public:
    BOOL Startup();
    void Clearup();
    void MesGetErrMsg( LPSTR pszErrMsg, DWORD dwSize );
    
    MES_RESULT MesConnect();
    MES_RESULT MesSnInput( LPCSTR pszSN );
    MES_RESULT MesCheckFlow( LPCSTR pszSN );
    MES_RESULT MesSendTestResult( LPCDLMES_RESULT lpResult );
    MES_RESULT MesCheckCRC( LPCTSTR lpPacPath );
    
private:
    BOOL    CheckCRC( LPSTR pszPath, LPSTR pszCRC );
    BOOL    CheckPacCRC( INT nMemory, LPSTR pszPath, LPSTR pszCRC );
    
private:
    UINT m_uiACP;
    CHAR m_szErrorMsg[512] = {0};
    CHAR m_szGuid[128] = {0};
    CMESDriver m_fnMesDrv;
    CSimpleLock m_LockMes;
};

