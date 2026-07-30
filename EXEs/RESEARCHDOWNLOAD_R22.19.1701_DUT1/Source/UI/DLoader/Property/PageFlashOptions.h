#if !defined(AFX_PageFlashOptions_H__F0AE4C5F_B91F_4AF2_A1BD_831B0C22BF22__INCLUDED_)
#define AFX_PageFlashOptions_H__F0AE4C5F_B91F_4AF2_A1BD_831B0C22BF22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageFlashOptions.h : header file
//
#include "DLFWDef.h"
/////////////////////////////////////////////////////////////////////////////
// CPageFlashOptions dialog

class CPageFlashOptions : public CPropertyPage
{
    DECLARE_DYNCREATE( CPageFlashOptions )
    
    // Construction
public:
    CPageFlashOptions();
    ~CPageFlashOptions();
    // Dialog Data
    //{{AFX_DATA(CPageFlashOptions)
    enum { IDD = IDD_PROPPAGE_FLASH };
    CListCtrl   m_lstWrite;
    CListCtrl   m_lstErase;
    CListCtrl   m_lstRead;
    BOOL    m_bTmpActiveRead;
    BOOL    m_bTmpEraseAll;
    BOOL    m_bTmpActiveErase;
    BOOL    m_bTmpActiveWrite;
    //}}AFX_DATA
    
    CUIntArray m_agFlashOpr;
    BOOL       m_bActiveRead;
    BOOL       m_bActiveErase;
    BOOL       m_bActiveWrite;
    BOOL       m_bEraseAll;
    
    BOOL    LoadSettings();
    BOOL    SaveSettings();
    BOOL    SetProperty();
    BOOL    GetProperty();

    // Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CPageFlashOptions)
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
    //{{AFX_MSG(CPageFlashOptions)
    virtual BOOL OnInitDialog();
    afx_msg void OnClickLstRead( NMHDR* pNMHDR, LRESULT* pResult );
    afx_msg void OnEndlabeleditLstRead( NMHDR* pNMHDR, LRESULT* pResult );
    afx_msg void OnBtnClear();
    afx_msg void OnChkActiveReadFlash();
    afx_msg void OnEraseClear();
    afx_msg void OnWriteClear();
    afx_msg void OnChkActiveEraseFlash();
    afx_msg void OnChkActiveWriteFlash();
    afx_msg void OnClickLstErase( NMHDR* pNMHDR, LRESULT* pResult );
    afx_msg void OnClickLstWrite( NMHDR* pNMHDR, LRESULT* pResult );
    afx_msg void OnEndlabeleditLstErase( NMHDR* pNMHDR, LRESULT* pResult );
    afx_msg void OnEndlabeleditLstWrite( NMHDR* pNMHDR, LRESULT* pResult );
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
    
protected:
    CStringArray m_agReadFlash;
    CStringArray m_agEraseFlash;
    CStringArray m_agWriteFlash;
    
public:
    afx_msg void OnSelAllReadflash();
    BOOL m_bSelAllReadItems;
    afx_msg void OnReadItemchanged( NMHDR* pNMHDR, LRESULT* pResult );
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PageFlashOptions_H__F0AE4C5F_B91F_4AF2_A1BD_831B0C22BF22__INCLUDED_)
