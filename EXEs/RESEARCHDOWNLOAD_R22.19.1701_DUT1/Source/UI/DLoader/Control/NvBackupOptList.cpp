// NvBackupOptList.cpp : implementation file
//

#include "stdafx.h"
#include "dloader.h"
#include "NvBackupOptList.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNvBackupOptList

CNvBackupOptList::CNvBackupOptList()
{
    m_pImageList = NULL;
    m_pNvBkpItmArray = NULL;
    m_nNvBkpItmCount = 0;
    m_strCurID.Empty();
}


CNvBackupOptList::~CNvBackupOptList()
{
    /*lint -save -e1551 */
    if ( NULL != m_pImageList )
    {
        m_pImageList->DeleteImageList();
        delete m_pImageList;
        m_pImageList = NULL;
    }
    g_theApp.m_fnOperationHelper.ClearMapNvBakItems( m_mapNvBkpItm );
    /*lint -restore */
}

BEGIN_MESSAGE_MAP( CNvBackupOptList, CSuperGridCtrl )
    //{{AFX_MSG_MAP(CNvBackupOptList)
    ON_WM_CREATE()
    //}}AFX_MSG_MAP
    ON_NOTIFY_REFLECT( NM_CLICK, OnItemCheck )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNvBackupOptList message handlers

int CNvBackupOptList::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
    if ( CSuperGridCtrl::OnCreate( lpCreateStruct ) == -1 )
    {
        return -1;
    }
    
    // TODO: Add your specialized creation code here
    m_pImageList = new CImageList;
    
    if ( !m_pImageList->Create( IDB_BMP_FOLDER, 16, 1, RGB( 0, 255, 255 ) ) )
    {
        return -1;
    }
    
    SetImageList( m_pImageList, LVSIL_SMALL );
    CImageList* pImageList = GetImageList( LVSIL_SMALL );
    if ( pImageList )
    {
        ImageList_GetIconSize( pImageList->m_hImageList, &m_cxImage, &m_cyImage );
    }
    else
    {
        return -1;
    }
    
    Init();
    
    return 0;
}
void CNvBackupOptList::Init()
{
    SetExtendedStyle( LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES );
    CString strClmn;
    VERIFY( strClmn.LoadString( IDS_NV_BKP_OPTION_COLUMN ) );
    
    int     nLen = strClmn.GetLength();
    LPTSTR  lpBuf = strClmn.GetBuffer( nLen );
    LPTSTR  lpFind = _tcschr( lpBuf, _T( ',' ) );
    int     nIndex = 0;
    while ( lpFind != NULL )
    {
        *lpFind = _T( '\0' );
        InsertColumn( nIndex++, lpBuf, LVCFMT_LEFT, 308 );
        lpBuf = lpFind + 1;
        lpFind = _tcschr( lpBuf, _T( ',' ) );
    }
    InsertColumn( nIndex, lpBuf, LVCFMT_LEFT, 80 );
    
    
    
    
}

//override called when OnLButtondown
void CNvBackupOptList::OnControlLButtonDown( UINT nFlags, CPoint point, LVHITTESTINFO& ht )
{
    UNUSED_ALWAYS( nFlags );
    UNUSED_ALWAYS( point );
    UNUSED_ALWAYS( ht );
    return;
}

BOOL CNvBackupOptList::OnItemExpanding( CTreeItem* pItem, int iItem )
{
    UNUSED_ALWAYS( iItem );
    CItemInfo* lp = GetData( pItem );
    if ( lp != NULL && lp->GetCheck() )
    {
        return TRUE;
    }
    return FALSE;
}


BOOL CNvBackupOptList::OnItemExpanded( CTreeItem* pItem, int iItem )
{
    UNUSED_ALWAYS( pItem );
    UNUSED_ALWAYS( iItem );
    return TRUE;
}


BOOL CNvBackupOptList::OnCollapsing( CTreeItem* pItem )
{
    UNUSED_ALWAYS( pItem );
    return TRUE;
}

BOOL CNvBackupOptList::OnItemCollapsed( CTreeItem* pItem )
{
    UNUSED_ALWAYS( pItem );
    return TRUE;
}


