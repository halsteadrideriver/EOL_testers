// PageFlashOptions.cpp : implementation file
//

#include "stdafx.h"
#include "dloader.h"
#include "PageFlashOptions.h"
#include "EdtBtnCtrl.h"
#include "BMAGlobal.h"
#include "SettingsSheet.h"
#include "Utility.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef _lint // bad pc-lint
extern BOOL g_bInitSheet;
#endif

static _TCHAR g_sz_SEC_READFLASH[]      = _T( "ReadFlash" );
static _TCHAR g_sz_KEY_FLASH[]          = _T( "flash" );


#define READ_FLASH  0
#define ERASE_FLASH 1
#define WRITE_FLASH 2

#define MAX_BUFF_SIZE         (1024*4)
/////////////////////////////////////////////////////////////////////////////
// CPageFlashOptions property page

IMPLEMENT_DYNCREATE( CPageFlashOptions, CPropertyPage )

CPageFlashOptions::CPageFlashOptions() : CPropertyPage( CPageFlashOptions::IDD )
    , m_bSelAllReadItems( FALSE )
{
    //{{AFX_DATA_INIT(CPageFlashOptions)
    m_bTmpActiveRead = FALSE;
    m_bTmpActiveErase = FALSE;
    m_bTmpActiveWrite = FALSE;
    m_bTmpEraseAll = FALSE;
    //}}AFX_DATA_INIT
    m_agReadFlash.RemoveAll();
    m_bActiveRead = m_bTmpActiveRead;
    m_bActiveErase = m_bTmpActiveErase;
    m_bActiveWrite = m_bTmpActiveWrite;
    m_bEraseAll = m_bTmpEraseAll;
}

CPageFlashOptions::~CPageFlashOptions()
{
    /*lint -save -e1551 */
    Clear();
    m_agReadFlash.RemoveAll();
    m_agEraseFlash.RemoveAll();
    m_agWriteFlash.RemoveAll();
    /*lint -restore */
}

