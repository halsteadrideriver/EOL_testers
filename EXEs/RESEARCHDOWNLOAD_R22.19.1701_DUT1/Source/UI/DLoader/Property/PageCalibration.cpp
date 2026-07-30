// CalibrationgPage.cpp : implementation file
//

#include "stdafx.h"
#include "dloader.h"
#include "PageCalibration.h"
#include "supergridctrl.h"
#include "SettingsSheet.h"
#include "FileDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageCalibration dialog
IMPLEMENT_DYNCREATE( CPageCalibration, CPropertyPage )


CPageCalibration::CPageCalibration()
    : CPropertyPage( CPageCalibration::IDD )
{
    //{{AFX_DATA_INIT(CPageCalibration)
    m_strPath = _T( "" );
    //}}AFX_DATA_INIT
    
    m_strCurProduct = _T( "" );
    m_strLocalPath = _T( "Backup" );
    m_strPath = m_strLocalPath;
    
    m_bSaveToLocal = FALSE;
    m_nBackupProdnvMiscdata = 0;
    
}

CPageCalibration::~CPageCalibration()
{
}
void CPageCalibration::DoDataExchange( CDataExchange* pDX )
{
    CPropertyPage::DoDataExchange( pDX );
    //{{AFX_DATA_MAP(CPageCalibration)
    DDX_Control( pDX, IDC_LST_FILE_BACKUP, m_lstFileBackup );
    DDX_Control( pDX, IDC_LIST_FRAME, m_stcFrame );
    DDX_Text( pDX, IDC_DC_EDT_PATH, m_strPath );
    DDV_MaxChars( pDX, m_strPath, 256 );
    DDX_Check( pDX, IDC_CHK_SAVE_TO_LOCAL, m_bSaveToLocal );
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP( CPageCalibration, CPropertyPage )
    //{{AFX_MSG_MAP(CPageCalibration)
    ON_BN_CLICKED( IDC_CHK_SAVE_TO_LOCAL, OnChkSaveToLocal )
    ON_BN_CLICKED( IDC_DC_BTN_BROWSE, OnDcBtnBrowse )
    //}}AFX_MSG_MAP
    ON_NOTIFY( NM_CLICK, IDC_LST_FILE_BACKUP, OnNMClickLstFileBackup )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageCalibration message handlers

BOOL CPageCalibration::OnInitDialog()
{
    CPropertyPage::OnInitDialog();
    
    m_lstBackup.Create( WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect( 0, 0, 0, 0 ), this, 0x1001 );
    m_lstBackup.ModifyStyleEx( 0, WS_EX_CLIENTEDGE );
    
    m_lstBackup.FillList( m_arrBackFiles.size() ? m_arrBackFiles[0].szID : NULL );
    
    InitFileBackupList();
    
    CRect rc;
    m_stcFrame.GetWindowRect( rc );
    ScreenToClient( rc );
    m_stcFrame.ShowWindow( SW_HIDE );
    if ( m_lstBackup.GetSafeHwnd() )
    { m_lstBackup.MoveWindow( rc ); }
    
    GetDlgItem( IDC_DC_EDT_PATH )->EnableWindow( m_bSaveToLocal );
    GetDlgItem( IDC_STC_VER_SAP )->EnableWindow( m_bSaveToLocal );
    GetDlgItem( IDC_DC_BTN_BROWSE )->EnableWindow( m_bSaveToLocal );
    GetDlgItem( IDC_STC_LOCAL_PATH )->EnableWindow( m_bSaveToLocal );
    
    ( ( CEdit* )GetDlgItem( IDC_DC_EDT_PATH ) )->UpdateData( FALSE );
    
    UpdateData( FALSE );
    
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
BOOL CPageCalibration::LoadSettings()
{
    if ( NULL == g_theApp.m_pCfgObject )
    {
        ASSERT( 0 );
        AfxMessageBox( _T( "[DL2082][E]Configure file is empty!" ) );
        return FALSE;
    }
    
    m_strLocalPath = g_theApp.m_strAppPath;
    m_strLocalPath += _T( "Backup" );
    
    // Download need not save nv to local
    // Dloader can set by GUI
    // So SPUpgrade must be initialized by configure
#ifdef _SPUPGRADE
    m_bSaveToLocal = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, _T( "NV" ), _T( "SaveToLocalFlag" ), 0 );
    m_nBackupProdnvMiscdata = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, _T( "Options" ), _T( "BackupProdnvMiscData" ), 0 );
    BOOL bSupportCU = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, _T( "Options" ), _T( "SupportCU" ), FALSE );
    BOOL bSupportInproduction = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, _T( "Options" ), _T( "SupportInproduction" ), FALSE );
    if ( ( bSupportCU || bSupportInproduction ) && 0 == m_nBackupProdnvMiscdata )
    {
        m_nBackupProdnvMiscdata = 2;
    }
