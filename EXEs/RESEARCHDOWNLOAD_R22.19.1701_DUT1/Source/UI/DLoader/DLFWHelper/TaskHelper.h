#pragma once
#include "HelperBase.h"

//////////////////////////////////////////////////////////////////////////
class CTaskHelper: public CHelperBase
{
public:
    CTaskHelper( CDLFWDriver* pDrv );
    virtual ~CTaskHelper( void );
    
public:
    SPRESULT CreatTask( UINT nPort );
    SPRESULT RunTask( UINT nPort );
    SPRESULT StopTask( );
    void     FreeTask( );
    
    SPRESULT OpenLog( UINT nPort );
    void     CloseLog( );
    void     CopyLog( LPCTSTR lpszLogPath );
    
    SPRESULT GetProperty( INT nProperty, INT nFlags, LPVOID lpValue );
    SPRESULT SetProperty( INT nProperty, INT nFlags, LPVOID lpValue );
    
    LPCTSTR GetLogPath( ) { return m_strLogPath.operator LPCWSTR(); };
    
    static DWORD WINAPI ThreadFunc( LPVOID lpParam );
    DWORD ThreadFunc();
    
private:
    SPRESULT CreatLog();
    void FreeLog();
    void FreeCallback();
    void InitLogPath( UINT nPort );
    BOOL RenameLogPath();
    void CompressedLog();
    void RunCmdline( LPCTSTR lpCmd );
    
private:
    SP_HANDLE m_hTask = NULL;
    BOOL m_bRename;
};
