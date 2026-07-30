#if !defined(AFX_SETTINGSSHEET_H__3B16967D_5736_432E_A398_2FE0117D093C__INCLUDED_)
#define AFX_SETTINGSSHEET_H__3B16967D_5736_432E_A398_2FE0117D093C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingsSheet.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CSettingsSheet
#include "DLFWDef.h"
#include "MainPage.h"
#include "PageOptions.h"
#include "PageCustomization.h"
#include "PageMiscOptions.h"
#include "PageMcp.h"
#include "PageMultiLang.h"
#include "PageVolFreq.h"
#include "PageUartPortSwitch.h"
#include "PageCalibration.h"
#include "PageFlashOptions.h"
#include "PageFlashOptionsEx.h"
#include "PageLcdConfig.h"


class CSettingsSheet : public CPropertySheet
{
    DECLARE_DYNAMIC( CSettingsSheet )
    
    // Construction
public:
    CSettingsSheet( UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0 );
public:

    BOOL LoadSettings();
    
    BOOL GetProperty();
    BOOL SetProperty();
    void ReleaseProperty();

    CFont                   m_fntPage;
    CMainPage               m_pageMain;
    CPageOptions            m_pageOptions;
    CPageCustomization      m_pageCustomization;
    CPageMiscOptions        m_pageMiscOptions;
    CPageMcp                m_pageMcp;
    CPageMultiLang          m_pageMultiLang;
    CPageVolFreq            m_pageVolFreq;
    CPageUartPortSwitch     m_pageUartPortSwitch;
    CPageCalibration        m_pageCalibration;
    CPageFlashOptions       m_pageFlashOptions;
    CPageFlashOptionsEx     m_pageFlashOptionsEx;
    CPageLcdConfig          m_pageLcdConfig;
    // Operations
public:
    RECT m_rctPage;
    
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSettingsSheet)
public:
    virtual BOOL OnInitDialog();
    
protected:
    void    Resize( int nChangeWidth, int nChangeHeight );
    virtual BOOL OnNotify( WPARAM wParam, LPARAM lParam, LRESULT* pResult );
    virtual BOOL OnCommand( WPARAM wParam, LPARAM lParam );
    //}}AFX_VIRTUAL
    
    // Implementation
public:
    virtual ~CSettingsSheet();
    
    // Generated message map functions
protected:
    virtual void BuildPropPageArray();
    //{{AFX_MSG(CSettingsSheet)
    afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
    //}}AFX_MSG
    afx_msg LONG OnResizePage( UINT, LONG );
    //  afx_msg void OnPressButton(int nButtion);
    afx_msg void OnApply();
    DECLARE_MESSAGE_MAP()
    
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTINGSSHEET_H__3B16967D_5736_432E_A398_2FE0117D093C__INCLUDED_)