void CPageFlashOptions::DoDataExchange( CDataExchange* pDX )
{
    CPropertyPage::DoDataExchange( pDX );
    //{{AFX_DATA_MAP(CPageFlashOptions)
    DDX_Control( pDX, IDC_FOD_LST_WRITE, m_lstWrite );
    DDX_Control( pDX, IDC_FOD_LST_ERASE, m_lstErase );
    DDX_Control( pDX, IDC_FOD_LST_READ, m_lstRead );
    DDX_Check( pDX, IDC_FOD_CHK_ACTIVE_READ_FLASH, m_bTmpActiveRead );
    DDX_Check( pDX, IDC_FOD_CHK_ACTIVE_ERASE_FLASH, m_bTmpActiveErase );
    DDX_Check( pDX, IDC_FOD_CHK_ACTIVE_WRITE_FLASH, m_bTmpActiveWrite );
    DDX_Check( pDX, IDC_FOD_CHK_ERASE_ALL, m_bTmpEraseAll );
    DDX_Check( pDX, IDC_CHECK_READFLASH_ALL, m_bSelAllReadItems );
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP( CPageFlashOptions, CPropertyPage )
    //{{AFX_MSG_MAP(CPageFlashOptions)
    ON_NOTIFY( NM_CLICK, IDC_FOD_LST_READ, OnClickLstRead )
    ON_NOTIFY( LVN_ENDLABELEDIT, IDC_FOD_LST_READ, OnEndlabeleditLstRead )
    ON_BN_CLICKED( IDC_FOD_BTN_DEL, OnBtnClear )
    ON_BN_CLICKED( IDC_FOD_CHK_ACTIVE_READ_FLASH, OnChkActiveReadFlash )
    ON_BN_CLICKED( IDC_FOD_BTN_DELE, OnEraseClear )
    ON_BN_CLICKED( IDC_FOD_BTN_DELW, OnWriteClear )
    ON_BN_CLICKED( IDC_FOD_CHK_ACTIVE_ERASE_FLASH, OnChkActiveEraseFlash )
    ON_BN_CLICKED( IDC_FOD_CHK_ACTIVE_WRITE_FLASH, OnChkActiveWriteFlash )
    ON_NOTIFY( NM_CLICK, IDC_FOD_LST_ERASE, OnClickLstErase )
    ON_NOTIFY( NM_CLICK, IDC_FOD_LST_WRITE, OnClickLstWrite )
    ON_NOTIFY( LVN_ENDLABELEDIT, IDC_FOD_LST_ERASE, OnEndlabeleditLstErase )
    ON_NOTIFY( LVN_ENDLABELEDIT, IDC_FOD_LST_WRITE, OnEndlabeleditLstWrite )
    ON_BN_CLICKED( IDC_CHECK_READFLASH_ALL, &CPageFlashOptions::OnSelAllReadflash )
    ON_NOTIFY( LVN_ITEMCHANGED, IDC_FOD_LST_READ, &CPageFlashOptions::OnReadItemchanged )
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageFlashOptions message handlers

BOOL CPageFlashOptions::OnInitDialog()
{
    CPropertyPage::OnInitDialog();
    
    // TODO: Add extra initialization here
    InitList( m_lstRead, READ_FLASH );
    InitList( m_lstErase, ERASE_FLASH );
    InitList( m_lstWrite, WRITE_FLASH );
    
    LoadSettings();
    
    FillList( m_lstRead, m_agReadFlash, READ_FLASH );
    FillList( m_lstErase, m_agEraseFlash, ERASE_FLASH );
    FillList( m_lstWrite, m_agWriteFlash, WRITE_FLASH );
    
    m_bTmpActiveRead = m_bActiveRead;
    m_bTmpActiveErase = m_bActiveErase;
    m_bTmpActiveWrite = m_bActiveWrite;
    m_bTmpEraseAll   = m_bEraseAll;
    
    UpdateData( FALSE );
    OnChkActiveReadFlash();
    OnChkActiveEraseFlash();
    OnChkActiveWriteFlash();
    
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageFlashOptions::ClickList( NMHDR* pNMHDR, CListCtrl& ctrList, int nList )
{
    LPNMITEMACTIVATE pnia = ( LPNMITEMACTIVATE )pNMHDR;
    
    CEdtBtnCtrl* pCtrl = NULL;
    int nItem = pnia->iItem;
    int nSubItem = pnia->iSubItem;
    if ( nItem == -1 )
    {
        int nCount = ctrList.GetItemCount();
        CString strText;
        if ( nCount == 0 ||
             ( nList == READ_FLASH && !ctrList.GetItemText( nCount - 1, 1 ).IsEmpty() &&
               !ctrList.GetItemText( nCount - 1, 2 ).IsEmpty() &&
               !ctrList.GetItemText( nCount - 1, 3 ).IsEmpty() ) ||
             ( nList == ERASE_FLASH && !ctrList.GetItemText( nCount - 1, 1 ).IsEmpty() &&
               !ctrList.GetItemText( nCount - 1, 2 ).IsEmpty() ) ||
             ( nList == WRITE_FLASH && !ctrList.GetItemText( nCount - 1, 1 ).IsEmpty() &&
               !ctrList.GetItemText( nCount - 1, 3 ).IsEmpty() ) )
        {
            ctrList.InsertItem( nCount, _T( "" ) );
            if ( nCount > 0 )
            {
                ctrList.SetCheck( nCount - 1 );
            }
            nItem = nCount;
        }
        else if ( ctrList.GetItemText( nCount - 1, 1 ).IsEmpty() &&
                  ctrList.GetItemText( nCount - 1, 2 ).IsEmpty() &&
                  ctrList.GetItemText( nCount - 1, 3 ).IsEmpty() )
        {
            ctrList.DeleteItem( nCount - 1 );
            return;
        }
        else
        {
            return;
        }
    }
    
    if ( nSubItem == 0 || ( nSubItem == 3 && nList == ERASE_FLASH ) || ( nSubItem == 2 && nList == WRITE_FLASH ) )
    {
        return;
    }
    
    
    pCtrl = new CEdtBtnCtrl();
    
    BOOL bEnablBtn = FALSE;
    BOOL bOpenFile = FALSE;
    if ( nSubItem == 3 )
    {
        bEnablBtn = TRUE;
        if ( nList == WRITE_FLASH )
        {
            bOpenFile = TRUE;
        }
    }
    
    
    if ( NULL != pCtrl )
    {
        pCtrl->EnableFileSelButton( bEnablBtn );
        pCtrl->SetBtnOpenFlag( bOpenFile );
        //if(nSubItem == 1 || nSubItem == 2 )
        //{
        //  pCtrl->SetEditFmtMode(EX_FMT_HEX);
        //}
        pCtrl->m_nItem = nItem;
        pCtrl->m_nSubItem = nSubItem;
        pCtrl->m_strText = ctrList.GetItemText( nItem, nSubItem );
        if ( !pCtrl->Create( IDD_EDT_BTN_DLG, FromHandle( ctrList.m_hWnd ) ) )
        {
            AfxMessageBox( _T( "Error to create EdtBtnCtrl" ) );
            return;
        }
    }
    else
    {
        return;
    }
    
    CRect rect;
    ctrList.GetSubItemRect( nItem, nSubItem, LVIR_BOUNDS, rect );
    CRect rtList;
    ctrList.GetClientRect( &rtList );
    if ( rect.right > rtList.right )
    { rect.right = rtList.right; }
    if ( rect.left < rtList.left )
    { rect.left = rtList.left; }
    if ( rect.bottom > rtList.bottom )
    { rect.bottom = rtList.bottom; }
    if ( rect.top < rtList.top )
    { rect.top = rtList.top; }
    
    pCtrl->SetWindowPos( &wndTop, rect.left, rect.top - 1, rect.right - rect.left, rect.bottom - rect.top, NULL );
    
    pCtrl->ShowWindow( SW_SHOW );
}

void CPageFlashOptions::OnClickLstRead( NMHDR* pNMHDR, LRESULT* pResult )
{
    ClickList( pNMHDR, m_lstRead, 0 );
    
    *pResult = 0;
}

void CPageFlashOptions::OnEndlabeleditLstRead( NMHDR* pNMHDR, LRESULT* pResult )
{
    LV_DISPINFO* pDispInfo = ( LV_DISPINFO* )pNMHDR;
    // TODO: Add your control notification handler code here
    m_lstRead.SetItemText( pDispInfo->item.iItem, pDispInfo->item.iSubItem, pDispInfo->item.pszText );
    *pResult = 0;
}

void CPageFlashOptions::OnEndlabeleditLstErase( NMHDR* pNMHDR, LRESULT* pResult )
{
    LV_DISPINFO* pDispInfo = ( LV_DISPINFO* )pNMHDR;
    // TODO: Add your control notification handler code here
    m_lstErase.SetItemText( pDispInfo->item.iItem, pDispInfo->item.iSubItem, pDispInfo->item.pszText );
    *pResult = 0;
}

void CPageFlashOptions::OnEndlabeleditLstWrite( NMHDR* pNMHDR, LRESULT* pResult )
{
    LV_DISPINFO* pDispInfo = ( LV_DISPINFO* )pNMHDR;
    // TODO: Add your control notification handler code here
    m_lstWrite.SetItemText( pDispInfo->item.iItem, pDispInfo->item.iSubItem, pDispInfo->item.pszText );
    *pResult = 0;
}

void CPageFlashOptions::OnBtnClear()
{
    // TODO: Add your control notification handler code here
    m_lstRead.DeleteAllItems();
    m_bSelAllReadItems = FALSE;
    UpdateData( FALSE );
}

BOOL CPageFlashOptions::OnKillActive()
{
    // TODO: Add your specialized code here and/or call the base class
    this->SetFocus();
    if ( !g_bInitSheet )
    {
        if ( m_bTmpActiveRead )
        {
            int nCount = m_lstRead.GetItemCount();
            for ( int i = 0; i < nCount; i++ )
            {
                if ( m_lstRead.GetCheck( i ) )
                {
                    CString strBase = m_lstRead.GetItemText( i, 1 );
                    CString strSize = m_lstRead.GetItemText( i, 2 );
                    CString strFile = m_lstRead.GetItemText( i, 3 );
                    if ( strBase.IsEmpty() || strFile.IsEmpty() || strSize.IsEmpty() )
                    {
                        m_lstRead.SetFocus();
                        m_lstRead.SetItemState( i, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED );
                        AfxMessageBox( IDS_ERR_ITEM_EMPTY );
                        return FALSE;
                    }
                }
            }
            if ( nCount > 0 && m_lstRead.GetItemText( nCount - 1, 1 ).IsEmpty() &&
                 m_lstRead.GetItemText( nCount - 1, 2 ).IsEmpty() &&
                 m_lstRead.GetItemText( nCount - 1, 3 ).IsEmpty() )
            {
                m_lstRead.DeleteItem( nCount - 1 );
            }
        }
        if ( m_bTmpActiveErase )
        {
            int nCount = m_lstErase.GetItemCount();
            for ( int i = 0; i < nCount; i++ )
            {
                if ( m_lstErase.GetCheck( i ) )
                {
                    CString strBase = m_lstErase.GetItemText( i, 1 );
                    CString strSize = m_lstErase.GetItemText( i, 2 );
                    if ( strBase.IsEmpty() || strSize.IsEmpty() )
                    {
                        m_lstErase.SetFocus();
                        m_lstErase.SetItemState( i, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED );
                        AfxMessageBox( IDS_ERR_ITEM_EMPTY );
                        return FALSE;
                    }
                }
            }
            if ( nCount > 0 && m_lstErase.GetItemText( nCount - 1, 1 ).IsEmpty() &&
                 m_lstErase.GetItemText( nCount - 1, 2 ).IsEmpty() )
            {
                m_lstErase.DeleteItem( nCount - 1 );
            }
        }
        if ( m_bTmpActiveWrite )
        {
            CFileFind finder;
            int nCount = m_lstWrite.GetItemCount();
            for ( int i = 0; i < nCount; i++ )
            {
                if ( m_lstWrite.GetCheck( i ) )
                {
                    CString strBase = m_lstWrite.GetItemText( i, 1 );
                    //CString strSize = m_lstRead.GetItemText(i,2);
                    CString strFile = m_lstWrite.GetItemText( i, 3 );
                    if ( strBase.IsEmpty() || strFile.IsEmpty() || ( !strFile.IsEmpty() && !finder.FindFile( strFile ) ) )
                    {
                        m_lstWrite.SetFocus();
                        m_lstWrite.SetItemState( i, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED );
                        if ( !strFile.IsEmpty() && !finder.FindFile( strFile ) )
                        {
                            CString strErr;
                            strErr.Format( _T( "[DL3012]File [%s] not exist!" ), strFile.operator LPCWSTR() );
                            AfxMessageBox( strErr );
                        }
                        else
                        {
                            AfxMessageBox( IDS_ERR_ITEM_EMPTY );
                        }
                        
                        return FALSE;
                    }
                }
            }
            if ( nCount > 0 && m_lstWrite.GetItemText( nCount - 1, 1 ).IsEmpty() &&
                 m_lstWrite.GetItemText( nCount - 1, 3 ).IsEmpty() )
            {
                m_lstWrite.DeleteItem( nCount - 1 );
            }
        }
    }
    
    return CPropertyPage::OnKillActive();
}

void CPageFlashOptions::OnOK()
{
    // TODO: Add your specialized code here and/or call the base class
    UpdateData();
    m_bActiveRead = m_bTmpActiveRead;
    m_bActiveErase = m_bTmpActiveErase;
    m_bActiveWrite = m_bTmpActiveWrite;
    m_bEraseAll = m_bTmpEraseAll;
    Clear();
    int nCount = m_lstRead.GetItemCount();
    
    CFileFind finder;
    int i = 0;
    for ( i = 0; i < nCount && m_bActiveRead; i++ )
    {
        if ( m_lstRead.GetCheck( i ) )
        {
            CString strBase = m_lstRead.GetItemText( i, 1 );
            CString strSize = m_lstRead.GetItemText( i, 2 );
            CString strFile = m_lstRead.GetItemText( i, 3 );
            CString strID;
            
            if ( finder.FindFile( strFile ) )
            {
                DWORD dwAttr = GetFileAttributes( strFile );
                if ( MAXDWORD != dwAttr )
                {
                    dwAttr &= ~FILE_ATTRIBUTE_READONLY;
                    ::SetFileAttributes( strFile, dwAttr );
                }
            }
            
            PFILE_INFO_T pFileInfo = new FILE_INFO_T;
            pFileInfo->arrBlock[0].llBase = GetDigitEx( strBase );
            pFileInfo->arrBlock[0].llSize = GetDigitEx( strSize );
            _tcscpy( pFileInfo->arrBlock[0].szRepID, strBase );
            strID.Format( _T( "R_%s" ), strBase.operator LPCWSTR() );
            strID.Replace( _T( " " ), _T( "" ) );
            _tcscpy( pFileInfo->szID, strID );
            _tcscpy( pFileInfo->szIDAlias, strID );
            _tcscpy( pFileInfo->szFilePath, strFile );
            _tcscpy( pFileInfo->szType, _T( "ReadFlashAndSave" ) );
            pFileInfo->dwCheckFlag = TRUE;
            
            m_agFlashOpr.Add( ( DWORD )pFileInfo );
        }
    }
    
    nCount = m_lstErase.GetItemCount();
    for ( i = 0; i < nCount && m_bActiveErase; i++ )
    {
        if ( m_lstErase.GetCheck( i ) )
        {
            CString strBase = m_lstErase.GetItemText( i, 1 );
            CString strSize = m_lstErase.GetItemText( i, 2 );
            CString strFile = m_lstErase.GetItemText( i, 3 );
            CString strID;
            
            PFILE_INFO_T pFileInfo = new FILE_INFO_T;
            pFileInfo->arrBlock[0].llBase = GetDigitEx( strBase );
            pFileInfo->arrBlock[0].llSize = GetDigitEx( strSize );
            _tcscpy( pFileInfo->arrBlock[0].szRepID, strBase );
            strID.Format( _T( "E_%s" ), strBase.operator LPCWSTR() );
            strID.Replace( _T( " " ), _T( "" ) );
            _tcscpy( pFileInfo->szID, strID );
            _tcscpy( pFileInfo->szIDAlias, strID );
            //_tcscpy(pFileInfo->szFilePath,strFile);
            _tcscpy( pFileInfo->szType, _T( "EraseFlash" ) );
            pFileInfo->dwCheckFlag = TRUE;
            
            m_agFlashOpr.Add( ( DWORD )pFileInfo );
        }
    }
    
    nCount = m_lstWrite.GetItemCount();
    for ( i = 0; i < nCount && m_bActiveWrite; i++ )
    {
        if ( m_lstWrite.GetCheck( i ) )
        {
            CString strBase = m_lstWrite.GetItemText( i, 1 );
            CString strSize = m_lstWrite.GetItemText( i, 2 );
            CString strFile = m_lstWrite.GetItemText( i, 3 );
            CString strID;
            
            PFILE_INFO_T pFileInfo = new FILE_INFO_T;
            pFileInfo->arrBlock[0].llBase = GetDigitEx( strBase );
            pFileInfo->arrBlock[0].llSize = GetDigitEx( strSize );
            _tcscpy_s( pFileInfo->arrBlock[0].szRepID, sizeof( pFileInfo->arrBlock[0].szRepID ) / sizeof( TCHAR ), strBase );
            strID.Format( _T( "W_%s" ), strBase.operator LPCWSTR() );
            strID.Replace( _T( " " ), _T( "" ) );
            _tcscpy_s( pFileInfo->szID, sizeof( pFileInfo->szIDAlias ) / sizeof( TCHAR ), strID );
            _tcscpy_s( pFileInfo->szIDAlias, sizeof( pFileInfo->szIDAlias ) / sizeof( TCHAR ), strID );
            _tcscpy_s( pFileInfo->szFilePath, sizeof( pFileInfo->szFilePath ) / sizeof( TCHAR ), strFile );
            _tcscpy( pFileInfo->szType, _T( "CODE" ) );
            pFileInfo->dwCheckFlag = TRUE;
            
            m_agFlashOpr.Add( ( DWORD )pFileInfo );
        }
    }
    
    //保存设置
    SaveSettings();
    
    CPropertyPage::OnOK();
    
}
void CPageFlashOptions::Clear()
{
    for ( int i = 0; i < m_agFlashOpr.GetSize(); i++ )
    {
        delete []( PFILE_INFO_T )m_agFlashOpr[i];
    }
    m_agFlashOpr.RemoveAll();
}


void CPageFlashOptions::OnChkActiveReadFlash()
{
    // TODO: Add your control notification handler code here
    UpdateData();
    if ( m_bTmpActiveRead )
    {
        m_lstRead.EnableWindow( TRUE );
        GetDlgItem( IDC_FOD_BTN_DEL )->EnableWindow( TRUE );
        GetDlgItem( IDC_CHECK_READFLASH_ALL )->EnableWindow( TRUE );
    }
    else
    {
        m_lstRead.EnableWindow( FALSE );
        GetDlgItem( IDC_FOD_BTN_DEL )->EnableWindow( FALSE );
        GetDlgItem( IDC_CHECK_READFLASH_ALL )->EnableWindow( FALSE );
    }
}

BOOL CPageFlashOptions::LoadSettings( )
{
    //  UNUSED_ALWAYS(strErrorMsg);
    if ( NULL == g_theApp.m_pCfgObject )
    {
        ASSERT( 0 );
        AfxMessageBox( _T( "[DL2082][F]Configure file is empty!" ) );
        return FALSE;
    }
    
    LoadFlashOpr( _T( "ReadFlash" ), m_agReadFlash );
    LoadFlashOpr( _T( "EraseFlash" ), m_agEraseFlash );
    LoadFlashOpr( _T( "WriteFlash" ), m_agWriteFlash );
    
#ifdef _FACTORY
    m_bEraseAll = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, _T( "Options" ), _T( "EraseAll" ), TRUE );
#else
    m_bEraseAll = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, _T( "Options" ), _T( "EraseAll" ), FALSE );
#endif
    m_bTmpEraseAll = m_bEraseAll;
    
    return TRUE;
}

