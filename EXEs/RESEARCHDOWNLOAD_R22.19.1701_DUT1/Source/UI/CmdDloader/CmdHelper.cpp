#include "StdAfx.h"
#include "CmdHelper.h"
#include "global_err.h"
#include <algorithm>

extern CMD_PARAMS g_tCmdParams;
//////////////////////////////////////////////////////////////////////////
#define _SHOW_FROMART "%-40s%s\n"
void CCmdHelper::ShowUsage()
{
    printf( "\n\n" );
    printf( "CmdDloader usage:\n" );
    printf( "CmdDloader.exe [-Pac PacFilePath-String] [-Version Version-Number] [-Port Port-Number] [-Policy Policy-Number]\n" );
    printf( "               [-Baudrate Baudrate-Number] [-SkipFileID FileID-String] [-Timeout Timeout-Number] [-WriteSN1 SN1-String] [-WriteSN2 SN2-String]\n" );
    printf( "               [-MutiFileID FileID-String] [-MutiProjectID ProjectID-String] [-MutiProjectBase ProjectBase-String] [-ReadNV NVFile-String]\n" );
    printf( "               [-SendFlag] [-WriteEfuse] [-WriteTimeStamp] [-PowerOff] [-Reset] [-EZMode] [-DDDP] [-Help]\n\n" );
    printf( "e.g.\n" );
    printf( "CmdDloader.exe -pac d:\\test.pac -port 5\n\n" );
    printf( "Commands( case insensitive ) and option:\n" );
    printf( _SHOW_FROMART, "-Pac <PacFilePath-String>",                "Download packet file path; Arguments must be configured." );
    printf( _SHOW_FROMART, "-Version <Version-Number>",                "0:ResearchDownload; 1:FactoryDownload; 2:UpgradeDownload; Default is 0:ResearchDownload;" );
    printf( _SHOW_FROMART, "-Port <Port-Number>",                      "Download port; Default is 0( auto find the available device );" );
    printf( _SHOW_FROMART, "-Policy <Policy-Number>",                  "Load download packet file policy; Refer to the definition in BinPack.ini; Default is 0;" );
    printf( _SHOW_FROMART, "-Baudrate <Baudrate-Number>",              "Baudrate for serial port; Default is 115200;" );
    printf( _SHOW_FROMART, "-SkipFileID <FileID-String>",              "FileID which want to skip download, split with ','; Default is null;" );
    printf( _SHOW_FROMART, "-Timeout <Timeout-Number>",                "Timeout( uint:ms ) from execute to start downloading; Default is 60000ms;" );
    printf( _SHOW_FROMART, "-SendFlag <SendFlag-Number>",              "0:Disable the SendFlag function; 1:Enable the SendFlag function; Default is 0;");
    printf( _SHOW_FROMART, "-WriteSN1 <SN1-String>",                   "Set with sn1 string; Default is null;" );
    printf( _SHOW_FROMART, "-WriteSN2 <SN2-String>",                   "Set with sn2 string; Default is null;" );
    printf( _SHOW_FROMART, "-MutiFileID <FileID-String>",              "FileID for Multi-project co-software; Default is null;" );
    printf( _SHOW_FROMART, "-MutiProjectID <ProjectID-String>",        "ProjectID for Multi-project co-software; Default is null;" );
    printf( _SHOW_FROMART, "-MutiProjectBase <ProjectBase-String>",    "ProjectBase for Multi-project co-software; Default is null;" );
    printf( _SHOW_FROMART, "-ReadNV <NVFile-String>",                  "Read the NV according to the configured NV file; Default is null;" );
    printf( _SHOW_FROMART, "-WriteEfuse",                              "Enable the write fuse function;" );
    printf( _SHOW_FROMART, "-WriteTimeStamp",                          "Write Time Stamp;" );
    printf( _SHOW_FROMART, "-PowerOff",                                "Power off device after download;" );
    printf( _SHOW_FROMART, "-Reset",                                   "Reset device to normal after download;" );
    printf( _SHOW_FROMART, "-EZMode",                                  "Only output status and results in the console window;" );
    printf( _SHOW_FROMART, "-DDDP",                                    "Disable detail download process printing; Default is false;" );
    printf( _SHOW_FROMART, "-Help",                                    "Help information; Default is false;" );
    fflush( stdout );
}

BOOL CCmdHelper::CheckPac()
{
    if ( g_tCmdParams.strPacFile.empty() )
    {
        _tprintf( _T( "Invalid arguments, download packet file must be configured.\n" ) );
        fflush( stdout );
        return FALSE;
    }
    else
    {
        if ( ( _waccess( g_tCmdParams.strPacFile.c_str(), 0 ) ) == -1 ) //pac路径是否有效
        {
            _tprintf( _T( "Invalid arguments, download packet file [%s] don't exist.\n" ), g_tCmdParams.strPacFile.c_str() );
            fflush( stdout );
            return FALSE;
        }
    }
    return TRUE;
}

