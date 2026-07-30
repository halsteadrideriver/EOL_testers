// PageOptions.cpp : implementation file
//

#include "stdafx.h"
#include "dloader.h"
#include "PageOptions.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageOptions property page

IMPLEMENT_DYNCREATE( CPageOptions, CPropertyPage )

CPageOptions::CPageOptions() : CPropertyPage( CPageOptions::IDD )
    , m_bTmpReadChipUID( FALSE )
    , m_bTmpCheckMatch( FALSE )
    , m_bTmpSecureBoot( FALSE )
    , m_bTmpUartDownload( FALSE )
    , m_bTmpAutoCompare( FALSE )
    , m_strTmpCompareApp( _T( "" ) )
    , m_strTmpDebugLevel( _T( "" ) )
    , m_bReadPartition( FALSE )
    , m_bTmpEnableDumpUbootLog( FALSE )
    , m_bEnableDumpUbootLog( FALSE )
{
    //{{AFX_DATA_INIT(CPageOptions)
    m_bTmpRepart        = FALSE;
    m_bTmpReset         = FALSE;
    m_bTmpReadMcpType   = FALSE;
    m_bTmpKeepCharge    = FALSE;
    m_bTmpDdrCheck      = FALSE;
    m_bTmpSelfRefresh   = FALSE;
    m_bTmpOldMemoryCheck = FALSE;
    m_bTmpEmmcCheck     = FALSE;
    m_bTmpEnableLog     = FALSE;
    m_bPowerOff      = FALSE;
    //}}AFX_DATA_INIT
    
    m_bReadMcpType      = m_bTmpReadMcpType;
    m_bRepart           = m_bTmpRepart;
    m_bReset            = m_bTmpReset;
    m_bKeepCharge       = m_bTmpKeepCharge;
    m_bUartDownload     = m_bTmpUartDownload;
    m_bAutoCompare      = m_bTmpAutoCompare;
    m_strCompareApp     = m_strTmpCompareApp;
    m_bDdrCheck         = m_bTmpDdrCheck;
    m_bSelfRefresh      = m_bTmpSelfRefresh;
    m_bOldMemoryCheck   = m_bTmpOldMemoryCheck;
    m_bEmmcCheck        = m_bTmpEmmcCheck;
    m_bReadChipUID      = m_bTmpReadChipUID;
    m_bCheckMatch       = m_bTmpCheckMatch;
    m_bEnableLog        = m_bTmpEnableLog;
    m_bEnableSecureBoot = m_bTmpSecureBoot;
    m_strDebugLevel     = m_strTmpDebugLevel;
    m_bCheckVpac        = m_bTmpCheckVpac;
    m_nFlashPageType    = 0;
    m_bEnableEndProcess = FALSE;
    m_strCompareParameter = _T( "\"%s\" \"%s\"" );
    m_bPowerOff = FALSE;
    m_bEnable7e2DumpLog  = FALSE;
}

CPageOptions::~CPageOptions()
{
}

void CPageOptions::DoDataExchange( CDataExchange* pDX )
{
    CPropertyPage::DoDataExchange( pDX );
    //{{AFX_DATA_MAP(CPageOptions)
    DDX_Check( pDX, IDC_DOP_REPART, m_bTmpRepart );
    DDX_Check( pDX, IDC_DOP_RESET, m_bTmpReset );
    DDX_Check( pDX, IDC_FOD_CHK_READ_MCPTYPE, m_bTmpReadMcpType );
    DDX_Check( pDX, IDC_DOP_CHK_KEEP_CHARGE, m_bTmpKeepCharge );
    DDX_Check( pDX, IDC_CHK_DDR_CHECK, m_bTmpDdrCheck );
    DDX_Check( pDX, IDC_CHK_SELF_REFRESH, m_bTmpSelfRefresh );
    DDX_Check( pDX, IDC_CHK_OLD_MEMORY, m_bTmpOldMemoryCheck );
    DDX_Check( pDX, IDC_CHK_EMMC_CHECK, m_bTmpEmmcCheck );
    DDX_Check( pDX, IDC_DOP_POWER_OFF, m_bPowerOff );
    DDX_Check( pDX, IDC_CHECK_READ_CHIPUID, m_bTmpReadChipUID );
    DDX_Check( pDX, IDC_CHK_CHECK_MATCH, m_bTmpCheckMatch );
    DDX_Check( pDX, IDC_CHK_ENABLE_DL_LOG, m_bTmpEnableLog );
    DDX_Check( pDX, IDC_CHK_ENABLE_SECUREBOOT, m_bTmpSecureBoot );
    DDX_Check( pDX, IDC_CHECK_UART_MODE, m_bTmpUartDownload );
    DDX_Check( pDX, IDC_CHECK_AUTO_COMPARE, m_bTmpAutoCompare );
    DDX_Check( pDX, IDC_CHECK_READ_PARTITION, m_bReadPartition );
    DDX_Check( pDX, IDC_DOP_DUMP_UBOOT_LOG, m_bTmpEnableDumpUbootLog );
    DDX_Check( pDX, IDC_CHK_VPAC_CHECK, m_bTmpCheckVpac );
    DDX_Text( pDX, IDC_EDIT_COMPARE_APP, m_strTmpCompareApp );
    DDV_MaxChars( pDX, m_strTmpCompareApp, 256 );
    //}}AFX_DATA_MAP
    DDX_Control( pDX, IDC_COMBO_DEBUG_LEVEL, m_comboDebugLevel );
    DDX_Check( pDX, IDC_CHK_NVITEM_READ_CRC_CHECK, m_bCheckNvItemReadCrc );
    DDX_Check( pDX, IDC_CHK_ENABLE_7E2DUMLOG, m_bEnable7e2DumpLog );
}