BOOL CPageFlashOptions::SaveSettings( )
{
    if ( NULL == g_theApp.m_pCfgObject )
    {
        ASSERT( 0 );
        return FALSE;
    }
    
    SaveFlashOpr( m_lstRead, _T( "ReadFlash" ) );
    SaveFlashOpr( m_lstErase, _T( "EraseFlash" ) );
    SaveFlashOpr( m_lstWrite, _T( "WriteFlash" ) );
    
    return TRUE;
}

void  CPageFlashOptions::SaveFlashOpr( CListCtrl& ctrList, LPCTSTR lpszSec )
{
    g_theApp.m_pCfgObject->SetSection( CFGTYPE_DL, lpszSec, _T( "" ) );
    int nCount = ctrList.GetItemCount();
    for ( int i = 0; i < nCount; i++ )
    {
        CString strText;
        strText.Format( _T( "%d:%s:%s:%s" ),
                        ctrList.GetCheck( i ),
                        ctrList.GetItemText( i, 1 ).operator LPCTSTR(),
                        ctrList.GetItemText( i, 2 ).operator LPCTSTR(),
                        ctrList.GetItemText( i, 3 ).operator LPCTSTR() );
        CString strKey;
        strKey.Format( _T( "%s%02d" ), g_sz_KEY_FLASH, i );
        g_theApp.m_pCfgObject->SetString( CFGTYPE_DL, lpszSec, strKey, strText.operator LPCWSTR() );
    }
}

