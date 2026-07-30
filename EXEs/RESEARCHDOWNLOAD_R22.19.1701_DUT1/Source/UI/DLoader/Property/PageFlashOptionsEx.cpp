// PageFlashOptionsEx.cpp : implementation file
//

#include "stdafx.h"
#include "dloader.h"
#include "PageFlashOptionsEx.h"
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
extern HWND g_hHwndMain;
#endif

static _TCHAR g_sz_SEC_READPARTITIONS[]   = _T( "ReadPartitions" );
static _TCHAR g_sz_SEC_READFLASH[]        = _T( "ReadFlash" );
static _TCHAR g_sz_KEY_FLASH[]            = _T( "flash" );


#define READ_FLASH  0
#define READ_PARTITIONS  1

#define MAX_BUFF_SIZE         (1024*4)
/////////////////////////////////////////////////////////////////////////////
// CPageFlashOptionsEx property page

IMPLEMENT_DYNCREATE( CPageFlashOptionsEx, CPropertyPage )

CPageFlashOptionsEx::CPageFlashOptionsEx() : CPropertyPage( CPageFlashOptionsEx::IDD )
    , m_bSelAllReadFlashItems( FALSE )
    , m_bSelAllReadPartitionsItems( FALSE )
{
    //{{AFX_DATA_INIT(CPageFlashOptionsEx)
    m_bActiveReadFlash = FALSE;
    m_bActiveReadPartitions = FALSE;
    //}}AFX_DATA_INIT
    m_agReadFlash.RemoveAll();
    m_agReadPartitions.RemoveAll();
}

CPageFlashOptionsEx::~CPageFlashOptionsEx()
{
    /*lint -save -e1551 */
    Clear();
    m_agReadFlash.RemoveAll();
    m_agReadPartitions.RemoveAll();
    /*lint -restore */
}

