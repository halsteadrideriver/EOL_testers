#pragma once
//////////////////////////////////////////////////////////////////////////
class CConsoleHelper
{
    // TODO: To simply the invoke flow, here only support single thread.
    // Framework can definitely support multi-threads.
    //
public:
    CConsoleHelper( void );
    virtual ~CConsoleHelper( void );

public:
    void InitConsole();
    void ReleaseConsole();
    void SetConsole();
    LPCTSTR GetConsoleOutput();
private:
    HANDLE m_hConsole = NULL;
    TCHAR  m_szBuff[1024] = { 0 };
};