void CPageFlashOptions::FillList( CListCtrl& ctrList, CStringArray& agList, int nList )
{
    int nFlashCount = agList.GetSize();
    for ( int i = 0; i < nFlashCount; i++ )
    {
        CString strText = agList[i];
        if ( strText.IsEmpty() || strText.Find( L"_only.bin" ) >= 0 )
        {
            continue;
        }
        
        CString strBase;
        CString strSize;
        CString strFile;
        BOOL    bCheck = FALSE;
        
        LPTSTR pBuf = strText.GetBuffer( 1 );
        LPTSTR pFind = _tcschr( pBuf, ':' );
        if ( pFind != NULL )
        {
            *pFind = '\0';
            bCheck = ( BOOL )GetDigitEx( pBuf );
            pBuf = pFind + 1;
        }
        else
        {
            strText.ReleaseBuffer();
            continue;
        }
        
        pFind = _tcschr( pBuf, ':' );
        if ( pFind != NULL )
        {
            *pFind = '\0';
            strBase = pBuf;
            pBuf = pFind + 1;
        }
        else
        {
            strText.ReleaseBuffer();
            continue;
        }
        
        pFind = _tcschr( pBuf, ':' );
        if ( pFind != NULL )
        {
            *pFind = '\0';
            strSize = pBuf;
            pBuf = pFind + 1;
        }
        else
        {
            if ( nList == READ_FLASH || nList == ERASE_FLASH )
            {
                strText.ReleaseBuffer();
                continue;
            }
            
        }
        strFile = pBuf;
        
        int nCount = ctrList.GetItemCount();
        if ( /*!bCheck || */
            ( nList == READ_FLASH && !strBase.IsEmpty() && !strSize.IsEmpty() && !strFile.IsEmpty() ) ||
            ( nList == ERASE_FLASH && !strBase.IsEmpty() && !strSize.IsEmpty() ) ||
            ( nList == WRITE_FLASH && !strBase.IsEmpty() && !strFile.IsEmpty() ) )
        {
            ctrList.InsertItem( nCount, _T( "" ) );
            ctrList.SetItemText( nCount, 1, strBase );
            if ( nList == READ_FLASH )
            {
                ctrList.SetItemText( nCount, 2, strSize );
                ctrList.SetItemText( nCount, 3, strFile );
            }
            else if ( nList == ERASE_FLASH )
            {
                ctrList.SetItemText( nCount, 2, strSize );
                ctrList.SetItemText( nCount, 3, _T( "" ) );
            }
            else if ( nList == WRITE_FLASH )
            {
                ctrList.SetItemText( nCount, 2, _T( "" ) );
                ctrList.SetItemText( nCount, 3, strFile );
            }
            
            ctrList.SetCheck( nCount, bCheck );
        }
    }
    
    int nCount = ctrList.GetItemCount();
    if ( nCount > 0 && ctrList.GetItemText( nCount - 1, 1 ).IsEmpty() &&
         ctrList.GetItemText( nCount - 1, 2 ).IsEmpty() &&
         ctrList.GetItemText( nCount - 1, 3 ).IsEmpty() )
    {
        ctrList.DeleteItem( nCount - 1 );
    }
}


