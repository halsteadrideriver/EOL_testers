#include "StdAfx.h"
#include "DevMoniter.h"
#include "GlobalHelper.h"

extern CConsoleHelper g_fnConsoleHelper;
extern CEvnetHelper g_fnEventHelper;
extern CMD_PARAMS g_tCmdParams;
//////////////////////////////////////////////////////////////////////////
BOOL CDevMoniter::StartMoniter( BOOL bStart, DWORD dwTimeout /*= INFINITE*/ )
{
    if ( bStart )
    {
        m_bOK = FALSE;
        m_dwCurTime = 0;
        m_dwTimeout = dwTimeout;
        m_dwStartTime = GetTickCount();
        return CThread::start();
    }
    else
    {
        CThread::join( );
        return TRUE;
    }
}

void CDevMoniter::run( void )
{
    while ( TRUE )
    {
        DWORD dwWaitTime = 0;
        DWORD dwEndTime = GetTickCount();
        if ( dwEndTime > m_dwStartTime )
        {
            dwWaitTime = dwEndTime - m_dwStartTime;
        }
        DWORD dwRestTime = ( DWORD )( ( m_dwTimeout - dwWaitTime ) / 1000 );
        if ( dwWaitTime <= m_dwTimeout && ( 0 == m_dwCurTime || dwRestTime != m_dwCurTime ) )
        {
            DWORD dwCode = WaitForSingleObject( g_fnEventHelper.m_hEventDevInsert, 5 );
            if ( WAIT_OBJECT_0 == dwCode )
            {
                m_bOK = TRUE;
                break;
            }
            m_dwCurTime = dwRestTime;
            if ( !g_tCmdParams.bEZMode )
            {
                wstring strBuff = g_fnConsoleHelper.GetConsoleOutput();
                if ( strBuff.size() > 0 && wstring::npos != strBuff.find( L"Please connect the DUT in" ) )
                {
                    g_fnConsoleHelper.SetConsole();
                }
                printf( "Please connect the DUT in %3ds\n", dwRestTime );
                fflush( stdout );
            }
        }
        else if ( dwWaitTime > m_dwTimeout )
        {
            m_bOK = FALSE;
            if ( !g_tCmdParams.bEZMode )
            {
                printf( "Connect DUT timeout( %ds )\n", m_dwTimeout / 1000 );
                fflush( stdout );
            }
            break;
        }
    }
}

BOOL CDevMoniter::GetRunResult()
{
    return m_bOK;
}