// CPageCustomization.cpp : implementation file
//

#include "stdafx.h"
#include "dloader.h"
#include "PageCustomization.h"
#include "BMAGlobal.h"
#include "MainFrm.h"
#include <atlconv.h>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BOOL g_bInitSheet;


/////////////////////////////////////////////////////////////////////////////
// CPageCustomization property page

IMPLEMENT_DYNCREATE( CPageCustomization, CPropertyPage )

CPageCustomization::CPageCustomization() : CPropertyPage( CPageCustomization::IDD )
    , m_strEditCU( _T( "" ) )
    , m_bChangedCU( FALSE )
    , m_bCheckRoot( FALSE )
    , m_bCheckBadFlashRate( FALSE )
    , m_nEditBadFlashRate( 0 )
    , m_nBadFlashRate( -1 )
    , m_bCheckInproduction( FALSE )
{

}

CPageCustomization::~CPageCustomization()
{
}

void CPageCustomization::DoDataExchange( CDataExchange* pDX )
{
    CPropertyPage::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_CHECK_CU, m_btnCheckCU );
    DDX_Control( pDX, IDC_EDIT_CU, m_editCU );
    DDX_Text( pDX, IDC_EDIT_CU, m_strEditCU );
    DDX_Check( pDX, IDC_CHECK_CU, m_bChangedCU );
    DDX_Check( pDX, IDC_CHECK_ROOT, m_bCheckRoot );
    DDX_Control( pDX, IDC_CHECK_ROOT, m_btnCheckRoot );
    DDX_Check( pDX, IDC_CHECK_BAD_FLASH_RATE, m_bCheckBadFlashRate );
    DDX_Control( pDX, IDC_CHECK_BAD_FLASH_RATE, m_btnBadFlashRate );
    DDX_Control( pDX, IDC_EDIT_BAD_FLASH_RATE, m_editBadFlashRate );
    DDX_Text( pDX, IDC_EDIT_BAD_FLASH_RATE, m_nEditBadFlashRate );
    DDX_Control( pDX, IDC_GROUP_CU, m_GroupCu );
    DDX_Control( pDX, IDC_GROUP_ROOT, m_GroupRoot );
    DDX_Check( pDX, IDC_CHECK_INPRODUCTION, m_bCheckInproduction );
}


BEGIN_MESSAGE_MAP( CPageCustomization, CPropertyPage )
    ON_BN_CLICKED( IDC_CHECK_CU, &CPageCustomization::OnBnClickedCheckCu )
    ON_BN_CLICKED( IDC_CHECK_BAD_FLASH_RATE, &CPageCustomization::OnBnClickedCheckBadFlashRate )
END_MESSAGE_MAP()


BOOL CPageCustomization::LoadSettings( )
{
    if ( NULL == g_theApp.m_pCfgObject )
    {
        ASSERT( 0 );
        AfxMessageBox( _T( "[DL2082][F]Configure file is empty!" ) );
        return FALSE;
    }
    
    m_bSupportCU         = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, _T( "Options" ), _T( "SupportCU" ), FALSE );
    m_strEditCU          = g_theApp.m_pCfgObject->GetString( CFGTYPE_DL, _T( "Customization" ), _T( "CU" ), _T( "" ) );
    m_bChangedCU         = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, _T( "Customization" ), _T( "ChangeCU" ), 0 );
    m_bCheckRoot         = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, _T( "Customization" ), _T( "CheckRoot" ), 0 );
    m_nEditBadFlashRate  = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, _T( "Customization" ), _T( "BadFlashRate" ), 0 );
    m_bCheckBadFlashRate = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, _T( "Customization" ), _T( "CheckBadFlashRate" ), 0 );
    m_bCheckInproduction = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, _T( "Customization" ), _T( "Inproduction" ), 0 );
    
    return TRUE;
}

