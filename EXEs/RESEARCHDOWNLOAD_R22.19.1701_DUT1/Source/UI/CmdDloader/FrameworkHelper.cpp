#include "StdAfx.h"
#include <iostream>
#include <vector>
#include "FrameworkHelper.h"
#include "Shlwapi.h"
#pragma comment(lib,"Shlwapi.lib")

extern wstring GetAppPath( void );

class CPrintf sealed
{
public:
    CPrintf( LPCTSTR lpszFnName, LPCTSTR lpszFmt = NULL, ... )
        : m_strFnName( lpszFnName )
    {
#ifdef _DEBUG
        if ( NULL == lpszFmt )
        {
            _tprintf( _T( "[ ENTER ] %s( )\n" ), m_strFnName.c_str() );
        }
        else
        {
            va_list args = NULL;
            va_start( args, lpszFmt );
            size_t nLength = _vsctprintf( lpszFmt, args ) + 1;
            std::vector<TCHAR> vBuff( nLength, _T( '\0' ) );
            if ( _vsntprintf_s( &vBuff[0], vBuff.size(), nLength, lpszFmt, args ) > 0 )
            {
                _tprintf( _T( "[ ENTER ] %s( %s )\n" ), m_strFnName.c_str(), ( LPCTSTR )&vBuff[0] );
            }
            va_end( args );
        }
        fflush( stdout );
#endif // _DEBUG
    }
    
    ~CPrintf( void )
    {
#ifdef _DEBUG
        _tprintf( _T( "[ LEAVE ] %s( )\n" ), m_strFnName.c_str() );
        fflush( stdout );
#endif // _DEBUG       
    }
    
private:
    string_t m_strFnName;
};
//////////////////////////////////////////////////////////////////////////
// Callback function
BOOL CALLBACK MyCallBack( LPCSPCALLBACK_DATA lpcbData, LPCVOID pParam )
{
    if ( NULL == lpcbData )
    {
        assert( 0 );
        return FALSE;
    }
    
    CCallbackHelper* pThis = ( CCallbackHelper* )pParam;
    assert( NULL != pThis );
    
    return pThis->HandleCallbackData( lpcbData );
}

//////////////////////////////////////////////////////////////////////////
CFrameworkHelper::CFrameworkHelper( void )
    : m_pFramLog( NULL )
    , m_pTaskLog( NULL )
{

}

CFrameworkHelper::~CFrameworkHelper( void )
{
}

SPRESULT CFrameworkHelper::Startup( DL_TYPE_ENUM eDLType, UINT nLv )
{
    CPrintf print( L"DLFW_Startup" );
    m_eDLType = eDLType;
    m_strAppPath = GetAppPath();
    if ( !m_fnDLFWDriver.Startup( m_strAppPath ) )
    {
        return SP_E_FAIL;
    }
    
    LoadCfgDLFramework();
    InitLogPath();
    
    CHKRESULT( CreatFramLog() );
    CHKRESULT( OpenFramLog() );
    
    SPCALLBACK_PARAM cb;
    cb.pFunc = MyCallBack;
    cb.pParam = &m_fnCallHelper;
    
    CHKRESULT( m_fnDLFWDriver.DLFW_Startup( eDLType, &cb, m_pFramLog ) );
    //CHKRESULT( m_fnDLFWDriver.DLFW_DeviceMoniter( TRUE ) );
    SetProperty( DLFW_ATTR_START_PATH, 0, ( LPVOID )m_strAppPath.c_str() );
    return SP_OK;
}

SPRESULT CFrameworkHelper::DeviceMoniter( BOOL bStart )
{
    return m_fnDLFWDriver.DLFW_DeviceMoniter( bStart );
}

void CFrameworkHelper::Cleanup()
{
    CPrintf print( L"DLFW_Cleanup" );
    //m_fnDLFWDriver.DLFW_DeviceMoniter( FALSE );
    m_fnDLFWDriver.DLFW_Cleanup();
    PrintFramLog();
    FreeFramLog();
    if ( RenameLogPath() )
    {
        CompressedLog();
    }
    m_fnDLFWDriver.Cleanup();
}

SPRESULT CFrameworkHelper::LoadPacket( LPCTSTR lpPacket, LPCTSTR lpImageDir )
{
    CPrintf print( L"DLFW_LoadPacket", L"%s", lpPacket );
    
    SPRESULT spRlt = m_fnDLFWDriver.DLFW_LoadPacket( lpPacket, lpImageDir );
    if ( SP_OK != spRlt )
    {
        printf( "DLFW_LoadPacket Fail.\n" );
        fflush( stdout );
    }
    
    return spRlt;
}

