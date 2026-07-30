#include "stdafx.h"
#include "DLFWDriver.h""
#include "TaskHelper.h"

//////////////////////////////////////////////////////////////////////////
CTaskHelper::CTaskHelper( CDLFWDriver* pDLFWDriver )
    : m_pDLFWDriver( pDLFWDriver )
{
    assert( pDLFWDriver );
}

CTaskHelper::~CTaskHelper( void )
{
}

void CTaskHelper::FreeLog()
{
    if ( NULL != m_pLogTask )
    {
        m_pLogTask->Close();
        m_pLogTask->Release();
        m_pLogTask = NULL;
    }
}

void CTaskHelper::FreeCallback()
{
    if ( NULL != m_pCbkTask )
    {
        delete m_pCbkTask;
        m_pCbkTask = NULL;
    }
}

SPRESULT CTaskHelper::OpenLog( int nPort, UINT nLv )
{
    OpenArgs_T arg;
    arg.nLogLevel = nLv;
    strcpy_s( arg.szModule, "Task" );
    TCHAR szLogName[50] = { 0 };
    _stprintf_s( szLogName, L"MyTask_%d.Log", nPort );
    wcscpy_s( arg.Local.szLogFile, szLogName );
    return m_pLogTask->Open( ( LPCVOID )&arg ) ? SP_OK : SP_E_OPEN_FILE;
}

SPRESULT CTaskHelper::CreatTask( int nPort, UINT nLv )
{
    m_pDLFWDriver->CreateISpLogObject( &m_pLogTask );
    if ( NULL == m_pLogTask )
    {
        return SP_E_LOAD_LIBRARY;
    }
    
    CHKRESULT( OpenLog( nPort, nLv ) );
    
    m_pCbkTask = new CCallbackHelper();
    SPCALLBACK_PARAM cb;
    cb.pFunc = MyCallBack;
    cb.pParam = m_pCbkTask;
    m_hTask = m_pDLFWDriver->DLFW_CreateTask( &cb, m_pLogTask );
    if ( NULL == m_hTask )
    {
        return SP_E_LOAD_LIBRARY;
    }
    
    return SP_OK;
}

SPRESULT CTaskHelper::RunTask( int nPort )
{
    return m_pDLFWDriver->DLFW_RunTask( m_hTask, nPort );
}

SPRESULT CTaskHelper::StopTask( )
{
    SPRESULT spRlt = m_pDLFWDriver->DLFW_StopTask( m_hTask );
    m_pLogTask->Close();
    return spRlt;
}

void CTaskHelper::FreeTask()
{
    m_pDLFWDriver->DLFW_FreeTask( m_hTask );
    FreeCallback();
    FreeLog();
}