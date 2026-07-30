#pragma once
#include "ISpLogExport.h"
#include "DLFrameworkExport.h"

typedef SPRESULT( SP_API* PDLFW_Startup )( DL_TYPE_ENUM eDLType, LPCSPCALLBACK_PARAM lpCallBack, LPVOID pLogUtil );
typedef void ( SP_API* PDLFW_Cleanup )( void );

typedef SPRESULT( SP_API* PDLFW_ReloadSettings )( void );
typedef SPRESULT( SP_API* PDLFW_SyncParameters )( void );
typedef SPRESULT( SP_API* PDLFW_LoadPacket )( LPCWSTR lpszPacketPath, LPCWSTR lpszImageFiles, uint32 u32Timeout );
typedef SPRESULT( SP_API* PDLFW_LoadFiles )( LPCWSTR lpszImagesDir, uint32 u32Timeout );
typedef SPRESULT( SP_API* PDLFW_DoPacket )( LPCDLFW_DOPACKET_PARAMS lpParams );
typedef SPRESULT( SP_API* PDLFW_DeviceMoniter )( BOOL bStart );

typedef SP_HANDLE( SP_API* PDLFW_CreateTask )( LPCSPCALLBACK_PARAM lpCallBack, LPVOID pLogUtil, BOOL bUart );
typedef void( SP_API* PDLFW_FreeTask )( SP_HANDLE hTask );
typedef SPRESULT( SP_API* PDLFW_RunTask )( SP_HANDLE hTask, DWORD dwPort );
typedef SPRESULT( SP_API* PDLFW_StopTask )( SP_HANDLE hTask );

typedef SPRESULT( SP_API* PDLFW_SetProperty )( INT nProperty, INT nFlags, LPCVOID lpValue );
typedef SPRESULT( SP_API* PDLFW_GetProperty )( INT nProperty, INT nFlags, LPVOID lpValue );
typedef SPRESULT( SP_API* PDLFW_TaskSetProperty )( SP_HANDLE hTask, INT nProperty, INT nFlags, LPCVOID lpValue );
typedef SPRESULT( SP_API* PDLFW_TaskGetProperty )( SP_HANDLE hTask, INT nProperty, INT nFlags, LPVOID lpValue );

typedef BOOL( SP_API* PCreateISpLogObject )( ISpLog** lppObject );

typedef struct _tagDLFW_DRIVER_T
{
    PDLFW_Startup pDLFW_Startup;
    PDLFW_Cleanup pDLFW_Cleanup;
    
    PDLFW_ReloadSettings pDLFW_ReloadSettings;
    PDLFW_SyncParameters pDLFW_SyncParameters;
    PDLFW_LoadPacket pDLFW_LoadPacket;
    PDLFW_DoPacket pDLFW_DoPacket;
    PDLFW_LoadFiles pDLFW_LoadFiles;
    PDLFW_DeviceMoniter pDLFW_DeviceMoniter;
    
    PDLFW_CreateTask pDLFW_CreateTask;
    PDLFW_FreeTask pDLFW_FreeTask;
    PDLFW_RunTask pDLFW_RunTask;
    PDLFW_StopTask pDLFW_StopTask;
    
    PDLFW_SetProperty pDLFW_SetProperty;
    PDLFW_GetProperty pDLFW_GetProperty;
    
    PDLFW_TaskSetProperty pDLFW_TaskSetProperty;
    PDLFW_TaskGetProperty pDLFW_TaskGetProperty;
    
    PCreateISpLogObject pCreateISpLogObject;
    
    _tagDLFW_DRIVER_T( void )
    {
        Reset();
    };
    
    void Reset( void )
    {
        pDLFW_Startup = NULL;
        pDLFW_Cleanup = NULL;
        
        pDLFW_ReloadSettings = NULL;
        pDLFW_SyncParameters = NULL;
        pDLFW_LoadPacket = NULL;
        pDLFW_LoadFiles = NULL;
        pDLFW_DoPacket = NULL;
        pDLFW_DeviceMoniter = NULL;
        
        pDLFW_CreateTask = NULL;
        pDLFW_FreeTask = NULL;
        pDLFW_RunTask = NULL;
        pDLFW_StopTask = NULL;
        
        pDLFW_SetProperty = NULL;
        pDLFW_GetProperty = NULL;
        
        pDLFW_TaskSetProperty = NULL;
        pDLFW_TaskGetProperty = NULL;
        pCreateISpLogObject = NULL;
    };
    
    BOOL IsValid( void )
    {
        return ( ( NULL != pDLFW_Startup ) &&
                 ( NULL != pDLFW_Cleanup ) &&
                 ( NULL != pDLFW_ReloadSettings ) &&
                 ( NULL != pDLFW_SyncParameters ) &&
                 ( NULL != pDLFW_LoadPacket ) &&
                 ( NULL != pDLFW_LoadFiles ) &&
                 ( NULL != pDLFW_DoPacket ) &&
                 ( NULL != pDLFW_DeviceMoniter ) &&
                 ( NULL != pDLFW_CreateTask ) &&
                 ( NULL != pDLFW_FreeTask ) &&
                 ( NULL != pDLFW_RunTask ) &&
                 ( NULL != pDLFW_StopTask ) &&
                 ( NULL != pDLFW_SetProperty ) &&
                 ( NULL != pDLFW_GetProperty ) &&
                 ( NULL != pDLFW_TaskSetProperty ) &&
                 ( NULL != pDLFW_TaskGetProperty ) &&
                 ( NULL != pCreateISpLogObject ) );
    };
    
} DLFW_DRIVER_T;