BOOL CNvBackupOptList::OnItemLButtonDown( LVHITTESTINFO& ht )
{
    UNUSED_ALWAYS( ht );
    return TRUE;
}

BOOL CNvBackupOptList::OnVkReturn()
{
    return TRUE;
}

BOOL CNvBackupOptList::OnDeleteItem( CTreeItem* pItem, int nIndex )
{
    UNUSED_ALWAYS( pItem );
    UNUSED_ALWAYS( nIndex );
    return 0;
}

COLORREF CNvBackupOptList::GetCellRGB()
{
    return RGB( 192, 0, 0 );
}

CImageList* CNvBackupOptList::CreateDragImageEx( int nItem )
{
    UNUSED_ALWAYS( nItem );
    return NULL;
}

int CNvBackupOptList::GetIcon( const CTreeItem* pItem )
{
    if ( pItem != NULL )
    {
        int n = GetData( pItem )->GetImage();
        if ( n != -1 )
        {
            return n;
        }
        
        int iImage = 0;
        if ( ItemHasChildren( pItem ) )
        {
            IsCollapsed( pItem ) ? iImage = 1/*close icon*/ : iImage = 0; /*open icon*/
        }
        else
        {
            iImage = 2;
        }//doc icon
        return iImage;
    }
    return 0;
}
void CNvBackupOptList::AddNvBackupItemInfo( PNV_BACKUP_ITEM_T pNvBkpItmInfo )
{
    ASSERT( pNvBkpItmInfo != NULL );
    if ( pNvBkpItmInfo == NULL )
    {
        return;
    }
    
    CItemInfo* lp = new CItemInfo();
    lp->SetCheck( pNvBkpItmInfo->wIsBackup );
    lp->SetItemText( pNvBkpItmInfo->szItemName );
    //add subitem text
    CString strValue;
    if ( pNvBkpItmInfo->dwID != 0xFFFFFFFF )
    {
        strValue.Format( _T( "0x%X" ), pNvBkpItmInfo->dwID );
    }
    else
    {
        strValue = _T( "" );
    }
    lp->AddSubItemText( strValue );  // 0 zero based subitems...
    //Create root item
    CTreeItem* pRoot = InsertRootItem( lp ); //previous on N.Y.P.D we call it CreateTreeCtrl(lp)
    if ( pRoot == NULL )
    {
        /*lint -save -e429*/
        return;
        /*lint -restore*/
    }
    //insert items
    int nSubCount = pNvBkpItmInfo->dwFlagCount;
    if ( pNvBkpItmInfo->wIsUseFlag )
    {
        CString strText;
        for ( int i = 0; i < nSubCount; i++ )
        {
            CItemInfo* lpItemInfo = new CItemInfo();
            //add items text
            //lpItemInfo->SetItemText(pNvBkpItmInfo->nbftArray[i].szFlagName);
            if ( _tcscmp( pNvBkpItmInfo->nbftArray[i].szFlagName, _T( "Replace" ) ) == 0 )
            {
                //strText.LoadString(IDS_NV_BKP_FLAG_REPLACE);
                strText.Format( IDS_NV_BKP_FLAG_REPLACE, pNvBkpItmInfo->szItemName );
            }
            else
            {
                //strText.LoadString(IDS_NV_BKP_FLAG_CONTINUE);
                strText.Format( IDS_NV_BKP_FLAG_CONTINUE, pNvBkpItmInfo->szItemName );
            }
            
            
            lpItemInfo->SetItemText( strText );
            lpItemInfo->SetCheck( pNvBkpItmInfo->nbftArray[i].dwCheck );
            //lpItemInfo->SetCheck(1);
            //insert the iteminfo with ParentPtr
            InsertItem( pRoot, lpItemInfo );
            /*lint -save -e429*/
        }
        /*lint -restore*/
    }
    //  Expand(pRoot, 0 /*listview index 0*/);
    //  UINT uflag = LVIS_SELECTED | LVIS_FOCUSED;
    //  SetItemState(0, uflag, uflag);
    
    /*lint -save -e429*/
}/*lint -restore*/

BOOL CNvBackupOptList::SaveSettings()
{
    return TRUE;
}

