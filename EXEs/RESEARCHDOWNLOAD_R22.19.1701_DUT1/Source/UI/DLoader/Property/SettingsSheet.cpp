// SettingsSheet.cpp : implementation file
//

#include "stdafx.h"
#include "dloader.h"
#include "SettingsSheet.h"
#include "MainFrm.h"
#include <afxpriv.h>
#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_RESIZEPAGE (WM_APP+1)

enum { CDF_CENTER, CDF_TOPLEFT, CDF_NONE };

BOOL g_bInitSheet = FALSE;

static void ChangeDialogFont( CWnd* pWnd, CFont* pFont, int nFlag )
{
    CRect windowRect;
    
    // grab old and new text metrics
    TEXTMETRIC tmOld, tmNew;
    CDC* pDC = pWnd->GetDC();
    CFont* pSavedFont = pDC->SelectObject( pWnd->GetFont() );
    pDC->GetTextMetrics( &tmOld );
    pDC->SelectObject( pFont );
    pDC->GetTextMetrics( &tmNew );
    pDC->SelectObject( pSavedFont );
    pWnd->ReleaseDC( pDC );
    
    long oldHeight = tmOld.tmHeight + tmOld.tmExternalLeading;
    long newHeight = tmNew.tmHeight + tmNew.tmExternalLeading;
    
    if ( nFlag != CDF_NONE )
    {
        // calculate new dialog window rectangle
        CRect clientRect, newClientRect, newWindowRect;
        
        pWnd->GetWindowRect( windowRect );
        pWnd->GetClientRect( clientRect );
        long xDiff = windowRect.Width() - clientRect.Width();
        long yDiff = windowRect.Height() - clientRect.Height();
        
        newClientRect.left = newClientRect.top = 0;
        newClientRect.right = clientRect.right * tmNew.tmAveCharWidth / tmOld.tmAveCharWidth;
        newClientRect.bottom = clientRect.bottom * newHeight / oldHeight;
        
        if ( nFlag == CDF_TOPLEFT ) // resize with origin at top/left of window
        {
            newWindowRect.left = windowRect.left;
            newWindowRect.top = windowRect.top;
            newWindowRect.right = windowRect.left + newClientRect.right + xDiff;
            newWindowRect.bottom = windowRect.top + newClientRect.bottom + yDiff;
        }
        else if ( nFlag == CDF_CENTER ) // resize with origin at center of window
        {
            newWindowRect.left = windowRect.left -
                                 ( newClientRect.right - clientRect.right ) / 2;
            newWindowRect.top = windowRect.top -
                                ( newClientRect.bottom - clientRect.bottom ) / 2;
            newWindowRect.right = newWindowRect.left + newClientRect.right + xDiff;
            newWindowRect.bottom = newWindowRect.top + newClientRect.bottom + yDiff;
        }
        pWnd->MoveWindow( newWindowRect );
    }
    
    pWnd->SetFont( pFont );
    
    // iterate through and move all child windows and change their font.
    CWnd* pChildWnd = pWnd->GetWindow( GW_CHILD );
    
    while ( pChildWnd )
    {
        pChildWnd->SetFont( pFont );
        pChildWnd->GetWindowRect( windowRect );
        
        CString strClass;
        ::GetClassName( pChildWnd->m_hWnd, strClass.GetBufferSetLength( 32 ), 31 );
        strClass.MakeUpper();
        if ( strClass == _T( "COMBOBOX" ) )
        {
            CRect rect;
            pChildWnd->SendMessage( CB_GETDROPPEDCONTROLRECT, 0, ( LPARAM )&rect );
            windowRect.right = rect.right;
            windowRect.bottom = rect.bottom;
        }
        
        pWnd->ScreenToClient( windowRect );
        windowRect.left = windowRect.left * tmNew.tmAveCharWidth / tmOld.tmAveCharWidth;
        windowRect.right = windowRect.right * tmNew.tmAveCharWidth / tmOld.tmAveCharWidth;
        windowRect.top = windowRect.top * newHeight / oldHeight;
        windowRect.bottom = windowRect.bottom * newHeight / oldHeight;
        pChildWnd->MoveWindow( windowRect );
        
        pChildWnd = pChildWnd->GetWindow( GW_HWNDNEXT );
    }
}
/////////////////////////////////////////////////////////////////////////////
// CSettingsSheet

