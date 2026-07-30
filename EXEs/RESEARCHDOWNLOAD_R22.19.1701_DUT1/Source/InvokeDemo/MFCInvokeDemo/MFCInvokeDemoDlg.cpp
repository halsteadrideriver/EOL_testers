
// MFCInvokeDemoDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "framework.h"
#include "MFCInvokeDemo.h"
#include "MFCInvokeDemoDlg.h"
#include "afxdialogex.h"
#include "Layout.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND g_hHwndDidg = NULL;
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();
    
    // 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_ABOUTBOX };
#endif
    
protected:
    virtual void DoDataExchange( CDataExchange* pDX );  // DDX/DDV 支持
    
    // 实现
protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx( IDD_ABOUTBOX )
{
}

void CAboutDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialogEx::DoDataExchange( pDX );
}

BEGIN_MESSAGE_MAP( CAboutDlg, CDialogEx )
END_MESSAGE_MAP()


// CMFCInvokeDemoDlg 对话框



CMFCInvokeDemoDlg::CMFCInvokeDemoDlg( CWnd* pParent /*=nullptr*/ )
    : CDialogEx( IDD_MFCINVOKEDEMO_DIALOG, pParent )
    , m_fnFrameHelper( &m_fnDLFWDriver )
{
    m_hIcon = AfxGetApp()->LoadIcon( IDR_MAINFRAME );
}

void CMFCInvokeDemoDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialogEx::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_LIST_DL, m_CtrlListReport );
    DDX_Control( pDX, IDC_PROGRESS_LOAD_PACKET, m_CtrlProgressLoadPac );
}

BEGIN_MESSAGE_MAP( CMFCInvokeDemoDlg, CDialogEx )
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED( IDC_BUTTON_LOAD_PACKET, &CMFCInvokeDemoDlg::OnBnClickedButtonLoadPacket )
    ON_BN_CLICKED( IDC_BUTTON_START, &CMFCInvokeDemoDlg::OnBnClickedButtonStart )
    ON_BN_CLICKED( IDC_BUTTON_STOP, &CMFCInvokeDemoDlg::OnBnClickedButtonStop )
    ON_MESSAGE( WM_DL_PORT, &CMFCInvokeDemoDlg::OnDlPort )
    ON_MESSAGE( WM_LOAD_PACKET, &CMFCInvokeDemoDlg::OnLoadPacket )
    ON_MESSAGE( WM_DL_STEP, &CMFCInvokeDemoDlg::OnDlStep )
    ON_MESSAGE( WM_DL_PROGRESS, &CMFCInvokeDemoDlg::OnDlProgress )
    ON_MESSAGE( WM_DL_TIME, &CMFCInvokeDemoDlg::OnDlTime )
    ON_MESSAGE( WM_DL_SIZE, &CMFCInvokeDemoDlg::OnDlSize )
    ON_MESSAGE( WM_DL_RESULT, &CMFCInvokeDemoDlg::OnDlResult )
    ON_MESSAGE( WM_START_DOWNLOAD, &CMFCInvokeDemoDlg::OnStartDownload )
    ON_MESSAGE( WM_STOP_DOWNLOAD, &CMFCInvokeDemoDlg::OnStopDownload )
    ON_MESSAGE( WM_DL_TITLE, &CMFCInvokeDemoDlg::OnDlTitle )
    ON_WM_SIZE()
END_MESSAGE_MAP()


// CMFCInvokeDemoDlg 消息处理程序