CString CNvBackupOptList::GetTmpNotChkItemName()
{
    CString strTmpNotChkItemName;
    CString  strID;
    PNV_BACKUP_INFO_T  pStruct = NULL;
    POSITION pos = m_mapNvBkpItm.GetStartPosition();
    while ( pos )
    {
        m_mapNvBkpItm.GetNextAssoc( pos, strID, pStruct );
        if ( pStruct )
        {
            if ( _tcslen( pStruct->szNotChkItemName ) )
            {
                strTmpNotChkItemName = pStruct->szNotChkItemName;
                break;
            }
            
        }
    }
    return strTmpNotChkItemName;
}

void CNvBackupOptList::Update()
{
    CString  strID;
    PNV_BACKUP_INFO_T  pStruct = NULL;
    m_bBackNV = FALSE;
    POSITION pos = m_mapNvBkpItm.GetStartPosition();
    while ( pos )
    {
        m_mapNvBkpItm.GetNextAssoc( pos, strID, pStruct );
        if ( pStruct )
        {
            int nNvBkpItmChkCount = 0;
            CString strNotChkItemName;
            for ( int i = 0; i < pStruct->nNvBkpItmCount; ++i )
            {
                PNV_BACKUP_ITEM_T   pNvBkpItm = pStruct->pNvBkpItmArray + i;
                if ( pNvBkpItm->wIsBackup )
                {
                    ++nNvBkpItmChkCount;
                    m_bBackNV = TRUE;
                }
                else
                {
                    strNotChkItemName += pNvBkpItm->szItemName;
                    strNotChkItemName += _T( "," );
                }
                
            }
            if ( !strNotChkItemName.IsEmpty() )
            {
                strNotChkItemName.TrimRight( _T( ',' ) );
            }
            pStruct->bBackNV = nNvBkpItmChkCount ? TRUE : FALSE;
            pStruct->nNvBkpItmChkCount = nNvBkpItmChkCount;
            //_tcsncpy(pStruct->szNotChkItemName,strNotChkItemName.operator LPCTSTR(),sizeof(pStruct->szNotChkItemName)-sizeof(TCHAR));
            _tcscpy_s( pStruct->szNotChkItemName, ARRAY_SIZE( pStruct->szNotChkItemName ), strNotChkItemName.operator LPCTSTR() );
            //set m_bTempBackNV
            if ( pStruct->bBackNV && !m_bBackNV )
            {
                m_bBackNV = TRUE;
            }
        }
    }
}
void CNvBackupOptList::OnItemCheck( NMHDR* pNMHDR, LRESULT* pResult )
{

    LPNMITEMACTIVATE   lpnmitem = ( LPNMITEMACTIVATE )pNMHDR;
    CTreeItem* pTreeItem = GetTreeItem( lpnmitem->iItem );
    ASSERT( pTreeItem != NULL );
    CItemInfo* lp = GetData( pTreeItem );
    if ( lp == NULL )
    {
        return;
    }
    
    if ( !lp->GetCheck() && ItemHasChildren( pTreeItem ) )
    {
        Collapse( pTreeItem );
    }
    
    BOOL bParentItem = TRUE;
    CTreeItem* pParent = pTreeItem;
    int nIndex = GetRootIndex( pTreeItem );
    if ( -1 == nIndex )
    {
        bParentItem = FALSE;
        pParent = GetParentItem( pTreeItem );
        nIndex = GetRootIndex( pParent );
    }
    
    if (
        -1 == nIndex ||
        NULL == m_pNvBkpItmArray ||
        nIndex >= m_nNvBkpItmCount
    )
    {
        return;
    }
    
    if ( bParentItem || ItemHasChildren( pTreeItem ) )
    {
        if ( lp->GetCheck() )
        {
            m_pNvBkpItmArray[nIndex].wIsBackup = TRUE;
        }
        else
        {
            m_pNvBkpItmArray[nIndex].wIsBackup = FALSE;
        }
    }
    else
    {
        int nParentIndex = GetCurIndex( pParent );
        int nChildIndex = GetCurIndex( pTreeItem );
        int nChild = nChildIndex - nParentIndex - 1;
        lp = GetData( pTreeItem );
        if ( lp->GetCheck() )
        {
            m_pNvBkpItmArray[nIndex].nbftArray[nChild].dwCheck = TRUE;
        }
        else
        {
            m_pNvBkpItmArray[nIndex].nbftArray[nChild].dwCheck = FALSE;
        }
    }
    *pResult = 0;
    
}

