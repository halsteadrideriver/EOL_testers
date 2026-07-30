#include "StdAfx.h"
#include <iostream>
#include <vector>
#include "FrameworkHelper.h"

using namespace std;

class CPrintf sealed
{
public:
    CPrintf( LPCTSTR lpszFnName, LPCTSTR lpszFmt = NULL, ... )
        : m_sFnName( lpszFnName )
    {
        if ( NULL == lpszFmt )
        {
            _tprintf( _T( "[ ENTER ] %s( )\r\n" ), m_sFnName.c_str() );
        }
        else
        {
            va_list args = NULL;
            va_start( args, lpszFmt );
            size_t nLength = _vsctprintf( lpszFmt, args ) + 1;
            std::vector<TCHAR> vBuff( nLength, _T( '\0' ) );
            if ( _vsntprintf_s( &vBuff[0], vBuff.size(), nLength, lpszFmt, args ) > 0 )
            {
                _tprintf( _T( "[ ENTER ] %s( %s )\r\n" ), m_sFnName.c_str(), ( LPCTSTR )&vBuff[0] );
            }
            va_end( args );
        }
    }
    
    ~CPrintf( void )
    {
    
        _tprintf( _T( "[ LEAVE ] %s( )\r\n" ), m_sFnName.c_str() );
        
    }
    
private:
    string_t m_sFnName;
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

std::wstring CFrameworkHelper::GetAppPath( void )
{
    WCHAR szAppPath[MAX_PATH] = { 0 };
    GetModuleFileNameW( NULL, szAppPath, MAX_PATH );
    LPWSTR lpChar = wcsrchr( szAppPath, L'\\' );
    if ( NULL != lpChar )
    {
        *lpChar = L'\0';
    }
    
    return ( std::wstring )szAppPath + L"\\";
}


SPRESULT CFrameworkHelper::Startup( UINT nLv )
{
    CPrintf print( L"DLFW_Startup" );
    
    string_t strPath = GetAppPath();
    if ( !m_fnDLFWDriver.Startup( strPath ) )
    {
        return SP_E_FAIL;
    }
    
    m_fnDLFWDriver.CreateISpLogObject( &m_pFramLog );
    if ( NULL == m_pFramLog )
    {
        std::cout << "CreateISpLogObject Fail." << endl;
        return SP_E_LOAD_LIBRARY;
    }
    
    OpenArgs_T arg;
    arg.nLogLevel = nLv;
    strcpy_s( arg.szModule, "Frame" );
    wcscpy_s( arg.Local.szLogFile, L"MyFrame.Log" );
    m_pFramLog->Open( ( LPCVOID )&arg );
    
    SPCALLBACK_PARAM cb;
    cb.pFunc = MyCallBack;
    cb.pParam = &m_fnCallHelper;
    
    CHKRESULT( m_fnDLFWDriver.DLFW_Startup( FACTORY_TYPE, &cb, m_pFramLog ) );
    CHKRESULT( m_fnDLFWDriver.DLFW_DeviceMoniter( TRUE ) );
    
    return SP_OK;
}

void CFrameworkHelper::Cleanup()
{
    CPrintf print( L"DLFW_Cleanup" );
    m_fnDLFWDriver.DLFW_DeviceMoniter( FALSE );
    m_fnDLFWDriver.DLFW_Cleanup();
    
    if ( NULL != m_pFramLog )
    {
        m_pFramLog->Close();
        m_pFramLog->Release();
        m_pFramLog = NULL;
    }
    
    m_fnDLFWDriver.Cleanup();
}

SPRESULT CFrameworkHelper::LoadPacket()
{
    //LPCTSTR lpPacket = L"D:\\06.UnisocPackages\\SharkL3(9863A)Ontim\\s9863a1h10_go_32b_2g-user-gms_SUPER_SHARKL3_32b_SIGN.pac";
    //LPCTSTR lpPacket = L"D:\\06.UnisocPackages\\SharkL3(9863A)\\s9863a1h10_1_go_32b_2g-user-gms_SHARKL3_R11.pac";
    LPCTSTR lpPacket = L"D:\\06.UnisocPackages\\SharkL5(UMS312)\\ATT.pac";
    CPrintf print( L"DLFW_LoadPacket", L"%s", lpPacket );
    
    wstring strImageDir = GetAppPath() + L"ImageFiles";
    SPRESULT spRlt = m_fnDLFWDriver.DLFW_LoadPacket( lpPacket, strImageDir.c_str() );
    if ( SP_OK != spRlt )
    {
        std::cout << "DLFW_LoadPacket Fail." << endl;
    }
    
    return SP_OK;
}

SPRESULT CFrameworkHelper::CreatTask( UINT nLv )
{
    CPrintf print( L"DLFW_CreateTask" );
    
    m_fnDLFWDriver.CreateISpLogObject( &m_pTaskLog );
    if ( NULL == m_pTaskLog )
    {
        std::cout << "CreateISpLogObject Fail." << endl;
        return SP_E_LOAD_LIBRARY;
    }
    
    OpenArgs_T arg;
    arg.nLogLevel = nLv;
    strcpy_s( arg.szModule, "Task" );
    wcscpy_s( arg.Local.szLogFile, L"MyTask.Log" );
    m_pTaskLog->Open( ( LPCVOID )&arg );
    
    SPCALLBACK_PARAM cb;
    cb.pFunc = MyCallBack;
    cb.pParam = &m_fnCallHelper;
    m_hTask = m_fnDLFWDriver.DLFW_CreateTask( &cb, m_pTaskLog );
    if ( NULL == m_hTask )
    {
        std::cout << "DLFW_CreateTask Fail." << endl;
        return SP_E_LOAD_LIBRARY;
    }
    return SP_OK;
}

SPRESULT CFrameworkHelper::SyncParameters()
{
    CPrintf print( L"DLFW_SyncParameters" );
    return m_fnDLFWDriver.DLFW_SyncParameters( );
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
    if ( NULL != m_pTaskLog )
    {
        m_pTaskLog->Close();
        m_pTaskLog->Release();
        m_pTaskLog = NULL;
    }
}

SPRESULT CFrameworkHelper::TaskSetProperty( INT nProperty, INT nFlags, LPCVOID lpValue )
{
    CPrintf print( L"DLFW_TaskSetProperty" );
    return m_fnDLFWDriver.DLFW_TaskSetProperty( m_hTask, nProperty, nFlags, lpValue );
}