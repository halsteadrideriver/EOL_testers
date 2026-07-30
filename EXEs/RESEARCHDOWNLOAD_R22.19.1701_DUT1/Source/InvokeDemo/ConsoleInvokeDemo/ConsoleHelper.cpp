#include "StdAfx.h"
#include <iostream>
#include "ConsoleHelper.h"

using namespace std;
//////////////////////////////////////////////////////////////////////////
CConsoleHelper::CConsoleHelper( void )
{
    InitConsole();
}

CConsoleHelper::~CConsoleHelper( void )
{
    ReleaseConsole();
}

void CConsoleHelper::InitConsole()
{
    m_hConsole = GetStdHandle( STD_OUTPUT_HANDLE );
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