SPRESULT CFrameworkHelper::CreatTask( UINT nLv )
{
    CPrintf print( L"DLFW_CreateTask" );
    
    CHKRESULT( CreatTaskLog() );
    CHKRESULT( OpenTaskLog() );
    SPCALLBACK_PARAM cb;
    cb.pFunc = MyCallBack;
    cb.pParam = &m_fnCallHelper;
    m_hTask = m_fnDLFWDriver.DLFW_CreateTask( &cb, m_pTaskLog );
    if ( NULL == m_hTask )
    {
        printf( "DLFW_CreateTask Fail.\n" );
        fflush( stdout );
        return SP_E_LOAD_LIBRARY;
    }
    CHKRESULT( TaskSetProperty( DLFW_ATTR_LOG_PATH, 0, ( LPVOID )m_strLogPath.c_str() ) )
    return SP_OK;
}

SPRESULT CFrameworkHelper::SyncParameters()
{
    CPrintf print( L"DLFW_SyncParameters" );
    return m_fnDLFWDriver.DLFW_SyncParameters();
}

SPRESULT CFrameworkHelper::ReloadSettings()
{
    CPrintf print( L"DLFW_ReloadSettings" );
    return m_fnDLFWDriver.DLFW_ReloadSettings();
}

SPRESULT CFrameworkHelper::GetProperty( INT nProperty, INT nFlags, LPVOID lpValue )
{
    CPrintf print( L"DLFW_GetProperty" );
    return m_fnDLFWDriver.DLFW_GetProperty( nProperty, nFlags, lpValue );
}

SPRESULT CFrameworkHelper::SetProperty( INT nProperty, INT nFlags, LPVOID lpValue )
{
    CPrintf print( L"DLFW_SetProperty" );
    return m_fnDLFWDriver.DLFW_SetProperty( nProperty, nFlags, lpValue );
}

SPRESULT CFrameworkHelper::RunTask()
{
    CPrintf print( L"DLFW_RunTask", L"Port: %d", m_fnCallHelper.m_nPort );
    return m_fnDLFWDriver.DLFW_RunTask( m_hTask, m_fnCallHelper.m_nPort );
}

SPRESULT CFrameworkHelper::StopTask()
{
    CPrintf print( L"DLFW_StopTask" );
    return m_fnDLFWDriver.DLFW_StopTask( m_hTask );
}

void CFrameworkHelper::FreeTask()
{
    CPrintf print( L"DLFW_FreeTask" );
    m_fnDLFWDriver.DLFW_FreeTask( m_hTask );
    PrintTaskLog();
    FreeTaskLog();
}

void CFrameworkHelper::PrintTaskLog()
{
    if ( 0 != m_fnCallHelper.m_tDlEnd.u32ErrCode && NULL != m_fnCallHelper.m_tDlEnd.lpErrMsg )
    {
        m_pTaskLog->LogFmtStr( SPLOGLV_ERROR, m_fnCallHelper.m_tDlEnd.lpErrMsg );
    }
}

void CFrameworkHelper::PrintFramLog()
{
    if ( 0 != m_fnCallHelper.m_tDlEnd.u32ErrCode && NULL != m_fnCallHelper.m_tDlEnd.lpErrMsg )
    {
        m_pFramLog->LogFmtStr( SPLOGLV_ERROR, m_fnCallHelper.m_tDlEnd.lpErrMsg );
    }
}

SPRESULT CFrameworkHelper::TaskSetProperty( INT nProperty, INT nFlags, LPCVOID lpValue )
{
    CPrintf print( L"DLFW_TaskSetProperty" );
    return m_fnDLFWDriver.DLFW_TaskSetProperty( m_hTask, nProperty, nFlags, lpValue );
}

void CFrameworkHelper::LoadCfgDLFramework()
{
    string_t strIniPath = m_strAppPath;
    strIniPath += _T( "App\\DLFramework.ini" );
    m_nLogLevel = GetPrivateProfileInt( _T( "Log" ), _T( "Level" ), SPLOGLV_ERROR, strIniPath.c_str() );
    
    TCHAR szPath[MAX_PATH * 2] = { 0 };
    GetPrivateProfileString( _T( "Log" ), _T( "LogPath" ), _T( ".\\" ), szPath, MAX_PATH * 2, strIniPath.c_str() );
    m_strCfgLogPath = szPath;
    if ( m_strCfgLogPath.empty() || m_strCfgLogPath.compare( _T( ".\\" ) ) == 0 )
    {
        m_strCfgLogPath = m_strAppPath;
        m_strCfgLogPath += _T( "Log" );
    }
}

