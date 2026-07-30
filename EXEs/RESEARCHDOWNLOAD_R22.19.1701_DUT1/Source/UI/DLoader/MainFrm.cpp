// MainFrm.cpp : implementation of the CMainFrame class
//
#include "stdafx.h"
#include "DLoader.h"
#include "XAboutDlg.h"
#include "MainFrm.h"
#include "DLoaderView.h"
#include "BarcodeDlg.h"
#include <atlconv.h>
#pragma warning(push,3)
#include <vector>
#include <algorithm>
#pragma warning(pop)

#include "DlgPassword.h"
#include "Utility.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define OPR_SUCCESS         (0)
#define OPR_FAIL            (1)
#define   DETECT_TIMER   0x100000
//////////////////////////////////////////////////////////////////////////
static HWND g_hLayer1TesterHwnd = NULL;
static BOOL CALLBACK EnumWindowsProc( HWND hParent, LPARAM lParam )
{
    UNREFERENCED_PARAMETER( lParam );
    if ( NULL == hParent )
    {
        return FALSE;
    }
    else
    {
        _TCHAR szTitle[_MAX_PATH] = {0};
        ::GetWindowText( hParent, szTitle, _MAX_PATH );
        if ( NULL != _tcsstr( szTitle, _T( "Layer1Tester" ) ) )
        {
            g_hLayer1TesterHwnd = hParent;
            return FALSE;
        }
    }
    return TRUE;
}

int CALLBACK BrowseCallbackProc( HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData )
{
    switch ( uMsg )
    {
    case BFFM_INITIALIZED:    //初始化消息
        //::SendMessage(hwnd,BFFM_SETSELECTION,TRUE,(LPARAM)"C://Program Files");   //传递默认打开路径 （方法一）
        ::SendMessage( hwnd, BFFM_SETSELECTION, TRUE, lpData );      //传递默认打开路径 （方法二，前提是lpData提前设置好）
        
        break;
    case BFFM_SELCHANGED:    //选择路径变化，
        {
            char curr[2560];
            SHGetPathFromIDList( ( LPCITEMIDLIST )lParam, ( LPWSTR )curr );
            ::SendMessage( hwnd, BFFM_SETSTATUSTEXT, 0, ( LPARAM )curr );
        }
        break;
    default:
        break;
    }
    
    return 0;
}

HWND g_hWndFrame = NULL;
/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE( CMainFrame, CFrameWnd )

BEGIN_MESSAGE_MAP( CMainFrame, CFrameWnd )
    //{{AFX_MSG_MAP(CMainFrame)
    ON_WM_CREATE()
    ON_WM_TIMER()
    ON_WM_CLOSE()
    ON_WM_HELPINFO()
    ON_COMMAND( ID_SETTINGS, OnSettings )
    ON_COMMAND( ID_START, OnStart )
    ON_COMMAND( ID_STOP, OnStop )
    ON_COMMAND( ID_LOAD_PACKET, OnLoadPacket )
    ON_COMMAND( ID_OPEN_LOG, OnOpenLog )
    ON_COMMAND( ID_RELOAD_SETTING, OnReloadSetting )
    ON_UPDATE_COMMAND_UI( ID_START, OnUpdateStart )
    ON_UPDATE_COMMAND_UI( ID_STOP, OnUpdateStop )
    ON_UPDATE_COMMAND_UI( ID_SETTINGS, OnUpdateSettings )
    ON_UPDATE_COMMAND_UI( ID_LOAD_PACKET, OnUpdateLoadPacket )
    ON_CBN_SELCHANGE( ID_ISP_LOG_LEVEL, OnSelIspLogLevel )
    ON_CBN_SELCHANGE( ID_LOAD_POLICY_LEVEL, OnSelLoadPolicy )
    ON_MESSAGE( WM_WARN_MESSAGEBOX, OnWarnMessageBox )
    ON_MESSAGE( WM_INIT_PACKET, OnInitalPacket )
    ON_MESSAGE( WM_STOP_AUTODLOADER, OnStopAutoDloader )
    ON_MESSAGE( WM_STOP_ONE_PORT, OnStopOnePort )
    ON_MESSAGE( WM_POWER_MANAGE, OnPowerManage )
    ON_MESSAGE( WM_DWONLOAD_START, OnStartDownload )
    ON_MESSAGE( WM_USER_HELP, OnHelp )
    ON_MESSAGE( WM_BINPAC_PROG_MSG, OnProgressMsg )
    ON_BN_CLICKED( IDC_FILE_MODE, OnCheckFileMode )
    ON_MESSAGE( WM_TOOL_BAR_TEXT, &CMainFrame::OnToolBarText )
    ON_MESSAGE( WM_MUTI_SOFTWARE_CONFIG, &CMainFrame::OnMutiSoftwareConfig )

#ifdef _RESEARCH
    ON_MESSAGE( VM_PRELOAD_CONFIG, &CMainFrame::OnPreloadSelectConfig )
#endif

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame() : m_sheetSettings( IDS_DL_SETTINGS )
{
    m_bStarted              = FALSE;
    m_plStartNVRef          = NULL;
    m_bPacketOpen           = FALSE;
    m_bPacketInit           = FALSE;
    m_strPacketPath         = _T( "" );
    m_bFailedMsgboxShowed   = FALSE;
    m_bPacLoading           = FALSE;
    m_bCheckFileMode        = FALSE;
    m_strPacDirBase         = _T( "" );
    m_bUserIntervene        = FALSE;
}

CMainFrame::~CMainFrame()
{

}