BEGIN_MESSAGE_MAP( CPageOptions, CPropertyPage )
    //{{AFX_MSG_MAP(CPageOptions)
    ON_BN_CLICKED( IDC_DOP_RESET, OnReset )
    ON_BN_CLICKED( IDC_DOP_POWER_OFF, OnPowerOff )
    ON_BN_CLICKED( IDC_BUTTON_SELECT_COMPARE_APP, &CPageOptions::OnSelectCompareApp )
    ON_BN_CLICKED( IDC_CHECK_AUTO_COMPARE, &CPageOptions::OnAutoCompare )
    //}}AFX_MSG_MAP
    ON_CBN_SELCHANGE( IDC_COMBO_DEBUG_LEVEL, &CPageOptions::OnCbnSelchangeComboDebugLevel )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageOptions message handlers

BOOL CPageOptions::OnInitDialog()
{
    CPropertyPage::OnInitDialog();
    
    // TODO: Add extra initialization here
    /// No.         : 001
    /// Reporter    : Kiki.Zhao
    /// Developer   : Hong.Yang
    /// Description : Load 支持power off 的PAC包（xml 中有配置poweroff=1）后，poweroff 勾选框没有自动勾选上
    if ( m_bReset )
    {
        m_bPowerOff = FALSE;
    }
    
    m_bTmpRepart        = m_bRepart;
    m_bTmpReset         = m_bReset;
    m_bTmpKeepCharge    = m_bKeepCharge;
    m_bTmpUartDownload  = m_bUartDownload;
    m_bTmpAutoCompare   = m_bAutoCompare;
    m_strTmpCompareApp  = m_strCompareApp;
    m_bTmpDdrCheck      = m_bDdrCheck;
    m_bTmpSelfRefresh   = m_bSelfRefresh;
    m_bTmpOldMemoryCheck = m_bOldMemoryCheck;
    m_bTmpEmmcCheck     = m_bEmmcCheck;
    m_bTmpReadMcpType   = m_bReadMcpType;
    m_bTmpReadChipUID   = m_bReadChipUID;
    m_bTmpCheckMatch    = m_bCheckMatch;
    m_bTmpEnableLog     = m_bEnableLog;
    m_bTmpSecureBoot    = m_bEnableSecureBoot;
    m_strTmpDebugLevel  = m_strDebugLevel;
    m_bTmpCheckVpac     = m_bCheckVpac;
    EnableAutoCompareCtrl( m_bTmpAutoCompare );
    m_bTmpEnableDumpUbootLog = m_bEnableDumpUbootLog;
    if ( m_strDebugLevel.Right( 1 ) == _T( '-' ) )
    {
        m_comboDebugLevel.SetCurSel( 0 );
    }
    else
    {
        int nLevel = _tstoi( m_strDebugLevel.Right( 1 ) ) + 1;
        m_comboDebugLevel.SetCurSel( nLevel );
    }
    
    UpdateData( FALSE );
    
#if defined(_SPUPGRADE) || defined(_FACTORY)
    if ( !g_theApp.m_bShowOtherPage )
    {
        GetDlgItem( IDC_DOP_REPART )->EnableWindow( FALSE );
        GetDlgItem( IDC_DOP_CHK_KEEP_CHARGE )->EnableWindow( FALSE );
        GetDlgItem( IDC_CHK_DDR_CHECK )->EnableWindow( FALSE );
        GetDlgItem( IDC_CHK_SELF_REFRESH )->EnableWindow( FALSE );
        GetDlgItem( IDC_CHK_OLD_MEMORY )->EnableWindow( FALSE );
        GetDlgItem( IDC_CHK_EMMC_CHECK )->EnableWindow( FALSE );
        GetDlgItem( IDC_CHECK_UART_MODE )->EnableWindow( FALSE );
        //GetDlgItem(IDC_DOP_RESET)->EnableWindow(FALSE);
        //GetDlgItem(IDC_DOP_POWER_OFF)->EnableWindow(FALSE);
        GetDlgItem( IDC_FOD_CHK_READ_MCPTYPE )->EnableWindow( FALSE );
        GetDlgItem( IDC_CHECK_READ_CHIPUID )->EnableWindow( FALSE );
        GetDlgItem( IDC_CHK_CHECK_MATCH )->EnableWindow( FALSE );
        GetDlgItem( IDC_CHK_ENABLE_SECUREBOOT )->EnableWindow( FALSE );
        GetDlgItem( IDC_COMBO_DEBUG_LEVEL )->ShowWindow( SW_HIDE );
        GetDlgItem( IDC_STATIC_DEBUG_MODE )->ShowWindow( SW_HIDE );
        GetDlgItem( IDC_CHECK_AUTO_COMPARE )->ShowWindow( SW_HIDE );
        GetDlgItem( IDC_STATIC_COMPARISON_TEXT )->ShowWindow( SW_HIDE );
        GetDlgItem( IDC_EDIT_COMPARE_APP )->ShowWindow( SW_HIDE );
        GetDlgItem( IDC_BUTTON_SELECT_COMPARE_APP )->ShowWindow( SW_HIDE );
        GetDlgItem( IDC_CHECK_READ_PARTITION )->ShowWindow( SW_HIDE );
        GetDlgItem( IDC_CHK_NVITEM_READ_CRC_CHECK )->EnableWindow( FALSE );
        GetDlgItem( IDC_CHK_ENABLE_7E2DUMLOG )->EnableWindow( FALSE );
        GetDlgItem( IDC_CHK_ENABLE_7E2DUMLOG )->ShowWindow( FALSE );
    }
#endif
    
#if defined(_RESEARCH) || defined(_FACTORY)  || defined(_SPUPGRADE)
    GetDlgItem( IDC_CHK_VPAC_CHECK )->EnableWindow( FALSE );
#endif
    
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageOptions::OnOK()
{
    // TODO: Add your specialized code here and/or call the base class
    UpdateData();
    
    m_bReadMcpType  = m_bTmpReadMcpType;
    m_bRepart       = m_bTmpRepart;
    m_bReset        = m_bTmpReset;
    m_bKeepCharge   = m_bTmpKeepCharge;
    m_bUartDownload = m_bTmpUartDownload;
    m_bAutoCompare  = m_bTmpAutoCompare;
    m_strCompareApp = m_strTmpCompareApp;
    m_bDdrCheck     = m_bTmpDdrCheck;
    m_bSelfRefresh  = m_bTmpSelfRefresh;
    m_bOldMemoryCheck = m_bTmpOldMemoryCheck;
    m_bEmmcCheck    = m_bTmpEmmcCheck;
    m_bReadChipUID  = m_bTmpReadChipUID;
    m_bCheckMatch   = m_bTmpCheckMatch;
    m_bEnableLog    = m_bTmpEnableLog;
    m_bEnableSecureBoot = m_bTmpSecureBoot;
    m_strDebugLevel     = m_strTmpDebugLevel;
    m_bEnableDumpUbootLog = m_bTmpEnableDumpUbootLog;
    m_bCheckVpac    = m_bTmpCheckVpac;
    
    SaveSettings();
    
    CPropertyPage::OnOK();
}

BOOL CPageOptions::LoadSettings()
{
    //  UNUSED_ALWAYS(strErrorMsg);
    if ( NULL == g_theApp.m_pCfgObject )
    {
        ASSERT( 0 );
        MessageBox( _T( "[DL2082][H]Configure file is empty!" ) );
        return FALSE;
    }
    
    m_bReset            = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, _T( "Options" ), _T( "Reset" ), 0 );
    m_nFlashPageType    = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, _T( "Options" ), _T( "FlashPageType" ), 0 );
    m_bReadMcpType      = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, _T( "Options" ), _T( "ReadMCPType" ), 0 );
    m_bKeepCharge       = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, _T( "Options" ), _T( "KeepCharge" ), 0 );
    m_bUartDownload     = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, _T( "Options" ), _T( "UartDownload" ), 0 );
    m_bAutoCompare      = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, _T( "Comparison" ), _T( "Enable" ), 0 );
    m_bDdrCheck         = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, _T( "Options" ), _T( "DDRCheck" ), 0 );
    m_bSelfRefresh      = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, _T( "Options" ), _T( "SelfRefresh" ), 0 );
    m_bOldMemoryCheck   = g_theApp.m_pCfgObject->GetInt( CFGTYPE_BMFT, _T( "Misc" ), _T( "NeedCheckOldMemory" ), 0 );
    m_bEmmcCheck        = g_theApp.m_pCfgObject->GetInt( CFGTYPE_BMFT, _T( "Misc" ), _T( "DoCheckSum" ), 0 );
    m_bReadChipUID      = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, _T( "Options" ), _T( "ReadChipUID" ), 0 );
    m_bReadPartition    = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, _T( "Options" ), _T( "ReadPartition" ), 0 );
    m_bEnableLog        = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, _T( "Options" ), _T( "EnableLog" ), 0 );
    m_bEnableSecureBoot = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, _T( "Options" ), _T( "EnableSecureBoot" ), 0 );
    m_bEnableEndProcess = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, _T( "Options" ), _T( "EnableEndProcess" ), 0 );
    m_bCheckVpac        = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, _T( "Options" ), _T( "VpacCheck" ), 0 );
    m_bWatchdog         = g_theApp.m_pCfgObject->GetInt( CFGTYPE_BMFT, _T( "WatchDog" ), _T( "WatchDog" ), 0 );
    m_bEnableDumpUbootLog = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, _T( "Options" ), _T( "DumpUbootLog" ), 0 );
    m_bCheckNvItemReadCrc = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, _T( "Options" ), _T( "CheckNvItemReadCrc" ), 0 );
    