IMPLEMENT_DYNAMIC( CSettingsSheet, CPropertySheet )

CSettingsSheet::CSettingsSheet( UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage )
    : CPropertySheet( nIDCaption, pParentWnd, iSelectPage )
{
    memset( &m_rctPage, 0, sizeof( m_rctPage ) );
    m_pageMain.SetCaliPage( &m_pageCalibration );
    m_pageMain.SetOptionsPage( &m_pageOptions );
    m_pageMain.SetMultiLangPage( &m_pageMultiLang );
    m_pageMain.SetLCDCfigPage( &m_pageLcdConfig );
    m_pageMain.SetFlashOptPage( &m_pageFlashOptions );
    m_pageMain.SetFlashOptPageEx( &m_pageFlashOptionsEx );
    m_pageMain.SetCustomizationPage( &m_pageCustomization );
    m_pageMain.SetMiscOptionsPage( &m_pageMiscOptions );
    
    AddPage( &m_pageMain );
    AddPage( &m_pageOptions );
#if defined(_SPUPGRADE) || defined(_FACTORY)
    if ( g_theApp.m_bShowOtherPage )
    {
        AddPage( &m_pageCalibration );
    }
#else
    /* @hongliang.xin 2009-6-29 must show other page for DloaderR,
       otherwise, the Calibration Page will assert for debug */
    AddPage( &m_pageCalibration );
    AddPage( &m_pageFlashOptions );
    AddPage( &m_pageFlashOptionsEx );
#endif
    
    AddPage( &m_pageMultiLang );
    AddPage( &m_pageLcdConfig );
#if defined(_SPUPGRADE) || defined(_FACTORY)
    if ( g_theApp.m_bShowMcpTypePage )
    {
        AddPage( &m_pageMcp );
    }
#else
    AddPage( &m_pageMcp );
    AddPage( &m_pageVolFreq );
    AddPage( &m_pageUartPortSwitch );
    AddPage( &m_pageMiscOptions );
    
    m_pageMain.SetVolFreqPage( &m_pageVolFreq );
    m_pageMain.SetUartPortSwitchPage( &m_pageUartPortSwitch );
    
#endif
    
#if defined(_SPUPGRADE) || defined(_FACTORY)
    AddPage( &m_pageCustomization );
#endif
    
}

CSettingsSheet::~CSettingsSheet()
{
    /*lint -save -e1551*/
    if ( m_fntPage.m_hObject )
    { VERIFY( m_fntPage.DeleteObject() ); }
    /*lint -restore */
}


BEGIN_MESSAGE_MAP( CSettingsSheet, CPropertySheet )
    //{{AFX_MSG_MAP(CSettingsSheet)
    ON_WM_CREATE()
    //}}AFX_MSG_MAP
    ON_MESSAGE( WM_RESIZEPAGE, OnResizePage )
    ON_BN_CLICKED( ID_APPLY_NOW, OnApply )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingsSheet message handlers
void CSettingsSheet::BuildPropPageArray()
{
    CPropertySheet::BuildPropPageArray();
    
    // get first page
    CPropertyPage* pPage = GetPage( 0 );
    ASSERT( pPage );
    if ( pPage == NULL )
    {
        return;
    }
    
    // dialog template class in afxpriv.h
    CDialogTemplate dlgtemp;
    // load the dialog template
    VERIFY( dlgtemp.Load( pPage->m_psp.pszTemplate ) );
    // get the font information
    CString strFace;
    WORD    wSize;
    VERIFY( dlgtemp.GetFont( strFace, wSize ) );
    if ( m_fntPage.m_hObject )
    { VERIFY( m_fntPage.DeleteObject() ); }
    // create a font using the info from first page
    VERIFY( m_fntPage.CreatePointFont( wSize * 10, strFace ) );
}