BOOL CFrameworkHelper::CreateDeepDirectory( string_t strDir )
{
    string_t::size_type tmp_pos_begin = 0;
    string_t::size_type tmp_pos = 0;
    string_t strTmpDir = _T( "" );
    do
    {
        tmp_pos = strDir.find( '\\', tmp_pos_begin );
        if ( tmp_pos == strDir.npos )
        {
            tmp_pos = strDir.length();
            if ( strTmpDir == strDir )
            {
                break;
            }
        }
        strTmpDir = strDir.substr( 0, tmp_pos );
        if ( strTmpDir.empty() )
        {
            return FALSE;
        }
        if ( !PathIsDirectory( strTmpDir.c_str() ) )
        {
            CreateDirectory( strTmpDir.c_str(), NULL );
        }
        tmp_pos_begin = tmp_pos + 1;
    } while ( true );
    return TRUE;
}

SPRESULT CFrameworkHelper::CreatFramLog()
{
    m_fnDLFWDriver.CreateISpLogObject( &m_pFramLog );
    if ( NULL == m_pFramLog )
    {
        printf( "CreateISpLogObject Fail.\n" );
        fflush( stdout );
        return SP_E_LOAD_LIBRARY;
    }
    return SP_OK;
}

SPRESULT CFrameworkHelper::OpenFramLog()
{
    if ( NULL != m_pFramLog )
    {
        OpenArgs_T arg;
        arg.nLogLevel = m_nLogLevel;
        strcpy_s( arg.szModule, "DLFramework" );
        swprintf_s( arg.Local.szLogFile, L"%s\\DLFramework.log", m_strLogPath.c_str() );
        arg.Local.nOpenFlags = splog::defaultFlags;
        return  m_pFramLog->Open( ( LPCVOID )&arg ) ? SP_OK : SP_E_OPEN_FILE;
    }
    return SP_OK;
}

void CFrameworkHelper::CloseFramLog()
{
    if ( NULL != m_pFramLog )
    {
        m_pFramLog->Close();
    }
}

void CFrameworkHelper::FreeFramLog()
{
    if ( NULL != m_pFramLog )
    {
        m_pFramLog->Close();
        m_pFramLog->Release();
        m_pFramLog = NULL;
    }
}

void CFrameworkHelper::InitLogPath()
{
    m_strLogPath = m_strCfgLogPath;
    SYSTEMTIME time;
    GetLocalTime( &time );
    
    TCHAR szTime[128] = { 0 };
    _stprintf_s( szTime, _T( "\\%04d_%02d_%02d\\%s-%04d_%02d_%02d-%02d_%02d_%02d_%03d" ),
                 time.wYear, time.wMonth, time.wDay,
                 UNISOC_DLTYPE_ARR[m_eDLType].name,
                 time.wYear, time.wMonth, time.wDay,
                 time.wHour, time.wMinute, time.wSecond, time.wMilliseconds );
    m_strLogPath += szTime;
    CreateDeepDirectory( m_strLogPath.c_str() );
}

SPRESULT CFrameworkHelper::CreatTaskLog()
{
    m_fnDLFWDriver.CreateISpLogObject( &m_pTaskLog );
    if ( NULL == m_pTaskLog )
    {
        printf( "CreateISpLogObject Fail.\n" );
        fflush( stdout );
        return SP_E_LOAD_LIBRARY;
    }
    return SP_OK;
}

SPRESULT CFrameworkHelper::OpenTaskLog( )
{
    OpenArgs_T arg;
    arg.nLogLevel = m_nLogLevel;
    strcpy_s( arg.szModule, "DLFWTask" );
    swprintf_s( arg.Local.szLogFile, L"%s\\Trace.log", m_strLogPath.c_str() );
    return m_pTaskLog->Open( ( LPCVOID )&arg ) ? SP_OK : SP_E_OPEN_FILE;
}

void CFrameworkHelper::CloseTaskLog()
{
    if ( NULL != m_pTaskLog )
    {
        m_pTaskLog->Close();
    }
}

