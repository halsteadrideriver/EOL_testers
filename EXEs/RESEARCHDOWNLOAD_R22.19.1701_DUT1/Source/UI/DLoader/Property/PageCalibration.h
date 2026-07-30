#if !defined(AFX_CALIBRATIONGPAGE_H__5EBF3FE0_ED59_4346_8F73_C3E8B91AE400__INCLUDED_)
#define AFX_CALIBRATIONGPAGE_H__5EBF3FE0_ED59_4346_8F73_C3E8B91AE400__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalibrationgPage.h : header file
//
#include "NvBackupOptList.h"

extern BOOL g_bInitSheet;
/////////////////////////////////////////////////////////////////////////////
// CPageCalibration dialog

class CPageCalibration : public CPropertyPage
{
    DECLARE_DYNCREATE(CPageCalibration)
        
// Construction
public:
	CPageCalibration();   // standard constructor
    ~CPageCalibration(); 

// Dialog Data
	//{{AFX_DATA(CPageCalibration)
	enum { IDD = IDD_DIALOG_CALIBRATION_SETTINGS };
	CListCtrl	m_lstFileBackup;
	CStatic	    m_stcFrame;	
	CString	    m_strPath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageCalibration)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual void OnOK();
	virtual BOOL OnKillActive();
	//}}AFX_VIRTUAL

// Implementation
public:
    BOOL SaveSettings();
    BOOL LoadSettings();

	BOOL GetProperty();
	BOOL SetProperty();
	void ReleaseProperty();

	void InitFileBackupList();

public:
	CNvBackupOptList	m_lstBackup;
	CString             m_strCurProduct;
	CString				m_strLocalPath;
	BOOL                m_bSaveToLocal;
	VEC_FILE_INFO       m_arrBackFiles;
	int                 m_nBackupProdnvMiscdata;
    // 0 - no backup prodnv & miscdata; 1 - backup prodnv & miscdata; 2 - only backup miscdata
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageCalibration)
	virtual BOOL OnInitDialog();
	afx_msg void OnChkSaveToLocal();
	afx_msg void OnDcBtnBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnNMClickLstFileBackup(NMHDR *pNMHDR, LRESULT *pResult);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALIBRATIONGPAGE_H__5EBF3FE0_ED59_4346_8F73_C3E8B91AE400__INCLUDED_)
