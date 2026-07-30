#include "StdAfx.h"
#include "DownloadMoniter.h"
#include "GlobalHelper.h"

extern CEvnetHelper g_fnEventHelper;
extern CMD_PARAMS g_tCmdParams;
//////////////////////////////////////////////////////////////////////////
CDownloadMoniter::CDownloadMoniter( void )
{
}

CDownloadMoniter::~CDownloadMoniter( void )
{
}

BOOL CDownloadMoniter::StartMoniter( BOOL bStart, DWORD dwTimeout /*= INFINITE*/ )
{
    if ( bStart )
    {
        m_dwStartTime = GetTickCount();
        m_dwTimeout = dwTimeout;
        return CThread::start();
    }
    else
    {
        CThread::join( );
        return TRUE;
    }
}

void CDownloadMoniter::run( void )
{
    while ( true )
    {
        DWORD dwWaitTime = 0;
        DWORD dwEndTime = GetTickCount();
        if ( dwEndTime > m_dwStartTime )
        {
            dwWaitTime = dwEndTime - m_dwStartTime;
        }
        if ( dwWaitTime <= m_dwTimeout )
        {
            const HANDLE arrHandles[] = { g_fnEventHelper.m_hEventDevRemove, g_fnEventHelper.m_hEventDownloadEnd };
            DWORD dwCode = WaitForMultipleObjects( ARRAY_SIZE( arrHandles ), arrHandles, FALSE, 200 );
            if ( WAIT_OBJECT_0 == dwCode || ( WAIT_OBJECT_0 + 1 ) == dwCode )
            {
                break;
            }
        }
        else
        {
            if ( !g_tCmdParams.bEZMode )
            {
                printf( "Download timeout( %ds )\n", m_dwTimeout / 1000 );
                fflush( stdout );
            }
            break;
        }
    }
}