void CFrameworkHelper::FreeTaskLog()
{
    if ( NULL != m_pTaskLog )
    {
        m_pTaskLog->Close();
        m_pTaskLog->Release();
        m_pTaskLog = NULL;
    }
}

BOOL CFrameworkHelper::RenameLogPath()
{
    if ( PathFileExists( m_strLogPath.c_str() ) )
    {
        TCHAR szDescription[256] = { 0 };
        if ( m_fnCallHelper.m_tDlEnd.u32ErrCode )
        {
            if ( !m_fnCallHelper.m_strSN1.empty() )
            {
                _stprintf_s( szDescription, _T( "FAIL(%d)_SN%s_" ), m_fnCallHelper.m_tDlEnd.u32ErrCode, m_fnCallHelper.m_strSN1.c_str() );
            }
            else if ( !m_fnCallHelper.m_strIMEI.empty() )
            {
                _stprintf_s( szDescription, _T( "FAIL(%d)_IMEI%s_" ), m_fnCallHelper.m_tDlEnd.u32ErrCode, m_fnCallHelper.m_strIMEI.c_str() );
            }
            else
            {
                _stprintf_s( szDescription, _T( "FAIL(%d)_" ), m_fnCallHelper.m_tDlEnd.u32ErrCode );
            }
        }
        else
        {
            if ( !m_fnCallHelper.m_strSN1.empty() )
            {
                _stprintf_s( szDescription, _T( "PASS_SN%s_" ), m_fnCallHelper.m_strSN1.c_str() );
            }
            else if ( !m_fnCallHelper.m_strIMEI.empty() )
            {
                _stprintf_s( szDescription, _T( "PASS_IMEI%s_" ), m_fnCallHelper.m_strIMEI.c_str() );
            }
            else
            {
                _tcscpy_s( szDescription, _T( "PASS_" ) );
            }
        }
        
        _stprintf_s( szDescription, _T( "%sCOM%d" ), szDescription, m_fnCallHelper.m_tDlEnd.u32Port );
        
        string_t strOldNmae = m_strLogPath;
        
        string_t::size_type pos = m_strLogPath.rfind( '\\' ) + 1;
        m_strLogPath.replace( pos, _tcsclen( UNISOC_DLTYPE_ARR[m_eDLType].name ), szDescription );
        for ( int i = 0; i < 50; i++ )
        {
            if ( PathFileExists( m_strLogPath.c_str() ) )
            {
                break;
            }
            _trename( strOldNmae.c_str(), m_strLogPath.c_str() );
            Sleep( 100 );
        }
        
        if ( !PathFileExists( m_strLogPath.c_str() ) )
        {
            return FALSE;
        }
    }
    return TRUE;
}

void CFrameworkHelper::CompressedLog()
{
    TCHAR szCmd[512] = { 0 };
    _stprintf_s( szCmd, _T( "%sSystem\\Tools\\7zip\\32\\7z.exe" ), m_strAppPath.c_str() );
    if ( PathFileExists( szCmd ) )
    {
        _stprintf_s( szCmd, _T( "\"%sSystem\\Tools\\7zip\\32\\7z.exe\" a -tzip \"%s.zip\" \"%s\" -r" ), m_strAppPath.c_str(), m_strLogPath.c_str(), m_strLogPath.c_str() );
        RunCmdline( szCmd );
        
        _stprintf_s( szCmd, _T( "cmd /C rd /s /q \"%s\"" ), m_strLogPath.c_str() );
        RunCmdline( szCmd );
    }
    m_strLogPath = _T( "" );
}

void CFrameworkHelper::RunCmdline( LPCTSTR lpCmd )
{
    STARTUPINFO si;
    memset( &si, 0, sizeof( STARTUPINFO ) );
    si.cb = sizeof( STARTUPINFO );
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
    PROCESS_INFORMATION pi;
    
    TCHAR szCommandLine[1024] = _T( "" );
    _tcscpy_s( szCommandLine, ARRAY_SIZE( szCommandLine ), lpCmd );
    
    BOOL bRes = CreateProcess( NULL, szCommandLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi );
    CloseHandle( pi.hThread );
    if ( bRes )
    {
        WaitForSingleObject( pi.hProcess, INFINITE );
        DWORD dwExitCode;
        GetExitCodeProcess( pi.hProcess, &dwExitCode );
        
    }
    CloseHandle( pi.hProcess );
}
