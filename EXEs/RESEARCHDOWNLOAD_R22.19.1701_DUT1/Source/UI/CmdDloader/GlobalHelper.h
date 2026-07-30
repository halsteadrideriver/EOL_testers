#pragma once
//////////////////////////////////////////////////////////////////////////
class CConsoleHelper
{
public:
    BOOL InitConsole();
    void ReleaseConsole();

    void SetConsole();
    LPCTSTR GetConsoleOutput();
    
private:
    HANDLE m_hConsole = NULL;
    TCHAR  m_szBuff[1024] = { 0 };
};

class CEvnetHelper
{
public:
    BOOL CreateAllEvent();
    void ResetAllEvent();
    void CloseAllEvent();
    
public:
    HANDLE m_hEventDevInsert = NULL;
    HANDLE m_hEventDevRemove = NULL;
    HANDLE m_hEventDownloadEnd = NULL;
};