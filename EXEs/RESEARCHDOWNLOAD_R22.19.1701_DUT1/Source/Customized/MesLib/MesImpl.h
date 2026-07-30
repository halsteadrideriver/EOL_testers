#pragma once
#include "Tr.h"
#include "IMes.h"
#include "UnisocMesHelper.h"

class CMesImpl : public IMes, public CTr
{
public:
    CMesImpl( LPVOID pLogUtil );
    virtual ~CMesImpl();
    
public:
    virtual BOOL Startup();
    virtual void Clearup();
    virtual void Release( void );
    virtual void MES_GetLastError( LPSTR pszErrMsg, DWORD dwSize );
    virtual SPRESULT MES_Connect();
    virtual SPRESULT MES_Disconnect();
    virtual SPRESULT MES_CheckFlow( LPCSTR lpszSN );
    virtual SPRESULT MES_SnInput( LPCSTR lpszSN );
    virtual SPRESULT MES_SendTestResult( LPCDLMES_RESULT lpResult );
    virtual SPRESULT MES_CheckCRC( LPCTSTR lpPacPath );
    
    
private:
    CUnisocMesHelper m_fnUnisocMesHelper;
};