#endif
    
    if ( m_bSaveToLocal && !g_theApp.m_fnOperationHelper.CreateDeepDirectory( m_strLocalPath ) )
    {
        CString strErr;
        strErr.Format( _T( "Create directory [%s] fail." ), m_strLocalPath.operator LPCTSTR() );
        AfxMessageBox( strErr );
        return FALSE;
    }
    return TRUE;
}
BOOL CPageCalibration::SaveSettings()
{
    if ( NULL == g_theApp.m_pCfgObject )
    {
        ASSERT( 0 );
        return FALSE;
    }
    
    g_theApp.m_pCfgObject->SetInt( CFGTYPE_DL, _T( "NV" ), _T( "SaveToLocalFlag" ), m_bSaveToLocal );
    
    
    return m_lstBackup.SaveSettings();
}

void CPageCalibration::OnOK()
{
    CPropertyPage::OnOK();
    
    m_lstBackup.Update();
    
    ( ( CEdit* )GetDlgItem( IDC_DC_EDT_PATH ) )->UpdateData();
    
    UINT i = 0;
    for ( i = 0; i < m_arrBackFiles.size(); i++ )
    {
        m_arrBackFiles[i].isBackup = ( BYTE )m_lstFileBackup.GetCheck( i );
    }
    
    //  if(!m_strPath.IsEmpty())
    //  {
    //      m_strLocalPath=m_strPath;
    //  }
    
    if ( m_bSaveToLocal && m_strLocalPath.GetLength() > 2 && !g_theApp.m_fnOperationHelper.CreateDeepDirectory( m_strLocalPath ) )
    {
        _ASSERTE( 0 );  //lint !e506 !e774
    }
    //±£´æÉèÖÃ
    
    SaveSettings();
    
}


BOOL CPageCalibration::OnKillActive()
{
    // TODO: Add your specialized code here and/or call the base class
    this->SetFocus();
    /*if(m_lstBackup.m_pNvBkpItmArray == NULL && m_lstBackup.m_pTempNvBkpItmArray != NULL)
    {
        m_lstBackup.Update();
    }*/
    
    ( ( CEdit* )GetDlgItem( IDC_DC_EDT_PATH ) )->UpdateData();
    
    if ( m_bSaveToLocal && m_strLocalPath.GetLength() > 2 && !g_theApp.m_fnOperationHelper.CreateDeepDirectory( m_strLocalPath ) )
    {
        CString strErr;
        strErr.Format( _T( "Create directory [%s] fail." ), m_strLocalPath.operator LPCTSTR() );
        AfxMessageBox( strErr );
        return FALSE;
    }
    return CPropertyPage::OnKillActive();
}

void CPageCalibration::OnChkSaveToLocal()
{
    // TODO: Add your control notification handler code here
    m_bSaveToLocal = !m_bSaveToLocal;
    GetDlgItem( IDC_DC_EDT_PATH )->EnableWindow( m_bSaveToLocal );
    GetDlgItem( IDC_STC_VER_SAP )->EnableWindow( m_bSaveToLocal );
    GetDlgItem( IDC_DC_BTN_BROWSE )->EnableWindow( m_bSaveToLocal );
    GetDlgItem( IDC_STC_LOCAL_PATH )->EnableWindow( m_bSaveToLocal );
}

void CPageCalibration::OnDcBtnBrowse()
{
    CFileDlg fdlg;
    fdlg.SetHwnOwner( this->m_hWnd );
    CString strDir = fdlg.GetSelectDir();
    if ( strDir.IsEmpty() )
    { return; }
    
    m_strPath = strDir;
    GetDlgItem( IDC_DC_EDT_PATH )->SetWindowText( m_strPath );
}

