// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__1DDDF3B9_ED10_47BF_8EF9_3AD8B814C217__INCLUDED_)
#define AFX_MAINFRM_H__1DDDF3B9_ED10_47BF_8EF9_3AD8B814C217__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ButtonEx.h"
#include "XRandom.h"
#include "DLFWDef.h"
#include "RepartitionDef.h"
#include "CoolToolBar.h"
#include "CoolStatusBar.h"
#include "SettingsSheet.h"
#include "BarcodeDlg.h"
#include "FrameHelper.h"
#include "TaskHelper.h"
#include "MesLoginHelper.h"
#include "MutiSoftSelectDlg.h"
#include "PreloadSelectDlg.h"
#include <map>


enum ENUM_STATUSBAR_INDEX : INT
{
    E_STATUSBAR_READY,
    E_STATUSBAR_CTRL,
    E_STATUSBAR_LOGO
};
//StatusBar
struct STATUSBAR_CRECT_CFG
{
    STATUSBAR_CRECT_CFG() :
        ReloadBtn( 250, 0, 288, 28 ),
        OpenLogBtn( 290, 0, 328, 28 ),
        LevelStatic( 350, 4, 430, 25 ),
        LevelComboBox( 430, 2, 485, 250 ),
        LoadStatic( 510, 4, 610, 25 ),
        LoadCheck( 610, 0, 640, 30 ),
        LoadPolicyStatic( 645, 4, 740, 25 ),
        LoadPolicyComboBox( 740, 2, 795, 250 )
    {
    }
    
    CRect OpenLogBtn;
    CRect LevelStatic;
    CRect LevelComboBox;
    CRect LoadStatic;
    CRect LoadCheck;
    CRect ReloadBtn;
    CRect LoadPolicyStatic;
    CRect LoadPolicyComboBox;
};

typedef map<uint16, CTaskHelper*> MAP_TASK_HELPER;

struct TASK_HELPER_MANAGE
{
public:
    TASK_HELPER_MANAGE()
    {
        arrTaskHelper.clear();
    }
    
    CTaskHelper* Find( uint16 port )
    {
        MAP_TASK_HELPER::iterator it = arrTaskHelper.find( port );
        if ( arrTaskHelper.end() == it )
        {
            return NULL;
        }
        return it->second;
    }
    
    void Insert( uint16 port, CTaskHelper* task )
    {
        arrTaskHelper.insert( std::make_pair( port, task ) );
    }
    
    void StopAll()
    {
        for ( uint16 i = 0; i < ( uint16 )arrTaskHelper.size(); i++ )
        {
            CTaskHelper* pTask = arrTaskHelper[i];
            if ( NULL != pTask )
            {
                pTask->StopTask();
            }
        }
    }
    
    void CloseAll()
    {
        for ( uint16 i = 0; i < ( uint16 )arrTaskHelper.size(); i++ )
        {
            CTaskHelper* pTask = arrTaskHelper[i];
            if ( NULL != pTask )
            {
                pTask->FreeTask();
            }
        }
        arrTaskHelper.clear();
    }
    
private:
    MAP_TASK_HELPER arrTaskHelper;
};

struct PORT_INFO
{
    uint32 u32Port;
};

int CALLBACK BrowseCallbackProc( HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData );

class CMainFrame : public CFrameWnd
{

protected: // create from serialization only
    CMainFrame();
    virtual ~CMainFrame();
    
    DECLARE_DYNCREATE( CMainFrame )
    
    // Attributes
public:
    BOOL           m_bPacLoading = FALSE;       //加载PAC
    BOOL           m_bStarted = FALSE;          //开始下载
    BOOL           m_bPacketOpen = FALSE;       //打开PAC
    BOOL           m_bPacketInit = FALSE;       //打开PAC
    BOOL           m_bCheckFileMode = FALSE;
    BOOL           m_bUserIntervene = FALSE;
    
    PLONG          m_plStartNVRef; //a flag used to prevent user to stop downloading when downloading NV
    
public:
    CString        m_strPacketPath;
    CString        m_strPacDirBase;
    CString        m_strFilePath;
    // Operations
public:
    BOOL LoadSetting();
    SPRESULT StartDownload( UINT nPort );
    
    void OnCreateFileMode();
    void OnCreateReload();
    void OnCreateIspLogLevel();
    void OnCreateLoadPolicy();
    void OnCreateOpenLog();
    void OnCreateMesInfo();
    void OnCreatePacInfo();
    
    BOOL OnCreateToolBar();
    BOOL OnCreateStatusBar();
    
