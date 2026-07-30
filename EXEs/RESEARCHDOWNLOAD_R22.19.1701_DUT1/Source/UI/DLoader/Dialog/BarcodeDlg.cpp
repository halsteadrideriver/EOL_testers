// BarcodeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BarcodeDlg.h"
#include "DLoader.h"
#include <atlconv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDLE_TIMER      11
#define IDLE_TIMEOUT    100


/////////////////////////////////////////////////////////////////////////////
// CBarcodeDlg dialog

CBarcodeDlg::CBarcodeDlg( CWnd* pParent /*=NULL*/ )
    : CDialog( CBarcodeDlg::IDD, pParent )
{
    m_strBarcode = _T( "" );
    m_dwTick = 0;
    m_nPort = -1;
    m_nBarcodeLength = 14;
    m_bAutoStart = FALSE;
}

CBarcodeDlg::CBarcodeDlg( int nPort, CWnd* pParent /*= NULL*/ )
    : CDialog( CBarcodeDlg::IDD, pParent )
{
    CBarcodeDlg::CBarcodeDlg( pParent );
    
    m_dwTick = 0;
    m_nPort = nPort;
    m_nBarcodeLength = 14;
    m_bAutoStart = FALSE;
}

CBarcodeDlg::~CBarcodeDlg()
{

}

void CBarcodeDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    //{{AFX_DATA_MAP(CBarcodeDlg)
    DDX_Control( pDX, IDC_BUTTON_SUSPEND, m_btnPause );
    DDX_Control( pDX, IDOK, m_btnOK );
    DDX_Control( pDX, IDC_EDIT_BARCODE, m_edtBarcode );
    DDX_Text( pDX, IDC_EDIT_BARCODE, m_strBarcode );
    //  DDV_MaxChars(pDX, m_strBarcode, 14);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP( CBarcodeDlg, CDialog )
    //{{AFX_MSG_MAP(CBarcodeDlg)
    ON_BN_CLICKED( IDC_BUTTON_SUSPEND, OnButtonSuspend )
    ON_WM_CTLCOLOR()
    ON_EN_CHANGE( IDC_EDIT_BARCODE, OnChangeEditBarcode )
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBarcodeDlg message handlers

BOOL CBarcodeDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    
    UNUSED_ALWAYS( IDI_ICON_OK );
    UNUSED_ALWAYS( IDI_ICON_PAUSE );
    ASSERT( g_theApp.m_pCfgObject );
    //Set ICON
    //m_btnOK.SetIcon(IDI_ICON_OK);
    //m_btnOK.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 10);
    m_btnOK.SetIcon( ::LoadIcon( NULL, MAKEINTRESOURCE( IDI_ICON_OK ) ) );
    
    m_btnOK.EnableWindow( FALSE );
    
    //m_btnPause.SetIcon(IDI_ICON_PAUSE);
    //m_btnPause.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 10);
    
    //Load Config
    m_nBarcodeLength    = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, _T( "SN" ),  _T( "SN_LENGTH" ), 14 );
    m_bAutoStart        = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, _T( "SN" ),  _T( "AutoStartAfterInputSN" ), 1 );
    
    //Set Title
    m_str_title.Format( _T( "PORT:%d, Please input SN(Length: %d):" ), m_nPort, m_nBarcodeLength );
    SetWindowText( m_str_title );
    m_edtBarcode.SetLimitText( m_nBarcodeLength );
    
    UpdateData( FALSE );
    m_edtBarcode.SetFocus();
    
    return FALSE;
}

void CBarcodeDlg::OnCancel()
{
    EndDialog( IDIGNORE );
    return;
}

void CBarcodeDlg::OnButtonSuspend()
{
    EndDialog( IDIGNORE );
}

void CBarcodeDlg::OnOK()
{
    UpdateData( TRUE );
    
    BOOL bIsSNValid = TRUE;
    int i = 0;
    for ( i = 0; i < m_strBarcode.GetLength(); i++ )
    {
        if ( !( _istdigit( m_strBarcode.GetAt( i ) ) ||
                ( ( m_strBarcode.GetAt( i ) >= 0X61 && m_strBarcode.GetAt( i ) <= 0X7A ) //  >'a'  <'z'
                  || ( m_strBarcode.GetAt( i ) >= 0x41 && m_strBarcode.GetAt( i ) <= 0x5A ) )
              ) )
        {
            bIsSNValid = FALSE;
            break;
        }
    }
    if ( !bIsSNValid )
    {
        AfxMessageBox( _T( "[DL3001][A]There are invalid chars in the SN inputted, please input again" ) );
        return;
    }
    
    if ( m_strBarcode.IsEmpty() )
    {
        GetDlgItem( IDC_EDIT_BARCODE )->SetFocus();
        return;
    }
    else if ( m_strBarcode.GetLength() !=  m_nBarcodeLength )
    {
        GetDlgItem( IDC_EDIT_BARCODE )->SetFocus();
        return;
    }
    
    for ( i = 0; i < m_strBarcode.GetLength(); i++ )
    {
        TCHAR ch = m_strBarcode.GetAt( i );
        if ( !_istalnum( ch ) )
        {
            AfxMessageBox( _T( "[DL3001][B]There are invalid chars in the SN inputted, please input again" ) );
            return;
        }
    }
    
    EndDialog( IDOK );
}

BOOL CBarcodeDlg::PreTranslateMessage( MSG* pMsg )
{
    /*if(pMsg->message == WM_KEYUP || pMsg->message == WM_KEYDOWN)
    {
        DWORD dwTick = m_dwTick;
        m_dwTick = ::GetTickCount();
    
        char c = (char)pMsg->wParam;
        if(isdigit(c))
        {
            return FALSE;
        }
    
        if(isalpha(c))
        {
            return FALSE;
        }
    
        if(m_dwTick - dwTick < 100)
        {
            return TRUE;
        }
    }*/
    
    return CDialog::PreTranslateMessage( pMsg );
}

void CBarcodeDlg::OnChangeEditBarcode()
{
    CString strTitle;
    GetWindowText( strTitle );
    CString strText;
    m_edtBarcode.GetWindowText( strText );
    
    CString str;
    str.Format( _T( "%d" ), strText.GetLength() );
    str += _T( "  :  " );
    str += m_str_title;
    SetWindowText( str );
    
    if ( strText.GetLength() == m_nBarcodeLength )
    {
        m_btnOK.EnableWindow( TRUE );
        if ( m_bAutoStart )
        {
            OnOK();
        }
    }
    else
    {
        m_btnOK.EnableWindow( FALSE );
        m_edtBarcode.SetFocus();
        Invalidate();
    }
}