int CMainFrame::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
    if ( CFrameWnd::OnCreate( lpCreateStruct ) == -1 )
    {
        return -1;
    }
    
    if ( !OnCreateToolBar() )
    {
        return -1;
    }
    
    if ( !OnCreateStatusBar() )
    {
        return -1;
    }
    
    SetMenu( NULL );
    
    g_hWndFrame = m_hWnd;
    m_fontStatusBarCtrl.CreateFont( 18, 0, 0, 0, 0, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_SWISS, _T( "Arial" ) );
    InitializeCriticalSection( &m_csLock );
    
    string_t strPath = g_theApp.m_strAppPath.operator LPCWSTR();
    if ( !m_fnDLFWDrv.Startup( strPath ) )
    {
        MessageBox( _T( "CDLFWDriver Startup Fail." ) );
        return -1;
    }
    
    m_pFrameHelper = new CFrameHelper( &m_fnDLFWDrv );
    if ( NULL == m_pFrameHelper )
    {
        MessageBox( _T( "Creat CFrameHelper Fail." ) );
        return -1;
    }
    
    m_pFrameHelper->LoadCfgDLFramework();
    SPRESULT spRlt = m_pFrameHelper->Startup();
    if ( SP_OK != spRlt )
    {
        MessageBox( _T( "DLFramework Startup Fail." ) );
        return -1;
    }
    
    spRlt = m_pFrameHelper->SetProperty( DLFW_ATTR_START_PATH, 0, ( LPVOID )strPath.c_str() );
    if ( SP_OK != spRlt )
    {
        MessageBox( _T( "Set Start Path to DLFramework Fail." ) );
        return -1;
    }
    
    spRlt = m_pFrameHelper->GetProperty( DLFW_ATTR_CONFIG_OBJECT, 0, &g_theApp.m_pCfgObject );
    if ( SP_OK != spRlt || NULL == g_theApp.m_pCfgObject )
    {
        MessageBox( _T( "Get Config Object from DLFramework Fail." ) );
        return -1;
    }
    
    spRlt = m_pFrameHelper->GetProperty( DLFW_ATTR_START_NV_REF, 0, &m_plStartNVRef );
    if ( SP_OK != spRlt || NULL == m_plStartNVRef )
    {
        MessageBox( _T( "Get StartNVRef from DLFramework Fail." ) );
        return -1;
    }
    
    if ( !LoadSetting() )
    {
        MessageBox( _T( "Failed to get configuration parameters." ) );
        return -1;
    }
    
    OnCreateMesInfo();
    OnCreatePacInfo();
    OnCreateOpenLog();
    OnCreateIspLogLevel();
#ifdef _RESEARCH
    OnCreateFileMode();
    OnCreateReload();
    OnCreateLoadPolicy();
#endif
    
    SetDLTitle();
    PostMessage( WM_INIT_PACKET );
    
    return 0;
}

BOOL CMainFrame::PreCreateWindow( CREATESTRUCT& cs )
{
    if ( !CFrameWnd::PreCreateWindow( cs ) )
    { return FALSE; }
    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs
    cs.style &= ~FWS_ADDTOTITLE;
    
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
    CFrameWnd::AssertValid();
}

