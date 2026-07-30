// CPreloadSelectDlg.cpp: 实现文件
//
#include "StdAfx.h"
#include "PreloadSelectDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "dloader.h"
// CPreloadSelectDlg 对话框

IMPLEMENT_DYNAMIC( CPreloadSelectDlg, CDialog )

CPreloadSelectDlg::CPreloadSelectDlg( CWnd* pParent /*=nullptr*/ )
    : CDialog( IDD_DIALOG_MUTI_SOFT, pParent )
{
    m_nCurSel = 0;
}

CPreloadSelectDlg::~CPreloadSelectDlg()
{

}

BOOL CPreloadSelectDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    this->SetWindowText( _T( "Preload Select" ) );
    m_nCurSel = 0;
    if ( m_arrPreloadConfig.size() > 0 )
    {
        for ( int i = 0; i < ( int )m_arrPreloadConfig.size(); i++ )
        {
            if ( 1 == m_arrPreloadConfig[i].nCheck )
            {
                m_nCurSel = i;
            }
            CString strSelectText = m_arrPreloadConfig[i].szCountryName;
            strSelectText += _T( "-" );
            strSelectText += m_arrPreloadConfig[i].szCountryID;
            m_ComboProdSelect.AddString( strSelectText );
        }
    }
    m_ComboProdSelect.SetCurSel( m_nCurSel );
    m_ComboProdSelect.SetFocus();
    return TRUE;
}
void CPreloadSelectDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_COMBO_PROD_SELECT, m_ComboProdSelect );
}


BEGIN_MESSAGE_MAP( CPreloadSelectDlg, CDialog )
    ON_CBN_SELCHANGE( IDC_COMBO_PROD_SELECT, &CPreloadSelectDlg::OnCbnSelchangeComboProdSelect )
END_MESSAGE_MAP()


// CPreloadSelectDlg 消息处理程序

void CPreloadSelectDlg::OnCbnSelchangeComboProdSelect()
{
    // TODO: 在此添加控件通知处理程序代码
    m_arrPreloadConfig[m_nCurSel].nCheck = 0;
    m_nCurSel = m_ComboProdSelect.GetCurSel();
    m_arrPreloadConfig[m_nCurSel].nCheck = 1;
}

BOOL CPreloadSelectDlg::GetProperty()
{
    CMainFrame* pMainFrame = ( CMainFrame* )AfxGetMainWnd();
    SPRESULT spRlt = SP_OK;

    DLFW_ATTRDATA_COMMON data;
    g_theApp.m_fnOperationHelper.CreatCommonData( g_theApp.m_attrStackSize.u32ProjectConfig, &data );
    spRlt &= pMainFrame->m_pFrameHelper->GetProperty( DLFW_ATTR_PRELOAD_CONFIG_ALL, 0, &data );
    spRlt &= g_theApp.m_fnOperationHelper.ConfigPreloadConfigByAttrData( m_arrPreloadConfig, &data );
    g_theApp.m_fnOperationHelper.ReleaseCommonData( &data );

    if ( SP_OK != spRlt )
    {
        MessageBox( _T( "Get Parameters From DLFramework Fail." ) );
    }
    return SP_OK == spRlt ? TRUE : FALSE;
}

BOOL CPreloadSelectDlg::SetProperty()
{
    CMainFrame* pMainFrame = ( CMainFrame* )AfxGetMainWnd();
    SPRESULT spRlt = SP_OK;
    spRlt &= pMainFrame->m_pFrameHelper->SetProperty( DLFW_ATTR_PRELOAD_CONFIG, 0, ( LPVOID )&m_arrPreloadConfig[m_nCurSel] );
    if ( SP_OK != spRlt )
    {
        MessageBox( _T( "Get Parameters From DLFramework Fail." ) );
    }

    return SP_OK == spRlt ? TRUE : FALSE;
}