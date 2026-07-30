// CMutiSoftSelectDlg.cpp: 实现文件
//
#include "StdAfx.h"
#include "MutiSoftSelectDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "dloader.h"
// CMutiSoftSelectDlg 对话框

IMPLEMENT_DYNAMIC( CMutiSoftSelectDlg, CDialog )

CMutiSoftSelectDlg::CMutiSoftSelectDlg( CWnd* pParent /*=nullptr*/ )
    : CDialog( IDD_DIALOG_MUTI_SOFT, pParent )
{
    m_nCurSel = 0;
}

CMutiSoftSelectDlg::~CMutiSoftSelectDlg()
{

}

BOOL CMutiSoftSelectDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    this->SetWindowText( _T( "Multiple Software" ) );
    m_nCurSel = 0;
    if ( m_arrProjectConfig.size() > 0 )
    {
        for ( int i = 0; i < ( int )m_arrProjectConfig.size(); i++ )
        {
            if ( 1 == m_arrProjectConfig[i].nCheck )
            {
                m_nCurSel = i;
            }
            m_ComboProdSelect.AddString( m_arrProjectConfig[i].szProjectID );
        }
    }
    m_ComboProdSelect.SetCurSel( m_nCurSel );
    m_ComboProdSelect.SetFocus();
    return TRUE;
}
void CMutiSoftSelectDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_COMBO_PROD_SELECT, m_ComboProdSelect );
}


BEGIN_MESSAGE_MAP( CMutiSoftSelectDlg, CDialog )
    ON_CBN_SELCHANGE( IDC_COMBO_PROD_SELECT, &CMutiSoftSelectDlg::OnCbnSelchangeComboProdSelect )
END_MESSAGE_MAP()


// CMutiSoftSelectDlg 消息处理程序

void CMutiSoftSelectDlg::OnCbnSelchangeComboProdSelect()
{
    // TODO: 在此添加控件通知处理程序代码
    m_arrProjectConfig[m_nCurSel].nCheck = 0;
    m_nCurSel = m_ComboProdSelect.GetCurSel();
    m_arrProjectConfig[m_nCurSel].nCheck = 1;
}

BOOL CMutiSoftSelectDlg::GetProperty()
{
    CMainFrame* pMainFrame = ( CMainFrame* )AfxGetMainWnd();
    SPRESULT spRlt = SP_OK;
    
    DLFW_ATTRDATA_COMMON data;
    g_theApp.m_fnOperationHelper.CreatCommonData( g_theApp.m_attrStackSize.u32ProjectConfig, &data );
    spRlt &= pMainFrame->m_pFrameHelper->GetProperty( DLFW_ATTR_PROJECT_CONFIG_ALL, 0, &data );
    spRlt &= g_theApp.m_fnOperationHelper.ConfigProjectConfigByAttrData( m_arrProjectConfig, &data );
    g_theApp.m_fnOperationHelper.ReleaseCommonData( &data );
    
    if ( SP_OK != spRlt )
    {
        MessageBox( _T( "Get Parameters From DLFramework Fail." ) );
    }
    return SP_OK == spRlt ? TRUE : FALSE;
}

BOOL CMutiSoftSelectDlg::SetProperty()
{
    CMainFrame* pMainFrame = ( CMainFrame* )AfxGetMainWnd();
    SPRESULT spRlt = SP_OK;
    spRlt &= pMainFrame->m_pFrameHelper->SetProperty( DLFW_ATTR_PROJECT_CONFIG, 0, ( LPVOID )&m_arrProjectConfig[m_nCurSel] );
    if ( SP_OK != spRlt )
    {
        MessageBox( _T( "Get Parameters From DLFramework Fail." ) );
    }
    
    return SP_OK == spRlt ? TRUE : FALSE;
}