void CMainFrame::Dump( CDumpContext& dc ) const
{
    CFrameWnd::Dump( dc );
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnSettings()
{
    if ( g_theApp.m_bNeedPassword )
    {
        CDlgPassword dlgPW;
        if ( dlgPW.DoModal() == IDCANCEL )
        {
            return;
        }
    }
    m_bUserIntervene = FALSE;
    m_pFrameHelper->OpenLog();
    m_TaskHelperManage.CloseAll();
    if ( m_sheetSettings.GetProperty() )
    {
        INT_PTR nRlt = m_sheetSettings.DoModal();
        if ( IDOK == nRlt )
        {
            //同步UI层修改的参数
            if ( LoadSetting() )
            {
                //同步DLFramework层修改的参数
                if ( SP_OK == m_pFrameHelper->ReloadSettings() )
                {
                    //同步非INI文件(内存中的数据)的参数到DLFramework层修改的参数
                    if ( m_sheetSettings.SetProperty() )
                    {
                        //同步Frame的参数到Task中
                        if ( SP_OK == m_pFrameHelper->SyncParameters() )
                        {
                            m_pFrameHelper->GetProperty( DLFW_ATTR_CHECK_DL_FILES, 0, &m_bPacketInit );
                        }
                    }
                }
            }
        }
        m_sheetSettings.ReleaseProperty();
        SetDLTitle();
    }
}

void CMainFrame::OnStart()
{
    if ( g_theApp.m_bShowSafetyTips && !g_theApp.m_strSafetyTipsMsg.IsEmpty() && !g_theApp.m_bScriptCtrl )
    {
        if ( MessageBox( g_theApp.m_strSafetyTipsMsg.operator LPCTSTR(), _T( "Warning" ), MB_YESNO | MB_ICONWARNING ) == IDNO )
        {
            return;
        }
    }
    
    ( ( CDLoaderView* )GetActiveView() )->InitFixedProg();
    
    if ( SP_OK == m_pFrameHelper->DeviceMoniter( TRUE ) )
    {
        m_bStarted = TRUE;
        m_wndToolBar.GetToolBarCtrl().EnableButton( ID_START, FALSE );
        m_wndToolBar.GetToolBarCtrl().EnableButton( ID_SETTINGS, FALSE );
        m_wndToolBar.GetToolBarCtrl().EnableButton( ID_LOAD_PACKET, FALSE );
        return;
    }
}

BOOL CMainFrame::OnCreateToolBar()
{
    if ( !m_wndToolBar.CreateEx( this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
                                 | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC ) ||
         !m_wndToolBar.LoadToolBar( IDR_MAINFRAME ) )
    {
        TRACE0( "Failed to create toolbar\n" );
        return FALSE;      // fail to create
    }
    
    UINT uStyle = m_wndToolBar.GetButtonStyle( 1 );
    uStyle |= BS_FLAT;
    m_wndToolBar.SetButtonStyle( 1, uStyle );
    //  just for pc-lint
    UNUSED_ALWAYS( IDB_BMP_TB_FACTORY );
    UNUSED_ALWAYS( IDB_BMP_TB_FACTORY_DIS );
    UNUSED_ALWAYS( IDB_BMP_TB_FACTORY_HOT );
    UNUSED_ALWAYS( IDB_BMP_TB_UPGRADE );
    UNUSED_ALWAYS( IDB_BMP_TB_UPGRADE_DIS );
    UNUSED_ALWAYS( IDB_BMP_TB_UPGRADE_HOT );
    UNUSED_ALWAYS( IDB_BMP_TOOLBAR );
    UNUSED_ALWAYS( IDB_BMP_TOOLBAR_DIS );
    UNUSED_ALWAYS( IDB_BMP_TOOLBAR_HOT );
    
#ifdef _SPUPGRADE
    m_wndToolBar.SetBitmaps( IDB_BMP_TB_FACTORY, IDB_BMP_TB_FACTORY_DIS, IDB_BMP_TB_FACTORY_HOT );
#elif defined _FACTORY
    m_wndToolBar.SetBitmaps( IDB_BMP_TB_UPGRADE, IDB_BMP_TB_UPGRADE_DIS, IDB_BMP_TB_UPGRADE_HOT );
#else
    m_wndToolBar.SetBitmaps( IDB_BMP_TOOLBAR, IDB_BMP_TOOLBAR_DIS, IDB_BMP_TOOLBAR_HOT );
#endif
    
    return TRUE;
}

BOOL CMainFrame::OnCreateStatusBar()
{
    static UINT indicators[] =
    {
        ID_SEPARATOR,
        ID_SEPARATOR,
        ID_SEPARATOR
    };
    
    if ( !m_wndStatusBar.Create( this ) ||
         !m_wndStatusBar.SetIndicators( indicators, sizeof( indicators ) / sizeof( UINT ) ) )
    {
        TRACE0( "Failed to create status bar\n" );
        return FALSE;
    }
    
    m_wndStatusBar.GetStatusBarCtrl().SetMinHeight( 28 );
    
    LOGFONT logfont;
    GetObject( ::GetStockObject( DEFAULT_GUI_FONT ), sizeof( logfont ), &logfont );
    logfont.lfWeight = FW_NORMAL;
    logfont.lfHeight = 18;
    _tcscpy_s( logfont.lfFaceName, _T( "Arial" ) );
    static CFont statusBarFont;
    statusBarFont.CreateFontIndirect( &logfont );
    logfont.lfPitchAndFamily = FF_SWISS;
    m_wndStatusBar.SetFont( &statusBarFont );
    
    m_wndStatusBar.SetPaneInfo( E_STATUSBAR_READY, ID_SEPARATOR, SBPS_STRETCH | SBPS_NOBORDERS, 0 );
    m_wndStatusBar.SetPaneInfo( E_STATUSBAR_CTRL, ID_SEPARATOR, SBPS_NORMAL, 65 );
    m_wndStatusBar.SetPaneInfo( E_STATUSBAR_LOGO, ID_SEPARATOR, SBPS_NORMAL, 60 );
    
    SetStatusBarText( E_STATUSBAR_READY, _T( "Ready" ) );
    SetStatusBarText( E_STATUSBAR_CTRL, _T( "" ) );
    SetStatusBarText( E_STATUSBAR_LOGO, _T( "UNISOC" ) );
    
    return TRUE;
}

void CMainFrame::OnCreatePacInfo()
{
    CRect rcStatic;
    int nIndex = m_wndToolBar.CommandToIndex( ID_SPD_VERSION );
    m_wndToolBar.GetItemRect( nIndex, &rcStatic );
    rcStatic.top += 2;
    rcStatic.bottom -= 2;
    rcStatic.right = rcStatic.left + 700;
    m_StaticPacInfo.Create( _T( "" ), WS_CHILD | WS_VISIBLE | SS_LEFTNOWORDWRAP, rcStatic, &m_wndToolBar, ID_SPD_PAC_INFO );
    m_StaticPacInfo.SetWindowText( _T( "" ) );
    m_StaticPacInfo.ShowWindow( SW_SHOWNA );
    m_wndToolBar.GetToolBarCtrl().EnableButton( ID_SPD_VERSION, TRUE );
    
    m_StaticPacInfo.SetFontSize( 20 );
}


void CMainFrame::OnCreateMesInfo()
{
    if ( m_fnMesLoginHelper.MesEnable() )
    {
        m_fnMesLoginHelper.MesLogin();
        LPTSTR lpMesInfo = new TCHAR[256];
        ZeroMemory( lpMesInfo, sizeof( TCHAR ) * 256 );
        m_fnMesLoginHelper.MesGetInfo( lpMesInfo, 256 );
        
        CRect rcStatic;
        int nIndex = m_wndToolBar.CommandToIndex( ID_SPD_VERSION );
        m_wndToolBar.GetItemRect( nIndex, &rcStatic );
        rcStatic.left += 700;
        rcStatic.right += 1700;
        rcStatic.top -= 2;
        rcStatic.bottom += 2;
        m_StaticMesInfo.Create( _T( "" ), WS_CHILD | WS_VISIBLE | SS_LEFTNOWORDWRAP, rcStatic, &m_wndToolBar, ID_SPD_MES_INFO );
        m_StaticMesInfo.SetWindowText( _T( "" ) );
        m_StaticMesInfo.ShowWindow( SW_SHOWNA );
        m_wndToolBar.GetToolBarCtrl().EnableButton( ID_SPD_MES_INFO, TRUE );
        
        m_StaticMesInfo.SetFontSize( 16 );
        m_StaticMesInfo.SetBKGColor( RGB( 0, 255, 255 ) );
        m_StaticMesInfo.SetTextColor( RGB( 0, 0, 0 ) );
        
        m_StaticMesInfo.SetText( lpMesInfo );
        delete[]lpMesInfo;
    }
}

void CMainFrame::OnCreateOpenLog()
{
    m_CtrlBtnOpenLog.Create( _T( "" ), WS_VISIBLE | WS_CHILD | BS_VCENTER | BS_ICON, m_tStatusBarRect.OpenLogBtn, &m_wndStatusBar, ID_OPEN_LOG );
    HINSTANCE hInstResource = AfxFindResourceHandle( MAKEINTRESOURCE( IDI_OPEN_LOG ), RT_GROUP_ICON );
    HICON hIcon = ( HICON )::LoadImage( hInstResource, MAKEINTRESOURCE( IDI_OPEN_LOG ), IMAGE_ICON, 28, 28, 0 );
    m_CtrlBtnOpenLog.SetIcon( hIcon );
}

void CMainFrame::OnCreateReload()
{
    m_CtrlBtnReload.Create( _T( "" ), WS_VISIBLE | WS_CHILD | BS_VCENTER | BS_ICON, m_tStatusBarRect.ReloadBtn, &m_wndStatusBar, ID_RELOAD_SETTING );
    HINSTANCE hInstResource = AfxFindResourceHandle( MAKEINTRESOURCE( IDI_RELOAD ), RT_GROUP_ICON );
    HICON hIcon = ( HICON )::LoadImage( hInstResource, MAKEINTRESOURCE( IDI_RELOAD ), IMAGE_ICON, 28, 28, 0 );
    m_CtrlBtnReload.SetIcon( hIcon );
}

void CMainFrame::OnOpenLog()
{
    CString strLogDir = g_theApp.m_strAppPath;
    strLogDir += _T( "Log" );
    ShellExecute( NULL, _T( "open" ), NULL, NULL, strLogDir, SW_SHOWNORMAL );
}

void CMainFrame::OnReloadSetting()
{
    /*
    if ( m_bStarted )
    {
        AfxMessageBox(_T("Stop downloading and Reload Setting."));
        return;
    }
    */
    
    m_pFrameHelper->OpenLog();
    SPRESULT spRlt = SP_E_INVALID_PARAMETER;
    if ( SP_OK == m_pFrameHelper->ReloadSettings() )
    {
        //同步Frame的参数到Task中
        if ( SP_OK == m_pFrameHelper->SyncParameters() )
        {
            if ( m_bPacketOpen )
            {
                spRlt = m_pFrameHelper->GetProperty( DLFW_ATTR_CHECK_DL_FILES, 0, &m_bPacketInit );
            }
        }
    }
    if ( m_bPacketOpen && ( SP_OK != spRlt || !m_bPacketInit ) )
    {
        if ( AfxMessageBox( _T( "Reload Setting failed.\nDo you need to open Log to confirm the cause?" ), MB_YESNO | MB_ICONINFORMATION ) == IDYES )
        {
            OnOpenLog();
        }
    }
}

void CMainFrame::OnCreateIspLogLevel()
{
    m_CtrlStaticIspLogLevel.Create( _T( "LogLevel : " ), WS_VISIBLE | WS_CHILD | SS_LEFTNOWORDWRAP, m_tStatusBarRect.LevelStatic, &m_wndStatusBar, ID_ISP_LOG_TEXT );
    
    m_CtrlComboBoxIspLogLevel.Create( WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL | CBRS_BOTTOM, m_tStatusBarRect.LevelComboBox, &m_wndStatusBar, ID_ISP_LOG_LEVEL );
    m_CtrlComboBoxIspLogLevel.AddString( _T( "0" ) );
    m_CtrlComboBoxIspLogLevel.AddString( _T( "1" ) );
    m_CtrlComboBoxIspLogLevel.AddString( _T( "2" ) );
    m_CtrlComboBoxIspLogLevel.AddString( _T( "3" ) );
    m_CtrlComboBoxIspLogLevel.AddString( _T( "4" ) );
    m_CtrlComboBoxIspLogLevel.AddString( _T( "5" ) );
    
    CString strPath = g_theApp.m_strAppPath;
    strPath += _T( "App\\DLFramework.ini" );
    
    int nLogLevel = GetPrivateProfileInt( _T( "Log" ), _T( "Level" ), SPLOGLV_ERROR, strPath );
    
    m_CtrlComboBoxIspLogLevel.SetCurSel( nLogLevel );
    
    m_CtrlStaticIspLogLevel.SetFont( &m_fontStatusBarCtrl, true );
    m_CtrlComboBoxIspLogLevel.SetFont( &m_fontStatusBarCtrl, true );
}

void CMainFrame::OnCreateLoadPolicy()
{
    m_CtrlStaticLoadPolicy.Create( _T( "LoadPolicy : " ), WS_VISIBLE | WS_CHILD | SS_LEFTNOWORDWRAP, m_tStatusBarRect.LoadPolicyStatic, &m_wndStatusBar, ID_LOAD_POLICY_TEXT );
    m_CtrlComboBoxLoadPolicy.Create( WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL | CBRS_BOTTOM, m_tStatusBarRect.LoadPolicyComboBox, &m_wndStatusBar, ID_LOAD_POLICY_LEVEL );
    m_CtrlStaticLoadPolicy.SetFont( &m_fontStatusBarCtrl );
    m_CtrlComboBoxLoadPolicy.SetFont( &m_fontStatusBarCtrl );
    
    m_CtrlComboBoxLoadPolicy.AddString( _T( "0" ) );
    m_CtrlComboBoxLoadPolicy.AddString( _T( "1" ) );
    m_CtrlComboBoxLoadPolicy.AddString( _T( "2" ) );
    
    int nLoadPolicy = g_theApp.m_pCfgObject->GetInt( CFGTYPE_BINP, _T( "Setting" ), _T( "LoadPolicy" ), 0 );
    
    m_CtrlComboBoxLoadPolicy.SetCurSel( nLoadPolicy );
}


void CMainFrame::OnSelIspLogLevel()
{
    int nLogLevel = m_CtrlComboBoxIspLogLevel.GetCurSel();
    
    CString strPath = g_theApp.m_strAppPath;
    strPath += _T( "App\\DLFramework.ini" );
    
    _TCHAR szLevel[MAX_PATH] = { 0 };
    _stprintf_s( szLevel, _T( "%d" ), nLogLevel );
    WritePrivateProfileString( _T( "Log" ), _T( "Level" ), szLevel, strPath );
}

void CMainFrame::OnSelLoadPolicy()
{
    int nLoadPolicy = m_CtrlComboBoxLoadPolicy.GetCurSel();
    g_theApp.m_pCfgObject->SetInt( CFGTYPE_BINP, _T( "Setting" ), _T( "LoadPolicy" ), nLoadPolicy );
}

void CMainFrame::OnStop()
{
    m_pFrameHelper->DeviceMoniter( FALSE );
    m_TaskHelperManage.StopAll();
    ( ( CDLoaderView* )GetActiveView() )->StopDLTimer();
    ( ( CDLoaderView* )GetActiveView() )->RemoveAll();
    m_bStarted = FALSE;
}

void CMainFrame::OnUpdateStart( CCmdUI* pCmdUI )
{
    pCmdUI->Enable( m_bPacketOpen && !m_bStarted && !m_bUserIntervene );
}

void CMainFrame::OnUpdateStop( CCmdUI* pCmdUI )
{
#if defined( _FACTORY ) || defined(_SPUPGRADE)
    //not allowed to click stop button when dealing with NV
    pCmdUI->Enable( m_bStarted && ( *m_plStartNVRef ) == 0 );
#else
    pCmdUI->Enable( m_bStarted );
#endif
}

void CMainFrame::OnClose()
{
    if ( m_bPacLoading )
    {
        return;
    }
    if ( m_bStarted )
    {
        CString str;
        str.LoadString( IDS_QUIT_TEST );
        AfxMessageBox( str );
    }
    else
    {
        m_TaskHelperManage.CloseAll();
        m_pFrameHelper->Cleanup();
        if ( NULL != m_pFrameHelper )
        {
            delete m_pFrameHelper;
            m_pFrameHelper = NULL;
        }
        m_fnDLFWDrv.Cleanup();
        
        DeleteCriticalSection( &m_csLock );
        ShowWindow( SW_HIDE );
        CFrameWnd::OnClose();
    }
}

void CMainFrame::OnUpdateSettings( CCmdUI* pCmdUI )
{
    pCmdUI->Enable( m_bPacketOpen && !m_bStarted ); //lint !e730
}

LRESULT CMainFrame::WindowProc( UINT message, WPARAM wParam, LPARAM lParam )
{
    return CFrameWnd::WindowProc( message, wParam, lParam );
    // TODO: Add your specialized code here and/or call the base class
}

/* 为了防止线程调用回调函数时使用AfxMessageBox发生阻塞
* 该函数主要在回调函数中使用PostMessage调用
* lpParam为对话框显示的文本字符串指针
*/
LRESULT CMainFrame::OnWarnMessageBox( WPARAM wParam, LPARAM lpParam )
{
    if ( CBK_STRING_WARN_ERASE == wParam )
    {
        if ( AfxMessageBox( ( LPCTSTR )lpParam, MB_YESNO ) == IDNO )
        {
            m_bUserIntervene = TRUE;
        }
        else
        {
            m_bUserIntervene = FALSE;
        }
    }
    else if ( !m_bFailedMsgboxShowed )
    {
        DWORD dwPort = wParam;
        CString strWarnMsg;
        if ( lpParam != NULL )
        {
            strWarnMsg = ( LPCTSTR )lpParam;
        }
        else
        {
            strWarnMsg.Format( _T( "Port[%d] is failed, please check!!!" ), dwPort );
        }
        m_bFailedMsgboxShowed = TRUE;
        MessageBox( strWarnMsg, _T( "Warning" ), MB_OK | MB_ICONWARNING );
        m_bFailedMsgboxShowed = FALSE;
    }
    
    return 0L;
}

BOOL CMainFrame::OnHelpInfo( HELPINFO* pHelpInfo )
{
    UNUSED_ALWAYS( pHelpInfo );
    // TODO: Add your message handler code here and/or call default
    return TRUE;
    //return CFrameWnd::OnHelpInfo(pHelpInfo);
}

BOOL CMainFrame::GetFilePathByDialog( BOOL bPackets )
{
    if ( bPackets )
    {
#ifdef _RESEARCH
        static _TCHAR BASED_CODE szFilter[] = _T( "Packet Files (*.pac;*.bin)|*.pac;*.bin|All Files(*.*)|*.*||" );
#else
        static _TCHAR BASED_CODE szFilter[] = _T( "Packet Files (*.pac)|*.pac|All Files(*.*)|*.*||" );
#endif // DEBUG
        CString strFile = _T( "" );
        CFileDialog dlg( TRUE, _T( ".pac" ), strFile, OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, szFilter, NULL );
        if ( dlg.DoModal() == IDCANCEL )
        {
            return FALSE;
        }
        m_strPacketPath = dlg.GetPathName();
    }
    else
    {
        CString strPath = g_theApp.m_strAppPath + _T( "UnpackedImgs" );
        
        BROWSEINFO br;
        br.hwndOwner = this->GetSafeHwnd();
        br.pidlRoot = 0;
        br.pszDisplayName = 0;
        br.lpszTitle = _T( "Select File Path" );
        br.ulFlags = BIF_STATUSTEXT;
        br.lpfn = BrowseCallbackProc;
        br.iImage = 0;
        br.lParam = long( strPath.operator LPCWSTR() );
        
        ITEMIDLIST* pItem = SHBrowseForFolder( &br );
        if ( pItem == NULL )
        {
            return FALSE;
        }
        
        TCHAR szPath[MAX_PATH] = {0};
        if ( SHGetPathFromIDList( pItem, szPath ) == TRUE )
        {
            CString strTemp = _T( "\\*" );
            m_strFilePath = szPath + strTemp;
        }
    }
    return TRUE;
}

BOOL CMainFrame::GetFilePathByIni( BOOL bPackets )
{
    if ( bPackets )
    {
        CString strPathPac = g_theApp.m_pCfgObject->GetString( CFGTYPE_DL, _T( "Download_Packet" ), _T( "packet" ), _T( "" ) );
        if ( strPathPac.GetLength() == 0 )
        {
            return FALSE;
        }
        
        CString strAbsolutePac;
        TCHAR szDirve[_MAX_DRIVE] = { 0 };
        _tsplitpath( strPathPac, szDirve, NULL, NULL, NULL );
        if ( szDirve[0] != _T( '\0' ) ||
             ( strAbsolutePac.GetLength() > 2 &&
               strAbsolutePac[0] == _T( '\\' ) &&
               strAbsolutePac[1] == _T( '\\' ) ) )
        {
            strAbsolutePac = strPathPac;
        }
        else
        {
            strAbsolutePac.Format( _T( "%s\\%s" ), g_theApp.m_strAppPath.operator LPCWSTR(), strPathPac.operator LPCWSTR() );
        }
        
        CFileFind finder;
        if ( !finder.FindFile( strAbsolutePac ) )
        {
            return FALSE;
        }
        
        BOOL bShowConfirmDialog = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, _T( "Download_Packet" ), _T( "ShowConfirmDialog" ), 0 );
        if ( !g_theApp.m_bScriptCtrl && bShowConfirmDialog )
        {
            CString strComfirm;
            strComfirm.Format( _T( "Load latest pac file?\n\n%s" ), strAbsolutePac.operator LPCWSTR() );
            if ( AfxMessageBox( strComfirm.operator LPCTSTR(), MB_YESNO | MB_ICONINFORMATION ) == IDNO )
            {
                return FALSE;
            }
        }
        m_strPacketPath = strAbsolutePac;
    }
    else
    {
        m_strFilePath = g_theApp.m_pCfgObject->GetString( CFGTYPE_DL, _T( "Download_Packet" ), _T( "Floader" ), _T( "" ) );
        CString strPath( m_strFilePath );
        strPath = strPath.Left( strPath.GetLength() - 2 );
        if ( !PathIsDirectory( strPath ) )
        {
            return FALSE;
        }
    }
    return TRUE;
}

