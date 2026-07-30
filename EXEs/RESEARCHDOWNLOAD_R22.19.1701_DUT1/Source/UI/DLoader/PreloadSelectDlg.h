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
// CPreloadSelectDlg 对话框

class CPreloadSelectDlg : public CDialog
{
    DECLARE_DYNAMIC( CPreloadSelectDlg )

public:
    CPreloadSelectDlg( CWnd* pParent = nullptr ); // 标准构造函数
    virtual ~CPreloadSelectDlg();

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
    VEC_PRELOAD_CONFIG m_arrPreloadConfig;
    CComboBox m_ComboProdSelect;
    int m_nCurSel;

public:
    virtual BOOL OnInitDialog();
    afx_msg void OnCbnSelchangeComboProdSelect();
};
