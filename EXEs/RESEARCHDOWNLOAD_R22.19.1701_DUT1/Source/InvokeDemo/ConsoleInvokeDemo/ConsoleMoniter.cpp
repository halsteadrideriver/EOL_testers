#include "StdAfx.h"
#include "ConsoleMoniter.h"
#include "ConsoleHelper.h"
extern CConsoleHelper g_fnConsole;
//////////////////////////////////////////////////////////////////////////
CConsoleMoniter::CConsoleMoniter( void )
{
    m_vKey.clear();
}

CConsoleMoniter::~CConsoleMoniter( void )
{
    m_vKey.clear();
}

BOOL CConsoleMoniter::StartMoniter( BOOL bStart, DWORD dwTimeout /*= INFINITE*/ )
{
    if ( bStart )
    {
        return CThread::start();
    }
    else
    {
        CThread::join( dwTimeout );
        return TRUE;
    }
}

void CConsoleMoniter::run( void )
{
    while ( !GetRunResult() )
    {
        Sleep( 2 );
    }
}

void CConsoleMoniter::operator()( LPCTSTR lpszkey )
{
    m_vKey.push_back( lpszkey );
}

BOOL CConsoleMoniter::GetRunResult()
{
    BOOL bOK = TRUE;
    wstring strBuff = g_fnConsole.GetConsoleOutput();
    if ( strBuff.size() > 0 )
    {
        for ( size_t i = 0; i < m_vKey.size(); i++ )
        {
            if ( wstring::npos != strBuff.find( m_vKey[i] ) )
            {
                return TRUE;
            }
        }
    }
    return FALSE;
}
