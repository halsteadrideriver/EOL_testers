#include "StdAfx.h"
#include "GlobalHelper.h"

//////////////////////////////////////////////////////////////////////////
BOOL CConsoleHelper::InitConsole()
{
    m_hConsole = GetStdHandle( STD_OUTPUT_HANDLE );
    if ( NULL == m_hConsole )
    {
        printf( "GetStdHandle Fail.\n" );
        fflush( stdout );
        return FALSE;
    }
    return TRUE;
}

void CConsoleHelper::ReleaseConsole()
{
    if ( NULL != m_hConsole )
    {
        CloseHandle( m_hConsole );
        m_hConsole = NULL;
    }
}

void CConsoleHelper::SetConsole()
{
    CONSOLE_SCREEN_BUFFER_INFO  console_info;
    GetConsoleScreenBufferInfo( m_hConsole, &console_info );
    COORD coordScreen = { 0, 0 };
    coordScreen.Y = console_info.dwCursorPosition.Y - 1;
    SetConsoleCursorPosition( m_hConsole, coordScreen );
}

LPCTSTR CConsoleHelper::GetConsoleOutput()
{
    CONSOLE_SCREEN_BUFFER_INFO  console_info;
    GetConsoleScreenBufferInfo( m_hConsole, &console_info );
    COORD coordScreen = { 0, 0 };
    coordScreen.Y = console_info.dwCursorPosition.Y - 1;
    
    DWORD dwLength = 0;
    ReadConsoleOutputCharacter( m_hConsole, m_szBuff, _countof( m_szBuff ), coordScreen, &dwLength );
    return m_szBuff;
}

BOOL CEvnetHelper::CreateAllEvent()
{
    m_hEventDevInsert = CreateEvent( NULL, TRUE, FALSE, NULL );
    if ( NULL == m_hEventDevInsert )
    {
        printf( "Create Device Insert Event Fail.\n" );
        fflush( stdout );
        return FALSE;
    }
    ResetEvent( m_hEventDevInsert );
    m_hEventDevRemove = CreateEvent( NULL, TRUE, FALSE, NULL );
    if ( NULL == m_hEventDevRemove )
    {
        printf( "Create Device Remove Event Fail.\n" );
        fflush( stdout );
        return FALSE;
    }
    ResetEvent( m_hEventDevRemove );
    m_hEventDownloadEnd = CreateEvent( NULL, TRUE, FALSE, NULL );
    if ( NULL == m_hEventDownloadEnd )
    {
        printf( "Create Download End Event Fail.\n" );
        fflush( stdout );
        return FALSE;
    }
    ResetEvent( m_hEventDownloadEnd );
    return TRUE;
}

void CEvnetHelper::ResetAllEvent()
{
    ResetEvent( m_hEventDevInsert );
    ResetEvent( m_hEventDevRemove );
    ResetEvent( m_hEventDownloadEnd );
}

void CEvnetHelper::CloseAllEvent()
{
    CloseHandle( m_hEventDevInsert );
    m_hEventDevInsert = NULL;
    CloseHandle( m_hEventDevRemove );
    m_hEventDevRemove = NULL;
    CloseHandle( m_hEventDownloadEnd );
    m_hEventDownloadEnd = NULL;
}