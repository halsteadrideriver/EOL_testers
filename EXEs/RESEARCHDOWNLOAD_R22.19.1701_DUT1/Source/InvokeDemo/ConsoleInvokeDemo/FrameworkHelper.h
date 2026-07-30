#pragma once
#include "Tr.h"
#include "DLFWDriver.h"
#include "CallbackHelper.h"

//////////////////////////////////////////////////////////////////////////
class CFrameworkHelper
{
public:
    CFrameworkHelper( void );
    virtual ~CFrameworkHelper( void );
    
public:
    std::wstring GetAppPath( void );
    
    SPRESULT Startup( UINT nLv );
    void Cleanup();
    
    SPRESULT LoadPacket();
    SPRESULT SyncParameters();
    SPRESULT ReloadSettings();
    SPRESULT GetProperty( INT nProperty, INT nFlags, LPVOID lpValue );
    SPRESULT SetProperty( INT nProperty, INT nFlags, LPVOID lpValue );
    
    SPRESULT CreatTask( UINT nLv );
    SPRESULT RunTask();
    SPRESULT StopTask();
    void FreeTask();
    SPRESULT TaskSetProperty( INT nProperty, INT nFlags, LPCVOID lpValue );
    
private:
    ISpLog* m_pTaskLog = NULL;
    ISpLog* m_pFramLog = NULL;
    SP_HANDLE m_hTask = NULL;
    CCallbackHelper m_fnCallHelper;
    CDLFWDriver m_fnDLFWDriver;
};