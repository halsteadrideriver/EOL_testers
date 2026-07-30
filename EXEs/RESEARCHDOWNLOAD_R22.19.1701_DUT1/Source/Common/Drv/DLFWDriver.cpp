#include "stdafx.h"
#include "assert.h"
#include "DLFWDriver.h"

CDLFWDriver::CDLFWDriver( void )
    : m_hDLL( NULL )
    , m_hHandle( NULL )
{
}

CDLFWDriver::~CDLFWDriver( void )
{

}

void CDLFWDriver::Cleanup( void )
{
    if ( NULL != m_hDLL )
    {
        FreeLibrary( m_hDLL );
        m_hDLL = NULL;
        m_drv.Reset();
    }
}

void CDLFWDriver::CopyDependency( string_t& strPath )
{
    string_t strDependency = strPath + _T( "System\\Dependency\\xcopy.bat" );
    ShellExecute( NULL, _T( "open" ), NULL, NULL, strDependency.c_str(), SW_HIDE );
}

#define GET_PROC_ADD(name) m_drv.p##name = ( P##name )GetProcAddress( m_hDLL, #name )

BOOL CDLFWDriver::Startup( string_t& strPath )
{
    string_t strLog = strPath + _T( "App\\iSpLog.dll" );
    m_hLog = LoadLibraryEx( strLog.c_str(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH );
    if ( NULL == m_hLog )
    {
        CopyDependency( strPath );
        return FALSE;
    }
    m_drv.pCreateISpLogObject = ( PCreateISpLogObject )GetProcAddress( m_hLog, "CreateISpLogObject" );
    
    string_t strDLFW = strPath + _T( "App\\DLFramework.dll" );
    m_hDLL = LoadLibraryEx( strDLFW.c_str(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH );
    if ( NULL == m_hDLL )
    {
        CopyDependency( strPath );
        return FALSE;
    }
    
    GET_PROC_ADD( DLFW_Startup );
    GET_PROC_ADD( DLFW_Cleanup );
    
    GET_PROC_ADD( DLFW_ReloadSettings );
    GET_PROC_ADD( DLFW_SyncParameters );
    GET_PROC_ADD( DLFW_LoadPacket );
    GET_PROC_ADD( DLFW_LoadFiles );
    GET_PROC_ADD( DLFW_DoPacket );
    GET_PROC_ADD( DLFW_DeviceMoniter );
    
    GET_PROC_ADD( DLFW_CreateTask );
    GET_PROC_ADD( DLFW_FreeTask );
    GET_PROC_ADD( DLFW_RunTask );
    GET_PROC_ADD( DLFW_StopTask );
    
    GET_PROC_ADD( DLFW_SetProperty );
    GET_PROC_ADD( DLFW_GetProperty );
    
    GET_PROC_ADD( DLFW_TaskSetProperty );
    GET_PROC_ADD( DLFW_TaskGetProperty );
    
    if ( !m_drv.IsValid() )
    {
        Cleanup();
        return FALSE;
    }
    
    return TRUE;
}

SPRESULT CDLFWDriver::DLFW_Startup( DL_TYPE_ENUM eDLType, LPCSPCALLBACK_PARAM lpCallBack, LPVOID pLogUtil )
{
    if ( NULL != m_drv.pDLFW_Startup )
    {
        return m_drv.pDLFW_Startup( eDLType, lpCallBack, pLogUtil );
    }
    else
    {
        assert( 0 );
        return SP_E_POINTER;
    }
}

void CDLFWDriver::DLFW_Cleanup( void )
{
    if ( NULL != m_drv.pDLFW_Cleanup )
    {
        return m_drv.pDLFW_Cleanup();
    }
    else
    {
        assert( 0 );
    }
}

SPRESULT CDLFWDriver::DLFW_ReloadSettings()
{
    if ( NULL != m_drv.pDLFW_ReloadSettings )
    {
        return m_drv.pDLFW_ReloadSettings();
    }
    else
    {
        assert( 0 );
        return SP_E_POINTER;
    }
}

SPRESULT CDLFWDriver::DLFW_SyncParameters()
{
    if ( NULL != m_drv.pDLFW_SyncParameters )
    {
        return m_drv.pDLFW_SyncParameters();
    }
    else
    {
        assert( 0 );
        return SP_E_POINTER;
    }
}

SPRESULT CDLFWDriver::DLFW_LoadPacket( LPCWSTR lpszPacketPath, LPCWSTR lpszImageFiles, uint32 u32Timeout/* = INFINITE*/ )
{
    if ( NULL != m_drv.pDLFW_LoadPacket )
    {
        return m_drv.pDLFW_LoadPacket( lpszPacketPath, lpszImageFiles, u32Timeout );
    }
    else
    {
        assert( 0 );
        return SP_E_POINTER;
    }
}

SPRESULT CDLFWDriver::DLFW_LoadFiles( LPCWSTR lpszImagesDir, uint32 u32Timeout )
{
    if ( NULL != m_drv.pDLFW_LoadFiles )
    {
        return m_drv.pDLFW_LoadFiles( lpszImagesDir, u32Timeout );
    }
    else
    {
        assert( 0 );
        return SP_E_POINTER;
    }
}

SPRESULT CDLFWDriver::DLFW_DoPacket( LPCDLFW_DOPACKET_PARAMS lpParams )
{
    if ( NULL != m_drv.pDLFW_DoPacket )
    {
        return m_drv.pDLFW_DoPacket( lpParams );
    }
    else
    {
        assert( 0 );
        return SP_E_POINTER;
    }
}

SPRESULT CDLFWDriver::DLFW_DeviceMoniter( BOOL bStart/* = TRUE*/ )
{
    if ( NULL != m_drv.pDLFW_DeviceMoniter )
    {
        return m_drv.pDLFW_DeviceMoniter( bStart );
    }
    else
    {
        assert( 0 );
        return SP_E_POINTER;
    }
}

SP_HANDLE CDLFWDriver::DLFW_CreateTask( LPCSPCALLBACK_PARAM lpCallBack, LPVOID pLogUtil, BOOL bUart/* = FALSE*/ )
{
    if ( NULL != m_drv.pDLFW_CreateTask )
    {
        return m_drv.pDLFW_CreateTask( lpCallBack, pLogUtil, bUart );
    }
    else
    {
        assert( 0 );
        return NULL;
    }
}
void CDLFWDriver::DLFW_FreeTask( SP_HANDLE hTask )
{
    if ( NULL != m_drv.pDLFW_FreeTask )
    {
        return m_drv.pDLFW_FreeTask( hTask );
    }
    else
    {
        assert( 0 );
    }
}

SPRESULT CDLFWDriver::DLFW_RunTask( SP_HANDLE hTask, DWORD dwPort )
{
    if ( NULL != m_drv.pDLFW_RunTask )
    {
        return m_drv.pDLFW_RunTask( hTask, dwPort );
    }
    else
    {
        assert( 0 );
        return SP_E_POINTER;
    }
}

SPRESULT CDLFWDriver::DLFW_StopTask( SP_HANDLE hTask )
{
    if ( NULL != m_drv.pDLFW_StopTask )
    {
        return m_drv.pDLFW_StopTask( hTask );
    }
    else
    {
        assert( 0 );
        return SP_E_POINTER;
    }
}

SPRESULT CDLFWDriver::DLFW_SetProperty( INT nProperty, INT nFlags, LPCVOID lpValue )
{
    if ( NULL != m_drv.pDLFW_SetProperty )
    {
        return m_drv.pDLFW_SetProperty( nProperty, nFlags, lpValue );
    }
    else
    {
        assert( 0 );
        return SP_E_POINTER;
    }
}

SPRESULT CDLFWDriver::DLFW_GetProperty( INT nProperty, INT nFlags, LPVOID lpValue )
{
    if ( NULL != m_drv.pDLFW_GetProperty )
    {
        return m_drv.pDLFW_GetProperty( nProperty, nFlags, lpValue );
    }
    else
    {
        assert( 0 );
        return SP_E_POINTER;
    }
}

SPRESULT CDLFWDriver::DLFW_TaskSetProperty( SP_HANDLE hTask, INT nProperty, INT nFlags, LPCVOID lpValue )
{
    if ( NULL != m_drv.pDLFW_TaskSetProperty )
    {
        return m_drv.pDLFW_TaskSetProperty( hTask, nProperty, nFlags, lpValue );
    }
    else
    {
        assert( 0 );
        return SP_E_POINTER;
    }
}

SPRESULT CDLFWDriver::DLFW_TaskGetProperty( SP_HANDLE hTask, INT nProperty, INT nFlags, LPVOID lpValue )
{
    if ( NULL != m_drv.pDLFW_TaskGetProperty )
    {
        return m_drv.pDLFW_TaskGetProperty( hTask, nProperty, nFlags, lpValue );
    }
    else
    {
        assert( 0 );
        return SP_E_POINTER;
    }
}

BOOL CDLFWDriver::CreateISpLogObject( ISpLog** lppObject )
{
    if ( NULL != m_drv.pCreateISpLogObject )
    {
        return m_drv.pCreateISpLogObject( lppObject );
    }
    else
    {
        assert( 0 );
        return FALSE;
    }
}