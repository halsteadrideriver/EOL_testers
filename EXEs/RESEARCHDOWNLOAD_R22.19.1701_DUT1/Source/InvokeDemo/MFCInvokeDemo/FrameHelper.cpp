#include "stdafx.h"
#include "DLFWDriver.h"
#include "FrameHelper.h"

//////////////////////////////////////////////////////////////////////////
CFrameHelper::CFrameHelper( CDLFWDriver* pDLFWDriver )
    : m_pDLFWDriver( pDLFWDriver )
{
    assert( pDLFWDriver );
}

CFrameHelper::~CFrameHelper( void )
{
}

SPRESULT CFrameHelper::Startup( UINT nLv )
{
    m_pDLFWDriver->CreateISpLogObject( &m_pLogFrame );
    if ( NULL == m_pLogFrame )
    {
        return SP_E_LOAD_LIBRARY;
    }
    
    OpenArgs_T arg;
    arg.nLogLevel = nLv;
    strcpy_s( arg.szModule, "Framework" );
    wcscpy_s( arg.Local.szLogFile, L"MyFrame.Log" );
    m_pLogFrame->Open( ( LPCVOID )&arg );
    
    m_pCbkFrame = new CCallbackHelper();
    SPCALLBACK_PARAM cb;
    cb.pFunc = MyCallBack;
    cb.pParam = m_pCbkFrame;
    CHKRESULT( m_pDLFWDriver->DLFW_Startup( FACTORY_TYPE, &cb, m_pLogFrame ) );
    
    return SP_OK;
}

void CFrameHelper::Cleanup()
{
    m_pDLFWDriver->DLFW_Cleanup();
    if ( NULL != m_pCbkFrame )
    {
        delete m_pCbkFrame;
        m_pCbkFrame = NULL;
    }
    if ( NULL != m_pLogFrame )
    {
        m_pLogFrame->Close();
        m_pLogFrame->Release();
        m_pLogFrame = NULL;
    }
}

SPRESULT CFrameHelper::LoadPacket( LPCTSTR lpPacket, LPCTSTR lpImageDir )
{
    return m_pDLFWDriver->DLFW_LoadPacket( lpPacket, lpImageDir );
}

SPRESULT CFrameHelper::DeviceMoniter( BOOL bStart )
{
    return m_pDLFWDriver->DLFW_DeviceMoniter( bStart );
}