// PageMiscOptions.cpp : implementation file
//

#include "stdafx.h"
#include "dloader.h"
#include "PageMiscOptions.h"
#include "afxdialogex.h"



// CPageMiscOptions dialog

IMPLEMENT_DYNAMIC( CPageMiscOptions, CPropertyPage )

CPageMiscOptions::CPageMiscOptions()
    : CPropertyPage( CPageMiscOptions::IDD )
    , m_strTmpSelinux( _T( "" ) )
{

}

CPageMiscOptions::~CPageMiscOptions()
{
}

void CPageMiscOptions::DoDataExchange( CDataExchange* pDX )
{
    CPropertyPage::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_COMBO_SELINUX, m_comboSelinux );
}


BEGIN_MESSAGE_MAP( CPageMiscOptions, CPropertyPage )
    ON_CBN_SELCHANGE( IDC_COMBO_SELINUX, &CPageMiscOptions::OnCbnSelchangeComboSelinux )
END_MESSAGE_MAP()


// CPageMiscOptions message handlers


BOOL CPageMiscOptions::OnInitDialog()
{
    CPropertyPage::OnInitDialog();
    
    // TODO:  Add extra initialization here
    CString strSelinux = m_strSelinux.Right( 1 );
    int nIndex = 0;
    if ( strSelinux.Compare( _T( "-" ) ) == 0 )
    {
        nIndex = 0;
    }
    else if ( strSelinux.Compare( _T( "0" ) ) == 0 )
    {
        nIndex = 1;
    }
    else if ( strSelinux.Compare( _T( "1" ) ) == 0 )
    {
        nIndex = 2;
    }
    m_comboSelinux.SetCurSel( nIndex );
    m_strTmpSelinux = m_strSelinux;
    UpdateData( FALSE );
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}


void CPageMiscOptions::OnOK()
{
    // TODO: Add your specialized code here and/or call the base class
    UpdateData();
    m_strSelinux = m_strTmpSelinux;
    
    SaveSettings( );
    
    CPropertyPage::OnOK();
}

BOOL CPageMiscOptions::LoadSettings()
{
    //  UNUSED_ALWAYS(strErrorMsg);
    if ( NULL == g_theApp.m_pCfgObject )
    {
        ASSERT( 0 );
        AfxMessageBox( _T( "[DL2082][E]Configure file is empty!" ) );
        return FALSE;
    }
    
    m_strSelinux = g_theApp.m_pCfgObject->GetString( CFGTYPE_DL, _T( "Selinux" ), _T( "Data" ), _T( "Selinux:-" ) );
    
    return TRUE;
}

BOOL CPageMiscOptions::SaveSettings( )
{
    if ( NULL == g_theApp.m_pCfgObject )
    {
        ASSERT( 0 );
        return FALSE;
    }
    
    g_theApp.m_pCfgObject->SetString( CFGTYPE_DL, _T( "Selinux" ), _T( "Data" ), m_strSelinux.operator LPCWSTR() );
    
    return TRUE;
}


void CPageMiscOptions::OnCbnSelchangeComboSelinux()
{
    // TODO: Add your control notification handler code here
    CString strSelinux;
    int nSel = m_comboSelinux.GetCurSel();
    m_comboSelinux.GetLBText( nSel, strSelinux );
    int nIndex = m_strTmpSelinux.GetLength() - 1;
    m_strTmpSelinux.SetAt( nIndex, strSelinux.GetBuffer()[0] );
}