void CNvBackupOptList::FillList( LPCTSTR lpNVID /*= NULL*/, BOOL bChangeItem /*= FALSE*/ )
{
    UNUSED_ALWAYS( bChangeItem );
    
    this->DeleteAll();
    CString strNVID( lpNVID );
    if ( !strNVID.IsEmpty() )
    {
        m_strCurID = lpNVID;
    }
    
    int i = 0;
    
    NV_BACKUP_INFO_T* pNvBkpInfo = NULL;
    if ( m_mapNvBkpItm.Lookup( m_strCurID, pNvBkpInfo ) && pNvBkpInfo )
    {
        PNV_BACKUP_ITEM_T pNvBkpItmArray = pNvBkpInfo->pNvBkpItmArray;
        if ( pNvBkpItmArray != NULL )
        {
            for ( i = 0; i < pNvBkpInfo->nNvBkpItmCount; ++i )
            {
                AddNvBackupItemInfo( pNvBkpItmArray + i );
            }
        }
        
        //Updated mapTempNvBkp
        //UpdatedNvBkpMap( m_mapNvBkpItm, m_strCurID.operator LPCTSTR(), pNvBkpInfo );
    }
    
    if ( m_mapNvBkpItm.Lookup( m_strCurID, pNvBkpInfo ) && pNvBkpInfo )
    {
        m_pNvBkpItmArray = pNvBkpInfo->pNvBkpItmArray;
        m_nNvBkpItmCount = pNvBkpInfo->nNvBkpItmCount;
    }
    
}

void CNvBackupOptList::ReleaseProperty()
{
    m_mapNvBkpItm.RemoveAll();
}

BOOL CNvBackupOptList::GetProperty()
{
    CMainFrame* pMainFrame = ( CMainFrame* )AfxGetMainWnd();
    
    SPRESULT spRlt = SP_OK;
    
    DLFW_ATTRDATA_COMMON data;
    g_theApp.m_fnOperationHelper.CreatCommonData( g_theApp.m_attrStackSize.u32BackupNvItems, &data );
    spRlt &= pMainFrame->m_pFrameHelper->GetProperty( DLFW_ATTR_BACKUP_NV_ITEMS, 0, &data );
    spRlt &= g_theApp.m_fnOperationHelper.ConfigBackupNvItemsByAttrData( m_mapNvBkpItm, &data );
    g_theApp.m_fnOperationHelper.ReleaseCommonData( &data );
    
    spRlt &= pMainFrame->m_pFrameHelper->GetProperty( DLFW_ATTR_BACKUP_NV_ENABLE, 0, &m_bBackNV );
    
    if ( SP_OK != spRlt )
    {
        MessageBox( _T( "Get Parameters From DLFramework Fail." ) );
    }
    
    return SP_OK == spRlt ? TRUE : FALSE;
    
}

BOOL CNvBackupOptList::SetProperty()
{
    CMainFrame* pMainFrame = ( CMainFrame* )AfxGetMainWnd();
    
    SPRESULT spRlt = SP_OK;
    
    DLFW_ATTRDATA_COMMON data;
    g_theApp.m_fnOperationHelper.CreatCommonData( m_mapNvBkpItm.GetCount(), &data );
    spRlt &= g_theApp.m_fnOperationHelper.ConfigAttrDataByBackupNvItems( m_mapNvBkpItm, &data );
    spRlt &= pMainFrame->m_pFrameHelper->SetProperty( DLFW_ATTR_BACKUP_NV_ITEMS, 0, &data );
    g_theApp.m_fnOperationHelper.ReleaseCommonData( &data );
    
    spRlt &= pMainFrame->m_pFrameHelper->SetProperty( DLFW_ATTR_BACKUP_NV_ENABLE, 0, &m_bBackNV );
    
    if ( SP_OK != spRlt )
    {
        MessageBox( _T( "Set Parameters To DLFramework Fail." ) );
    }
    
    return SP_OK == spRlt ? TRUE : FALSE;
}