BOOL CMFCInvokeDemoDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    
    // 将“关于...”菜单项添加到系统菜单中。
    
    // IDM_ABOUTBOX 必须在系统命令范围内。
    ASSERT( ( IDM_ABOUTBOX & 0xFFF0 ) == IDM_ABOUTBOX );
    ASSERT( IDM_ABOUTBOX < 0xF000 );
    
    CMenu* pSysMenu = GetSystemMenu( FALSE );
    if ( pSysMenu != nullptr )
    {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString( IDS_ABOUTBOX );
        ASSERT( bNameValid );
        if ( !strAboutMenu.IsEmpty() )
        {
            pSysMenu->AppendMenu( MF_SEPARATOR );
            pSysMenu->AppendMenu( MF_STRING, IDM_ABOUTBOX, strAboutMenu );
        }
    }
    
    // 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
    //  执行此操作
    SetIcon( m_hIcon, TRUE );       // 设置大图标
    SetIcon( m_hIcon, FALSE );      // 设置小图标
    
    ShowWindow( SW_MAXIMIZE );
    //ShowWindow(SW_MINIMIZE);
    
    // TODO: 在此添加额外的初始化代码
    g_hHwndDidg = m_hWnd;
    InitListCtrl();
    
    if ( !m_fnDLFWDriver.Startup( GetAppPath() ) )
    {
        return FALSE;
    }
    
    std::wstring strIniPath = GetAppPath() + L"\\DLFramework.ini";
    UINT nLv = GetPrivateProfileIntW( _T( "Log" ), _T( "Level" ), SPLOGLV_ERROR, strIniPath.c_str() );
    
    SPRESULT spRlt = SP_OK;
    spRlt = m_fnFrameHelper.Startup( nLv );
    if ( SP_OK != spRlt )
    {
        return FALSE;
    }
    
    GetDlgItem( IDC_BUTTON_START )->EnableWindow( FALSE );
    GetDlgItem( IDC_BUTTON_STOP )->EnableWindow( FALSE );
    GetDlgItem( IDC_BUTTON_LOAD_PACKET )->EnableWindow( TRUE );
    
    InitializeCriticalSection( &m_csLock );
    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCInvokeDemoDlg::OnSysCommand( UINT nID, LPARAM lParam )
{
    if ( ( nID & 0xFFF0 ) == IDM_ABOUTBOX )
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialogEx::OnSysCommand( nID, lParam );
    }
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCInvokeDemoDlg::OnPaint()
{
    if ( IsIconic() )
    {
        CPaintDC dc( this ); // 用于绘制的设备上下文
        
        SendMessage( WM_ICONERASEBKGND, reinterpret_cast<WPARAM>( dc.GetSafeHdc() ), 0 );
        
        // 使图标在工作区矩形中居中
        int cxIcon = GetSystemMetrics( SM_CXICON );
        int cyIcon = GetSystemMetrics( SM_CYICON );
        CRect rect;
        GetClientRect( &rect );
        int x = ( rect.Width() - cxIcon + 1 ) / 2;
        int y = ( rect.Height() - cyIcon + 1 ) / 2;
        
        // 绘制图标
        dc.DrawIcon( x, y, m_hIcon );
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCInvokeDemoDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>( m_hIcon );
}


void CMFCInvokeDemoDlg::PostNcDestroy()
{
    // TODO: 在此添加专用代码和/或调用基类
    for ( uint16 i = 0; i < (uint16)m_mapTaskHelper.size(); i++ )
    {
        CTaskHelper* pTask = m_mapTaskHelper[i];
        if ( NULL != pTask )
        {
            pTask->FreeTask();
        }
    }
    m_fnFrameHelper.Cleanup();
    DeleteCriticalSection( &m_csLock );
    m_fnDLFWDriver.Cleanup();
    CDialogEx::PostNcDestroy();
}

void CMFCInvokeDemoDlg::OnSize( UINT nType, int cx, int cy )
{
    CDialogEx::OnSize( nType, cx, cy );
    
    // TODO: 在此处添加消息处理程序代码
    CWnd* pWnd = GetDlgItem( IDC_LIST_DL );
    if ( pWnd && pWnd->GetSafeHwnd() )
    {
        static RULE rules[] =
        {
            {lSTRETCH, lTOP( IDC_LIST_DL ), lTOP( lPARENT ), 2},
            {lSTRETCH, lHEIGHT( IDC_LIST_DL ), lHEIGHTOF( lPARENT, 80 ), 0},
            {lSTRETCH, lLEFT( IDC_LIST_DL ), lLEFT( lPARENT ), 2},
            {lSTRETCH, lRIGHT( IDC_LIST_DL ), lRIGHT( lPARENT ), -2},
            
            {lSTRETCH, lTOP( IDC_PROGRESS_LOAD_PACKET ), lBOTTOM( IDC_LIST_DL ),  2},
            {lSTRETCH, lHEIGHT( IDC_PROGRESS_LOAD_PACKET ), lHEIGHTOF( lPARENT, 6 ), 0},
            {lSTRETCH, lLEFT( IDC_PROGRESS_LOAD_PACKET ), lLEFT( lPARENT ), 2},
            {lSTRETCH, lRIGHT( IDC_PROGRESS_LOAD_PACKET ), lRIGHT( lPARENT ), -2},
            
            {lSTRETCH, lTOP( IDC_BUTTON_LOAD_PACKET ), lBOTTOM( IDC_PROGRESS_LOAD_PACKET ), 2},
            {lSTRETCH, lHEIGHT( IDC_BUTTON_LOAD_PACKET ), lHEIGHTOF( lPARENT, 8 ), 0},
            {lSTRETCH, lLEFT( IDC_BUTTON_LOAD_PACKET ), lLEFT( lPARENT ), 6},
            {lSTRETCH, lWIDTH( IDC_BUTTON_LOAD_PACKET ), lWIDTHOF( lPARENT, 20 ), 0},
            
            {lSTRETCH, lTOP( IDC_BUTTON_START ), lBOTTOM( IDC_PROGRESS_LOAD_PACKET ), 2},
            {lSTRETCH, lHEIGHT( IDC_BUTTON_START ), lHEIGHTOF( lPARENT, 8 ), 0},
            {lSTRETCH, lLEFT( IDC_BUTTON_START ), lRIGHT( IDC_BUTTON_LOAD_PACKET ), 6},
            {lSTRETCH, lWIDTH( IDC_BUTTON_START ), lWIDTHOF( lPARENT, 20 ), 0},
            
            {lSTRETCH, lTOP( IDC_BUTTON_STOP ), lBOTTOM( IDC_PROGRESS_LOAD_PACKET ), 2},
            {lSTRETCH, lHEIGHT( IDC_BUTTON_STOP ), lHEIGHTOF( lPARENT, 8 ), 0},
            {lSTRETCH, lLEFT( IDC_BUTTON_STOP ), lRIGHT( IDC_BUTTON_START ), 6},
            {lSTRETCH, lWIDTH( IDC_BUTTON_STOP ), lWIDTHOF( lPARENT, 20 ), 0},
            
            {lEND}
        };
        Layout_ComputeLayout( m_hWnd, rules );
        RepositionBars( AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0 );
    }
}

void CMFCInvokeDemoDlg::InitListCtrl()
{
    CRect rc;
    GetWindowRect( &rc );
    
    while ( m_CtrlListReport.DeleteItem( 0 ) );
    while ( m_CtrlListReport.DeleteColumn( 0 ) );
    
    m_CtrlListReport.ModifyStyle( 0, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL );
    m_CtrlListReport.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP );
    
    for ( size_t i = 0; i < MAX_DL_LIST; i++ )
    {
        m_CtrlListReport.InsertColumn( M_DL_LIST[i].type, M_DL_LIST[i].name, LVCFMT_CENTER, M_DL_LIST[i].width );
    }
}

