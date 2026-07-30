// InvokeDemo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "FrameworkHelper.h"
#include "DevMoniter.h"
#include "ConsoleMoniter.h"
#include "ConsoleHelper.h"
#include <iostream>
#include "IConfig.h"

CFrameworkHelper g_fnFrameWork;
CDevMoniter g_fnDevMoniter;
CConsoleMoniter g_fnConsoleMoniter;
IConfig* g_pCfgObject = NULL;
CConsoleHelper g_fnConsole;

SPRESULT Download()
{
    std::wstring strIniPath = g_fnFrameWork.GetAppPath() + L"App\\DLFramework.ini";
    UINT nLv = GetPrivateProfileIntW( _T( "Log" ), _T( "Level" ), SPLOGLV_ERROR, strIniPath.c_str() );
    
    SPRESULT spRlt = SP_OK;
    /// DLFramework内部创建公共资源、初始化公共资源
    spRlt = g_fnFrameWork.Startup( nLv );
    if ( SP_OK != spRlt )
    {
        goto _END_CLEANUP;
    }
    /// 加载Packet，并对DLFramework内部公共资源进行配置
    spRlt = g_fnFrameWork.LoadPacket();
    if (SP_OK != spRlt)
    {
        goto _END_FREE;
    }
    /// DLFramework内部创建任务资源
    spRlt = g_fnFrameWork.CreatTask( nLv );
    if ( SP_OK != spRlt )
    {
        goto _END_FREE;
    }
    
#ifdef CFG_PARAMS_DEMO
    /// 获取Config模块的Object
    spRlt = g_fnFrameWork.GetProperty( DLFW_ATTR_CONFIG_OBJECT, 0, &g_pCfgObject );
    if ( SP_OK != spRlt || NULL == g_pCfgObject )
    {
        goto _END_FREE;
    }
    
    /// 共软体参数配置示例
    PROJECT_CONFIG tProjectCfg;
    tProjectCfg.nCheck = 1;
    wcscpy_s( tProjectCfg.szFileID, L"OCDT" );
    wcscpy_s( tProjectCfg.szProjectBase, L"CUST" );
    wcscpy_s( tProjectCfg.szProjectID, L"1234" );
    
    spRlt = g_fnFrameWork.SetProperty( DLFW_ATTR_PROJECT_CONFIG, 0, ( LPVOID )&tProjectCfg );
    if ( SP_OK != spRlt )
    {
        goto _END_FREE;
    }
    
    /// 写SN参数配置示例
    /// 通过Config模块配置Download.ini -> SN -> NeedPhaseCheck -> 0
    spRlt = g_pCfgObject->SetInt( CFGTYPE_DL, _T( "SN" ), _T( "NeedPhaseCheck" ), 0 );
    if ( SP_OK != spRlt )
    {
        goto _END_FREE;
    }
    
    string_t strSN1 = _T( "123456" );
    string_t strSN2 = _T( "654321" );
    DLFW_ATTRDATA_BARCODE dlSN;
    dlSN.pszSN1 = strSN1.c_str();
    dlSN.pszSN2 = strSN2.c_str();
    dlSN.u8SN1Len = ( uint8 )strSN1.length();
    dlSN.u8SN2Len = ( uint8 )strSN2.length();
    spRlt = g_fnFrameWork.TaskSetProperty( DLFW_ATTR_BARCODE_SN, 0, &dlSN );
    if ( SP_OK != spRlt )
    {
        goto _END_FREE;
    }
    
    /// DLFramework内部公共资源重新获取ini配置
    spRlt = g_fnFrameWork.ReloadSettings();
    if ( SP_OK != spRlt )
    {
        goto _END_FREE;
    }
    /// DLFramework内部同步公共资源的配置到任务资源
    spRlt = g_fnFrameWork.SyncParameters();
    if ( SP_OK != spRlt )
    {
        goto _END_FREE;
    }
#endif // CFG_PARAMS_DEMO
    
    /// 开辟线程监测Console是否有"Download Start"
    /// 有"Download Start"，进行下载
    /// 120S没有"Download Start"，推出等待
    g_fnDevMoniter.StartMoniter( TRUE );
    g_fnDevMoniter.StartMoniter( FALSE, 120000 );
    if ( g_fnDevMoniter.GetRunResult() )
    {
        /// 开始下载任务(下载任务是在新开启的线程中进行)
        spRlt = g_fnFrameWork.RunTask();
        if ( SP_OK != spRlt )
        {
            goto _END_STOP;
        }
        /// 开辟线程监测Console是否有"PASS"和"FAIL"，确认下载是否完成
        g_fnConsoleMoniter( KEY_PASS );
        g_fnConsoleMoniter( KEY_FAIL );
        g_fnConsoleMoniter.StartMoniter( TRUE );
        g_fnConsoleMoniter.StartMoniter( FALSE );
_END_STOP:
        /// 停止下载任务(终止RunTask启动的线程)
        g_fnFrameWork.StopTask();
    }
    
_END_FREE:
    /// DLFramework内部释放任务资源
    g_fnFrameWork.FreeTask();
    
_END_CLEANUP:
    /// DLFramework内部释放公共资源
    g_fnFrameWork.Cleanup();
    
    return SP_OK;
}

int _tmain( int argc, _TCHAR* argv[] )
{
    Download();
    getchar();
    return 0;
}