BOOL CSettingsSheet::OnInitDialog()
{
    CPropertySheet::OnInitDialog();
    
    CWnd* pWnd = GetDlgItem( IDOK );
    if ( pWnd != NULL )
    {
        pWnd->SetWindowText( _T( "OK" ) );
    }
    
    pWnd = GetDlgItem( IDCANCEL );
    if ( pWnd != NULL )
    {
        pWnd->SetWindowText( _T( "Cancel" ) );
    }
    
    pWnd = GetDlgItem( ID_APPLY_NOW );
    if ( pWnd != NULL )
    {
#if defined(_SPUPGRADE) || defined(_FACTORY)
        pWnd->SetWindowText( _T( "Apply" ) );
#else
        pWnd->SetWindowText( _T( "Packet" ) );
        pWnd->EnableWindow( TRUE );
#endif
    }
    
    g_bInitSheet = TRUE;
    // get the font for the first active page
    CPropertyPage* pCurActPage = GetActivePage();
    ASSERT( pCurActPage );
    
    // change the font for the sheet
    ChangeDialogFont( this, &m_fntPage, CDF_CENTER );
    
    // change the font for each page
    for ( int iCntr = 0; iCntr < GetPageCount(); iCntr++ )
    {
        VERIFY( SetActivePage( iCntr ) );
        CPropertyPage* pPage = GetActivePage();
        ASSERT( pPage );
        ChangeDialogFont( pPage, &m_fntPage, CDF_CENTER );
    }
    
    VERIFY( SetActivePage( pCurActPage ) );
    
    Resize( 0, 0 );
    g_bInitSheet = FALSE;
    
    return TRUE;
}


BOOL CSettingsSheet::OnNotify( WPARAM wParam, LPARAM lParam, LRESULT* pResult )
{
    NMHDR* pnmh = ( LPNMHDR ) lParam;
    
    // the sheet resizes the page whenever it is activated so we need to size it correctly
    if ( TCN_SELCHANGE == pnmh->code ) //lint !e648
    {
        PostMessage( WM_RESIZEPAGE );
    }
    
    return CPropertySheet::OnNotify( wParam, lParam, pResult );
}

LONG CSettingsSheet::OnResizePage( UINT, LONG )
{
    // resize the page
    CPropertyPage* pPage = GetActivePage();
    if ( pPage != NULL )
    {
        pPage->MoveWindow( &m_rctPage );
    }
    
    return 0;
}

BOOL CSettingsSheet::OnCommand( WPARAM wParam, LPARAM lParam )
{
    // the sheet resizes the page whenever the Apply button is clicked so we need to size it correctly
    if ( ID_APPLY_NOW == wParam ||
         ID_WIZNEXT == wParam ||
         ID_WIZBACK == wParam )
    {
        PostMessage( WM_RESIZEPAGE );
    }
    
    return CPropertySheet::OnCommand( wParam, lParam );
}

void CSettingsSheet::Resize( int nChangeWidth, int nChangeHeight )
{
    RECT rc;
    
    // resize the sheet
    //@ Liu Kai 2004-11-19
    // Avoid flickering
    GetWindowRect( &rc );
    rc.left -= nChangeWidth / 2;
    rc.top -= nChangeHeight / 2;
    rc.right += nChangeWidth / 2;
    rc.bottom += nChangeHeight / 2;
    
    MoveWindow( &rc );
    
    // resize the CTabCtrl
    CTabCtrl* pTab = GetTabControl();
    ASSERT( pTab );
    if ( pTab != NULL )
    {
        pTab->GetWindowRect( &rc );
        ScreenToClient( &rc );
        rc.right += nChangeWidth;
        rc.bottom += nChangeHeight;
        pTab->MoveWindow( &rc );
    }
    
    // resize the page
    CPropertyPage* pPage = GetActivePage();
    ASSERT( pPage );
    if ( pPage != NULL )
    {
        // store page size in m_PageRect
        pPage->GetWindowRect( &m_rctPage );
        ScreenToClient( &m_rctPage );
        m_rctPage.right += nChangeWidth;
        m_rctPage.bottom += nChangeHeight;
        pPage->MoveWindow( &m_rctPage );
    }
    
    // move the OK, Cancel, and Apply buttons
    CWnd* pWnd = GetDlgItem( IDOK );
    if ( pWnd != NULL )
    {
        pWnd->GetWindowRect( &rc );
        rc.bottom += nChangeHeight;
        rc.top += nChangeHeight;
        ScreenToClient( &rc );
        pWnd->MoveWindow( &rc );
    }
    
    pWnd = GetDlgItem( IDCANCEL );
    if ( pWnd != NULL )
    {
        pWnd->GetWindowRect( &rc );
        rc.bottom += nChangeHeight;
        rc.top += nChangeHeight;
        ScreenToClient( &rc );
        pWnd->MoveWindow( &rc );
    }
    
    pWnd = GetDlgItem( ID_APPLY_NOW );
    if ( pWnd != NULL )
    {
        pWnd->GetWindowRect( &rc );
        rc.bottom += nChangeHeight;
        rc.top += nChangeHeight;
        ScreenToClient( &rc );
        pWnd->MoveWindow( &rc );
    }
}