void CMFCInvokeDemoDlg::InsertItemList( int nItem )
{
    CString strInfo;
    strInfo.Format( L"%d", nItem );
    LV_ITEM lvItem;
    lvItem.mask = LVIF_TEXT;
    lvItem.iItem = nItem;
    lvItem.iSubItem = 0;
    lvItem.pszText = strInfo.GetBuffer( 1 );
    m_CtrlListReport.InsertItem( &lvItem );
}

void CMFCInvokeDemoDlg::InsertSubItemList( int nItem, int nSubItem, LPCTSTR LpszText )
{
    m_CtrlListReport.SetItemText( nItem, nSubItem, LpszText );
}

void CMFCInvokeDemoDlg::OnBnClickedButtonLoadPacket()
{
    // TODO: 在此添加控件通知处理程序代码
    static _TCHAR BASED_CODE szFilter[] = _T( "Packet Files (*.pac)|*.pac|All Files(*.*)|*.*||" );
    CString strFile = _T( "" );
    CFileDialog dlg( TRUE, _T( ".pac" ), strFile, OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, szFilter, NULL );
    if ( dlg.DoModal() == IDCANCEL )
    {
        return;
    }
    CString strPath = dlg.GetPathName();
    string_t strImageDir = GetAppPath() + L"\\ImageFiles";
    SPRESULT spRlt = m_fnFrameHelper.LoadPacket( strPath.operator LPCWSTR(), strImageDir.c_str() );
    if ( SP_OK == spRlt )
    {
        GetDlgItem( IDC_BUTTON_START )->EnableWindow( TRUE );
        GetDlgItem( IDC_BUTTON_LOAD_PACKET )->EnableWindow( FALSE );
    }
}


