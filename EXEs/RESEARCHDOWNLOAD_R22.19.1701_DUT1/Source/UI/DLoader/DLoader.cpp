// DLoader.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "DLoader.h"
#include "MainFrm.h"
#include "DLoaderDoc.h"
#include "DLoaderView.h"
#include "Utility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDLoaderApp

BEGIN_MESSAGE_MAP( CDLoaderApp, CWinApp )
    //{{AFX_MSG_MAP(CDLoaderApp)
    ON_COMMAND( ID_APP_ABOUT, OnAppAbout )
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG_MAP
    // Standard file based document commands
    ON_COMMAND( ID_FILE_NEW, CWinApp::OnFileNew )
    ON_COMMAND( ID_FILE_OPEN, CWinApp::OnFileOpen )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLoaderApp construction

CDLoaderApp::CDLoaderApp()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDLoaderApp object

CDLoaderApp g_theApp;

/////////////////////////////////////////////////////////////////////////////
// CDLoaderApp initialization

BOOL CDLoaderApp::InitInstance()
{
    AfxEnableControlContainer();
    
    OleInitialize( NULL );
    
    // Standard initialization
    // If you are not using these features and wish to reduce the size
    //  of your final executable, you should remove from the following
    //  the specific initialization routines you do not need.
    
    /*
    #ifdef _AFXDLL
    Enable3dControls();         // Call this when using MFC in a shared DLL
    #else
    Enable3dControlsStatic();   // Call this when linking to MFC statically
    #endif
    */
    
    TCHAR szPath[512] = {0};
    m_fnOperationHelper.GetModuleFilePath( m_hInstance, szPath, _countof( szPath ) );
    m_strAppPath = szPath;
    // Change the registry key under which our settings are stored.
    // TODO: You should modify this string to be something appropriate
    // such as the name of your company or organization.
    SetRegistryKey( _T( "Unisoc" ) );
    
    LoadStdProfileSettings();  // Load standard INI file options (including MRU)
    
    // Register the application's document templates.  Document templates
    //  serve as the connection between documents, frame windows and views.
    CSingleDocTemplate* pDocTemplate;
    pDocTemplate = new CSingleDocTemplate(
        IDR_MAINFRAME,
        RUNTIME_CLASS( CDLoaderDoc ),
        RUNTIME_CLASS( CMainFrame ),     // main SDI frame window
        RUNTIME_CLASS( CDLoaderView ) );
    AddDocTemplate( pDocTemplate );
    
    
    UNUSED_ALWAYS( IDR_DLOADETYPE );
    
    // Parse command line for standard shell commands, DDE, file open
    CCommandLineInfo cmdInfo;
    ParseCommandLine( cmdInfo );
    
    // Dispatch commands specified on the command line
    if ( !ProcessShellCommand( cmdInfo ) )
    { return FALSE; }
    
    // The one and only window has been initialized, so show and update it.
    m_pMainWnd->ShowWindow( SW_SHOW );
    m_pMainWnd->UpdateWindow();
    
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();
    
    // Dialog Data
    //{{AFX_DATA(CAboutDlg)
    enum { IDD = IDD_ABOUTBOX }; //lint !e30
    //}}AFX_DATA
    
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAboutDlg)
protected:
    virtual void DoDataExchange( CDataExchange* pDX );  // DDX/DDV support
    //}}AFX_VIRTUAL
    
    // Implementation
protected:
    //{{AFX_MSG(CAboutDlg)
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog( CAboutDlg::IDD )
{
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    //{{AFX_DATA_MAP(CAboutDlg)
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP( CAboutDlg, CDialog )
    //{{AFX_MSG_MAP(CAboutDlg)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CDLoaderApp::OnAppAbout()
{
    CAboutDlg aboutDlg;
    //aboutDlg.DoModal();
    
    TCHAR szPath[MAX_PATH] = { 0 };
    _tcscat( szPath, m_strAppPath.operator LPCWSTR() );
    _tcscat( szPath, _T( "..\\Doc" ) );
    
    CFileFind finder;
    if ( finder.FindFile( szPath ) )
    {
        ShellExecute( m_pMainWnd->GetSafeHwnd(), _T( "open" ), szPath, NULL, NULL, SW_SHOW );
    }
}

/////////////////////////////////////////////////////////////////////////////
// CDLoaderApp message handlers


BOOL CAboutDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    return TRUE;
}

int CDLoaderApp::ExitInstance()
{
    // TODO: Add your specialized code here and/or call the base class
    OleUninitialize();
    return CWinApp::ExitInstance();
}

