#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BarcodeDlg.h : header file
//
#include "FilterEdit.h"
#include "Resource.h"
#include "XRandom.h"
#include "BMAGlobal.h"
#include "DLFWDef.h"
// CMutiSoftSelectDlg 对话框

class CMutiSoftSelectDlg : public CDialog
{
    DECLARE_DYNAMIC( CMutiSoftSelectDlg )
    
public:
    CMutiSoftSelectDlg( CWnd* pParent = nullptr ); // 标准构造函数
    virtual ~CMutiSoftSelectDlg();
    
    BOOL GetProperty();
    BOOL SetProperty();
    
    // 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_MUTI_SOFT };
#endif
    
protected:
    virtual void DoDataExchange( CDataExchange* pDX );  // DDX/DDV 支持
    
    DECLARE_MESSAGE_MAP()
    
public:
    VEC_PROJECT_CONFIG m_arrProjectConfig;
    CComboBox m_ComboProdSelect;
    int m_nCurSel;
    
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnCbnSelchangeComboProdSelect();
};