afx_msg LRESULT CMFCInvokeDemoDlg::OnDlPort( WPARAM wParam, LPARAM lParam )
{
    TCHAR m_szMsg[256] = { 0 };
    _stprintf_s( m_szMsg, L"%d", wParam );
    CMapPort::iterator it = m_mPort.find( ( uint16 )wParam );
    if ( it == m_mPort.end() )
    {
        uint16 u16Flag = (uint16)m_mPort.size();
        InsertItemList( u16Flag );
        InsertSubItemList( u16Flag, E_LIST_PORT, m_szMsg );
        m_mPort.insert( std::make_pair( ( uint16 )wParam, u16Flag ) );
    }
    else
    {
        InsertSubItemList( it->second, E_LIST_PORT, m_szMsg );
    }
    return 0;
}




afx_msg LRESULT CMFCInvokeDemoDlg::OnLoadPacket( WPARAM wParam, LPARAM lParam )
{
    switch ( wParam )
    {
    case CBK_PROG_BEGIN:
        m_CtrlProgressLoadPac.SetRange32( 0, 100 );
        m_CtrlProgressLoadPac.SetPos( 0 );
        break;
    case CBK_PROG_PROCEED:
        m_CtrlProgressLoadPac.SetPos( ( DWORD )lParam );
        break;
    case CBK_PROG_END:
        if ( lParam == 100 )
        {
            int nLower = 0;
            int nUpper = 0;
            m_CtrlProgressLoadPac.GetRange( nLower, nUpper );
            m_CtrlProgressLoadPac.SetPos( nUpper );
        }
        break;
    default:
        break;
    }
    return 0;
}


void CMFCInvokeDemoDlg::OnBnClickedButtonStart()
{
    // TODO: 在此添加控件通知处理程序代码
    SPRESULT spRlt = SP_OK;
    spRlt = m_fnFrameHelper.DeviceMoniter( TRUE );
    if ( SP_OK == spRlt )
    {
        GetDlgItem( IDC_BUTTON_STOP )->EnableWindow( TRUE );
        GetDlgItem( IDC_BUTTON_START )->EnableWindow( FALSE );
        return;
    }
}


void CMFCInvokeDemoDlg::OnBnClickedButtonStop()
{
    // TODO: 在此添加控件通知处理程序代码
    m_fnFrameHelper.DeviceMoniter( FALSE );
    for (uint16 i = 0; i < (uint16)m_mapTaskHelper.size(); i++ )
    {
        CTaskHelper* pTask = m_mapTaskHelper[i];
        if ( NULL != pTask )
        {
            pTask->StopTask();
        }
    }
    GetDlgItem( IDC_BUTTON_STOP )->EnableWindow( FALSE );
    GetDlgItem( IDC_BUTTON_START )->EnableWindow( TRUE );
}



afx_msg LRESULT CMFCInvokeDemoDlg::OnDlStep( WPARAM wParam, LPARAM lParam )
{
    CMapPort::iterator it = m_mPort.find( ( uint16 )wParam );
    if ( it != m_mPort.end() )
    {
        InsertSubItemList( it->second, E_LIST_STEP, ( LPCTSTR )lParam );
    }
    return 0;
}


afx_msg LRESULT CMFCInvokeDemoDlg::OnDlProgress( WPARAM wParam, LPARAM lParam )
{
    CMapPort::iterator it = m_mPort.find( ( uint16 )wParam );
    if ( it != m_mPort.end() )
    {
        TCHAR m_szMsg[256] = { 0 };
        _stprintf_s( m_szMsg, L"%d%%", lParam );
        InsertSubItemList( it->second, E_LIST_PROGRESS, m_szMsg );
    }
    return 0;
}


