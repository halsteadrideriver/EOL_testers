#if !defined(AFX_NVBACKUPOPTLIST_H__1E401A66_75FF_404F_8D83_EEE15CA974A3__INCLUDED_)
#define AFX_NVBACKUPOPTLIST_H__1E401A66_75FF_404F_8D83_EEE15CA974A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NvBackupOptList.h : header file
//
#include "SuperGridCtrl.h"
#include "BMAGlobal.h"
#include "DLFWDef.h"
/////////////////////////////////////////////////////////////////////////////
// CNvBackupOptList window


class CNvBackupOptList : public CSuperGridCtrl
{
    // Construction
public:
    CNvBackupOptList();
    void Init();
    CString GetTmpNotChkItemName();
    
    BOOL SetProperty();
    BOOL GetProperty();
    void ReleaseProperty();
    // Attributes
public:
    PNV_BACKUP_ITEM_T   m_pNvBkpItmArray;
    int                 m_nNvBkpItmCount;
    
    MAP_NV_BAK          m_mapNvBkpItm;
    CString             m_strCurID;
    BOOL                m_bBackNV;
    
    // Operations
public:
    void AddNvBackupItemInfo( PNV_BACKUP_ITEM_T pNvBkpItmInfo );
    void Update();
    BOOL SaveSettings();
    void FillList( LPCTSTR lpNVID = NULL, BOOL bChangeItem = FALSE );
    
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CNvBackupOptList)
    //}}AFX_VIRTUAL
    int GetIcon( const CTreeItem* pItem );
    COLORREF GetCellRGB( void );
    CImageList* CreateDragImageEx( int nItem );
    void OnControlLButtonDown( UINT nFlags, CPoint point, LVHITTESTINFO& ht );
    BOOL OnItemExpanding( CTreeItem* pItem, int iItem );
    BOOL OnItemExpanded( CTreeItem* pItem, int iItem );
    BOOL OnCollapsing( CTreeItem* pItem );
    BOOL OnItemCollapsed( CTreeItem* pItem );
    BOOL OnItemLButtonDown( LVHITTESTINFO& ht );
    BOOL OnVkReturn( void );
    BOOL OnDeleteItem( CTreeItem* pItem, int nIndex );
    // Implementation
public:
    virtual ~CNvBackupOptList();
    
    // Generated message map functions
protected:
    //{{AFX_MSG(CNvBackupOptList)
    afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
    //}}AFX_MSG
    afx_msg void OnItemCheck( NMHDR* pNMHDR, LRESULT* pResult );
    DECLARE_MESSAGE_MAP()
    
private:
    CImageList* m_pImageList;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NVBACKUPOPTLIST_H__1E401A66_75FF_404F_8D83_EEE15CA974A3__INCLUDED_)
