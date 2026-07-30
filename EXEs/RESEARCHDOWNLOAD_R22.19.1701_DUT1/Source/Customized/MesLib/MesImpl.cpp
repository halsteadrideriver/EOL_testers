#include "StdAfx.h"
#include "MesImpl.h"



CMesImpl::CMesImpl( LPVOID pLogUtil )
    : m_fnUnisocMesHelper( ( ISpLog* )pLogUtil )
{
    BindLog( ( ISpLog* )pLogUtil, "MES" );
}

CMesImpl::~CMesImpl()
{

}

BOOL CMesImpl::Startup()
{
    return m_fnUnisocMesHelper.Startup();
}

void CMesImpl::Clearup()
{
    m_fnUnisocMesHelper.Clearup();
}

void CMesImpl::Release( void )
{
    Clearup();
    delete this;
}

SPRESULT CMesImpl::MES_Connect( )
{
    if ( MES_SUCCESS != m_fnUnisocMesHelper.MesConnect() )
    {
        return SP_E_FAIL;
    }
    return SP_OK;
}

SPRESULT CMesImpl::MES_Disconnect()
{
    return SP_OK;
}

SPRESULT CMesImpl::MES_CheckFlow( LPCSTR lpszSN )
{
    if ( MES_SUCCESS != m_fnUnisocMesHelper.MesCheckFlow( lpszSN ) )
    {
        return SP_E_FAIL;
    }
    return SP_OK;
}

SPRESULT CMesImpl::MES_SnInput( LPCSTR lpszSN )
{
    if ( MES_SUCCESS != m_fnUnisocMesHelper.MesSnInput( lpszSN ) )
    {
        return SP_E_FAIL;
    }
    return SP_OK;
}

SPRESULT CMesImpl::MES_SendTestResult( LPCDLMES_RESULT lpResult )
{
    if ( MES_SUCCESS != m_fnUnisocMesHelper.MesSendTestResult( lpResult ) )
    {
        return SP_E_FAIL;
    }
    return SP_OK;
}

SPRESULT CMesImpl::MES_CheckCRC( LPCTSTR lpPacPath )
{
    if ( MES_SUCCESS != m_fnUnisocMesHelper.MesCheckCRC( lpPacPath ) )
    {
        return SP_E_FAIL;
    }
    return SP_OK;
}

void CMesImpl::MES_GetLastError( LPSTR pszErrMsg, DWORD dwSize )
{
    m_fnUnisocMesHelper.MesGetErrMsg( pszErrMsg, dwSize );
}