void CMainFrame::OnLoadPacket()
{
    // TODO: Add your command handler code here
    if ( g_theApp.m_bNeedPassword )
    {
        CDlgPassword dlgPW;
        if ( dlgPW.DoModal() == IDCANCEL )
        {
            return;
        }
    }
    
    m_TaskHelperManage.CloseAll();
    m_sheetSettings.m_pageVolFreq.Init();
#ifdef _RESEARCH
    if ( m_bCheckFileMode )
    {
        if ( GetFilePathByDialog( FALSE ) )
        {
            __LoadPackets( FALSE );
        }
        return;
    }
#endif
    if ( GetFilePathByDialog() )
    {
        __LoadPackets();
    }
}

BOOL CMainFrame::LoadSetting()
{
    if ( NULL == g_theApp.m_pCfgObject )
    {
        ASSERT( 0 );
        return FALSE;
    }
    CHKBOOL( g_theApp.LoadSetting() );
    CHKBOOL( __LoadSetting() );
    CHKBOOL( m_sheetSettings.LoadSettings() );
    return TRUE;
}

BOOL CMainFrame::__LoadSetting()
{
    m_strPacDirBase = g_theApp.m_pCfgObject->GetString( CFGTYPE_DL, _T( "Download_Packet" ), _T( "UnpacketDir" ), _T( "" ) );
    if ( m_strPacDirBase.CompareNoCase( _T( ".\\" ) ) == 0 )
    {
        m_strPacDirBase = g_theApp.m_strAppPath;
        m_strPacDirBase += _T( "ImageFiles" );
    }
    return TRUE;
}

