
// MFCInvokeDemoDlg.h: 头文件
//

#pragma once
#include<map>
#include<vector>
#include "TaskHelper.h"
#include "FrameHelper.h"
#include "DLFWDriver.h"

using namespace std;
typedef map< uint16, uint16> CMapPort;

typedef enum DL_LIST_ENUM : uint16
{
    E_LIST_NUM           = 0,
    E_LIST_PORT          = 1,
    E_LIST_STEP          = 2,
    E_LIST_PROGRESS      = 3,                // Progress
    E_LIST_TIME          = 4,                // Time
    E_LIST_SIZE          = 5,                // Size
    E_LIST_RESULT        = 6,
    MAX_DL_LIST
} DL_LIST_ENUM;

typedef struct
{
    LPCTSTR      name;
    uint16       width;
    DL_LIST_ENUM type;
} DL_LIST;

static DL_LIST M_DL_LIST[] =
{
    {L"No.",       40,    E_LIST_NUM},
    {L"Port",      100,   E_LIST_PORT},
    {L"Step",      200,   E_LIST_STEP},
    {L"Progress",  100,   E_LIST_PROGRESS},
    {L"Time",      150,   E_LIST_TIME},
    {L"Size",      150,   E_LIST_SIZE},
    {L"Result",    500,   E_LIST_RESULT}
};

// CMFCInvokeDemoDlg 对话框
class CMFCInvokeDemoDlg : public CDialogEx
{
    // 构造
public:
    CMFCInvokeDemoDlg( CWnd* pParent = nullptr ); // 标准构造函数
    
    // 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_MFCINVOKEDEMO_DIALOG };
#endif
    
protected:
    virtual void DoDataExchange( CDataExchange* pDX );  // DDX/DDV 支持
    
    void InitListCtrl();
    void InsertItemList( int nItem );
    void InsertSubItemList( int nItem, int nSubItem, LPCTSTR LpszText );
    
    
public:

    CTaskHelper* FindTaskHelperHanle( int nPort );
    wstring GetAppPath( void );
    
public:
    CDLFWDriver m_fnDLFWDriver;
    CFrameHelper m_fnFrameHelper;
    CMapPort m_mPort;
    std::map<uint16, CTaskHelper*> m_mapTaskHelper;
    CRITICAL_SECTION  m_csLock;
    // 实现
protected:
    HICON m_hIcon;
    
    // 生成的消息映射函数
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand( UINT nID, LPARAM lParam );
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()
    
public:
    CListCtrl m_CtrlListReport;
    CProgressCtrl m_CtrlProgressLoadPac;
    afx_msg void OnBnClickedButtonLoadPacket();
    
    
protected:
    afx_msg LRESULT OnDlTitle( WPARAM wParam, LPARAM lParam );
    afx_msg LRESULT OnDlPort( WPARAM wParam, LPARAM lParam );
    afx_msg LRESULT OnLoadPacket( WPARAM wParam, LPARAM lParam );
    afx_msg LRESULT OnDlTime( WPARAM wParam, LPARAM lParam );
    afx_msg LRESULT OnDlSize( WPARAM wParam, LPARAM lParam );
    afx_msg LRESULT OnDlResult( WPARAM wParam, LPARAM lParam );
    afx_msg LRESULT OnDlStep( WPARAM wParam, LPARAM lParam );
    afx_msg LRESULT OnDlProgress( WPARAM wParam, LPARAM lParam );
    afx_msg LRESULT OnStartDownload( WPARAM wParam, LPARAM lParam );
    afx_msg LRESULT OnStopDownload( WPARAM wParam, LPARAM lParam );
public:
    afx_msg void OnBnClickedButtonStart();
    afx_msg void OnBnClickedButtonStop();
    
protected:
    virtual void PostNcDestroy();
    
    
public:
    afx_msg void OnSize( UINT nType, int cx, int cy );
};