void CPageFlashOptions::OnEraseClear()
{
    // TODO: Add your control notification handler code here
    m_lstErase.DeleteAllItems();
}

void CPageFlashOptions::OnWriteClear()
{
    // TODO: Add your control notification handler code here
    m_lstWrite.DeleteAllItems();
}

void CPageFlashOptions::InitList( CListCtrl& ctrList, int nList )
{
    ctrList.SetBkColor( RGB( 232, 232, 232 ) );
    ctrList.ModifyStyle( 0, LVS_SHOWSELALWAYS );
    
    DWORD dwExStyle = ctrList.GetExtendedStyle();
    dwExStyle |= LVS_EX_FULLROWSELECT;
    dwExStyle |= LVS_EX_GRIDLINES;
    dwExStyle |= LVS_EX_CHECKBOXES;
    
    ctrList.SetExtendedStyle( dwExStyle );
    
    //Load Column
    CString strClmn;
    VERIFY( strClmn.LoadString( IDS_READ_FLASH_LIST_COL ) );
    
    int nLen   = strClmn.GetLength();
    LPTSTR lpBuf  = strClmn.GetBuffer( nLen );
    LPTSTR lpFind = _tcschr( lpBuf, _T( ',' ) );
    int     nIndex = 0;
    while ( lpFind != NULL )
    {
        *lpFind = _T( '\0' );
        if ( nIndex == 0 )
        {
            ctrList.InsertColumn( nIndex++, lpBuf, LVCFMT_LEFT, 30 );
        }
        else
        {
            ctrList.InsertColumn( nIndex++, lpBuf, LVCFMT_LEFT, 80 );
        }
        lpBuf = lpFind + 1;
        lpFind = _tcschr( lpBuf, _T( ',' ) );
    }
    ctrList.InsertColumn( nIndex, lpBuf, LVCFMT_LEFT, 290 );
    strClmn.ReleaseBuffer();
    
    LVCOLUMN col;
    col.mask = LVCF_TEXT;
    col.pszText = NULL;
    if ( nList == ERASE_FLASH )
    {
        ctrList.SetColumn( 3, &col );
    }
    else if ( nList == WRITE_FLASH )
    {
        ctrList.SetColumn( 2, &col );
    }
    
}