SPRESULT CMainFrame::__LoadPackets( BOOL bPackets )
{
    SetStatusBarText( E_STATUSBAR_READY, _T( "Loading packet file..." ) );
    
    CWaitCursor wait;
    m_bPacLoading = TRUE;
    m_bPacketOpen = FALSE;
    m_bUserIntervene = FALSE;
    
    /// 20220622
    /// 第二次加载Pac时候，需要禁用Setting和Start按钮；
    m_wndToolBar.GetToolBarCtrl().EnableButton( ID_START, FALSE );
    m_wndToolBar.GetToolBarCtrl().EnableButton( ID_SETTINGS, FALSE );
    m_wndToolBar.GetToolBarCtrl().EnableButton( ID_LOAD_PACKET, FALSE );
    
    CMenu* pSysMenu = GetSystemMenu( FALSE );
    if ( pSysMenu != NULL )
    {
        pSysMenu->EnableMenuItem( SC_CLOSE, MF_BYCOMMAND | MF_DISABLED );
    }
    SPRESULT spRlt = SP_OK;
    
    if ( bPackets )
    {
        spRlt = m_pFrameHelper->LoadPacket( m_strPacketPath.operator LPCWSTR(), m_strPacDirBase.operator LPCWSTR() );
    }
    else
    {
        spRlt = m_pFrameHelper->LoadFiles( m_strFilePath.operator LPCWSTR() );
    }
    
    if ( SP_OK != spRlt )
    {
        if ( pSysMenu != NULL )
        {
            pSysMenu->EnableMenuItem( SC_CLOSE, MF_BYCOMMAND | MF_ENABLED );
        }
        m_bPacLoading = FALSE;
        SetStatusBarText( E_STATUSBAR_READY, _T( "Ready" ) );
        return spRlt;
    }
    m_pFrameHelper->GetProperty( DLFW_ATTR_CHECK_DL_FILES, 0, &m_bPacketInit );
    m_bPacLoading = FALSE;
    if ( pSysMenu != NULL )
    {
        pSysMenu->EnableMenuItem( SC_CLOSE, MF_BYCOMMAND | MF_ENABLED );
    }
    m_bPacketOpen = TRUE;
    
    SetDLTitle();
    SetStatusBarText( E_STATUSBAR_READY, _T( "Ready" ) );
    
    if ( g_theApp.m_bScriptCtrl )
    {
        ::PostMessage( m_hWnd, WM_COMMAND, ID_START, 0 );
    }
    return SP_OK;
}

