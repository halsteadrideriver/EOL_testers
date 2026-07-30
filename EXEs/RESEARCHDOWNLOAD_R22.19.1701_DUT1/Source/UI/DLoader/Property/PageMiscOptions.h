#pragma once
#include "afxwin.h"


// CPageMiscOptions dialog

class CPageMiscOptions : public CPropertyPage
{
    DECLARE_DYNAMIC( CPageMiscOptions )
    
public:
    CPageMiscOptions();
    virtual ~CPageMiscOptions();
    
    // Dialog Data
    enum { IDD = IDD_PROPPAGE_MISC_OPTIONS };
    
    BOOL LoadSettings( );
    BOOL SaveSettings( );
    
protected:
    virtual void DoDataExchange( CDataExchange* pDX );  // DDX/DDV support
    
    
public:
    CComboBox m_comboSelinux;
    CString m_strTmpSelinux;
    CString m_strSelinux;

    afx_msg void OnCbnSelchangeComboSelinux();
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    
    DECLARE_MESSAGE_MAP()
};
