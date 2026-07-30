#pragma once
#include <vector>
#include "afxwin.h"


/////////////////////////////////////////////////////////////////////////////
// CPageUartPortSwitch dialog

class CPageCustomization : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageCustomization)

// Construction
public:
	CPageCustomization();
	~CPageCustomization();

    BOOL	LoadSettings();
    BOOL	SaveSettings();

// Dialog Data
	//{{AFX_DATA(CPageCustomization)
	enum { IDD = IDD_PROPPAGE_CUST };
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageCustomization)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageCustomization)
    afx_msg void OnBnClickedCheckCu();
    afx_msg void OnBnClickedCheckBadFlashRate();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()   

    virtual void OnOK();
	virtual BOOL OnKillActive();

    CButton m_btnCheckCU;
    CButton m_btnCheckRoot;
    CButton m_btnBadFlashRate;

    CEdit m_editCU;  
    CEdit m_editBadFlashRate;

    CStatic m_GroupCu;
    CStatic m_GroupRoot;

    CString m_strCU;
    CString m_strEditCU;

    BOOL m_bChangedCU;
    BOOL m_bCheckRoot;
    BOOL m_bCheckBadFlashRate;
    BOOL m_bCheckInproduction;
    BOOL m_bSupportCU;

    int m_nBadFlashRate;
    int m_nEditBadFlashRate;
};