void CMainFrame::OnUpdateLoadPacket( CCmdUI* pCmdUI )
{
    // TODO: Add your command update UI handler code here
#if defined( _FACTORY ) || defined(_SPUPGRADE)
    pCmdUI->Enable( !m_bPacketOpen || !m_bStarted );  //lint !e730
#else // _DLOADERR
    pCmdUI->Enable( !m_bStarted );  //lint !e730
#endif
}

void CMainFrame::SetToolBarText( LPCTSTR lpText )
{
    if ( lpText != NULL )
    {
        m_StaticPacInfo.SetText( lpText );
        m_StaticPacInfo.Invalidate();
    }
}

void CMainFrame::OnTimer( UINT nIDEvent )
{
    UNUSED_ALWAYS( nIDEvent );
    if ( 1 == nIDEvent )
    {
        KillTimer( nIDEvent );
        keybd_event( VK_RETURN, 0, 0, 0 ); //按键下去
        keybd_event( VK_RETURN, 0, KEYEVENTF_KEYUP, 0 ); //按键上来，模拟"回车"命令
    }
}

LRESULT CMainFrame::OnInitalPacket( WPARAM wParam, LPARAM lpParam )
{
    UNUSED_ALWAYS( wParam );
    UNUSED_ALWAYS( lpParam );
    ASSERT( g_theApp.m_pCfgObject );
    
    BOOL bPacketMode = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, _T( "GUI" ), _T( "PacketMode" ), 1 );
    BOOL bFileMode = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, _T( "GUI" ), _T( "FileMode" ), 0 );
    
#if defined(_SPUPGRADE) || defined(_FACTORY)
    bPacketMode = TRUE;
    bFileMode = FALSE;
#endif
   
#ifdef _RESEARCH
    ShowPartition();
#endif

    if ( bPacketMode )
    {
        if ( GetFilePathByIni() )
        {
            __LoadPackets();
        }
        return 0;
    }
    
    if ( bFileMode )
    {
        if ( GetFilePathByIni( FALSE ) )
        {
            __LoadPackets( FALSE );
        }
        return 0;
    }
    return 1;
}

