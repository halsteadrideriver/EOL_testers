// PageLcdConfig.cpp : implementation file
//

#include "stdafx.h"
#include "dloader.h"
#include "PageLcdConfig.h"
#include "SettingsSheet.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageLcdConfig property page

IMPLEMENT_DYNCREATE( CPageLcdConfig, CPropertyPage )

CPageLcdConfig::CPageLcdConfig() : CPropertyPage( CPageLcdConfig::IDD )
{
    //{{AFX_DATA_INIT(CPageLcdConfig)
    m_strTips = _T( "" );
    //}}AFX_DATA_INIT
    
    m_bListInit = FALSE;
    m_bRightPS = FALSE;
    m_strPSFile = _T( "" );
}

CPageLcdConfig::~CPageLcdConfig()
{
}

void CPageLcdConfig::DoDataExchange( CDataExchange* pDX )
{
    CPropertyPage::DoDataExchange( pDX );
    //{{AFX_DATA_MAP(CPageLcdConfig)
    DDX_Control( pDX, IDC_LCP_LST_LCD_CONFIG, m_lstLCD );
    DDX_Text( pDX, IDC_LCD_STC_TIPS, m_strTips );
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP( CPageLcdConfig, CPropertyPage )
    //{{AFX_MSG_MAP(CPageLcdConfig)
    ON_NOTIFY( LVN_ITEMCHANGED, IDC_LCP_LST_LCD_CONFIG, OnItemchangedLst )
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageLcdConfig message handlers

BOOL CPageLcdConfig::OnInitDialog()
{
    CPropertyPage::OnInitDialog();
    
    // TODO: Add extra initialization here
    m_lstLCD.SetBkColor( RGB( 232, 232, 232 ) );
    m_lstLCD.ModifyStyle( 0, LVS_SHOWSELALWAYS );
    
    DWORD dwExStyle = m_lstLCD.GetExtendedStyle();
    dwExStyle |= LVS_EX_FULLROWSELECT;
    //  dwExStyle |= LVS_EX_GRIDLINES;
    dwExStyle |= LVS_EX_CHECKBOXES;
    
    m_lstLCD.SetExtendedStyle( dwExStyle );
    
    m_lstLCD.SetColumnWidth( 0, 200 );
    
    m_strTips.LoadString( IDS_LCD_TIPS );
    
    ShowLCDCfig( m_bRightPS );
    
    UpdateData( FALSE );
    
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
BOOL CPageLcdConfig::LoadSettings()
{
    return TRUE;
    
}
BOOL CPageLcdConfig::SaveSettings()
{
    return TRUE;
}

BOOL CPageLcdConfig::FillList()
{
    m_bListInit = FALSE;
    
    int nLcdCfigNum = m_arrLcdCfig.size();
    m_lstLCD.DeleteAllItems();
    
    for ( int i = 0; i < nLcdCfigNum; i++ )
    {
        int nItem = m_lstLCD.GetItemCount();
        m_lstLCD.InsertItem( nItem, m_arrLcdCfig[i].szName );
        
        if ( m_arrLcdCfig[i].dwFlag > 0 )
        {
            m_lstLCD.SetCheck( nItem );
        }
    }
    
    m_bListInit = TRUE;
    
    return TRUE;
}

void CPageLcdConfig::ShowLCDCfig( BOOL bShow )
{
    m_lstLCD.ShowWindow( bShow ? SW_SHOW : SW_HIDE );
    GetDlgItem( IDC_LCD_STC_TIPS )->ShowWindow( bShow ? SW_HIDE : SW_SHOW );
    FillList();
}

void CPageLcdConfig::OnOK()
{
    // TODO: Add your specialized code here and/or call the base class
    UpdateData();
    
    int nCount = m_lstLCD.GetItemCount();
    int nSize = m_arrLcdCfig.size();
    ASSERT( nSize == nCount );
    
    for ( int i = 0; i < nCount; i++ )
    {
        m_arrLcdCfig[i].dwFlag = m_lstLCD.GetCheck( i );
    }
    
    SavePSFile();
    
    CPropertyPage::OnOK();
    
}

BOOL CPageLcdConfig::OnKillActive()
{
    // TODO: Add your specialized code here and/or call the base class
    
    
    return CPropertyPage::OnKillActive();
}

void CPageLcdConfig::OnItemchangedLst( NMHDR* pNMHDR, LRESULT* pResult )
{
    if ( !m_bListInit )
    {
        return;
    }
    
    NM_LISTVIEW* pNMListView = ( NM_LISTVIEW* )pNMHDR;
    
    int nItem = pNMListView->iItem;
    int nIndex = m_lstLCD.GetItemData( nItem );
    
    if ( nIndex >= ( int )m_arrLcdCfig.size() )
    {
        return;
    }
    
    BOOL bExistCheckFlag = FALSE;
    int i = 0;
    for ( i = 0; i < m_lstLCD.GetItemCount(); i++ )
    {
        if ( i != nItem && m_lstLCD.GetCheck( i ) )
        {
            bExistCheckFlag = TRUE;
            break;
        }
    }
    
    if ( bExistCheckFlag )
    {
        if ( pNMListView->uNewState == 0x00002000 ) // check
        {
            for ( i = 0; i < m_lstLCD.GetItemCount(); i++ )
            {
                if ( i != nItem && m_lstLCD.GetCheck( i ) )
                {
                    m_lstLCD.SetCheck( i, FALSE );
                }
            }
        }
    }
    else
    {
        if ( pNMListView->uNewState == 0x00001000 ) // uncheck
        {
            AfxMessageBox( _T( "[DL3032]Must select one LCD configure!!!" ) );
            m_lstLCD.SetCheck( nItem );
        }
    }
    
    UpdateData( FALSE );
    *pResult = 0;
}

BOOL CPageLcdConfig::SavePSFile()
{
    CString strPSFileName = m_strPSFile;
    
    if ( m_bRightPS && !strPSFileName.IsEmpty() && strPSFileName.CompareNoCase( FILE_OMIT ) != 0 )
    {
        int nVSize = m_arrLcdCfig.size();
        if ( nVSize == 0 )
        {
            return TRUE;
        }
        
        CFileFind finder;
        if ( finder.FindFile( strPSFileName ) )
        {
            DWORD dwAtt = GetFileAttributes( strPSFileName );
            dwAtt &= ~FILE_ATTRIBUTE_READONLY;
            SetFileAttributes( strPSFileName, dwAtt );
        }
        else
        {
            return TRUE;
        }
        
        HANDLE hFile = INVALID_HANDLE_VALUE;
        hFile = ::CreateFile( strPSFileName,
                              GENERIC_READ,
                              FILE_SHARE_READ,
                              NULL,
                              OPEN_EXISTING,
                              FILE_ATTRIBUTE_NORMAL,
                              NULL );
                              
        if ( hFile == INVALID_HANDLE_VALUE )
        {
            CString strFormatted;
            strFormatted.Format( _T( "[DL3026][F]Can not open file [%s]." ), strPSFileName.operator LPCTSTR() );
            AfxMessageBox( strFormatted );
            return FALSE;
        }
        
        DWORD dwSize = GetFileSize( hFile, NULL );
        BYTE* pBuf = NULL;
        if ( dwSize != 0 )
        {
            pBuf = new BYTE[dwSize];
            DWORD dwRealRead =  0;
            ReadFile( hFile, pBuf, dwSize, &dwRealRead, NULL );
        }
        CloseHandle( hFile );
        hFile = INVALID_HANDLE_VALUE;
        
        if ( dwSize != 0 )
        {
            VEC_LCD_CFIG vLcdCfig;
            if ( CLcdHelper::FindLCDItem( strPSFileName, vLcdCfig, NULL ) )
            {
                if ( CLcdHelper::CheckLCDConfig( vLcdCfig, m_arrLcdCfig ) )
                {
                    int nSize = vLcdCfig.size();
                    for ( int i = 0; i < nSize; i++ )
                    {
                        *( pBuf + vLcdCfig[i].dwFlagOffset ) = ( BYTE )m_arrLcdCfig[i].dwFlag; //lint !e613
                    }
                    
                    if ( nSize > 0 )
                    {
                        hFile = ::CreateFile( strPSFileName,
                                              GENERIC_WRITE,
                                              FILE_SHARE_READ,
                                              NULL,
                                              CREATE_ALWAYS,
                                              FILE_ATTRIBUTE_NORMAL,
                                              NULL );
                                              
                        if ( hFile == INVALID_HANDLE_VALUE )
                        {
                            CString strFormatted;
                            strFormatted.Format( _T( "[PK3130][B]Can not create file [%s]." ), strPSFileName.operator LPCTSTR() );
                            AfxMessageBox( strFormatted );
                            SAFE_DELETE_ARRAY( pBuf );
                            return FALSE;
                        }
                        else
                        {
                            DWORD dwWrite = 0;
                            WriteFile( hFile, pBuf, dwSize, &dwWrite, NULL );
                            CloseHandle( hFile );
                            hFile = INVALID_HANDLE_VALUE;
                            SAFE_DELETE_ARRAY( pBuf );
                        }
                    }
                }
                else
                {
                    CString strFormatted;
                    strFormatted.Format( _T( "PS/USER_IMG file [%s] is changed,please select it again." ), strPSFileName.operator LPCTSTR() );
                    AfxMessageBox( strFormatted );
                    SAFE_DELETE_ARRAY( pBuf );
                    return FALSE;
                }
            }
        }
        SAFE_DELETE_ARRAY( pBuf );
    }
    
    return TRUE;
}


BOOL CPageLcdConfig::SetProperty()
{
    CMainFrame* pMainFrame = ( CMainFrame* )AfxGetMainWnd();
    SPRESULT spRlt = SP_OK;
    
    spRlt &= pMainFrame->m_pFrameHelper->SetProperty( DLFW_ATTR_LCD_HAS, 0, &m_bRightPS );
    DLFW_ATTRDATA_COMMON data;
    g_theApp.m_fnOperationHelper.CreatCommonData( m_arrLcdCfig.size(), &data );
    g_theApp.m_fnOperationHelper.ConfigAttrDataByLcdConfig( m_arrLcdCfig, &data );
    spRlt &= pMainFrame->m_pFrameHelper->SetProperty( DLFW_ATTR_LCD_CONFIG, 0, &data );
    g_theApp.m_fnOperationHelper.ReleaseCommonData( &data );
    
    spRlt &= pMainFrame->m_pFrameHelper->SetProperty( DLFW_ATTR_LCD_FILE, 0, ( PVOID )m_strPSFile.operator LPCWSTR() );
    
    if ( SP_OK != spRlt )
    {
        MessageBox( _T( "Set Parameters To DLFramework Fail." ) );
    }
    
    return SP_OK == spRlt ? TRUE : FALSE;
}

void CPageLcdConfig::ReleaseProperty()
{
    m_arrLcdCfig.clear();
}

BOOL CPageLcdConfig::GetProperty()
{
    CMainFrame* pMainFrame = ( CMainFrame* )AfxGetMainWnd();
    SPRESULT spRlt = SP_OK;
    
    spRlt &= pMainFrame->m_pFrameHelper->GetProperty( DLFW_ATTR_LCD_HAS, 0, &m_bRightPS );
    
    DLFW_ATTRDATA_COMMON data;
    g_theApp.m_fnOperationHelper.CreatCommonData(g_theApp.m_attrStackSize.u32LCDConfig, &data );
    spRlt &= pMainFrame->m_pFrameHelper->GetProperty( DLFW_ATTR_LCD_CONFIG, 0, &data );
    spRlt &= g_theApp.m_fnOperationHelper.ConfigLcdConfigByAttrData( m_arrLcdCfig, &data );
    g_theApp.m_fnOperationHelper.ReleaseCommonData( &data );
    
    LPCTSTR lpString = NULL;
    spRlt &= pMainFrame->m_pFrameHelper->GetProperty( DLFW_ATTR_LCD_FILE, 0, &lpString );
    m_strPSFile = lpString;
    
    if ( SP_OK != spRlt )
    {
        MessageBox( _T( "Get Parameters From DLFramework Fail." ) );
    }
    
    return SP_OK == spRlt ? TRUE : FALSE;
    
}