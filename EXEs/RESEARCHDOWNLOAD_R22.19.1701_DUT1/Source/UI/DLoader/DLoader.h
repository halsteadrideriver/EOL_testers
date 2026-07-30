// DLoader.h : main header file for the DLOADER application
//

#if !defined(AFX_DLOADER_H__09DF2514_AE78_4B20_8103_993EB56996BA__INCLUDED_)
#define AFX_DLOADER_H__09DF2514_AE78_4B20_8103_993EB56996BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#define UDSIK_IMG_NAME_LEN      (13)

#include "resource.h"       // main symbols
#include "IConfig.h"
#include "OperationHelper.h"
/////////////////////////////////////////////////////////////////////////////
// CDLoaderApp:
// See DLoader.cpp for the implementation of this class
//

struct DLFW_ATTR_STACK_SIZE
{
    uint32  u32ProductName = 0;
    uint32  u32ProductInfo = 0;
    uint32  u32DlImageInfo = 0;
    uint32  u32BackupNvItems = 0;
    uint32  u32BackupFiles = 0;
    uint32  u32LCDConfig = 0;
    uint32  u32MultiLanguage = 0;
    uint32  u32ProjectConfig = 0;
};

class CDLoaderApp : public CWinApp
{
public:
    CDLoaderApp();
    
public:
    BOOL    m_bColorFlag = TRUE;   // use color to mark the last failed.
    BOOL    m_bResultHolding;   // when start, the waiting state holding the previous result
    BOOL    m_bManual = FALSE;
    BOOL    m_bNeedPassword = FALSE;
    BOOL    m_bCheckDriverVer = FALSE;
    BOOL    m_bScriptCtrl = FALSE;
    BOOL    m_bFilterPort = FALSE;
    BOOL    m_bStopDownloadIfOldMemory = TRUE;
    BOOL    m_bShowOtherPage = FALSE;
    BOOL    m_bShowMcpTypePage = FALSE;
    CString m_strFileFilter;
    int     m_nEnumPortTimeOut;
    CString m_strEnumPortErrorMsg;
    CString m_strAppPath;
    BOOL    m_bNeedPhaseCheck = TRUE;
    BOOL    m_bShowRate = FALSE;
    BOOL    m_bPortSecondEnum = FALSE;
    BOOL    m_bPowerManage = FALSE;
    BOOL    m_bPMInDLProcess = TRUE;
    BOOL    m_bKeepPacNVState = FALSE;
    BOOL    m_bShowSafetyTips = TRUE;
    CString m_strSafetyTipsMsg = _T( "" );
    MAP_PORT m_mapFilterPort;
    DLFW_ATTR_STACK_SIZE m_attrStackSize;
    
    IConfig*         m_pCfgObject = NULL;
    COperationHelper m_fnOperationHelper;
    
public:
    BOOL LoadSetting();
    
protected:
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDLoaderApp)
public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
    //}}AFX_VIRTUAL
    
    // Implementation
    //{{AFX_MSG(CDLoaderApp)
    afx_msg void OnAppAbout();
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

// declare the global App instance
extern CDLoaderApp g_theApp;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLOADER_H__09DF2514_AE78_4B20_8103_993EB56996BA__INCLUDED_)
