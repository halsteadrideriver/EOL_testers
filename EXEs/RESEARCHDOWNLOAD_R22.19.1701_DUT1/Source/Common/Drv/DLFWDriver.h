#pragma once
#include "DLFWDrvDef.h"

class CDLFWDriver sealed
{
public:
    CDLFWDriver( void );
    ~CDLFWDriver( void );
    
    BOOL Startup( string_t& strPath );
    void Cleanup( void );
    
    SPRESULT DLFW_Startup( DL_TYPE_ENUM eDLType, LPCSPCALLBACK_PARAM lpCallBack, LPVOID pLogUtil );
    void     DLFW_Cleanup( void );
    
    SPRESULT DLFW_ReloadSettings();
    SPRESULT DLFW_SyncParameters();
    SPRESULT DLFW_LoadPacket( LPCWSTR lpszPacketPath, LPCWSTR lpszImageFiles, uint32 u32Timeout = INFINITE );
    SPRESULT DLFW_LoadFiles( LPCWSTR lpszImagesDir, uint32 u32Timeout = INFINITE );
    SPRESULT DLFW_DoPacket( LPCDLFW_DOPACKET_PARAMS lpParams );
    SPRESULT DLFW_DeviceMoniter( BOOL bStart = TRUE );
    
    SP_HANDLE DLFW_CreateTask( LPCSPCALLBACK_PARAM lpCallBack, LPVOID pLogUtil, BOOL bUart = FALSE );
    void     DLFW_FreeTask( SP_HANDLE hTask );
    SPRESULT DLFW_RunTask( SP_HANDLE hTask, DWORD dwPort );
    SPRESULT DLFW_StopTask( SP_HANDLE hTask );
    SPRESULT DLFW_SetProperty( INT nProperty, INT nFlags, LPCVOID lpValue );
    SPRESULT DLFW_GetProperty( INT nProperty, INT nFlags, LPVOID lpValue );
    SPRESULT DLFW_TaskSetProperty( SP_HANDLE hTask, INT nProperty, INT nFlags, LPCVOID lpValue );
    SPRESULT DLFW_TaskGetProperty( SP_HANDLE hTask, INT nProperty, INT nFlags, LPVOID lpValue );
    
    BOOL CreateISpLogObject( ISpLog** lppObject );
    
    void CopyDependency( string_t& strPath );
private:
    HMODULE m_hDLL;
    HMODULE m_hLog;
    SPHANDLE m_hHandle;
    DLFW_DRIVER_T m_drv;
};