LRESULT CMainFrame::OnStopOnePort( WPARAM wParam, LPARAM lpParam )
{
    UNUSED_ALWAYS( lpParam );
    CTaskHelper* pTask = m_TaskHelperManage.Find( ( uint16 )wParam );
    if ( NULL != pTask )
    {
        pTask->StopTask();
        pTask->CloseLog();
    }
    return 0;
}

LRESULT CMainFrame::OnPowerManage( WPARAM wParam, LPARAM lpParam )
{
    UNUSED_ALWAYS( wParam );
    UNUSED_ALWAYS( lpParam );
    CString strData;
    if ( lpParam != 0 )
    {
        strData.Format( _T( "DOWNLOAD:POWER-MANAGE:%s" ), lpParam );
    }
    else
    {
        strData = _T( "DOWNLOAD:POWER-MANAGE" );
    }
    
    COPYDATASTRUCT  CopyDataStruct;
    CopyDataStruct.dwData = 0;
    CopyDataStruct.cbData = ( strData.GetLength() + 1 ) * sizeof( TCHAR );
    CopyDataStruct.lpData = ( PVOID )( strData.operator LPCTSTR() );
    
    g_hLayer1TesterHwnd = NULL;
    ::EnumWindows( EnumWindowsProc, 0 );
    if ( NULL != g_hLayer1TesterHwnd )
    {
        ::SendMessage( g_hLayer1TesterHwnd, WM_COPYDATA, ( WPARAM )GetSafeHwnd(), ( long )&CopyDataStruct );
    }
    
    return 0;
}

void CMainFrame::SetDLTitle()
{
    ASSERT( g_theApp.m_pCfgObject );
    
    CString strTile = g_theApp.m_pCfgObject->GetString( CFGTYPE_DL, _T( "GUI" ), _T( "Title" ), _T( "" ) );
    BOOL bShowVer = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, _T( "GUI" ), _T( "ShowVer" ), 1 );
    BOOL bShowPac = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, _T( "GUI" ), _T( "ShowPacPath" ), 0 );
    
    CString strVersion;
    if ( bShowVer )
    {
        CXVerInfo ver;
        strVersion = ver.GetSPRDVersionString();
    }
    
#ifdef _SPUPGRADE
    if ( strTile.GetLength() > 0 )
    {
        strTile.Format( _T( "UpgradeDownload [%s]" ), strTile.operator LPCWSTR() );
    }
    else
    {
        strTile = _T( "UpgradeDownload" );
    }
#elif defined _FACTORY
    if ( strTile.GetLength() > 0 )
    {
        strTile.Format( _T( "FactoryDownload [%s]" ), strTile.operator LPCWSTR() );
    }
    else
    {
        strTile = _T( "FactoryDownload" );
    }
#else
    if ( strTile.GetLength() > 0 )
    {
        strTile.Format( _T( "ResearchDownload [%s]" ), strTile.operator LPCWSTR() );
    }
    else
    {
        strTile = _T( "ResearchDownload" );
    }
#endif
    
    if ( bShowVer )
    {
#ifdef _DEBUG
        strTile += _T( " - D" );
#else
        strTile += _T( " - R" );
#endif
        strTile += strVersion;
    }
    
    if ( bShowPac && !m_strPacketPath.IsEmpty() && m_bPacketOpen )
    {
        CString strTmp;
        strTmp.Format( _T( " - [%s]" ), m_strPacketPath.operator LPCWSTR() );
        strTile += strTmp;
    }
    
    SetWindowText( strTile );
}

SPRESULT CMainFrame::StartDownload( UINT nPort )
{
    SPRESULT spRlt = SP_OK;
    EnterCriticalSection( &m_csLock );
    /// hong.yang 20220630
    /// Close the DLFW log because the log size continues to grow during continuous testing
    m_pFrameHelper->CloseLog();
    ( ( CDLoaderView* )( this->GetActiveView() ) )->StartDLTimer();
    CTaskHelper* pTask = m_TaskHelperManage.Find( ( uint16 )nPort );
    if ( NULL == pTask )
    {
        ( ( CDLoaderView* )( GetActiveView() ) )->ClearAll();
        pTask = new CTaskHelper( &m_fnDLFWDrv );
        pTask->LoadCfgDLFramework();
        spRlt = pTask->CreatTask( nPort );
        if ( SP_OK != spRlt )
        {
            LeaveCriticalSection( &m_csLock );
            return spRlt;
        }
        m_TaskHelperManage.Insert( ( uint16 )nPort, pTask );
        if ( g_theApp.m_bNeedPhaseCheck && g_theApp.m_bManual )
        {
            ::SendMessage( GetActiveView()->GetSafeHwnd(), WM_REQUIRE_SN, nPort, NULL );
        }
    }
    else
    {
        spRlt = pTask->OpenLog( nPort );
        if ( SP_OK != spRlt )
        {
            LeaveCriticalSection( &m_csLock );
            return spRlt;
        }
    }
    /// hong.yang 20220701
    pTask->CopyLog( m_pFrameHelper->GetLogPath() );
    LeaveCriticalSection( &m_csLock );
    
    CHKRESULT( pTask->SetProperty( DLFW_ATTR_LOG_PATH, 0, ( LPVOID )pTask->GetLogPath() ) )
    CHKRESULT( pTask->RunTask( nPort ) );
    return SP_OK;
}

LRESULT CMainFrame::OnStartDownload( WPARAM wParam, LPARAM lpParam )
{
    UNUSED_ALWAYS( lpParam );

    m_tPortInfo.u32Port = wParam;
    ProtInfoVector.push_back( m_tPortInfo );
    
    if ( !g_theApp.m_bManual )
    {
        if ( SP_OK != StartDownload( wParam ) )
        {
            CString strErr = _T( "StartDownload Fail. Pls check the memory usage." );
            BM_DL_END tDlEnd;
            tDlEnd.dwErrCode = 1;
            tDlEnd.lpErrMsg = strErr.operator LPCWSTR();
            ( ( CDLoaderView* )GetActiveView() )->SetResult( wParam, &tDlEnd );
        }
    }
    return 1;
}

void CMainFrame::SetStatusBarText( ENUM_STATUSBAR_INDEX eIndex, LPCTSTR lpText )
{
    m_wndStatusBar.SetPaneText( eIndex, lpText, TRUE );
}

