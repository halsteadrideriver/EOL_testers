// DLoaderView.h : interface of the CDLoaderView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DLOADERVIEW_H__B2ECF75F_2A34_458E_931E_E0E18C280A96__INCLUDED_)
#define AFX_DLOADERVIEW_H__B2ECF75F_2A34_458E_931E_E0E18C280A96__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4786)
#include <Afxtempl.h>
#include "DLoaderDoc.h"
#include "ProgressCtrlST.h"
#include "BarcodeDlg.h"
#include "PortFixedHelper.h"
#include <map>

#define STRING_READY     _T("Ready")
#define STRING_PASS      _T("Passed")
#define STRING_FAILED    _T("Failed")
#define STRING_WAIT      _T("Waiting...")
#define STRING_DOING     _T("In progress")
#define STRING_BADPORT   _T(" X ")
#define STRING_UNPLUGGED _T("Unplugged")

#define NUMBER_INDEX            0
#define PORT_INDEX              1
#define STEP_INDEX              2
#define STATUS_INDEX            3
#define PROG_INDEX              4
#define TIME_INDEX              5
#define MCPTYPE_INDEX           6
#define IMEI_INDEX              7
#ifdef _RESEARCH
#define RATE_INDEX              8
#else
#define RATE_INDEX              7
#endif

#define  DL_NO_FILE            0
#define  DL_FIRST_FILE         1

#define MAX_INFO_LEN  50

typedef  struct _STATE_INFO_T
{
    TCHAR szCurFile[MAX_INFO_LEN];
    TCHAR szUsedTime[MAX_INFO_LEN];
    
} STATE_INFO_T;

typedef struct _PORT_STATE_INFO_T
{
    int              nIndex;
    int              nPort;
    int              nCurFile;
    int              nStage;
    DWORD            nUsedTime;
    BOOL             bLastSuccess;
    int              nLastFile;
    CProgressCtrlST* pProg;
    CButton*         pButton;
    int              nEnumPortStartTime;
    __int64          llImgSize;
    __int64          llCurImgDownloadSize;
    __int64          llDownloadedSize;
    __int64          llDownloadedSizeBeforeCurImg;
    DWORD            nImgStartTime;
    float            fPeakRate;
    
    _PORT_STATE_INFO_T()
    {
        memset( this, 0, sizeof( _PORT_STATE_INFO_T ) );
        bLastSuccess = TRUE;
    }
    
    void Clear()
    {
        if ( pProg )
        {
            delete pProg;
            pProg = NULL;
        }
        if ( pButton )
        {
            delete pButton;
            pButton = NULL;
        }
    }
    
} PORT_STATE_INFO, *PORT_STATE_INFO_PTR;

class CDLoaderView : public CListView
{
protected: // create from serialization only
    CDLoaderView();
    DECLARE_DYNCREATE( CDLoaderView )
    
    // Attributes
public:
    CDLoaderDoc* GetDocument();
    
    void StartDLTimer();
    void StopDLTimer();
    
    
    // Operations
public:
    BOOL SetStatus( int nPort, DL_STAGE eStage, BOOL bNeedProg,
                    int nMin = 0, int nMax = 0 );
    BOOL GetStatus( int nPort, DL_STAGE& eStage );
    
    BOOL SetResult( int nPort, LPCBM_DL_END lpBmDlEnd );
    
    BOOL SetStep( int nPort, __int64* pllCurCodeSize );
    
    void ResetStatus( int nPort );
    
    BOOL StartProg( int nPort, int nMin = 0, int nMax = 100 );
    BOOL StepProg( int nPort, int nStep = 1 );
    BOOL EndProg( int nPort );
    BOOL SetProgStep( int nPort, int nStep );
    BOOL SetProgPos( int nPort, int nPos );
    
    BOOL AddProg( int nPort, BOOL bOpen );
    BOOL RemoveAll();
    void ClearAll();
    void ClearPortInfo( int nPort );
    void SetMcpType( int nPort, LPCTSTR lpszMcpType );
    void SetIMEI( int nPort, LPCTSTR lpszIMEI );
    void SetRate( int nPort, LPCTSTR lpszRate );
    void StopOnePortTimer( int nItem );
    BOOL GetInfo( int nPort, STATE_INFO_T& info );
    std::map<int, double> mMapDownloadSizes;
    std::map<int, float> mMapAvgSpeeds;
    
