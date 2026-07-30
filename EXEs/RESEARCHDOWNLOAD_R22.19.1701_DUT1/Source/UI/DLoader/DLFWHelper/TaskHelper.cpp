#include "StdAfx.h"
#include "DLoader.h"
#include "TaskHelper.h"
#include "OperationHelper.h"

#ifdef _SPUPGRADE
#define LOG_PATH_START _T("_UpgreadDownloading_")
#elif defined _FACTORY
#define LOG_PATH_START _T("_FactoryDownloading_")
#else
#define LOG_PATH_START _T("_ResearchDownloading_")
#endif

CSimpleLock g_LockCmd;
//////////////////////////////////////////////////////////////////////////
CTaskHelper::CTaskHelper( CDLFWDriver* pDrv )
    : CHelperBase( pDrv )
    , m_bRename( FALSE )
{

}

CTaskHelper::~CTaskHelper( void )
{

}

DWORD WINAPI CTaskHelper::ThreadFunc( LPVOID lpParam )
{
    CTaskHelper* pThis = ( CTaskHelper* )lpParam;
    return pThis->ThreadFunc();
}

DWORD CTaskHelper::ThreadFunc()
{
    m_bRename = RenameLogPath();
    return 0;
}

void CTaskHelper::FreeLog()
{
    if ( NULL != m_pLog )
    {
        m_pLog->Close();
        m_pLog->Release();
        m_pLog = NULL;
        if ( RenameLogPath() )
        {
            CompressedLog();
        }
    }
}

void CTaskHelper::FreeCallback()
{
    if ( NULL != m_pCbkHelper )
    {
        delete m_pCbkHelper;
        m_pCbkHelper = NULL;
    }
}

void CTaskHelper::InitLogPath( UINT nPort )
{
    m_strLogPath = m_strCfgLogPath;
    SYSTEMTIME time;
    GetLocalTime( &time );
    m_strLogPath.AppendFormat( _T( "\\%04d_%02d_%02d\\%sCOM%d-%04d_%02d_%02d-%02d_%02d_%02d_%03d" ),
                               time.wYear, time.wMonth, time.wDay,
                               LOG_PATH_START, nPort,
                               time.wYear, time.wMonth, time.wDay,
                               time.wHour, time.wMinute, time.wSecond, time.wMilliseconds );
    COperationHelper op;
    op.CreateDeepDirectory( m_strLogPath.operator LPCTSTR() );
    
}

BOOL CTaskHelper::RenameLogPath()
{
    if ( NULL != m_pCbkHelper && m_strLogPath.Find( LOG_PATH_START ) > 0 &&
         PathFileExists( m_strLogPath.operator LPCTSTR() ) )
    {
        TCHAR szDescription[256] = {0};
        if ( m_pCbkHelper->m_tBmDlEnd.dwErrCode )
        {
            if ( !m_pCbkHelper->m_strSN1.empty() )
            {
                _stprintf( szDescription, _T( "FAIL(%d)_SN%s_" ), m_pCbkHelper->m_tBmDlEnd.dwErrCode, m_pCbkHelper->m_strSN1.c_str() );
            }
            else if ( !m_pCbkHelper->m_strIMEI.empty() )
            {
                _stprintf( szDescription, _T( "FAIL(%d)_IMEI%s_" ), m_pCbkHelper->m_tBmDlEnd.dwErrCode, m_pCbkHelper->m_strIMEI.c_str() );
            }
            else
            {
                _stprintf( szDescription, _T( "FAIL(%d)_" ), m_pCbkHelper->m_tBmDlEnd.dwErrCode );
            }
        }
        else
        {
            if ( !m_pCbkHelper->m_strSN1.empty() )
            {
                _stprintf( szDescription, _T( "PASS_SN%s_" ), m_pCbkHelper->m_strSN1.c_str() );
            }
            else if ( !m_pCbkHelper->m_strIMEI.empty() )
            {
                _stprintf( szDescription, _T( "PASS_IMEI%s_" ), m_pCbkHelper->m_strIMEI.c_str() );
            }
            else
            {
                _tcscpy_s( szDescription, _T( "PASS_" ) );
            }
        }
        
        CString strOldNmae = m_strLogPath;
        m_strLogPath.Replace( LOG_PATH_START, szDescription );
        for ( int i = 0; i < 50; i++ )
        {
            if ( PathFileExists( m_strLogPath.operator LPCTSTR() ) )
            {
                break;
            }
            _trename( strOldNmae.operator LPCTSTR(), m_strLogPath.operator LPCTSTR() );
            Sleep( 100 );
        }
        
        if ( !PathFileExists( m_strLogPath.operator LPCTSTR() ) )
        {
            return FALSE;
        }
    }
    return TRUE;
}

