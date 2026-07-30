#include "stdafx.h"
#include "DLoader.h"
#include "HelperBase.h"

CHelperBase::CHelperBase( CDLFWDriver* pDrv )
    : m_pDrv( pDrv )
{
    ASSERT( pDrv );
}

CHelperBase::~CHelperBase( void )
{

}

void CHelperBase::LoadCfgDLFramework()
{
    CString strIniPath = g_theApp.m_strAppPath;
    strIniPath += _T( "App\\DLFramework.ini" );
    m_nLogLevel = GetPrivateProfileInt( _T( "Log" ), _T( "Level" ), SPLOGLV_ERROR, strIniPath );
    m_bCompressDLFWLog = GetPrivateProfileInt( _T( "Log" ), _T( "CompressDLFWLog" ), SPLOGLV_ERROR, strIniPath );
    
    TCHAR szPath[MAX_PATH * 2] = {0};
    GetPrivateProfileString( _T( "Log" ), _T( "LogPath" ), _T( ".\\" ), szPath, MAX_PATH * 2, strIniPath );
    m_strCfgLogPath = szPath;
    if ( m_strCfgLogPath.IsEmpty() || m_strCfgLogPath.CompareNoCase( _T( ".\\" ) ) == 0 )
    {
        m_strCfgLogPath = g_theApp.m_strAppPath;
        m_strCfgLogPath += _T( "Log" );
    }
}