#ifdef _FACTORY
    m_bRepart           = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, _T( "Options" ), _T( "Repartition" ), 1 );
    m_bCheckMatch       = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, _T( "Options" ), _T( "CheckMatch" ), 0 );
    m_bCheckNvItemReadCrc = FALSE;
#endif
    
#ifdef _RESEARCH
    m_bRepart           = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, _T( "Options" ), _T( "Repartition" ), 0 );
    m_strCompareApp     = g_theApp.m_pCfgObject->GetString( CFGTYPE_DL, _T( "Comparison" ), _T( "Program" ), _T( "" ) );
    m_strCompareParameter = g_theApp.m_pCfgObject->GetString( CFGTYPE_DL, _T( "Comparison" ), _T( "Parameter" ), _T( "\"%s\" \"%s\"" ) );
    m_strDebugLevel     = g_theApp.m_pCfgObject->GetString( CFGTYPE_DL, _T( "Debug" ), _T( "Data" ), _T( "enable:level=0" ) );
    m_bEnable7e2DumpLog = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, _T( "Options" ), _T( "Enable7e2DumpLog" ), 0 );
#endif
    
#ifdef _SPUPGRADE
     m_bRepart          = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, _T( "Options" ), _T( "Repartition" ), 0 );
#endif

    m_bTmpRepart        = m_bRepart;
    m_bTmpReset         = m_bReset;
    m_bTmpReadMcpType   = m_bReadMcpType;
    m_bTmpKeepCharge    = m_bKeepCharge;
    m_bTmpUartDownload  = m_bUartDownload;
    m_bTmpAutoCompare   = m_bAutoCompare;
    m_strTmpCompareApp  = m_strCompareApp;
    m_bTmpDdrCheck      = m_bDdrCheck;
    m_bTmpSelfRefresh   = m_bSelfRefresh;
    m_bTmpOldMemoryCheck = m_bOldMemoryCheck;
    m_bTmpEmmcCheck     = m_bEmmcCheck;
    m_bTmpReadChipUID   = m_bReadChipUID;
    m_bTmpCheckMatch    = m_bCheckMatch;
    m_bTmpEnableLog     = m_bEnableLog;
    m_bTmpSecureBoot    = m_bEnableSecureBoot;
    m_strTmpDebugLevel  = m_strDebugLevel;
    m_bTmpEnableDumpUbootLog = m_bEnableDumpUbootLog;
    m_bTmpCheckVpac     = m_bCheckVpac;
    
    return TRUE;
}