void CSettingsSheet::OnApply()
{
    MSG msg;
    while ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
    {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
    }
    CWaitCursor wait;
    GetDlgItem( ID_APPLY_NOW )->SetWindowText( _T( "Packeting..." ) );
    GetDlgItem( ID_APPLY_NOW )->EnableWindow( FALSE );
    GetDlgItem( IDOK )->EnableWindow( FALSE );
    GetDlgItem( IDCANCEL )->EnableWindow( FALSE );
    m_pageMain.DoPacket();
    GetDlgItem( ID_APPLY_NOW )->EnableWindow( TRUE );
    GetDlgItem( IDOK )->EnableWindow( TRUE );
    GetDlgItem( IDCANCEL )->EnableWindow( TRUE );
    GetDlgItem( ID_APPLY_NOW )->SetWindowText( _T( "Packet" ) );
}

int CSettingsSheet::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
    if ( CPropertySheet::OnCreate( lpCreateStruct ) == -1 )
    {
        return -1;
    }
    // TODO: Add your specialized creation code here
    
    return 0;
}

BOOL CSettingsSheet::LoadSettings( )
{
    BOOL bOK = TRUE;
    bOK &= m_pageMain.LoadSettings();
    bOK &= m_pageOptions.LoadSettings();
    bOK &= m_pageCustomization.LoadSettings();
    bOK &= m_pageMiscOptions.LoadSettings();
    bOK &= m_pageMcp.LoadSettings();
    bOK &= m_pageMultiLang.LoadSettings();
    bOK &= m_pageVolFreq.LoadSettings();
    bOK &= m_pageUartPortSwitch.LoadSettings();
    bOK &= m_pageCalibration.LoadSettings();
    return bOK;
}

void CSettingsSheet::ReleaseProperty()
{
    m_pageMain.ReleaseProperty();
    m_pageCalibration.m_lstBackup.ReleaseProperty();
    m_pageCalibration.ReleaseProperty();
    m_pageMultiLang.ReleaseProperty();
    m_pageLcdConfig.ReleaseProperty();
}

BOOL CSettingsSheet::GetProperty()
{
    BOOL bOK = TRUE;
    bOK &= m_pageMain.GetProperty();
    bOK &= m_pageCalibration.m_lstBackup.GetProperty();
    bOK &= m_pageCalibration.GetProperty();
    bOK &= m_pageMultiLang.GetProperty();
    bOK &= m_pageFlashOptions.GetProperty();
    bOK &= m_pageFlashOptionsEx.GetProperty();
    bOK &= m_pageLcdConfig.GetProperty();
    bOK &= m_pageOptions.GetProperty();
    return bOK;
}

BOOL CSettingsSheet::SetProperty()
{
    CMainFrame* pMainFrame = ( CMainFrame* )AfxGetMainWnd();
    if ( !pMainFrame->m_bPacketOpen )
    {
        MessageBox( _T( "Please load packet first and then open the configuration UI." ) );
        return FALSE;
    }
    
    BOOL bOK = TRUE;
    bOK &= m_pageMain.SetProperty();
    bOK &= m_pageCalibration.m_lstBackup.SetProperty();
    bOK &= m_pageCalibration.SetProperty();
    bOK &= m_pageMultiLang.SetProperty();
    bOK &= m_pageFlashOptions.SetProperty();
    bOK &= m_pageFlashOptionsEx.SetProperty();
    bOK &= m_pageLcdConfig.SetProperty();
    bOK &= m_pageOptions.SetProperty();
    return bOK;
}