BOOL CMainFrame::PreTranslateMessage( MSG* pMsg )
{
    // TODO: Add your specialized code here and/or call the base class
    if ( pMsg->message == WM_KEYDOWN )
    {
        if ( pMsg->wParam == VK_F1 )
        {
            PostMessage( WM_USER_HELP );
        }
    }
    
    return CFrameWnd::PreTranslateMessage( pMsg );
}

LRESULT CMainFrame::OnHelp( WPARAM wParam, LPARAM lpParam )
{
    UNUSED_ALWAYS( wParam );
    UNUSED_ALWAYS( lpParam );
    TCHAR szPath[MAX_PATH] = {0};
    LPCTSTR lpName = g_theApp.m_pCfgObject->GetString( CFGTYPE_DL, _T( "GUI" ), _T( "UserGuide" ), _T( "" ) );
    _tcscat( szPath, g_theApp.m_strAppPath.operator LPCWSTR() );
    _tcscat( szPath, _T( "..\\Doc\\" ) );
    _tcscat( szPath, lpName );
    CFileFind finder;
    if ( finder.FindFile( szPath ) )
    {
        ShellExecute( this->GetSafeHwnd(), _T( "open" ), szPath, NULL, NULL, SW_SHOW );
    }
    
    return 0L;
}


LRESULT CMainFrame::OnProgressMsg( WPARAM wParam, LPARAM lParam )
{
    CProgressCtrl& prgCtrl = m_wndStatusBar.m_prgrsCtrl;
    switch ( wParam )
    {
    case CBK_PROG_BEGIN:
        prgCtrl.ShowWindow( SW_SHOW );
        prgCtrl.SetRange32( 0, 100 );
        prgCtrl.SetPos( 0 );
        break;
    case CBK_PROG_PROCEED:
        prgCtrl.SetPos( ( DWORD )lParam );
        break;
    case CBK_PROG_END:
        if ( lParam == 100 )
        {
            int nLower = 0;
            int nUpper = 0;
            prgCtrl.GetRange( nLower, nUpper );
            prgCtrl.SetPos( nUpper );
        }
        prgCtrl.ShowWindow( SW_HIDE );
        break;
    default:
        break;
    }
    return 0;
}

LRESULT CMainFrame::OnStopAutoDloader( WPARAM wParam, LPARAM lpParam )
{
    UNUSED_ALWAYS( wParam );
    UNUSED_ALWAYS( lpParam );

    
    for ( std::vector<PORT_INFO>::iterator it = ProtInfoVector.begin(); it != ProtInfoVector.end(); ++it )
    {
        if ( (*it).u32Port == wParam )
        {
            it = ProtInfoVector.erase(it);
            break;
        }
    }

    if ( g_theApp.m_bScriptCtrl && ( ProtInfoVector.size() == 0 ) )
    {
        PostMessage( WM_COMMAND, ID_STOP, 0 );
        PostMessage( WM_CLOSE, 0, 0 );
    }
    return 1;
}

afx_msg LRESULT CMainFrame::OnToolBarText( WPARAM wParam, LPARAM lParam )
{
    UNUSED_ALWAYS( wParam );
    SetToolBarText( ( LPCTSTR )lParam );
    return 0;
}

void CMainFrame::OnCreateFileMode()
{
    m_bCheckFileMode = FALSE;
    m_CtrlStaticFileMode.Create( _T( "LoadImages : " ), WS_VISIBLE | WS_CHILD | SS_LEFTNOWORDWRAP, m_tStatusBarRect.LoadStatic, &m_wndStatusBar, ID_STATIC_FILE_MODE );
    m_CtrlBtnChkFileMode.Create( _T( "" ), WS_VISIBLE | WS_CHILD | BS_VCENTER | BS_AUTOCHECKBOX | WS_TABSTOP, m_tStatusBarRect.LoadCheck, &m_wndStatusBar, IDC_FILE_MODE );
    m_CtrlStaticFileMode.SetFont( &m_fontStatusBarCtrl );
    m_CtrlBtnChkFileMode.SetFont( &m_fontStatusBarCtrl );
}

void CMainFrame::OnCheckFileMode()
{
    if ( m_CtrlBtnChkFileMode.GetCheck() )
    {
        m_bCheckFileMode = TRUE;
    }
    else
    {
        m_bCheckFileMode = FALSE;
    }
    
    /*
    CString strTemp;
    strTemp.Format( _T( "Select Images Folder: \n Default Path \"bin/UnpackedImgs\".\n Description doc \"doc/userguide-5.1.10\"" ) );
    AfxMessageBox( strTemp );
    */
}

afx_msg LRESULT CMainFrame::OnMutiSoftwareConfig(WPARAM wParam, LPARAM lParam)
{
    UNUSED_ALWAYS(wParam);
    UNUSED_ALWAYS(lParam);
    m_dlgMutiSoftSelect.GetProperty();
    if (m_dlgMutiSoftSelect.m_arrProjectConfig.size() > 0)
    {
        if (IDOK == m_dlgMutiSoftSelect.DoModal())
        {
            m_dlgMutiSoftSelect.SetProperty();
        }
    }
    return 0;
}

afx_msg LRESULT CMainFrame::OnPreloadSelectConfig(WPARAM wParam, LPARAM lParam)
{
    UNUSED_ALWAYS(wParam);
    UNUSED_ALWAYS(lParam);
    m_dlgPreloadSelect.GetProperty();
    if (m_dlgPreloadSelect.m_arrPreloadConfig.size() > 0)
    {
        if (IDOK == m_dlgPreloadSelect.DoModal())
        {
            m_dlgPreloadSelect.SetProperty();
        }
    }
    return 0;
}

void CMainFrame::ShowPartition()
{
    BOOL bComparePartitionInfo = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, _T( "Options" ), _T( "ComparePartitionInfo" ), 1 );
    if ( !bComparePartitionInfo )
    {
        CString strPartition;
        strPartition.Format( _T( "The partition comparison function is disabled, check whether the partition table is consistent." ) );
        SetTimer( 1, 5000, NULL );    //5s
        int nRet = AfxMessageBox( strPartition.operator LPCTSTR(), MB_OK );
        if ( nRet == IDOK )
        {
            return;
        }
        return;
    }
}