void CPageFlashOptions::OnChkActiveEraseFlash()
{
    // TODO: Add your control notification handler code here
    UpdateData();
    if ( m_bTmpActiveErase )
    {
        m_lstErase.EnableWindow( TRUE );
        GetDlgItem( IDC_FOD_BTN_DELE )->EnableWindow( TRUE );
    }
    else
    {
        m_lstErase.EnableWindow( FALSE );
        GetDlgItem( IDC_FOD_BTN_DELE )->EnableWindow( FALSE );
    }
}

void CPageFlashOptions::OnChkActiveWriteFlash()
{
    // TODO: Add your control notification handler code here
    UpdateData();
    if ( m_bTmpActiveWrite )
    {
        m_lstWrite.EnableWindow( TRUE );
        GetDlgItem( IDC_FOD_BTN_DELW )->EnableWindow( TRUE );
    }
    else
    {
        m_lstWrite.EnableWindow( FALSE );
        GetDlgItem( IDC_FOD_BTN_DELW )->EnableWindow( FALSE );
    }
}

BOOL CPageFlashOptions::LoadFlashOpr( LPCTSTR lpszSec, CStringArray& agList )
{
    return g_theApp.m_fnOperationHelper.GetSection( g_theApp.m_pCfgObject, CFGTYPE_DL, lpszSec, agList );
}