void CTaskHelper::CopyLog( LPCTSTR lpszLogPath )
{
    if ( m_bCompressDLFWLog && PathFileExists( lpszLogPath ) )
    {
        CString strLogPath( lpszLogPath );
        CString strName = strLogPath.Right( strLogPath.GetLength() - strLogPath.ReverseFind( '\\' ) - 1 );
        CString strNewLogPath = m_strLogPath;
        strNewLogPath += _T( "\\" );
        strNewLogPath += strName;
        CopyFileW( lpszLogPath, strNewLogPath.operator LPCWSTR(), FALSE );
    }
}

void CTaskHelper::CompressedLog()
{
    /// Solve the crash caused by compressing the log when multiple downloads
    CLocks Lock( g_LockCmd );
    CString strCmd = _T( "" );
    strCmd.Format( _T( "%sSystem\\Tools\\7zip\\32\\7z.exe" ), g_theApp.m_strAppPath.operator LPCWSTR() );
    if ( PathFileExists( m_strLogPath.operator LPCTSTR() ) && PathFileExists( strCmd.operator LPCTSTR() ) )
    {
        strCmd.Format( _T( "\"%sSystem\\Tools\\7zip\\32\\7z.exe\" a -tzip \"%s.zip\" \"%s\" -r" ), g_theApp.m_strAppPath.operator LPCWSTR(), m_strLogPath.operator LPCWSTR(), m_strLogPath.operator LPCWSTR() );
        RunCmdline( strCmd.operator LPCTSTR() );
        
        strCmd.Format( _T( "cmd /C rd /s /q \"%s\"" ), m_strLogPath.operator LPCWSTR() );
        RunCmdline( strCmd.operator LPCTSTR() );
    }
    m_strLogPath = _T( "" );
}

void CTaskHelper::RunCmdline( LPCTSTR lpCmd )
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
    
    //Close the thread handle as soon as
    //it is no longer needed!
    CloseHandle( pi.hThread );
    if ( bRes )
    {
        //Suspend our execution until
        //the child has terminated.
        WaitForSingleObject( pi.hProcess, INFINITE );
        
        //The child process terminated;
        //get its exit code.
        DWORD dwExitCode;
        GetExitCodeProcess( pi.hProcess, &dwExitCode );
        
    }
    //Close the process handle as soon as
    //it is no longer needed.
    CloseHandle( pi.hProcess );
}

SPRESULT CTaskHelper::CreatLog()
{
    m_pDrv->CreateISpLogObject( &m_pLog );
    if ( NULL == m_pLog )
    {
        return SP_E_LOAD_LIBRARY;
    }
    return SP_OK;
}

SPRESULT CTaskHelper::OpenLog( UINT nPort )
{
    if ( m_bRename )
    {
        CompressedLog();
    }
    InitLogPath( nPort );
    OpenArgs_T arg;
    arg.nLogLevel = m_nLogLevel;
    strcpy_s( arg.szModule, "DLFWTask" );
    CString strPath = m_strLogPath;
    strPath += _T( "\\Trace.log" );
    wcscpy_s( arg.Local.szLogFile, strPath.operator LPCWSTR() );
    return m_pLog->Open( ( LPCVOID )&arg ) ? SP_OK : SP_E_OPEN_FILE;
}

void CTaskHelper::CloseLog()
{
    if ( NULL != m_pLog )
    {
        m_pLog->Close();
        //RenameLogPath();
        CreateThread( NULL, 0, ( LPTHREAD_START_ROUTINE )ThreadFunc, this, 0, NULL );
    }
}

SPRESULT CTaskHelper::CreatTask( UINT nPort )
{
    CHKRESULT( CreatLog() );
    CHKRESULT( OpenLog( nPort ) );
    
    m_pCbkHelper = new CCallbackHelper();
    SPCALLBACK_PARAM cb;
    cb.pFunc = MyCallBack;
    cb.pParam = m_pCbkHelper;
    m_hTask = m_pDrv->DLFW_CreateTask( &cb, m_pLog );
    if ( NULL == m_hTask )
    {
        return SP_E_LOAD_LIBRARY;
    }
    
    return SP_OK;
}

SPRESULT CTaskHelper::RunTask( UINT nPort )
{
    return m_pDrv->DLFW_RunTask( m_hTask, nPort );
}

SPRESULT CTaskHelper::StopTask( )
{
    return m_pDrv->DLFW_StopTask( m_hTask );
}

void CTaskHelper::FreeTask()
{
    m_pDrv->DLFW_FreeTask( m_hTask );
    FreeLog();
    FreeCallback();
}

SPRESULT CTaskHelper::GetProperty( INT nProperty, INT nFlags, LPVOID lpValue )
{
    return m_pDrv->DLFW_TaskGetProperty( m_hTask, nProperty, nFlags, lpValue );
}

SPRESULT CTaskHelper::SetProperty( INT nProperty, INT nFlags, LPVOID lpValue )
{
    return m_pDrv->DLFW_TaskSetProperty( m_hTask, nProperty, nFlags, lpValue );
}