void CPageFlashOptionsEx::DoDataExchange( CDataExchange* pDX )
{
    CPropertyPage::DoDataExchange( pDX );
    //{{AFX_DATA_MAP(CPageFlashOptionsEx)
    DDX_Control( pDX, IDC_FOD_LST_READ_FLASH_ONLY, m_lstReadFlash );  //readflash list
    DDX_Check( pDX, IDC_CHK_ACTIVE_READ_FLASH_ONLY, m_bActiveReadFlash );   //active read flash
    DDX_Check( pDX, IDC_CHECK_READFLASH_ALL_ONLY, m_bSelAllReadFlashItems );  //read flash all
    DDX_Control( pDX, IDC_FOD_LST_READ_PARTITIONS_ONLY, m_lstReadPartitions );   //read partitions
    DDX_Check( pDX, IDC_CHK_ACTIVE_READ_PARTITIONS_ONLY, m_bActiveReadPartitions );   //active read partition
    DDX_Check( pDX, IDC_CHECK_READPARTITIONS_ALL_ONLY, m_bSelAllReadPartitionsItems );   //read partitions all
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP( CPageFlashOptionsEx, CPropertyPage )
    //{{AFX_MSG_MAP(CPageFlashOptionsEx)
    ON_NOTIFY( NM_CLICK, IDC_FOD_LST_READ_FLASH_ONLY, OnClickLstReadFlash )
    ON_NOTIFY( NM_CLICK, IDC_FOD_LST_READ_PARTITIONS_ONLY, OnClickLstReadPartitions )
    ON_NOTIFY( LVN_ENDLABELEDIT, IDC_FOD_LST_READ_FLASH_ONLY, OnEndlabeleditLstReadFlash )
    ON_NOTIFY( LVN_ENDLABELEDIT, IDC_FOD_LST_READ_PARTITIONS_ONLY, OnEndlabeleditLstReadPartitions )
    ON_BN_CLICKED( IDC_FOD_BTN_DEL_ONLY, OnBtnFlashClear )
    ON_BN_CLICKED( IDC_FOD_BTN_DEL_PARTITIONS_ONLY, OnBtnPartitionsClear )
    ON_BN_CLICKED( IDC_CHK_ACTIVE_READ_FLASH_ONLY, OnChkActiveReadFlash )
    ON_BN_CLICKED( IDC_CHK_ACTIVE_READ_PARTITIONS_ONLY, OnChkActiveReadPartitions )
    ON_BN_CLICKED( IDC_CHECK_READFLASH_ALL_ONLY, &CPageFlashOptionsEx::OnSelAllReadFlash )
    ON_BN_CLICKED( IDC_CHECK_READPARTITIONS_ALL_ONLY, &CPageFlashOptionsEx::OnSelAllReadPartitions )
    ON_NOTIFY( LVN_ITEMCHANGED, IDC_FOD_LST_READ_FLASH_ONLY, &CPageFlashOptionsEx::OnReadFlashItemchanged )
    ON_NOTIFY( LVN_ITEMCHANGED, IDC_FOD_LST_READ_PARTITIONS_ONLY, &CPageFlashOptionsEx::OnReadPartitionsItemchanged )
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageFlashOptionsEx message handlers

BOOL CPageFlashOptionsEx::OnInitDialog()
{
    CPropertyPage::OnInitDialog();
    
    // TODO: Add extra initialization here
    InitList( m_lstReadFlash, READ_FLASH );
    InitList( m_lstReadPartitions, READ_PARTITIONS );
    
    LoadSettings();
    
    FillList( m_lstReadFlash, m_agReadFlash, READ_FLASH );
    FillList( m_lstReadPartitions, m_agReadPartitions, READ_PARTITIONS );
    
    UpdateData( FALSE );
    OnChkActiveReadFlash();
    OnChkActiveReadPartitions();
    
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageFlashOptionsEx::ClickList( NMHDR* pNMHDR, CListCtrl& ctrList, int nList )
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
             ( nList == READ_PARTITIONS && !ctrList.GetItemText( nCount - 1, 1 ).IsEmpty() &&
               !ctrList.GetItemText( nCount - 1, 2 ).IsEmpty() &&
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
    
    if ( nSubItem == 0 )
    {
        return;
    }
    
    
    pCtrl = new CEdtBtnCtrl();
    
    BOOL bEnablBtn = FALSE;
    BOOL bOpenFile = FALSE;
    
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

void CPageFlashOptionsEx::OnClickLstReadFlash( NMHDR* pNMHDR, LRESULT* pResult )
{
    ClickList( pNMHDR, m_lstReadFlash, 0 );
    
    *pResult = 0;
}

void CPageFlashOptionsEx::OnClickLstReadPartitions( NMHDR* pNMHDR, LRESULT* pResult )
{
    ClickList( pNMHDR, m_lstReadPartitions, 1 );
    
    *pResult = 0;
}

void CPageFlashOptionsEx::OnEndlabeleditLstReadFlash( NMHDR* pNMHDR, LRESULT* pResult )
{
    LV_DISPINFO* pDispInfo = ( LV_DISPINFO* )pNMHDR;
    // TODO: Add your control notification handler code here
    m_lstReadFlash.SetItemText( pDispInfo->item.iItem, pDispInfo->item.iSubItem, pDispInfo->item.pszText );
    *pResult = 0;
}

void CPageFlashOptionsEx::OnEndlabeleditLstReadPartitions( NMHDR* pNMHDR, LRESULT* pResult )
{
    LV_DISPINFO* pDispInfo = ( LV_DISPINFO* )pNMHDR;
    // TODO: Add your control notification handler code here
    m_lstReadPartitions.SetItemText( pDispInfo->item.iItem, pDispInfo->item.iSubItem, pDispInfo->item.pszText );
    *pResult = 0;
}

void CPageFlashOptionsEx::OnBtnFlashClear()
{
    // TODO: Add your control notification handler code here
    m_lstReadFlash.DeleteAllItems();
    m_bSelAllReadFlashItems = FALSE;
    UpdateData( FALSE );
}

void CPageFlashOptionsEx::OnBtnPartitionsClear()
{
    // TODO: Add your control notification handler code here
    m_lstReadPartitions.DeleteAllItems();
    m_bSelAllReadPartitionsItems = FALSE;
    UpdateData( FALSE );
}

BOOL CPageFlashOptionsEx::OnKillActive()
{
    // TODO: Add your specialized code here and/or call the base class
    this->SetFocus();
    if ( !g_bInitSheet )
    {
        if ( m_bActiveReadFlash )
        {
            int nCount = m_lstReadFlash.GetItemCount();
            for ( int i = 0; i < nCount; i++ )
            {
                if ( m_lstReadFlash.GetCheck( i ) )
                {
                    CString strBase = m_lstReadFlash.GetItemText( i, 1 );
                    CString strSize = m_lstReadFlash.GetItemText( i, 2 );
                    CString strFile = m_lstReadFlash.GetItemText( i, 3 );
                    if ( strBase.IsEmpty() || strFile.IsEmpty() || strSize.IsEmpty() )
                    {
                        m_lstReadFlash.SetFocus();
                        m_lstReadFlash.SetItemState( i, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED );
                        AfxMessageBox( IDS_ERR_ITEM_EMPTY );
                        return FALSE;
                    }
                }
            }
            if ( nCount > 0 && m_lstReadFlash.GetItemText( nCount - 1, 1 ).IsEmpty() &&
                 m_lstReadFlash.GetItemText( nCount - 1, 2 ).IsEmpty() &&
                 m_lstReadFlash.GetItemText( nCount - 1, 3 ).IsEmpty() )
            {
                m_lstReadFlash.DeleteItem( nCount - 1 );
            }
        }
        if ( m_bActiveReadPartitions )
        {
            int nCount = m_lstReadPartitions.GetItemCount();
            for ( int i = 0; i < nCount; i++ )
            {
                if ( m_lstReadPartitions.GetCheck( i ) )
                {
                    CString strBase = m_lstReadPartitions.GetItemText( i, 1 );
                    CString strSize = m_lstReadPartitions.GetItemText( i, 2 );
                    CString strFile = m_lstReadPartitions.GetItemText( i, 3 );
                    if ( strBase.IsEmpty() || strFile.IsEmpty() || strSize.IsEmpty() )
                    {
                        m_lstReadPartitions.SetFocus();
                        m_lstReadPartitions.SetItemState( i, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED );
                        AfxMessageBox( IDS_ERR_ITEM_EMPTY );
                        return FALSE;
                    }
                }
            }
            if ( nCount > 0 && m_lstReadPartitions.GetItemText( nCount - 1, 1 ).IsEmpty() &&
                 m_lstReadPartitions.GetItemText( nCount - 1, 2 ).IsEmpty() &&
                 m_lstReadPartitions.GetItemText( nCount - 1, 3 ).IsEmpty() )
            {
                m_lstReadPartitions.DeleteItem( nCount - 1 );
            }
        }
    }
    
    return CPropertyPage::OnKillActive();
}

void CPageFlashOptionsEx::OnOK()
{
    // TODO: Add your specialized code here and/or call the base class
    UpdateData();
    Clear();
    
    CFileFind finder;
    int nCount = m_lstReadFlash.GetItemCount();
    for ( int i = 0; i < nCount && m_bActiveReadFlash; i++ )
    {
        if ( m_lstReadFlash.GetCheck( i ) )
        {
            CString strBase = m_lstReadFlash.GetItemText( i, 1 );
            CString strSize = m_lstReadFlash.GetItemText( i, 2 );
            CString strFile = m_lstReadFlash.GetItemText( i, 3 );
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
            
            m_agFlashOprEx.Add( ( DWORD )pFileInfo );
        }
    }
    
    nCount = m_lstReadPartitions.GetItemCount();
    for ( int i = 0; i < nCount && m_bActiveReadPartitions; i++ )
    {
        if ( m_lstReadPartitions.GetCheck( i ) )
        {
            CString strBase = m_lstReadPartitions.GetItemText( i, 1 );
            CString strSize = m_lstReadPartitions.GetItemText( i, 2 );
            CString strFile = m_lstReadPartitions.GetItemText( i, 3 );
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
            
            m_agFlashOprEx.Add( ( DWORD )pFileInfo );
        }
    }
    
    //保存设置
    SaveSettings();
    
    CPropertyPage::OnOK();
    
}

void CPageFlashOptionsEx::Clear()
{
    for ( int i = 0; i < m_agFlashOprEx.GetSize(); i++ )
    {
        delete []( PFILE_INFO_T )m_agFlashOprEx[i];
    }
    m_agFlashOprEx.RemoveAll();
}

BOOL CPageFlashOptionsEx::OnChkActiveRead()
{
    // TODO: Add your control notification handler code here
    if ( 1 == m_bActiveReadFlash && 1 == m_bActiveReadPartitions )
    {
        MessageBox( _T( "ActiveReadFlash and ActiveReadPartitions cannot both be set.\n" ) );
        return FALSE;
    }
    return TRUE;
}

void CPageFlashOptionsEx::OnChkActiveReadFlash( )
{
    // TODO: Add your control notification handler code here
    UpdateData( TRUE );
    if ( !OnChkActiveRead() )
    {
        m_bActiveReadFlash = FALSE;
    }
    if ( m_bActiveReadFlash )
    {
        m_lstReadFlash.EnableWindow( TRUE );
        GetDlgItem( IDC_FOD_BTN_DEL_ONLY )->EnableWindow( TRUE );
        GetDlgItem( IDC_CHECK_READFLASH_ALL_ONLY )->EnableWindow( TRUE );
    }
    else
    {
        m_lstReadFlash.EnableWindow( FALSE );
        GetDlgItem( IDC_FOD_BTN_DEL_ONLY )->EnableWindow( FALSE );
        GetDlgItem( IDC_CHECK_READFLASH_ALL_ONLY )->EnableWindow( FALSE );
        
    }
    
    CSettingsSheet* pParentWnd = NULL;
    pParentWnd = ( CSettingsSheet* )GetParent();
    pParentWnd->m_pageMain.OnReadOnly();
    
    UpdateData( FALSE );
}

void CPageFlashOptionsEx::OnChkActiveReadPartitions( )
{
    UpdateData( TRUE );
    if ( !OnChkActiveRead() )
    {
        m_bActiveReadPartitions = FALSE;
    }
    if ( m_bActiveReadPartitions )
    {
        m_lstReadPartitions.EnableWindow( TRUE );
        GetDlgItem( IDC_FOD_BTN_DEL_PARTITIONS_ONLY )->EnableWindow( TRUE );
        GetDlgItem( IDC_CHECK_READPARTITIONS_ALL_ONLY )->EnableWindow( TRUE );
    }
    else
    {
        m_lstReadPartitions.EnableWindow( FALSE );
        GetDlgItem( IDC_FOD_BTN_DEL_PARTITIONS_ONLY )->EnableWindow( FALSE );
        GetDlgItem( IDC_CHECK_READPARTITIONS_ALL_ONLY )->EnableWindow( FALSE );
    }
    
    CSettingsSheet* pParentWnd = NULL;
    pParentWnd = ( CSettingsSheet* )GetParent();
    pParentWnd->m_pageMain.OnReadOnly();
    
    UpdateData( FALSE );
}

BOOL CPageFlashOptionsEx::LoadSettings( )
{
    //  UNUSED_ALWAYS(strErrorMsg);
    if ( NULL == g_theApp.m_pCfgObject )
    {
        ASSERT( 0 );
        AfxMessageBox( _T( "[DL2082][F]Configure file is empty!" ) );
        return FALSE;
    }
    
    LoadFlashOpr( g_sz_SEC_READFLASH, m_agReadFlash );
    LoadFlashOpr( g_sz_SEC_READPARTITIONS, m_agReadPartitions );
    
    return TRUE;
}

BOOL CPageFlashOptionsEx::SaveSettings( )
{
    if ( NULL == g_theApp.m_pCfgObject )
    {
        ASSERT( 0 );
        return FALSE;
    }
    
    SaveFlashOpr( m_lstReadFlash, g_sz_SEC_READFLASH );
    SaveFlashOpr( m_lstReadPartitions, g_sz_SEC_READPARTITIONS );
    
    return TRUE;
}

void  CPageFlashOptionsEx::SaveFlashOpr( CListCtrl& ctrList, LPCTSTR lpszSec )
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

void CPageFlashOptionsEx::FillList( CListCtrl& ctrList, CStringArray& agList, int nList )
{
    int nFlashCount = agList.GetSize();
    for ( int i = 0; i < nFlashCount; i++ )
    {
        CString strText = agList[i];
        if ( strText.IsEmpty() )
        { continue; }
        
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
            if ( nList == READ_FLASH || nList == READ_PARTITIONS )
            {
                strText.ReleaseBuffer();
                continue;
            }
            
        }
        strFile = pBuf;
        
        int nCount = ctrList.GetItemCount();
        if ( /*!bCheck || */
            ( nList == READ_FLASH && !strBase.IsEmpty() && !strSize.IsEmpty() && !strFile.IsEmpty() ) ||
            ( nList == READ_PARTITIONS && !strBase.IsEmpty() && !strSize.IsEmpty() && !strFile.IsEmpty() ) )
        {
            ctrList.InsertItem( nCount, _T( "" ) );
            ctrList.SetItemText( nCount, 1, strBase );
            if ( nList == READ_FLASH )
            {
                ctrList.SetItemText( nCount, 2, strSize );
                ctrList.SetItemText( nCount, 3, strFile );
            }
            else if ( nList == READ_PARTITIONS )
            {
                ctrList.SetItemText( nCount, 2, strSize );
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

void CPageFlashOptionsEx::InitList( CListCtrl& ctrList, int nList )
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
    
}

BOOL CPageFlashOptionsEx::LoadFlashOpr( LPCTSTR lpszSec, CStringArray& agList )
{
    return g_theApp.m_fnOperationHelper.GetSection( g_theApp.m_pCfgObject, CFGTYPE_DL, lpszSec, agList );
}

void CPageFlashOptionsEx::OnSelAllReadFlash()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateData();
    BOOL bCheck = m_bSelAllReadFlashItems;
    int nCount = m_lstReadFlash.GetItemCount();
    for ( int i = 0; i < nCount && m_bActiveReadFlash; ++i )
    {
        m_lstReadFlash.SetCheck( i, bCheck );
    }
    
}

void CPageFlashOptionsEx::OnSelAllReadPartitions()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateData();
    BOOL bCheck = m_bSelAllReadPartitionsItems;
    int nCount = m_lstReadPartitions.GetItemCount();
    for ( int i = 0; i < nCount && m_bActiveReadPartitions; ++i )
    {
        m_lstReadPartitions.SetCheck( i, bCheck );
    }
}

void CPageFlashOptionsEx::OnReadFlashItemchanged( NMHDR* pNMHDR, LRESULT* pResult )
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>( pNMHDR );
    // TODO: 在此添加控件通知处理程序代码
    if ( pNMLV->iItem < 0 || pNMLV->iSubItem != 0 )
    { return; }
    
    if ( pNMLV->uNewState == 0x00001000 ) // uncheck
    {
        m_bSelAllReadFlashItems = FALSE;
    }
    else if ( pNMLV->uNewState == 0x00002000 ) // check
    {
        m_bSelAllReadFlashItems = TRUE;
        int nItem = m_lstReadFlash.GetItemCount();
        
        for ( int i = 0; i < nItem; ++i )
        {
            if ( !m_lstReadFlash.GetCheck( i ) )
            {
                m_bSelAllReadFlashItems = FALSE;
                break;
            }
        }
    }
    
    UpdateData( FALSE );
    *pResult = 0;
}

void CPageFlashOptionsEx::OnReadPartitionsItemchanged( NMHDR* pNMHDR, LRESULT* pResult )
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>( pNMHDR );
    // TODO: 在此添加控件通知处理程序代码
    if ( pNMLV->iItem < 0 || pNMLV->iSubItem != 0 )
    {
        return;
    }
    
    if ( pNMLV->uNewState == 0x00001000 ) // uncheck
    {
        m_bSelAllReadPartitionsItems = FALSE;
    }
    else if ( pNMLV->uNewState == 0x00002000 ) // check
    {
        m_bSelAllReadPartitionsItems = TRUE;
        int nItem = m_lstReadPartitions.GetItemCount();
        
        for ( int i = 0; i < nItem; ++i )
        {
            if ( !m_lstReadPartitions.GetCheck( i ) )
            {
                m_bSelAllReadPartitionsItems = FALSE;
                break;
            }
        }
    }
    
    UpdateData( FALSE );
    *pResult = 0;
}

__int64 CPageFlashOptionsEx::GetDigitEx( LPCTSTR lpszText )
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

BOOL CPageFlashOptionsEx::SetProperty()
{
    CMainFrame* pMainFrame = ( CMainFrame* )AfxGetMainWnd();
    SPRESULT spRlt = SP_OK;
    
    DLFW_ATTRDATA_COMMON data;
    g_theApp.m_fnOperationHelper.CreatCommonData( m_agFlashOprEx.GetCount(), &data );
    spRlt &= g_theApp.m_fnOperationHelper.ConfigAttrDataByCUIntArray( m_agFlashOprEx, &data );
    spRlt &= pMainFrame->m_pFrameHelper->SetProperty( DLFW_ATTR_FLASH_OPERATION_EX, 0, &data );
    g_theApp.m_fnOperationHelper.ReleaseCommonData( &data );
    
    if ( SP_OK != spRlt )
    {
        MessageBox( _T( "Set Parameters To DLFramework Fail." ) );
    }
    
    return SP_OK == spRlt ? TRUE : FALSE;
}

BOOL CPageFlashOptionsEx::GetProperty()
{
    //Flash通过UI层配置
    return TRUE;
}