BOOL CPageOptions::SaveSettings()
{
    if ( NULL == g_theApp.m_pCfgObject )
    {
        ASSERT( 0 );
        return FALSE;
    }
    
    g_theApp.m_pCfgObject->SetInt( CFGTYPE_DL, _T( "Options" ), _T( "Repartition" ), m_bRepart );
    g_theApp.m_pCfgObject->SetInt( CFGTYPE_DL, _T( "Options" ), _T( "Reset" ), m_bReset );
    g_theApp.m_pCfgObject->SetInt( CFGTYPE_DL, _T( "Options" ), _T( "ReadMCPType" ), m_bReadMcpType );
    g_theApp.m_pCfgObject->SetInt( CFGTYPE_DL, _T( "Options" ), _T( "KeepCharge" ), m_bKeepCharge );
    g_theApp.m_pCfgObject->SetInt( CFGTYPE_DL, _T( "Options" ), _T( "UartDownload" ), m_bUartDownload );
    g_theApp.m_pCfgObject->SetInt( CFGTYPE_DL, _T( "Options" ), _T( "EnableLog" ), m_bEnableLog );
    g_theApp.m_pCfgObject->SetInt( CFGTYPE_DL, _T( "Options" ), _T( "EnableSecureBoot" ), m_bEnableSecureBoot );
    g_theApp.m_pCfgObject->SetInt( CFGTYPE_DL, _T( "Options" ), _T( "EnableEndProcess" ), m_bEnableEndProcess );
    
#ifdef _RESEARCH
    g_theApp.m_pCfgObject->SetInt( CFGTYPE_DL, _T( "Comparison" ), _T( "Enable" ), m_bAutoCompare );
    g_theApp.m_pCfgObject->SetString( CFGTYPE_DL, _T( "Comparison" ), _T( "Program" ), m_strCompareApp );
    g_theApp.m_pCfgObject->SetInt( CFGTYPE_DL, _T( "Options" ), _T( "Enable7e2DumpLog" ), m_bEnable7e2DumpLog );
#endif
    
    g_theApp.m_pCfgObject->SetInt( CFGTYPE_DL, _T( "Options" ), _T( "DDRCheck" ), m_bDdrCheck );
    g_theApp.m_pCfgObject->SetInt( CFGTYPE_DL, _T( "Options" ), _T( "SelfRefresh" ), m_bSelfRefresh );
    g_theApp.m_pCfgObject->SetInt( CFGTYPE_DL, _T( "Options" ), _T( "PowerOff" ), m_bPowerOff );
    g_theApp.m_pCfgObject->SetInt( CFGTYPE_DL, _T( "Options" ), _T( "ReadChipUID" ), m_bReadChipUID );
    g_theApp.m_pCfgObject->SetInt( CFGTYPE_DL, _T( "Options" ), _T( "CheckMatch" ), m_bCheckMatch );
    g_theApp.m_pCfgObject->SetInt( CFGTYPE_DL, _T( "Options" ), _T( "ReadPartition" ), m_bReadPartition );
    g_theApp.m_pCfgObject->SetInt( CFGTYPE_DL, _T( "Options" ), _T( "DumpUbootLog" ), m_bEnableDumpUbootLog );
    g_theApp.m_pCfgObject->SetInt( CFGTYPE_DL, _T( "Options" ), _T( "VpacCheck" ), m_bCheckVpac );
    g_theApp.m_pCfgObject->SetInt( CFGTYPE_DL, _T( "Options" ), _T( "CheckNvItemReadCrc" ), m_bCheckNvItemReadCrc );
    
    g_theApp.m_pCfgObject->SetInt( CFGTYPE_BMFT, _T( "Misc" ), _T( "NeedCheckOldMemory" ), m_bOldMemoryCheck );
    g_theApp.m_pCfgObject->SetInt( CFGTYPE_BMFT, _T( "Misc" ), _T( "DoCheckSum" ), m_bEmmcCheck );
    g_theApp.m_pCfgObject->SetInt( CFGTYPE_BMFT, _T( "WatchDog" ), _T( "WatchDog" ), m_bWatchdog );
    
    g_theApp.m_pCfgObject->SetString( CFGTYPE_DL, _T( "Debug" ), _T( "Data" ), m_strDebugLevel );
    
    return TRUE;
}