void CPageFlashOptions::OnClickLstErase( NMHDR* pNMHDR, LRESULT* pResult )
{
    // TODO: Add your control notification handler code here
    ClickList( pNMHDR, m_lstErase, 1 );
    *pResult = 0;
}

void CPageFlashOptions::OnClickLstWrite( NMHDR* pNMHDR, LRESULT* pResult )
{
    // TODO: Add your control notification handler code here
    ClickList( pNMHDR, m_lstWrite, 2 );
    *pResult = 0;
}

void CPageFlashOptions::OnSelAllReadflash()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateData();
    BOOL bCheck = m_bSelAllReadItems;
    int nCount = m_lstRead.GetItemCount();
    for ( int i = 0; i < nCount && m_bTmpActiveRead; ++i )
    {
        m_lstRead.SetCheck( i, bCheck );
    }
    
}

void CPageFlashOptions::OnReadItemchanged( NMHDR* pNMHDR, LRESULT* pResult )
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>( pNMHDR );
    // TODO: 在此添加控件通知处理程序代码
    if ( pNMLV->iItem < 0 || pNMLV->iSubItem != 0 )
    { return; }
    
    if ( pNMLV->uNewState == 0x00001000 ) // uncheck
    {
        m_bSelAllReadItems = FALSE;
    }
    else if ( pNMLV->uNewState == 0x00002000 ) // check
    {
        m_bSelAllReadItems = TRUE;
        int nItem = m_lstRead.GetItemCount();
        
        for ( int i = 0; i < nItem; ++i )
        {
            if ( !m_lstRead.GetCheck( i ) )
            {
                m_bSelAllReadItems = FALSE;
                break;
            }
        }
    }
    
    UpdateData( FALSE );
    *pResult = 0;
}

