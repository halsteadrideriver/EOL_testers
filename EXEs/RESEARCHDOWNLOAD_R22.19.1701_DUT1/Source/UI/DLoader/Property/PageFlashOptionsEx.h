#if !defined(AFX_PageFlashOptionsEx_H__F0AE4C5F_B91F_4AF2_A1BD_831B0C22BF22__INCLUDED_)
#define AFX_PageFlashOptionsEx_H__F0AE4C5F_B91F_4AF2_A1BD_831B0C22BF22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageFlashOptions.h : header file
//
#include "DLFWDef.h"
/////////////////////////////////////////////////////////////////////////////
// CPageFlashOptions dialog

class CPageFlashOptionsEx : public CPropertyPage
{
    DECLARE_DYNCREATE( CPageFlashOptionsEx )
    
    // Construction
public:
    CPageFlashOptionsEx();
    ~CPageFlashOptionsEx();
    // Dialog Data
    //{{AFX_DATA(CPageFlashOptionsEx)
    enum { IDD = IDD_PROPPAGE_READ_FLASH};
    CListCtrl   m_lstReadFlash;
    CListCtrl   m_lstReadPartitions;
    //}}AFX_DATA
    
    CUIntArray m_agFlashOprEx;
    
    BOOL       m_bActiveReadFlash;
    BOOL       m_bActiveReadPartitions;
    
    BOOL    LoadSettings();
    BOOL    SaveSettings();
    BOOL    SetProperty();
    BOOL    GetProperty();
    
    BOOL    OnChkActiveRead();
    
    // Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CPageFlashOptionsEx)
public:
    virtual BOOL OnKillActive();
    virtual void OnOK();
protected:
    virtual void DoDataExchange( CDataExchange* pDX );  // DDX/DDV support
    //}}AFX_VIRTUAL
    
    // Implementation
protected:
    void    Clear();
    void    InitList( CListCtrl& ctrList, int nList );
    void    FillList( CListCtrl& ctrList, CStringArray& agList, int nList );
    
    BOOL    LoadFlashOpr( LPCTSTR lpszSec, CStringArray& agList );
    void    SaveFlashOpr( CListCtrl& ctrList, LPCTSTR lpszSec );
    
    void    ClickList( NMHDR* pNMHDR, CListCtrl& ctrList, int nList );
    __int64 GetDigitEx( LPCTSTR lpszText );
    // Generated message map functions
    //{{AFX_MSG(CPageFlashOptionsEx)
    virtual BOOL OnInitDialog();
    afx_msg void OnClickLstReadFlash( NMHDR* pNMHDR, LRESULT* pResult );
    afx_msg void OnClickLstReadPartitions( NMHDR* pNMHDR, LRESULT* pResult );
    afx_msg void OnEndlabeleditLstReadFlash( NMHDR* pNMHDR, LRESULT* pResult );
    afx_msg void OnEndlabeleditLstReadPartitions( NMHDR* pNMHDR, LRESULT* pResult );
    afx_msg void OnBtnFlashClear( );
    afx_msg void OnBtnPartitionsClear( );
    afx_msg void OnChkActiveReadFlash( );
    afx_msg void OnChkActiveReadPartitions( );
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
    
protected:
    CStringArray m_agReadFlash;
    CStringArray m_agReadPartitions;
    
public:
    afx_msg void OnSelAllReadFlash();
    afx_msg void OnSelAllReadPartitions();
    BOOL m_bSelAllReadFlashItems;
    BOOL m_bSelAllReadPartitionsItems;
    afx_msg void OnReadFlashItemchanged( NMHDR* pNMHDR, LRESULT* pResult );
    afx_msg void OnReadPartitionsItemchanged( NMHDR* pNMHDR, LRESULT* pResult );
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PageFlashOptionsEx_H__F0AE4C5F_B91F_4AF2_A1BD_831B0C22BF22__INCLUDED_)
