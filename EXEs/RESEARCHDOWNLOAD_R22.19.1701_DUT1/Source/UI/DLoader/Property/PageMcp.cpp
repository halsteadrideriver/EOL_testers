// PageMcp.cpp : implementation file
//

#include "stdafx.h"
#include "dloader.h"
#include "PageMcp.h"
#include "Utility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_BUFF_SIZE         (1024*4)
/////////////////////////////////////////////////////////////////////////////
// CPageMcp property page

IMPLEMENT_DYNCREATE( CPageMcp, CPropertyPage )

CPageMcp::CPageMcp() : CPropertyPage( CPageMcp::IDD )
{
    //{{AFX_DATA_INIT(CPageMcp)
    m_bTmpCheckMcpType = FALSE;
    //}}AFX_DATA_INIT
    
    m_bCheckMcpType = m_bTmpCheckMcpType;
    
    m_bListInit = FALSE;
}

CPageMcp::~CPageMcp()
{
}

void CPageMcp::DoDataExchange( CDataExchange* pDX )
{
    CPropertyPage::DoDataExchange( pDX );
    //{{AFX_DATA_MAP(CPageMcp)
    DDX_Control( pDX, IDC_MCP_LST_MCP, m_lstMcpType );
    DDX_Check( pDX, IDC_MCP_CHK_MCPTYPE, m_bTmpCheckMcpType );
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP( CPageMcp, CPropertyPage )
    //{{AFX_MSG_MAP(CPageMcp)
    ON_BN_CLICKED( IDC_MCP_CHK_MCPTYPE, OnChkMcptype )
    ON_BN_CLICKED( IDC_MCP_BTN_ADD_MCP, OnAddMcp )
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageMcp message handlers

BOOL CPageMcp::OnInitDialog()
{
    CPropertyPage::OnInitDialog();
    
    // TODO: Add extra initialization here
    m_lstMcpType.SetBkColor( RGB( 232, 232, 232 ) );
    m_lstMcpType.ModifyStyle( 0, LVS_SHOWSELALWAYS );
    
    DWORD dwExStyle = m_lstMcpType.GetExtendedStyle();
    dwExStyle |= LVS_EX_FULLROWSELECT;
    dwExStyle |= LVS_EX_GRIDLINES;
    dwExStyle |= LVS_EX_CHECKBOXES;
    
    m_lstMcpType.SetExtendedStyle( dwExStyle );
    
    //Load Column
    CString strClmn;
    VERIFY( strClmn.LoadString( IDS_MCP_LIST_COL ) );
    
    int nLen   = strClmn.GetLength();
    LPTSTR lpBuf  = strClmn.GetBuffer( nLen );
    LPTSTR lpFind = _tcschr( lpBuf, _T( ',' ) );
    int     nIndex = 0;
    while ( lpFind != NULL )
    {
        *lpFind = _T( '\0' );
        m_lstMcpType.InsertColumn( nIndex++, lpBuf, LVCFMT_LEFT, 200 );
        
        lpBuf = lpFind + 1;
        lpFind = _tcschr( lpBuf, _T( ',' ) );
    }
    m_lstMcpType.InsertColumn( nIndex, lpBuf, LVCFMT_LEFT, 229 );
    strClmn.ReleaseBuffer();
    
    FillList();
    
    m_bTmpCheckMcpType = m_bCheckMcpType;
    m_lstMcpType.EnableWindow( m_bCheckMcpType );
    
    UpdateData( FALSE );
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPageMcp::LoadSettings()
{
    if ( NULL == g_theApp.m_pCfgObject )
    {
        ASSERT( 0 );
        AfxMessageBox( _T( "[DL2082][F]Configure file is empty!" ) );
        return FALSE;
    }
    
    g_theApp.m_fnOperationHelper.GetSectionMcp( g_theApp.m_pCfgObject, m_agMcpIDs, m_mapAllMcpType );
    
#if defined(_SPUPGRADE) || defined(_FACTORY)
    if ( g_theApp.m_bShowMcpTypePage )
    {
        m_bCheckMcpType = g_theApp.m_pCfgObject->GetInt( CFGTYPE_MCPT, _T( "MCPSetting" ), _T( "CheckMCPType" ), FALSE );
    }
    else
    {
        m_bCheckMcpType = FALSE; // not check MCP type
    }
#else
    m_bCheckMcpType = g_theApp.m_pCfgObject->GetInt( CFGTYPE_MCPT, _T( "MCPSetting" ), _T( "CheckMCPType" ), FALSE );
#endif
    
    m_mapCheckMcpType.RemoveAll();
    
    INT nCount = 0;
    CUtility utility;
    CString strMCPTypeRange = g_theApp.m_pCfgObject->GetString( CFGTYPE_MCPT, _T( "MCPSetting" ), _T( "MCPTypeRange" ), _T( "" ) );
    if ( strMCPTypeRange.GetLength() > 0 )
    {
        LPTSTR* lppString = utility.GetTokenString( strMCPTypeRange.operator LPCWSTR(), _T( "," ), nCount );
        if ( NULL != lppString && 0 != nCount )
        {
            for ( int i = 0; i < nCount; i++ )
            {
                CString strDesc = _T( "" );
                if ( m_mapAllMcpType.Lookup( lppString[i], strDesc ) )
                {
                    m_mapCheckMcpType.SetAt( lppString[i], strDesc );
                }
            }
        }
    }
    
    return TRUE;
    
}
BOOL CPageMcp::SaveSettings()
{
    if ( NULL == g_theApp.m_pCfgObject )
    {
        ASSERT( 0 );
        return FALSE;
    }
    
    g_theApp.m_pCfgObject->SetInt( CFGTYPE_MCPT, _T( "MCPSetting" ), _T( "CheckMCPType" ), m_bCheckMcpType );
    
    CString strValue;
    int nCount = m_lstMcpType.GetItemCount();
    for ( int i = 0; i < nCount; i++ )
    {
        if ( m_lstMcpType.GetCheck( i ) )
        {
            strValue += m_lstMcpType.GetItemText( i, 0 );
            strValue += _T( "," );
        }
    }
    strValue.TrimRight( _T( ',' ) );
    g_theApp.m_pCfgObject->SetString( CFGTYPE_MCPT, _T( "MCPSetting" ), _T( "MCPTypeRange" ), strValue.operator LPCTSTR() );
    
    return TRUE;
}

BOOL CPageMcp::FillList()
{
    //m_bListInit = FALSE;
    
    int nMCP = m_agMcpIDs.GetSize();
    
    for ( int i = 0; i < nMCP; i++ )
    {
        CString strID = m_agMcpIDs.GetAt( i );
        m_lstMcpType.InsertItem( i, strID );
        CString strDesc;
        if ( m_mapAllMcpType.Lookup( strID, strDesc ) )
        {
            m_lstMcpType.SetItemText( i, 1, strDesc );
        }
        
        if ( m_mapCheckMcpType.Lookup( strID, strDesc ) )
        {
            m_lstMcpType.SetCheck( i );
        }
    }
    
    //m_bListInit = TRUE;
    
    return TRUE;
}

void CPageMcp::OnChkMcptype()
{
    // TODO: Add your control notification handler code here
    UpdateData();
    m_lstMcpType.EnableWindow( m_bTmpCheckMcpType );
}

void CPageMcp::OnOK()
{
    // TODO: Add your specialized code here and/or call the base class
    UpdateData();
    m_bCheckMcpType = m_bTmpCheckMcpType;
    
    int nCount = m_lstMcpType.GetItemCount();
    m_mapCheckMcpType.RemoveAll();
    
    for ( int i = 0; i < nCount; i++ )
    {
        if ( m_lstMcpType.GetCheck( i ) )
        {
            CString strID = m_lstMcpType.GetItemText( i, 0 );
            CString strDesc = m_lstMcpType.GetItemText( i, 1 );
            m_mapCheckMcpType.SetAt( strID, strDesc );
        }
    }
    
    SaveSettings();
    
    CPropertyPage::OnOK();
    
}

BOOL CPageMcp::OnKillActive()
{
    // TODO: Add your specialized code here and/or call the base class
    UpdateData();
    int nCount = m_lstMcpType.GetItemCount();
    int i = 0;
    for ( i = 0; i < nCount; i++ )
    {
        if ( m_lstMcpType.GetCheck( i ) )
        {
            break;
        }
    }
    
    if ( m_bTmpCheckMcpType && i >= nCount )
    {
        AfxMessageBox( _T( "[DL3033]Please select at least one MCP type or uncheck \"Check MCP type\"!" ) );
        return FALSE;
    }
    
    return CPropertyPage::OnKillActive();
}

void CPageMcp::OnAddMcp()
{
    // TODO: Add your control notification handler code here
    
}
