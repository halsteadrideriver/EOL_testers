#if !defined(AFX_PAGELCDCONFIG_H__ED0FAFA3_A254_4925_AE20_D9B44A92500B__INCLUDED_)
#define AFX_PAGELCDCONFIG_H__ED0FAFA3_A254_4925_AE20_D9B44A92500B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageLcdConfig.h : header file
//
#include "LcdHelper.h"
/////////////////////////////////////////////////////////////////////////////
// CPageLcdConfig dialog

class CPageLcdConfig : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageLcdConfig)

// Construction
public:
	CPageLcdConfig();
	~CPageLcdConfig();

	BOOL LoadSettings();
	BOOL SaveSettings();
	BOOL SetProperty();
	BOOL GetProperty();
	void ReleaseProperty();
// Dialog Data
	//{{AFX_DATA(CPageLcdConfig)
	enum { IDD = IDD_PROPPAGE_LCD };  //lint !e30
	CListCtrl	m_lstLCD;
	CString	    m_strTips;
	//}}AFX_DATA
	VEC_LCD_CFIG m_arrLcdCfig;
	BOOL		 m_bRightPS; 
	CString      m_strPSFile;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageLcdConfig)
	public:
	virtual void OnOK();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageLcdConfig)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedLst(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL m_bListInit;
private:
	BOOL FillList();
	void ShowLCDCfig(BOOL bShow);
	BOOL SavePSFile();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGELCDCONFIG_H__ED0FAFA3_A254_4925_AE20_D9B44A92500B__INCLUDED_)
