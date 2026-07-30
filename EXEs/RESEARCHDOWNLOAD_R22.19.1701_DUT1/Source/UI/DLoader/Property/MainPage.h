#if !defined(AFX_MAINPAGE_H__1E1CBF30_F40E_423E_913F_1D0D8057C816__INCLUDED_)
#define AFX_MAINPAGE_H__1E1CBF30_F40E_423E_913F_1D0D8057C816__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainPage.h : header file
//

#include "XListCtrl.h"
#include "CoolStatic.h"

#include "DLFWDef.h"
#include "DLFWNvDef.h"
#include "OperationHelper.h"
#include "IDevHound.h"

#include "PageCalibration.h"
#include "PageMultiLang.h"
#include "PageLcdConfig.h"
#include "PageOptions.h"
#include "PageVolFreq.h"
#include "PageUartPortSwitch.h"
#include "PageCustomization.h"
#include "PageFlashOptions.h"
#include "PageFlashOptionsEx.h"
#include "PageMiscOptions.h"

#pragma  warning(disable:4786)
#pragma  warning(push,3)
#include <vector>
#pragma  warning(pop)
using namespace std::placeholders;

// Product select section
static _TCHAR g_sz_SELECT[]              = _T( "Selection" );
static _TCHAR g_sz_SELECT_PRODUCT[]      = _T( "SelectProduct" );
static _TCHAR g_sz_NVITEM[]              = _T( "nvitem" );
/////////////////////////////////////////////////////////////////////////////
// CMainPage dialog

class CMainPage : public CPropertyPage
{
    DECLARE_DYNCREATE( CMainPage )
    
    // Construction
public:
    CMainPage();
    ~CMainPage();
    
    BOOL    LoadSettings( );
    BOOL    SaveSettings( );
    
    void    ReleaseProperty();
    BOOL    GetProperty();
    BOOL    SetProperty();
    
    BOOL    FillList( LPCTSTR lpcstrProduct );
    int     GetProductIndex( LPCTSTR lpcstrProuct );
    void    OnStorageTypeChangeFunc( int type );
    
    void    SetCaliPage( CPageCalibration* pCaliPage ) {m_pCaliPage = pCaliPage;}
    void    SetOptionsPage( CPageOptions* pOptionPage ) {m_pOptionsPage = pOptionPage; }
    void    SetMiscOptionsPage( CPageMiscOptions* pMiscOptionPage ) {m_pMiscOptionsPage = pMiscOptionPage;}
    void    SetMultiLangPage( CPageMultiLang* pMultiLangPage ) {m_pMultiLangPage = pMultiLangPage;}
    void    SetLCDCfigPage( CPageLcdConfig* pLcdCfigPage ) {m_pLCDCfigPage = pLcdCfigPage;}
    void    SetVolFreqPage( CPageVolFreq* pPage ) {m_pVolFreqPage = pPage; m_pVolFreqPage->m_cbkStorageType = std::bind( &CMainPage::OnStorageTypeChangeFunc, this, _1 );}
    void    SetUartPortSwitchPage( CPageUartPortSwitch* pPage ) {m_pUartPortSwitchPage = pPage;}
    void    SetFlashOptPage( CPageFlashOptions* pPage ) {m_pFlashOptPage = pPage;}
    void    SetFlashOptPageEx( CPageFlashOptionsEx* pPage ) { m_pFlashOptPageEx = pPage; }
    void    SetCustomizationPage( CPageCustomization* pPage ) {m_pCustomizationPage = pPage;}
    
    void    InitNandPage( PPRODUCT_INFO_T pCurProduct );
    void    InitVolFreqPage( BOOL bUpdateWnd = FALSE );
    void    InitUartPortSwitchPage( BOOL bUpdateWnd = FALSE );
    
    BOOL    LoadConfig(); // only for SPUpgrade
    BOOL    DoPacket();
    void    OnReadOnly();
    
    BOOL    GetFdl2Flag( LPCTSTR lpszFilePath, UINT nType );
    
    BOOL    IsLoadFromPac( LPCTSTR lpDLFile );
    DATA_INFO_T GetDataInfo( LPCTSTR lpDLFile );
    