int CPageOptions::GetGUINandRepartStrategy()
{
    if ( ::IsWindow( this->GetSafeHwnd() ) )
    {
        UpdateData( FALSE );
    }
    
    return ( m_bTmpRepart ? REPAR_STRATEGY_DO_ALWAYS : REPAR_STRATEGY_STOP );
}

int CPageOptions::GetNandRepartStrategy()
{
    return ( m_bRepart ? REPAR_STRATEGY_DO_ALWAYS : REPAR_STRATEGY_STOP );
}

void CPageOptions::OnReset()
{
    // TODO: Add your control notification handler code here
    UpdateData();
    if ( m_bTmpReset && m_bPowerOff )
    {
        m_bPowerOff = FALSE;
        UpdateData( FALSE );
    }
}

void CPageOptions::OnPowerOff()
{
    // TODO: Add your control notification handler code here
    UpdateData();
    if ( m_bTmpReset && m_bPowerOff )
    {
        m_bTmpReset = FALSE;
        UpdateData( FALSE );
    }
}

void CPageOptions::OnSelectCompareApp()
{
    // TODO: Add your control notification handler code here
    UpdateData();
    static _TCHAR BASED_CODE szFilter[] = _T( "Programs (*.exe)|*.exe|All Files(*.*)|*.*||" );
    CString strFile = m_strTmpCompareApp.IsEmpty() ? _T( "C:\\Program Files (x86)\\Beyond Compare 3\\BCompare.exe" ) : m_strTmpCompareApp;
    CFileDialog dlg( TRUE, _T( ".exe" ), strFile, OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, szFilter, NULL );
    
    if ( dlg.DoModal() == IDCANCEL )
    { return; }
    
    m_strTmpCompareApp = dlg.GetPathName();
    UpdateData( FALSE );
}