BOOL CCmdHelper::CheckVersion()
{
    if ( g_tCmdParams.eDlType > INVALID_DL_TYPE && g_tCmdParams.eDlType < MAX_DL_TYPE )
    {
        return TRUE;
    }
    _tprintf( _T( "-version must be configured as one of 0,1,2.\n" ) );
    fflush( stdout );
    return FALSE;
}

BOOL CCmdHelper::CheckPwrOffAndReset()
{
    if ( 1 == g_tCmdParams.nPoweroff && 1 == g_tCmdParams.nReset )
    {
        _tprintf( _T( "-poweroff and -reset cannot both be set.\n" ) );
        fflush( stdout );
        return FALSE;
    }
    return TRUE;
}

BOOL CCmdHelper::CheckSendFlag()
{
    if ( g_tCmdParams.nSendFlag != 0 && g_tCmdParams.nSendFlag != 1 )
    {
        _tprintf( _T( "-SendFlag must be set to 0 or 1.\n" ) );
        fflush( stdout );
        return FALSE;
    }
    return TRUE;
}

BOOL CCmdHelper::CheckLoadPolicy()
{
    if ( g_tCmdParams.nLoadPolicy != 0 && g_tCmdParams.nLoadPolicy != 1 && g_tCmdParams.nLoadPolicy != 2 )
    {
        _tprintf( _T( "-policy must be configured as one of 0,1,2.\n" ) );
        fflush( stdout );
        return FALSE;
    }
    return TRUE;
}

BOOL CCmdHelper::CheckMutiProject()
{
    if ( g_tCmdParams.bMutisoftware )
    {
        if ( g_tCmdParams.strMutiFileID.empty() || g_tCmdParams.strMutiProjectID.empty() || g_tCmdParams.strMutiProjectBase.empty() )
        {
            _tprintf( _T( "-MutiFileID, -MutiProjectID, and -MutiProjectBase must be configured together or not.\n" ) );
            fflush( stdout );
            return FALSE;
        }
    }
    return TRUE;
}

BOOL CCmdHelper::GetOneCmdParams( INT& nCmd, LPCTSTR lpCmd, LPTSTR* lppCmd, INT nCount, LPTSTR& lpParam )
{
    wstring strCmdA( lpCmd );
    transform( strCmdA.begin(), strCmdA.end(), strCmdA.begin(), ::toupper );
    wstring strCmdB( lppCmd[nCmd] );
    transform( strCmdB.begin(), strCmdB.end(), strCmdB.begin(), ::toupper );
    if ( strCmdA == strCmdB )
    {
        nCmd++;
        if ( nCmd < nCount )
        {
            wstring strParam( lppCmd[nCmd] );
            if ( 0 == strParam.find( L"-" ) )
            {
                _tprintf( _T( "%s is configured with invalid parameters.\n" ), lpCmd );
                fflush( stdout );
                return FALSE;
            }
            else
            {
                lpParam = lppCmd[nCmd];
            }
        }
        else
        {
            _tprintf( _T( "%s must have parameter.\n" ), lpCmd );
            fflush( stdout );
            return FALSE;
        }
    }
    return TRUE;
}

BOOL CCmdHelper::GetOneCmdParams( INT nCmd, LPCTSTR lpCmd, LPTSTR* lppCmd )
{
    wstring strCmdA( lpCmd );
    transform( strCmdA.begin(), strCmdA.end(), strCmdA.begin(), ::toupper );
    wstring strCmdB( lppCmd[nCmd] );
    transform( strCmdB.begin(), strCmdB.end(), strCmdB.begin(), ::toupper );
    if ( strCmdA == strCmdB )
    {
        return TRUE;
    }
    return FALSE;
}