    //Get Infomation from Config UI
    int     GetUDiskFiles( LPCTSTR lpszBaseUDiskFile, CStringArray& agFiles );
    int     GetMultiFiles( CStringArray& agFiles, CString& strErr );
    int     GetMultiNVFiles( CStringArray& agFiles, CString& strWarning );
    int     GetPageBlockFiles( CStringArray& agFiles );
    int     GetDspFiles( LPCTSTR lpszBaseDspFile, CStringArray& agFiles );
    BOOL    IsMapPBFileBuf();
    BOOL    CheckFileValidate( LPCTSTR lpszFile );
    BOOL    CheckFileChipPrefix( LPCTSTR lpszFile, CString& strChipPre );
    // Dialog Data
    //{{AFX_DATA(CMainPage)
    enum { IDD = IDD_PROPPAGE_MAIN }; //lint !e30
    CEdit   m_edtPrdVersion;
    CCoolStatic m_stcProdComment;
    CXListCtrl  m_lstProductInfo;
    CXListCtrl  m_lstMergeNV;
    CComboBox   m_cmbProduct;
    CComboBox   m_cmbPort;
    CComboBox   m_cmbBaudrate;
    BOOL    m_bAllFiles;
    BOOL    m_bChkWriteSN;
    BOOL    m_bChkMergeNV;
    //}}AFX_DATA
    int     m_nStorageType;
    
    // Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CMainPage)
public:
    virtual void OnOK();
    virtual BOOL OnKillActive();
    virtual BOOL OnSetActive();
    //  virtual BOOL OnApply();
protected:
    virtual void DoDataExchange( CDataExchange* pDX );  // DDX/DDV support
    //}}AFX_VIRTUAL
public:
    int             m_nBaudRate;
    int             m_nComPort;
    
    WORD            m_wLangNVItemID;
    CString         m_strPrdVersion;
    CString         m_strCurProduct;
    PRODUCT_INFO_T* m_pCurProductInfo;
    int             m_nCurProduct = 0;
    int             m_nProductCount;
    
    VEC_PRODUCT_INFO m_arrProductInfo;
    
    CStringArray m_arrProductName;
    
    //  CStringArray m_arrFileName;
    //  CStringArray m_arrAllFileName;
    CStringArray m_arrBaudRateDefault;
    CStringArray m_arrBaudRateConfig;
    
    MAP_DLFILE  m_mapDLFiles;
    MAP_DLSTATE m_mapDLState;
    MAP_DLSIZE  m_mapDLSize;
    MAP_DATAINFO m_mapDataInfo;
    
    BIN_PACKET_HEADER m_tBinPacketHeader;
    BOOL m_bOmaDM;
    BOOL m_bPreload;
    BOOL m_bKernelImg2;
    BOOL m_bRomDisk;
    BOOL m_bEnableChipDspMap;
    
private:
    CPageCalibration*    m_pCaliPage;
    CPageMultiLang*      m_pMultiLangPage;
    CPageLcdConfig*      m_pLCDCfigPage;
    CPageOptions*        m_pOptionsPage;
    CPageVolFreq*        m_pVolFreqPage;
    CPageUartPortSwitch* m_pUartPortSwitchPage;
    CPageFlashOptions*   m_pFlashOptPage;
    CPageFlashOptionsEx* m_pFlashOptPageEx;
    CPageCustomization*  m_pCustomizationPage;
    CPageMiscOptions*    m_pMiscOptionsPage;
    
    
    BOOL m_bCanSel;
    BOOL m_bCanEdit;
    
    BOOL m_bListLock;
    
    BOOL m_bShowChinese;
    
    IDevHound* m_pDevHound = NULL;
    // Implementation
protected:
    void ModifyItemGray();
    void InitBaudrate();
    void InitPort();
    void InitListCtrl();
    // Generated message map functions
    //{{AFX_MSG(CMainPage)
    virtual BOOL OnInitDialog();
    afx_msg void OnSelchangeCmbProduct();
    afx_msg void OnChkAll();
    afx_msg void OnChkMergeNV();
    afx_msg void OnDblclkLstProductInfo( NMHDR* pNMHDR, LRESULT* pResult );
    afx_msg void OnDblclkLstMergeNV( NMHDR* pNMHDR, LRESULT* pResult );
    afx_msg void OnEndlabeleditLstProductInfo( NMHDR* pNMHDR, LRESULT* pResult );
    afx_msg void OnEndlabeleditLstMergeNV( NMHDR* pNMHDR, LRESULT* pResult );
    afx_msg void OnItemchangedLstProductInfo( NMHDR* pNMHDR, LRESULT* pResult );
    afx_msg HBRUSH OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor );
    afx_msg void OnRclickLstProductInfo( NMHDR* pNMHDR, LRESULT* pResult );
    afx_msg void OnClickLstProductInfo( NMHDR* pNMHDR, LRESULT* pResult );
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
    
private:
    CString GetXmlFile();
    
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINPAGE_H__1E1CBF30_F40E_423E_913F_1D0D8057C816__INCLUDED_)