void CPageOptions::OnAutoCompare()
{
    // TODO: Add your control notification handler code here
    UpdateData();
    EnableAutoCompareCtrl( m_bTmpAutoCompare );
}

void CPageOptions::EnableAutoCompareCtrl( BOOL bEnable )
{
    GetDlgItem( IDC_EDIT_COMPARE_APP )->EnableWindow( bEnable );
    GetDlgItem( IDC_BUTTON_SELECT_COMPARE_APP )->EnableWindow( bEnable );
}

void CPageOptions::OnCbnSelchangeComboDebugLevel()
{
    // TODO: Add your control notification handler code here
    CString strLevel;
    int nSel = m_comboDebugLevel.GetCurSel();
    m_comboDebugLevel.GetLBText( nSel, strLevel );
    int nIndex = m_strTmpDebugLevel.GetLength() - 1;
    m_strTmpDebugLevel.SetAt( nIndex, strLevel.GetBuffer()[0] );
    if ( nSel >= 5 )
    {
        MessageBox( _T( "The debug level is too high,It may be generate wcnd assert.The recommended level is less than 5" ) );
    }
}


BOOL CPageOptions::GetProperty()
{
    CMainFrame* pMainFrame = ( CMainFrame* )AfxGetMainWnd();
    SPRESULT spRlt = SP_OK;
    spRlt &= pMainFrame->m_pFrameHelper->GetProperty( DLFW_ATTR_POWER_OFF, 0, &m_bPowerOff );
    if ( SP_OK != spRlt )
    {
        MessageBox( _T( "Get Parameters From DLFramework Fail." ) );
    }
    
    return SP_OK == spRlt ? TRUE : FALSE;
    
}

BOOL CPageOptions::SetProperty()
{
    CMainFrame* pMainFrame = ( CMainFrame* )AfxGetMainWnd();
    SPRESULT spRlt = SP_OK;
    spRlt &= pMainFrame->m_pFrameHelper->SetProperty( DLFW_ATTR_POWER_OFF, 0, &m_bPowerOff );
    
    if ( SP_OK != spRlt )
    {
        MessageBox( _T( "Set Parameters To DLFramework Fail." ) );
    }
    
    return SP_OK == spRlt ? TRUE : FALSE;
}