void CPageCalibration::InitFileBackupList()
{
    m_lstFileBackup.SetBkColor( RGB( 232, 232, 232 ) );
    m_lstFileBackup.ModifyStyle( 0, LVS_SHOWSELALWAYS );
    
    DWORD dwExStyle = m_lstFileBackup.GetExtendedStyle();
    dwExStyle |= LVS_EX_FULLROWSELECT;
    dwExStyle |= LVS_EX_GRIDLINES;
    dwExStyle |= LVS_EX_CHECKBOXES;
    m_lstFileBackup.SetExtendedStyle( dwExStyle );
    
    //Load Column
    CString strClmn;
    VERIFY( strClmn.LoadString( IDS_FILE_BACK_COLUMN ) );
    int nLen   = strClmn.GetLength();
    LPTSTR lpBuf  = strClmn.GetBuffer( nLen );
    LPTSTR lpFind = _tcschr( lpBuf, _T( ',' ) );
    int     nIndex = 0;
    while ( lpFind != NULL )
    {
        *lpFind = _T( '\0' );
        m_lstFileBackup.InsertColumn( nIndex++, lpBuf, LVCFMT_LEFT, 100 );
        lpBuf = lpFind + 1;
        lpFind = _tcschr( lpBuf, _T( ',' ) );
    }
    m_lstFileBackup.InsertColumn( nIndex, lpBuf, LVCFMT_LEFT, 100 );
    strClmn.ReleaseBuffer();
    
    m_lstFileBackup.EnableToolTips();
    
    for ( UINT i = 0; i < m_arrBackFiles.size(); i++ )
    {
        m_lstFileBackup.InsertItem( i, m_arrBackFiles[i].szID );
        CString strText;
        if ( _tcslen( m_arrBackFiles[i].arrBlock[0].szRepID ) != 0 )
        {
            m_lstFileBackup.SetItemText( i, 1, m_arrBackFiles[i].arrBlock[0].szRepID );
        }
        else
        {
            strText.Format( _T( "0x%08X" ), m_arrBackFiles[i].arrBlock[0].llBase );
            m_lstFileBackup.SetItemText( i, 1, strText );
        }
        
        strText.Format( _T( "0x%X" ), m_arrBackFiles[i].arrBlock[0].llSize );
        m_lstFileBackup.SetItemText( i, 2, strText );
        
        m_lstFileBackup.SetCheck( i, m_arrBackFiles[i].isBackup );
    }
    
    if ( m_arrBackFiles.size() )
    {
        m_lstFileBackup.SetItemState( 0, LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED );
        m_lstFileBackup.SetFocus();
    }
    
}

void CPageCalibration::OnNMClickLstFileBackup( NMHDR* pNMHDR, LRESULT* pResult )
{
    // TODO: Add your control notification handler code here
    
    LPNMITEMACTIVATE pnia = ( LPNMITEMACTIVATE )pNMHDR;
    int nItem = pnia->iItem;
    int nSubItem = pnia->iSubItem;
    if ( nItem == -1 || nSubItem == -1 )
    {
        return;
    }
    
    CString strID = m_lstFileBackup.GetItemText( nItem, 0 );
    if ( _tcsnicmp( strID.operator LPCTSTR(), _T( "NV" ), 2 ) == 0 ) //is NV file
    {
        m_lstBackup.FillList( strID.operator LPCTSTR(), TRUE );
    }
    else
    {
        m_lstBackup.DeleteAll();
    }
    *pResult = 0;
}

void CPageCalibration::ReleaseProperty()
{
    m_arrBackFiles.clear();
}

BOOL CPageCalibration::GetProperty()
{
    CMainFrame* pMainFrame = ( CMainFrame* )AfxGetMainWnd();
    
    SPRESULT spRlt = SP_OK;
    
    DLFW_ATTRDATA_COMMON data;
    g_theApp.m_fnOperationHelper.CreatCommonData( g_theApp.m_attrStackSize.u32BackupFiles, &data );
    spRlt &= pMainFrame->m_pFrameHelper->GetProperty( DLFW_ATTR_BACKUP_FILES, 0, &data );
    spRlt &= g_theApp.m_fnOperationHelper.ConfigBackupFilesByAttrData( m_arrBackFiles, &data );
    g_theApp.m_fnOperationHelper.ReleaseCommonData( &data );
    
    if ( SP_OK != spRlt )
    {
        MessageBox( _T( "Get Parameters From DLFramework Fail." ) );
    }
    
    return SP_OK == spRlt ? TRUE : FALSE;
    
}

BOOL CPageCalibration::SetProperty()
{
    CMainFrame* pMainFrame = ( CMainFrame* )AfxGetMainWnd();
    SPRESULT spRlt = SP_OK;
    
    DLFW_ATTRDATA_COMMON data;
    g_theApp.m_fnOperationHelper.CreatCommonData( m_arrBackFiles.size(), &data );
    spRlt &= g_theApp.m_fnOperationHelper.ConfigAttrDataByBackupFiles( m_arrBackFiles, &data );
    spRlt &= pMainFrame->m_pFrameHelper->SetProperty( DLFW_ATTR_BACKUP_FILES, 0, &data );
    g_theApp.m_fnOperationHelper.ReleaseCommonData( &data );
    
    if ( SP_OK != spRlt )
    {
        MessageBox( _T( "Set Parameters To DLFramework Fail." ) );
    }
    
    return SP_OK == spRlt ? TRUE : FALSE;
}