    void InitFixedProg();
    
protected:
    virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );
    void ResizeProg();
    int  GetPortIndex( int nPort );
    inline int  IsValidPort( int nPort );
    
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDLoaderView)
public:
    virtual void OnDraw( CDC* pDC ); // overridden to draw this view
    virtual BOOL PreCreateWindow( CREATESTRUCT& cs );
protected:
    virtual void OnInitialUpdate(); // called first time after construct
    //}}AFX_VIRTUAL
    
    // Implementation
public:
    virtual ~CDLoaderView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump( CDumpContext& dc ) const;
#endif
    
protected:
    void ResizeButton();
    afx_msg void OnBtnStart( UINT nID );
    int   m_nHeaderHeight;
    CFont m_Font;
    
    CMap<int, int, PORT_STATE_INFO_PTR, PORT_STATE_INFO_PTR> m_PortMap;
    
    std::vector<string_t> m_StepDescription;
    
    CImageList m_imgList;
    CArray<DWORD, DWORD> m_arrReadyPorts;
    BOOL m_bSNDlgShow;
    CBarcodeDlg m_dlgCurBarcode;
    HBITMAP m_hBtnBitmap;
    CPortFixedHelper m_fnPortFixed;
    // Generated message map functions
protected:
    //{{AFX_MSG(CDLoaderView)
    afx_msg void OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar );
    afx_msg void OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar );
    afx_msg void OnSize( UINT nType, int cx, int cy );
    afx_msg void OnTimer( UINT nIDEvent );
    //}}AFX_MSG
    afx_msg void OnEndTrack( NMHDR* pNMHDR, LRESULT* pResult );
    afx_msg void MeasureItem( LPMEASUREITEMSTRUCT pMeasureItem );
    afx_msg LRESULT OnBMCheckBaud( WPARAM wParam = 0, LPARAM lParam = 0 );
    afx_msg LRESULT OnBMConnect( WPARAM wParam = 0, LPARAM lParam = 0 );
    afx_msg LRESULT OnBMEraseFlash( WPARAM wParam = 0, LPARAM lParam = 0 );
    afx_msg LRESULT OnBMDownLoad( WPARAM wParam = 0, LPARAM lParam = 0 );
    afx_msg LRESULT OnBMDLoadProcess( WPARAM wParam = 0, LPARAM lParam = 0 );
    afx_msg LRESULT OnBMReadFlash( WPARAM wParam = 0, LPARAM lParam = 0 );
    afx_msg LRESULT OnBMReadFlashProcess( WPARAM wParam = 0, LPARAM lParam = 0 );
    afx_msg LRESULT OnBMReset( WPARAM wParam = 0, LPARAM lParam = 0 );
    afx_msg LRESULT OnBMSetFirstMode( WPARAM wParam = 0, LPARAM lParam = 0 );
    afx_msg LRESULT OnBMReadChipType( WPARAM wParam = 0, LPARAM lParam = 0 );
    afx_msg LRESULT OnBMReadNVItem( WPARAM wParam = 0, LPARAM lParam = 0 );
    afx_msg LRESULT OnBMChangeBuad( WPARAM wParam = 0, LPARAM lParam = 0 );
    afx_msg LRESULT OnBMBegin( WPARAM wParam = 0, LPARAM lParam = 0 );
    afx_msg LRESULT OnBMFileBegin( WPARAM wParam = 0, LPARAM lParam = 0 );
    afx_msg LRESULT OnBMEnd( WPARAM wParam = 0, LPARAM lParam = 0 );
    afx_msg LRESULT OnBMUnTransCode( WPARAM wParam = 0, LPARAM lParam = 0 );
    afx_msg LRESULT OnBmStepDescription( WPARAM wParam, LPARAM lParam );
    afx_msg LRESULT OnBmSetPort( WPARAM wParam, LPARAM lParam );
    afx_msg LRESULT OnAcquireBarcode( WPARAM wParam, LPARAM lParam );
    afx_msg void OnBeginTrack( NMHDR* pNMHDR, LRESULT* pResult );
    afx_msg void OnHdnItemchanged( NMHDR* pNMHDR, LRESULT* pResult );
    DECLARE_MESSAGE_MAP()
    
    
    afx_msg LRESULT OnBmSetImei( WPARAM wParam, LPARAM lParam );
    afx_msg LRESULT OnBmSetMcpType( WPARAM wParam, LPARAM lParam );
};

#ifndef _DEBUG  // debug version in DLoaderView.cpp
inline CDLoaderDoc* CDLoaderView::GetDocument()
{ return ( CDLoaderDoc* )m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLOADERVIEW_H__B2ECF75F_2A34_458E_931E_E0E18C280A96__INCLUDED_)
