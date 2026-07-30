#include "StdAfx.h"
#include "FrameHelper.h"
#include "DLoader.h"
#include "OperationHelper.h"

//////////////////////////////////////////////////////////////////////////
CFrameHelper::CFrameHelper( CDLFWDriver* pDrv )
    : CHelperBase( pDrv )
{
}

CFrameHelper::~CFrameHelper( void )
{
}

void CFrameHelper::InitLogPath()
{
    m_strLogPath = m_strCfgLogPath;
    SYSTEMTIME time;
    GetLocalTime( &time );
    m_strLogPath.AppendFormat( _T( "\\%04d_%02d_%02d" ), time.wYear, time.wMonth, time.wDay );
    COperationHelper op;
    op.CreateDeepDirectory( m_strLogPath.operator LPCTSTR() );
    
}

SPRESULT CFrameHelper::CreatLog()
{
    m_pDrv->CreateISpLogObject( &m_pLog );
    if ( NULL == m_pLog )
    {
        return SP_E_LOAD_LIBRARY;
    }
    return SP_OK;
}

SPRESULT CFrameHelper::OpenLog()
{
    if ( NULL != m_pLog && !m_bLogOpen )
    {
        OpenArgs_T arg;
        arg.nLogLevel = m_nLogLevel;
        strcpy_s( arg.szModule, "DLFramework" );
        if ( !PathFileExists( m_strLogPath.operator LPCWSTR() ) )
        {
            InitLogPath();
            SYSTEMTIME time;
            GetLocalTime( &time );
            swprintf_s( arg.Local.szLogFile, L"%s\\DLFramework_%04d_%02d_%02d-%02d_%02d_%02d_%03d.log",
                        m_strLogPath.operator LPCWSTR(),
                        time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds );
            m_strLogPath = arg.Local.szLogFile;
            arg.Local.nOpenFlags = splog::defaultFlags;
        }
        else
        {
            arg.Local.nOpenFlags = splog::addDateFlags;
            _tcscpy_s( arg.Local.szLogFile, m_strLogPath.operator LPCWSTR() );
        }
        m_bLogOpen = m_pLog->Open( ( LPCVOID )&arg );
        return  m_bLogOpen ? SP_OK : SP_E_OPEN_FILE;
    }
    return SP_OK;
}

void CFrameHelper::CloseLog()
{
    if ( NULL != m_pLog && m_bLogOpen )
    {
        m_pLog->Close();
        m_bLogOpen = FALSE;
    }
}

void CFrameHelper::FreeLog()
{
    if ( NULL != m_pLog )
    {
        m_pLog->Close();
        m_pLog->Release();
        m_pLog = NULL;
    }
}

SPRESULT CFrameHelper::Startup( )
{
    CHKRESULT( CreatLog() );
    CHKRESULT( OpenLog() );
    m_pCbkHelper = new CCallbackHelper();
    SPCALLBACK_PARAM cb;
    cb.pFunc = MyCallBack;
    cb.pParam = m_pCbkHelper;
    
#ifdef _SPUPGRADE
    CHKRESULT( m_pDrv->DLFW_Startup( UPGRADE_TYPE, &cb, m_pLog ) );
#elif defined _FACTORY
    CHKRESULT( m_pDrv->DLFW_Startup( FACTORY_TYPE, &cb, m_pLog ) );
#else
    CHKRESULT( m_pDrv->DLFW_Startup( RESEARCH_TYPE, &cb, m_pLog ) );
#endif
    
    return SP_OK;
}

SPRESULT CFrameHelper::DeviceMoniter( BOOL bStart )
{
    return m_pDrv->DLFW_DeviceMoniter( bStart );
}

SPRESULT CFrameHelper::GetProperty( INT nProperty, INT nFlags, LPVOID lpValue )
{
    return m_pDrv->DLFW_GetProperty( nProperty, nFlags, lpValue );
}

SPRESULT CFrameHelper::SetProperty( INT nProperty, INT nFlags, LPVOID lpValue )
{
    return m_pDrv->DLFW_SetProperty( nProperty, nFlags, lpValue );
}

void CFrameHelper::Cleanup()
{
    m_pDrv->DLFW_Cleanup();
    if ( NULL != m_pCbkHelper )
    {
        delete m_pCbkHelper;
        m_pCbkHelper = NULL;
    }
    FreeLog();
}

SPRESULT CFrameHelper::LoadPacket( LPCTSTR lpPacket, LPCTSTR lpImageDir )
{
    return m_pDrv->DLFW_LoadPacket( lpPacket, lpImageDir );
}

SPRESULT CFrameHelper::LoadFiles( LPCTSTR lpImageDir )
{
    return m_pDrv->DLFW_LoadFiles( lpImageDir );
}

SPRESULT CFrameHelper::DoPacket( LPCDLFW_DOPACKET_PARAMS lpParams )
{
    return m_pDrv->DLFW_DoPacket( lpParams );
}

SPRESULT CFrameHelper::ReloadSettings()
{
    return m_pDrv->DLFW_ReloadSettings();
}

SPRESULT CFrameHelper::SyncParameters()
{
    return m_pDrv->DLFW_SyncParameters();
}