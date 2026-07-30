#pragma once
#include <iostream>
#include <vector>
#include "Tr.h"
#include "CallbackHelper.h"
#include "DLFrameworkExport.h"
#include "DLFWDriver.h"
#include "DevMoniter.h"
#include "GlobalHelper.h"
#include "IConfig.h"
#include "CmdHelper.h"

//////////////////////////////////////////////////////////////////////////
class CFrameworkHelper
{
    // TODO: To simply the invoke flow, here only support single thread.
    // Framework can definitely support multi-threads.
    //
public:
    CFrameworkHelper( void );
    virtual ~CFrameworkHelper( void );
    
    SPRESULT Startup( DL_TYPE_ENUM eDLType, UINT nLv );
    SPRESULT DeviceMoniter( BOOL bStart );
    void Cleanup();
    
    SPRESULT LoadPacket( LPCTSTR lpPacket, LPCTSTR lpImageDir );
    SPRESULT SyncParameters();
    SPRESULT ReloadSettings();
    SPRESULT GetProperty( INT nProperty, INT nFlags, LPVOID lpValue );
    SPRESULT SetProperty( INT nProperty, INT nFlags, LPVOID lpValue );
    
    SPRESULT CreatTask( UINT nLv );
    SPRESULT RunTask();
    SPRESULT StopTask();
    SPRESULT TaskSetProperty( INT nProperty, INT nFlags, LPCVOID lpValue );
    void     FreeTask();
    
    void     LoadCfgDLFramework();
    
    INT GetPort()
    {
        return m_fnCallHelper.m_nPort;
    }
    
    LPCALLBACKDATA_DL_END GetDlEndData()
    {
        return &( m_fnCallHelper.m_tDlEnd );
    }
    
private:
    void     InitLogPath();
    void     CompressedLog();
    BOOL     RenameLogPath();
    
    SPRESULT CreatFramLog();
    SPRESULT OpenFramLog();
    void     PrintFramLog();
    void     CloseFramLog();
    void     FreeFramLog();
    
    SPRESULT CreatTaskLog();
    SPRESULT OpenTaskLog();
    void     PrintTaskLog();
    void     CloseTaskLog();
    void     FreeTaskLog();
    
    BOOL     CreateDeepDirectory( string_t strDir );
    void     RunCmdline( LPCTSTR lpCmd );
    
private:
    ISpLog* m_pTaskLog = NULL;
    ISpLog* m_pFramLog = NULL;
    SP_HANDLE m_hTask = NULL;
    CCallbackHelper m_fnCallHelper;
    CDLFWDriver m_fnDLFWDriver;
    INT m_nLogLevel = 0;
    string_t m_strCfgLogPath = _T( "" );
    string_t m_strLogPath = _T( "" );
    string_t m_strAppPath = _T( "" );
    DL_TYPE_ENUM m_eDLType = FACTORY_TYPE;
};
