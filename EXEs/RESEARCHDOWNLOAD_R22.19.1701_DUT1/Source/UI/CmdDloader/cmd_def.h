#pragma once
#include "global_def.h"

#define START_PROGRESS 1000
#define KEY_PASS _T("Download Result = Passed;")
#define KEY_FAIL _T("Download Result = Failed; Error Messag = ")
#define KEY_ERROR _T("[ERROR INFO]")
#define KEY_WARNING _T("[WARNING INFO]")
#define KEY_INSERT_DEV _T("Insert Device")
#define KEY_REMOVE_DEV _T("Remove Device")
#define DEVICE_FROMART_ALL _T( "%s; COM%d; %s;\n" )
#define DLIMAGE_FROMART_S _T( "No.%-03d %-26s Progress: %3d%%%%; Size: %.3fM; Time:%ds\n" )
#define DLIMAGE_FROMART_MS _T( "No.%-03d %-26s Progress: %3d%%%%; Size: %.3fM; Time:%dms\n" )


typedef struct _tagCMD_PARAMS
{
    _tagCMD_PARAMS()
    {
        Init();
    }
    
    void Init()
    {
        strPacFile = _T( "" );
        strSkipFileID = _T( "" );
        strSN1 = _T( "" );
        strSN2 = _T( "" );
        strSaveFixNV = _T( "" );
        strMutiFileID = _T( "" );
        strMutiProjectID = _T( "" );
        strMutiProjectBase = _T( "" );
        eDlType = RESEARCH_TYPE;
        nPort = 0;
        nTimeout = 60000;
        nDLTimeout = 1000 * 60 * 30;
        nBaudRate = 115200;
        nLoadPolicy = -1;
        nPoweroff = -1;
        nReset = -1;
        nWriteEfuse = -1;
        nWriteTimeStamp = -1;
        nSendFlag = -1;
        
        bHelp = FALSE;
        bEZMode = FALSE;
        bWriteSN1 = FALSE;
        bWriteSN2 = FALSE;
        bReadFixNV = FALSE;
        bMutisoftware = FALSE;
        bDetailDlProcess = TRUE;
    }
    
    string_t strPacFile;
    string_t strSkipFileID;
    string_t strSN1;
    string_t strSN2;
    string_t strSaveFixNV;
    string_t strMutiFileID;
    string_t strMutiProjectID;
    string_t strMutiProjectBase;
    DL_TYPE_ENUM eDlType;
    INT nPort;
    INT nTimeout;
    INT nDLTimeout;
    INT nBaudRate;
    INT nLoadPolicy;
    INT nPoweroff;
    INT nReset;
    INT nWriteEfuse;
    INT nWriteTimeStamp;
    INT nSendFlag;
    BOOL bWriteSN1;
    BOOL bWriteSN2;
    BOOL bReadFixNV;
    BOOL bMutisoftware;
    BOOL bDetailDlProcess;
    BOOL bEZMode;
    BOOL bHelp;
    
} CMD_PARAMS, * LPCMD_PARAMS;