#include "StdAfx.h"
#include "DLoader.h"
#include "MesLoginHelper.h"

#define MES_MID_PATH _T( "App\\Customized\\UnisocMES\\" )

CMesLoginHelper::CMesLoginHelper( void )
{
}

CMesLoginHelper::~CMesLoginHelper( void )
{
}

BOOL CMesLoginHelper::MesEnable()
{
    return g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, _T( "MES" ), _T( "Enable" ), 0 );
}

void CMesLoginHelper::MesGetInfo( LPWSTR lpMesInfo, DWORD dwSize )
{
    TCHAR szPathIni[256] = { 0 };
    TCHAR szMesInfo[128] = { 0 };
    _stprintf_s( szPathIni, _T( "%s%sUnisoc_Solution_MES.ini" ), g_theApp.m_strAppPath.operator LPCWSTR(), MES_MID_PATH );
    int nMesEnable = GetPrivateProfileInt( _T( "Setting" ), _T( "MES Enable" ), 0, szPathIni );
    if ( nMesEnable > 0 )
    {
        TCHAR szDB[64] = { 0 };
        TCHAR szBatch[64] = { 0 };
        TCHAR szStation[64] = { 0 };
        GetPrivateProfileString( _T( "MES 2.0" ), _T( "Database" ), _T( "" ), szDB, sizeof( szDB ), szPathIni );
        GetPrivateProfileString( _T( "MES 2.0" ), _T( "BatchName" ), _T( "" ), szBatch, sizeof( szBatch ), szPathIni );
        GetPrivateProfileString( _T( "MES 2.0" ), _T( "StationName" ), _T( "" ), szStation, sizeof( szStation ), szPathIni );
        
        _stprintf_s( szMesInfo, _T( "MES Online\r[DB] : %s\r[Batch] : %s [Station]: %s" ), szDB, szBatch, szStation );
    }
    else
    {
        _stprintf_s( szMesInfo, _T( "MES Offline" ) );
    }
    _tcsncpy_s( lpMesInfo, dwSize, szMesInfo, _tcsclen( szMesInfo ) );
}

void CMesLoginHelper::MesLogin()
{
    TCHAR szExePath[_MAX_PATH] = {0};
    
    _stprintf_s( szExePath, _T( "%s%sUnisoc_MES_Login.exe" ), g_theApp.m_strAppPath.operator LPCWSTR(), MES_MID_PATH );
    
    STARTUPINFO sinfo;
    PROCESS_INFORMATION pinfo;
    memset( &sinfo, 0, sizeof( sinfo ) );
    sinfo.cb = sizeof( STARTUPINFO );
    
    if ( CreateProcess( NULL, szExePath, NULL, NULL, FALSE, 0, NULL, NULL, &sinfo, &pinfo ) )
    {
        WaitForSingleObject( pinfo.hProcess, INFINITE );
        ::CloseHandle( pinfo.hThread );
        ::CloseHandle( pinfo.hProcess );
    }
}