BOOL CPageCustomization::SaveSettings( )
{
    if ( NULL == g_theApp.m_pCfgObject )
    {
        ASSERT( 0 );
        return FALSE;
    }
    
    g_theApp.m_pCfgObject->SetString( CFGTYPE_DL, _T( "Customization" ), _T( "CU" ), m_strEditCU.operator LPCWSTR() );
    g_theApp.m_pCfgObject->SetInt( CFGTYPE_DL, _T( "Customization" ), _T( "ChangeCU" ), m_bChangedCU );
    g_theApp.m_pCfgObject->SetInt( CFGTYPE_DL, _T( "Customization" ), _T( "CheckRoot" ), m_bCheckRoot );
    g_theApp.m_pCfgObject->SetInt( CFGTYPE_DL, _T( "Customization" ), _T( "BadFlashRate" ), m_nEditBadFlashRate );
    g_theApp.m_pCfgObject->SetInt( CFGTYPE_DL, _T( "Customization" ), _T( "CheckBadFlashRate" ), m_bCheckBadFlashRate );
    g_theApp.m_pCfgObject->SetInt( CFGTYPE_DL, _T( "Customization" ), _T( "Inproduction" ), m_bCheckInproduction );
    
    return TRUE;
}

BOOL CPageCustomization::OnInitDialog()
{
    CPropertyPage::OnInitDialog();
    m_strCU = m_strEditCU;
    m_editCU.EnableWindow( m_bChangedCU );
    m_editBadFlashRate.EnableWindow( m_bCheckBadFlashRate );
    UpdateData( FALSE );
    
    if ( !m_bSupportCU )
    {
        m_btnCheckCU.ShowWindow( SW_HIDE );
        m_editCU.ShowWindow( SW_HIDE );
        m_btnCheckRoot.ShowWindow( SW_HIDE );
        m_GroupCu.ShowWindow( SW_HIDE );
        m_GroupRoot.ShowWindow( SW_HIDE );
    }
    return TRUE;
}

BOOL CPageCustomization::OnKillActive()
{
    // TODO: Add your specialized code here and/or call the base class
    this->SetFocus();
    if ( !g_bInitSheet )
    {
        UpdateData();
        if ( m_btnCheckCU.GetCheck() )
        {
            CString strCu;
            m_editCU.GetWindowText( strCu );
            
            if ( strCu.IsEmpty() )
            {
                AfxMessageBox( _T( "[DL3029]Please input CU string." ) );
                return FALSE;
            }
            
            if ( strCu.GetLength() >= TCT_CU_REF_LEN )
            {
                AfxMessageBox( _T( "[DL3030]The length of CU can't be longer than 19 characters." ) );
                return FALSE;
            }
        }
        if ( m_btnBadFlashRate.GetCheck() )
        {
            CString strBadFlashRate;
            m_editBadFlashRate.GetWindowText( strBadFlashRate );
            
            if ( strBadFlashRate.IsEmpty() )
            {
                AfxMessageBox( _T( "[DL3031]Please input bad flash rate." ) );
                return FALSE;
            }
        }
    }
    return CPropertyPage::OnKillActive();
}

void CPageCustomization::OnOK()
{
    UpdateData( TRUE );
    m_strCU = _T( "" );
    m_nBadFlashRate = -1;
    if ( m_btnCheckCU.GetCheck() )
    {
        m_strCU = m_strEditCU;
    }
    if ( m_btnBadFlashRate.GetCheck() )
    {
        m_nBadFlashRate = m_nEditBadFlashRate;
    }
    SaveSettings( );
    CPropertyPage::OnOK();
}

void CPageCustomization::OnBnClickedCheckCu()
{
    if ( m_btnCheckCU.GetCheck() )
    {
        m_editCU.EnableWindow( TRUE );
    }
    else
    {
        m_editCU.EnableWindow( FALSE );
    }
}

void CPageCustomization::OnBnClickedCheckBadFlashRate()
{
    if ( m_btnBadFlashRate.GetCheck() )
    {
        m_editBadFlashRate.EnableWindow( TRUE );
    }
    else
    {
        m_editBadFlashRate.EnableWindow( FALSE );
    }
}
