#pragma once
//////////////////////////////////////////////////////////////////////////
class CCmdHelper
{
public:
    BOOL GetAllCmdParams( LPTSTR* lppCmd, INT nCount );
    void ShowUsage();
    
private:
    inline BOOL GetOneCmdParams( INT& nCmd, LPCTSTR lpCmd, LPTSTR* lppCmd, INT nCount, LPTSTR& lpParam );
    inline BOOL GetOneCmdParams( INT nCmd, LPCTSTR lpCmd, LPTSTR* lppCmd );
    inline BOOL CheckPac();
    inline BOOL CheckPwrOffAndReset();
    inline BOOL CheckSendFlag();
    inline BOOL CheckVersion();
    inline BOOL CheckMutiProject();
    inline BOOL CheckLoadPolicy();
};
