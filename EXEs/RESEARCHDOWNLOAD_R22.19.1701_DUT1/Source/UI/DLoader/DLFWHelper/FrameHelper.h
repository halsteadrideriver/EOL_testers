#pragma once
#include "IConfig.h"
#include "HelperBase.h"

class CFrameHelper : public CHelperBase
{
public:
    CFrameHelper( CDLFWDriver* pDrv );
    virtual ~CFrameHelper( void );
    
public:
    SPRESULT Startup();
    void     Cleanup();
    
    SPRESULT LoadPacket( LPCTSTR lpPacket, LPCTSTR lpImageDir );
    SPRESULT LoadFiles( LPCTSTR lpImageDir );
    SPRESULT DoPacket( LPCDLFW_DOPACKET_PARAMS lpParams );
    SPRESULT GetProperty( INT nProperty, INT nFlags, LPVOID lpValue );
    SPRESULT SetProperty( INT nProperty, INT nFlags, LPVOID lpValue );
    SPRESULT DeviceMoniter( BOOL bStart );
    SPRESULT ReloadSettings();
    SPRESULT SyncParameters();
    
    SPRESULT OpenLog();
    void     CloseLog();
    
private:
    SPRESULT CreatLog();
    void FreeLog();
    void InitLogPath();
};