    void SetToolBarText( LPCTSTR lpText );
    void SetStatusBarText( ENUM_STATUSBAR_INDEX eIndex, LPCTSTR lpText );
    
protected:
    void SetDLTitle();
    void ShowPartition();
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMainFrame)
public:
    virtual BOOL PreCreateWindow( CREATESTRUCT& cs );
    virtual BOOL PreTranslateMessage( MSG* pMsg );
    
protected:
    virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam );
    //}}AFX_VIRTUAL
    
    // Implementation
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump( CDumpContext& dc ) const;
#endif
    
protected:  // control bar embedded members
    BOOL       m_bFailedMsgboxShowed;
    
public:
    CDLFWDriver        m_fnDLFWDrv;
    CFrameHelper*      m_pFrameHelper;
    TASK_HELPER_MANAGE m_TaskHelperManage;
    
    CSettingsSheet     m_sheetSettings;
    CMutiSoftSelectDlg m_dlgMutiSoftSelect;
    CPreloadSelectDlg  m_dlgPreloadSelect;
    
    CMesLoginHelper    m_fnMesLoginHelper;

    std::vector<PORT_INFO> ProtInfoVector;
    PORT_INFO          m_tPortInfo;
    
private:
    CRITICAL_SECTION  m_csLock;
    
    CStringArray m_agATPort;
    CStringArray m_agDLPort;
    
    //Control
    STATUSBAR_CRECT_CFG m_tStatusBarRect;
    CButtonEx           m_CtrlBtnOpenLog;
    CButtonEx           m_CtrlBtnReload;
    CButton             m_CtrlBtnChkFileMode;
    CComboBox           m_CtrlComboBoxIspLogLevel;
    CComboBox           m_CtrlComboBoxLoadPolicy;
    CStatic             m_CtrlStaticIspLogLevel;
    CStatic             m_CtrlStaticFileMode;
    CStatic             m_CtrlStaticLoadPolicy;
    CCoolToolBar        m_wndToolBar;
    CCoolStatusBar      m_wndStatusBar;
    CFont               m_fontStatusBarCtrl;
    CXStatic            m_StaticMesInfo;
    CXStatic            m_StaticPacInfo;
    
private:
    BOOL GetFilePathByDialog( BOOL bPackets = TRUE );
    BOOL GetFilePathByIni( BOOL bPackets = TRUE );
    SPRESULT __LoadPackets( BOOL bPackets = TRUE );
    BOOL __LoadSetting();
    // Generated message map functions
protected:
    //{{AFX_MSG(CMainFrame)
    afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
    afx_msg void OnSettings();
    afx_msg void OnStart();
    afx_msg void OnStop();
    afx_msg void OnUpdateStart( CCmdUI* pCmdUI );
    afx_msg void OnUpdateStop( CCmdUI* pCmdUI );
    afx_msg void OnClose();
    afx_msg void OnUpdateSettings( CCmdUI* pCmdUI );
    afx_msg BOOL OnHelpInfo( HELPINFO* pHelpInfo );
    afx_msg void OnLoadPacket();
    afx_msg void OnUpdateLoadPacket( CCmdUI* pCmdUI );
    afx_msg void OnTimer( UINT nIDEvent );
    afx_msg void OnOpenLog();
    afx_msg void OnReloadSetting();
    afx_msg void OnSelIspLogLevel();
    afx_msg void OnCheckFileMode();
    afx_msg void OnSelLoadPolicy();
    //}}AFX_MSG
    afx_msg LRESULT OnWarnMessageBox( WPARAM wParam, LPARAM lpParam );
    afx_msg LRESULT OnInitalPacket( WPARAM wParam, LPARAM lpParam );
    afx_msg LRESULT OnStopAutoDloader( WPARAM wParam, LPARAM lpParam );
    afx_msg LRESULT OnStopOnePort( WPARAM wParam, LPARAM lpParam );
    afx_msg LRESULT OnPowerManage( WPARAM wParam, LPARAM lpParam );
    afx_msg LRESULT OnStartDownload( WPARAM wParam, LPARAM lpParam );
    afx_msg LRESULT OnHelp( WPARAM wParam, LPARAM lpParam );
    afx_msg LRESULT OnProgressMsg( WPARAM wParam, LPARAM lpParam );
    afx_msg LRESULT OnToolBarText( WPARAM wParam, LPARAM lParam );
    afx_msg LRESULT OnMutiSoftwareConfig( WPARAM wParam, LPARAM lParam );
    afx_msg LRESULT OnPreloadSelectConfig( WPARAM wParam, LPARAM lParam );
    DECLARE_MESSAGE_MAP()
    
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__1DDDF3B9_ED10_47BF_8EF9_3AD8B814C217__INCLUDED_)