BOOL CCmdHelper::GetAllCmdParams( LPTSTR* lppCmd, INT nCount )
{
    g_tCmdParams.Init();
    for ( int i = 1; i < nCount; i++ )
    {
        LPTSTR lpCmd = NULL;
        BOOL bCmd = FALSE;
        bCmd = GetOneCmdParams( i, _T( "-Help" ), lppCmd );
        if ( bCmd )
        {
            g_tCmdParams.bHelp = bCmd;
            return TRUE;
        }
        CHKBOOL( GetOneCmdParams( i, _T( "-pac" ), lppCmd, nCount, lpCmd ) );
        if ( NULL != lpCmd )
        {
            g_tCmdParams.strPacFile = lpCmd;
            continue;
        }
        CHKBOOL( GetOneCmdParams( i, _T( "-Version" ), lppCmd, nCount, lpCmd ) );
        if ( NULL != lpCmd )
        {
            INT nVersion = _ttoi( lpCmd );
            switch ( nVersion )
            {
            case 0: g_tCmdParams.eDlType = RESEARCH_TYPE; break;
            case 1: g_tCmdParams.eDlType = FACTORY_TYPE; break;
            case 2: g_tCmdParams.eDlType = UPGRADE_TYPE; break;
            default: g_tCmdParams.eDlType = INVALID_DL_TYPE; break;
            }
            CHKBOOL( CheckVersion() );
            continue;
        }
        CHKBOOL( GetOneCmdParams( i, _T( "-Port" ), lppCmd, nCount, lpCmd ) );
        if ( NULL != lpCmd )
        {
            g_tCmdParams.nPort = _ttoi( lpCmd );
            continue;
        }
        CHKBOOL( GetOneCmdParams( i, _T( "-Policy" ), lppCmd, nCount, lpCmd ) );
        if ( NULL != lpCmd )
        {
            g_tCmdParams.nLoadPolicy = _ttoi( lpCmd );
            CHKBOOL( CheckLoadPolicy() );
            continue;
        }
        CHKBOOL( GetOneCmdParams( i, _T( "-Baudrate" ), lppCmd, nCount, lpCmd ) );
        if ( NULL != lpCmd )
        {
            g_tCmdParams.nBaudRate = _ttoi( lpCmd );
            continue;
        }
        CHKBOOL( GetOneCmdParams( i, _T( "-SkipFileID" ), lppCmd, nCount, lpCmd ) );
        if ( NULL != lpCmd )
        {
            g_tCmdParams.strSkipFileID = lpCmd;
            continue;
        }
        CHKBOOL( GetOneCmdParams( i, _T( "-Timeout" ), lppCmd, nCount, lpCmd ) );
        if ( NULL != lpCmd )
        {
            g_tCmdParams.nTimeout = _ttoi( lpCmd );
            continue;
        }
        CHKBOOL( GetOneCmdParams( i, _T( "-SendFlag" ), lppCmd, nCount, lpCmd ) );
        if ( NULL != lpCmd )
        {
            g_tCmdParams.nSendFlag = _ttoi( lpCmd );
            CHKBOOL( CheckSendFlag() );
            continue;
        }
        CHKBOOL( GetOneCmdParams( i, _T( "-WriteSN1" ), lppCmd, nCount, lpCmd ) );
        if ( NULL != lpCmd )
        {
            g_tCmdParams.strSN1 = lpCmd;
            g_tCmdParams.bWriteSN1 = TRUE;
            continue;
        }
        CHKBOOL( GetOneCmdParams( i, _T( "-WriteSN2" ), lppCmd, nCount, lpCmd ) );
        if ( NULL != lpCmd )
        {
            g_tCmdParams.strSN2 = lpCmd;
            g_tCmdParams.bWriteSN2 = TRUE;
            continue;
        }
        CHKBOOL( GetOneCmdParams( i, _T( "-ReadNV" ), lppCmd, nCount, lpCmd ) );
        if ( NULL != lpCmd )
        {
            g_tCmdParams.bReadFixNV = TRUE;
            g_tCmdParams.strSaveFixNV = lpCmd;
            continue;
        }
        CHKBOOL( GetOneCmdParams( i, _T( "-MutiFileID" ), lppCmd, nCount, lpCmd ) );
        if ( NULL != lpCmd )
        {
            g_tCmdParams.bMutisoftware = TRUE;
            g_tCmdParams.strMutiFileID = lpCmd;
            continue;
        }
        CHKBOOL( GetOneCmdParams( i, _T( "-MutiProjectID" ), lppCmd, nCount, lpCmd ) );
        if ( NULL != lpCmd )
        {
            g_tCmdParams.bMutisoftware = TRUE;
            g_tCmdParams.strMutiProjectID = lpCmd;
            continue;
        }
        CHKBOOL( GetOneCmdParams( i, _T( "-MutiProjectBase" ), lppCmd, nCount, lpCmd ) );
        if ( NULL != lpCmd )
        {
            g_tCmdParams.bMutisoftware = TRUE;
            g_tCmdParams.strMutiProjectBase = lpCmd;
            continue;
        }
        bCmd = GetOneCmdParams( i, _T( "-WriteEfuse" ), lppCmd );
        if ( bCmd )
        {
            g_tCmdParams.nWriteEfuse = bCmd ? 1 : 0;
            continue;
        }
        bCmd = GetOneCmdParams( i, _T( "-DDDP" ), lppCmd );
        if ( bCmd )
        {
            g_tCmdParams.bDetailDlProcess = !bCmd;
            continue;
        }
        bCmd = GetOneCmdParams( i, _T( "-WriteTimeStamp" ), lppCmd );
        if ( bCmd )
        {
            g_tCmdParams.nWriteTimeStamp = bCmd ? 1 : 0;
            continue;
        }
        bCmd = GetOneCmdParams( i, _T( "-PowerOff" ), lppCmd );
        if ( bCmd )
        {
            g_tCmdParams.nPoweroff = bCmd ? 1 : 0;
            continue;
        }
        bCmd = GetOneCmdParams( i, _T( "-Reset" ), lppCmd );
        if ( bCmd )
        {
            g_tCmdParams.nReset = bCmd ? 1 : 0;
            continue;
        }
        bCmd = GetOneCmdParams( i, _T( "-EZMode" ), lppCmd );
        if ( bCmd )
        {
            g_tCmdParams.bEZMode = bCmd;
            continue;
        }
        _tprintf( _T( "%s is an invalid command.\n" ), lppCmd[i] );
        fflush( stdout );
        return FALSE;
    }
    CHKBOOL( CheckPac() );
    CHKBOOL( CheckPwrOffAndReset() );
    CHKBOOL( CheckMutiProject() );
    return TRUE;
}