afx_msg LRESULT CMFCInvokeDemoDlg::OnDlTime( WPARAM wParam, LPARAM lParam )
{
    CMapPort::iterator it = m_mPort.find( ( uint16 )wParam );
    if ( it != m_mPort.end() )
    {
        TCHAR m_szMsg[256] = { 0 };
        if ( lParam > 1000 )
        {
            _stprintf_s( m_szMsg, L"%.3fs", lParam / 1000.0 );
        }
        else
        {
            _stprintf_s( m_szMsg, L"%dms", lParam );
        }
        
        InsertSubItemList( it->second, E_LIST_TIME, m_szMsg );
    }
    return 0;
}


afx_msg LRESULT CMFCInvokeDemoDlg::OnDlSize( WPARAM wParam, LPARAM lParam )
{
    CMapPort::iterator it = m_mPort.find( ( uint16 )wParam );
    if ( it != m_mPort.end() )
    {
        TCHAR m_szMsg[256] = { 0 };
        _stprintf_s( m_szMsg, L"%.3fM", *( double* )lParam );
        InsertSubItemList( it->second, E_LIST_SIZE, m_szMsg );
    }
    return 0;
}


afx_msg LRESULT CMFCInvokeDemoDlg::OnDlResult( WPARAM wParam, LPARAM lParam )
{
    CMapPort::iterator it = m_mPort.find( ( uint16 )wParam );
    if ( it != m_mPort.end() )
    {
        InsertSubItemList( it->second, E_LIST_RESULT, ( LPCTSTR )lParam );
    }
    return 0;
}
CTaskHelper* CMFCInvokeDemoDlg::FindTaskHelperHanle( int nPort )
{
    std::map<uint16, CTaskHelper*>::iterator it = m_mapTaskHelper.find( nPort );
    if ( m_mapTaskHelper.end() != it )
    {
        return it->second;
    }
    return NULL;
}

afx_msg LRESULT CMFCInvokeDemoDlg::OnStartDownload( WPARAM wParam, LPARAM lParam )
{
    SPRESULT spRlt = SP_OK;
    EnterCriticalSection( &m_csLock );
    std::wstring strIniPath = GetAppPath() + L"\\DLFramework.ini";
    UINT nLv = GetPrivateProfileIntW( _T( "Log" ), _T( "Level" ), SPLOGLV_ERROR, strIniPath.c_str() );
    CTaskHelper* pTask = FindTaskHelperHanle( wParam );
    if ( NULL == pTask )
    {
        pTask = new CTaskHelper( &m_fnDLFWDriver );
        spRlt = pTask->CreatTask( wParam, nLv );
        if ( SP_OK != spRlt )
        {
            return 0;
        }
        m_mapTaskHelper.insert( std::make_pair( wParam, pTask ) );
    }
    else
    {
        spRlt = pTask->OpenLog( wParam, nLv );
        if ( SP_OK != spRlt )
        {
            return 0;
        }
    }
    LeaveCriticalSection( &m_csLock );
    
    spRlt = pTask->RunTask( wParam );
    if ( SP_OK != spRlt )
    {
        return 0;
    }
    return 0;
}


afx_msg LRESULT CMFCInvokeDemoDlg::OnStopDownload( WPARAM wParam, LPARAM lParam )
{
    CTaskHelper* pTask = FindTaskHelperHanle( wParam );
    if ( NULL != pTask )
    {
        pTask->StopTask();
    }
    return 0;
}

std::wstring CMFCInvokeDemoDlg::GetAppPath( void )
{
    WCHAR szAppPath[MAX_PATH] = { 0 };
    GetModuleFileNameW( NULL, szAppPath, MAX_PATH );
    LPWSTR lpChar = wcsrchr( szAppPath, L'\\' );
    if ( NULL != lpChar )
    {
        *lpChar = L'\0';
    }
    
    return ( std::wstring )szAppPath + L"\\";
}

afx_msg LRESULT CMFCInvokeDemoDlg::OnDlTitle( WPARAM wParam, LPARAM lParam )
{
    SetWindowText( ( LPCTSTR )lParam );
    return 0;
}