__int64 CPageFlashOptions::GetDigitEx( LPCTSTR lpszText )
{
    if ( lpszText == NULL )
    { return 0; }
    
    CString strText = lpszText;
    
    __int64 llRet = 0;
    if ( !strText.Left( 2 ).CompareNoCase( _T( "0x" ) ) )
    {
        // Hex mode
        _stscanf( strText, _T( "%llx" ), &llRet );
    }
    else
    {
        // Decimal mode
        _stscanf( strText, _T( "%lld" ), &llRet );
    }
    
    return llRet;
}

BOOL CPageFlashOptions::SetProperty()
{
    CMainFrame* pMainFrame = ( CMainFrame* )AfxGetMainWnd();
    SPRESULT spRlt = SP_OK;
    
    DLFW_ATTRDATA_COMMON data;
    g_theApp.m_fnOperationHelper.CreatCommonData( m_agFlashOpr.GetCount(), &data );
    spRlt &= g_theApp.m_fnOperationHelper.ConfigAttrDataByCUIntArray( m_agFlashOpr, &data );
    spRlt &= pMainFrame->m_pFrameHelper->SetProperty( DLFW_ATTR_FLASH_OPERATION, 0, &data );
    g_theApp.m_fnOperationHelper.ReleaseCommonData( &data );
    
    spRlt &= pMainFrame->m_pFrameHelper->SetProperty( DLFW_ATTR_FLASH_ERASE_ALL, 0, &m_bEraseAll );
    
    if ( SP_OK != spRlt )
    {
        MessageBox( _T( "Set Parameters To DLFramework Fail." ) );
    }
    
    return SP_OK == spRlt ? TRUE : FALSE;
}

BOOL CPageFlashOptions::GetProperty()
{
    //Flash通过UI层配置
    CMainFrame* pMainFrame = ( CMainFrame* )AfxGetMainWnd();
    SPRESULT spRlt = SP_OK;
    spRlt &= pMainFrame->m_pFrameHelper->GetProperty( DLFW_ATTR_FLASH_ERASE_ALL, 0, &m_bEraseAll );
    if ( SP_OK != spRlt )
    {
        MessageBox( _T( "Get Parameters From DLFramework Fail." ) );
    }

    return SP_OK == spRlt ? TRUE : FALSE;
}