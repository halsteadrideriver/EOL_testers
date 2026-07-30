#include "StdAfx.h"
#include "DevMoniter.h"
#include "ConsoleHelper.h"
extern CConsoleHelper g_fnConsole;
//////////////////////////////////////////////////////////////////////////
CDevMoniter::CDevMoniter( void )
{
}

CDevMoniter::~CDevMoniter( void )
{

}

BOOL CDevMoniter::StartMoniter( BOOL bStart, DWORD dwTimeout /*= INFINITE*/ )
{
    if ( bStart )
    {
        m_bOK = FALSE;
        m_u32Start = GetTickCount();
        return CThread::start();
    }
    else
    {
        m_u32Timeout = dwTimeout;
        CThread::join( dwTimeout );
        return TRUE;
    }
}

void CDevMoniter::run( void )
{
    while ( TRUE )
    {
        wstring strBuff = g_fnConsole.GetConsoleOutput();
        if ( strBuff.size() > 0 && wstring::npos != strBuff.find( L"Please connect the DUT in" ) )
        {
            g_fnConsole.SetConsole();
        }
        else
        {
            if ( strBuff.size() > 0 && wstring::npos != strBuff.find( KEY_START_DL ) )
            {
                m_bOK = TRUE;
                break;
            }
        }
        uint32 u32Time = ( uint32 )( m_u32Timeout - ( GetTickCount() - m_u32Start ) ) / 1000;
        printf( "Please connect the DUT in %3ds\r\n", u32Time );
        Sleep( 100 );
    }
}

BOOL CDevMoniter::GetRunResult()
{
    return m_bOK;
}