BOOL CDLoaderApp::LoadSetting()
{
    if ( NULL == m_pCfgObject )
    {
        ASSERT( 0 );
        return FALSE;
    }
    
    m_bColorFlag = m_pCfgObject->GetInt( CFGTYPE_DL, _T( "GUI" ), _T( "MarkLastState" ), 0 );
    m_bResultHolding = m_pCfgObject->GetInt( CFGTYPE_DL, _T( "GUI" ), _T( "ResultHolding" ), 1 );
    m_bManual = m_pCfgObject->GetInt( CFGTYPE_DL, _T( "GUI" ), _T( "Manual" ), 0 );
    m_bNeedPassword = m_pCfgObject->GetInt( CFGTYPE_DL, _T( "GUI" ), _T( "NeedPassword" ), 0 );
    m_bCheckDriverVer = m_pCfgObject->GetInt( CFGTYPE_DL, _T( "Settings" ), _T( "CheckDriverVer" ), 1 );
    m_nEnumPortTimeOut = m_pCfgObject->GetInt( CFGTYPE_DL, _T( "SecondEnumPort" ), _T( "EnumPortTimeOut" ), 60000 );
    m_bFilterPort = m_pCfgObject->GetInt( CFGTYPE_DL, _T( "PortSetting" ), _T( "EnableFilter" ), TRUE );
    m_strFileFilter = m_pCfgObject->GetString( CFGTYPE_DL, _T( "GUI" ), _T( "FileFilter" ), _T( "1,2" ) );
    m_bStopDownloadIfOldMemory = m_pCfgObject->GetInt( CFGTYPE_BMFT, _T( "Misc" ), _T( "StopDownloadIfOldMemory" ), 0 );
    m_strEnumPortErrorMsg = m_pCfgObject->GetString( CFGTYPE_DL, _T( "SecondEnumPort" ), _T( "EnumPortErrorMsg" ), _T( "The second enumemration port timeout" ) );
    m_bScriptCtrl = m_pCfgObject->GetInt( CFGTYPE_DL, _T( "GUI" ), _T( "ScriptControl" ), 0 );
    m_bShowMcpTypePage = m_pCfgObject->GetInt( CFGTYPE_DL, _T( "GUI" ), _T( "ShowMcpTypePage" ), 1 );
    m_bNeedPhaseCheck = m_pCfgObject->GetInt( CFGTYPE_DL, _T( "SN" ), _T( "NeedPhaseCheck" ), 1 );
    m_bShowRate = m_pCfgObject->GetInt( CFGTYPE_DL, _T( "GUI" ), _T( "ShowRate" ), FALSE );
    m_bPortSecondEnum = m_pCfgObject->GetInt( CFGTYPE_DL, _T( "Settings" ), _T( "EnablePortSecondEnum" ), FALSE );
    m_bPowerManage = m_pCfgObject->GetInt( CFGTYPE_DL, _T( "GUI" ), _T( "PowerManageFlag" ), FALSE );
    m_bPMInDLProcess = m_pCfgObject->GetInt( CFGTYPE_DL, _T( "GUI" ), _T( "InDLProcess" ), TRUE );
    m_bKeepPacNVState = m_pCfgObject->GetInt( CFGTYPE_DL, _T( "GUI" ), _T( "KeepPacNVState" ), 0 );
    m_bShowSafetyTips = m_pCfgObject->GetInt( CFGTYPE_DL, _T( "GUI" ), _T( "ShowSecurityTips" ), TRUE );
    m_strSafetyTipsMsg = m_pCfgObject->GetString( CFGTYPE_DL, _T( "GUI" ), _T( "SafetyTipsMsg" ), _T( "" ) );
    
    m_attrStackSize.u32ProductName = m_pCfgObject->GetInt( CFGTYPE_DLFW, _T( "DLFW_ATTR_STACK_SIZE" ), _T( "ProductName" ), 20 );
    m_attrStackSize.u32ProductInfo = m_pCfgObject->GetInt( CFGTYPE_DLFW, _T( "DLFW_ATTR_STACK_SIZE" ), _T( "ProductInfo" ), 200 );
    m_attrStackSize.u32DlImageInfo = m_pCfgObject->GetInt( CFGTYPE_DLFW, _T( "DLFW_ATTR_STACK_SIZE" ), _T( "DlImageInfo" ), 200 );
    m_attrStackSize.u32BackupNvItems = m_pCfgObject->GetInt( CFGTYPE_DLFW, _T( "DLFW_ATTR_STACK_SIZE" ), _T( "BackupNvItems" ), 200 );
    m_attrStackSize.u32BackupFiles = m_pCfgObject->GetInt( CFGTYPE_DLFW, _T( "DLFW_ATTR_STACK_SIZE" ), _T( "BackupFiles" ), 50 );
    m_attrStackSize.u32LCDConfig = m_pCfgObject->GetInt( CFGTYPE_DLFW, _T( "DLFW_ATTR_STACK_SIZE" ), _T( "LCDConfig" ), 50 );
    m_attrStackSize.u32MultiLanguage = m_pCfgObject->GetInt( CFGTYPE_DLFW, _T( "DLFW_ATTR_STACK_SIZE" ), _T( "MultiLanguage" ), 100 );
    m_attrStackSize.u32ProjectConfig = m_pCfgObject->GetInt( CFGTYPE_DLFW, _T( "DLFW_ATTR_STACK_SIZE" ), _T( "ProjectConfig" ), 50 );
    
    {
        CString strFilterPort = m_pCfgObject->GetString( CFGTYPE_DL, _T( "PortSetting" ), _T( "FilterPort" ), _T( "" ) );
        m_mapFilterPort.RemoveAll();
        CUtility utility;
        INT nCount = 0;
        LPTSTR* lppString = utility.GetTokenString( strFilterPort.operator LPCWSTR(), _T( "," ), nCount );
        if ( NULL != lppString && 0 != nCount )
        {
            for ( int i = 0; i < nCount; i++ )
            {
                int nPort = _tstoi( lppString[i] );
                if ( nPort > 0 )
                {
                    m_mapFilterPort.SetAt( nPort, nPort );
                }
            }
        }
    }
    
    return TRUE;
}