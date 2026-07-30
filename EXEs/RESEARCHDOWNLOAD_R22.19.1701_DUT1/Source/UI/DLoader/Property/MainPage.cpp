// MainPage.cpp : implementation file
//

#include "stdafx.h"
#include "dloader.h"
#include "MainPage.h"
#include "SettingsSheet.h"
#include "EdtBtnCtrl.h"
#include "DlgPacketSetting.h"
#include "MainFrm.h"
#include <atlconv.h>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "Utility.h"
#include "LcdHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef _lint // bad pc-lint
extern BOOL g_bInitSheet;
#endif

#define ALL_PORT _T("All")

// Serial ports section
static _TCHAR g_sz_SP[] = _T( "Serial port" );
static _TCHAR g_sz_BR[] = _T( "Baud rate" );
static _TCHAR g_sz_PORT[] = _T( "Port" );

// GUI main page
static _TCHAR g_sz_SEC_GUI[] = _T( "GUI" );
static _TCHAR g_sz_KEY_SEL[] = _T( "CanSelect" );
static _TCHAR g_sz_KEY_EDT[] = _T( "CanEdit" );
static _TCHAR g_sz_KEY_BRS[] = _T( "BaudRates" );
static _TCHAR g_sz_BR_DEFAULT[] = _T( "57600,115200,230400,460800,921600" );


static _TCHAR g_sz_SEC_NV[] = _T( "NV" );
static _TCHAR g_sz_KEY_LNII[] = _T( "LangNVItemID" );

static _TCHAR g_szUA[] = _T( "OmadmFota" );
static _TCHAR g_szEDU[] = _T( "EraseDU" );
static _TCHAR g_szPreload[] = _T( "Preload" );
static _TCHAR g_szKernelImg2[] = _T( "KernelImg2" );
static _TCHAR g_szRomDisk[] = _T( "RomDisk" );

static _TCHAR g_sz_SEC_CHIPDSPMAP[] = _T( "ChipDspMap" );
static _TCHAR g_sz_KEY_CDME[] = _T( "enable" );


#define CMD_PAC_EXIT \
{\
    if(g_theApp.m_bCMDPackage)\
{\
    GetParent()->PostMessage(WM_COMMAND, IDCANCEL);\
    AfxGetMainWnd()->PostMessage(WM_CLOSE);\
}\
}


#define PUMP_MESSAGES()  \
    do \
{\
    MSG msg;\
    while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))\
{\
    TranslateMessage(&msg);\
    DispatchMessage(&msg);\
}\
} while(0)

#define MAX_BUFF_SIZE         (1024*4)
/////////////////////////////////////////////////////////////////////////////
// CMainPage property page

IMPLEMENT_DYNCREATE( CMainPage, CPropertyPage )

CMainPage::CMainPage() : CPropertyPage( CMainPage::IDD )
{
    //{{AFX_DATA_INIT(CMainPage)
    m_bAllFiles = FALSE;
    m_bChkWriteSN = FALSE;
    //}}AFX_DATA_INIT
    m_pCurProductInfo = NULL;
    m_nBaudRate = -1;
    m_nComPort = -1;
    m_nCurProduct = -1;
    
    m_strCurProduct = _T( "" );
    
    m_nProductCount = 0;
    
    m_pCaliPage = NULL;
    m_pOptionsPage = NULL;
    m_pMultiLangPage = NULL;
    m_pLCDCfigPage = NULL;
    m_pOptionsPage = NULL;
    m_pVolFreqPage = NULL;
    m_pUartPortSwitchPage = NULL;
    m_pFlashOptPage = NULL;
    m_pCustomizationPage = NULL;
    m_pMiscOptionsPage = NULL;
    
    m_strPrdVersion = _T( "" );
    m_wLangNVItemID = NV_MULTI_LANG_ID;
    
#if defined(_SPUPGRADE) || defined(_FACTORY)
    m_bCanEdit = FALSE;
    m_bCanSel = FALSE;
#else
    m_bCanEdit = TRUE;
    m_bCanSel = TRUE;
#endif
    
    m_bOmaDM = TRUE;
    m_bPreload = TRUE;
    m_bKernelImg2 = TRUE;
    m_bRomDisk = TRUE;
    m_bListLock = FALSE;
    m_bShowChinese = FALSE;
    m_bChkMergeNV = FALSE;
    m_bEnableChipDspMap = FALSE;
    m_nStorageType = 0;
}

CMainPage::~CMainPage()
{
    /*lint -save -e1551 */
    m_pCurProductInfo = NULL;
    
    m_arrProductName.RemoveAll();
    m_pCaliPage =  NULL;
    m_pOptionsPage = NULL;
    m_pLCDCfigPage = NULL;
    m_pMultiLangPage = NULL;
    m_pVolFreqPage = NULL;
    m_pUartPortSwitchPage = NULL;
    m_pCustomizationPage = NULL;
    
    m_mapDLFiles.RemoveAll();
    m_mapDLState.RemoveAll();
    m_mapDLSize.RemoveAll();
    
    /*lint -restore */
}

void CMainPage::DoDataExchange( CDataExchange* pDX )
{
    CPropertyPage::DoDataExchange( pDX );
    //{{AFX_DATA_MAP(CMainPage)
    DDX_Control( pDX, IDC_EDT_PRD_VERSION, m_edtPrdVersion );
    DDX_Control( pDX, IDC_STC_PROD_COMMENT, m_stcProdComment );
    DDX_Control( pDX, IDC_LST_PRODUCT_INFO, m_lstProductInfo );
    DDX_Control( pDX, IDC_LST_MERGE_NV, m_lstMergeNV );
    DDX_Control( pDX, IDC_CMB_PRODUCT, m_cmbProduct );
    DDX_Control( pDX, IDC_CMB_PORT, m_cmbPort );
    DDX_Control( pDX, IDC_CMB_BAUDRATE, m_cmbBaudrate );
    DDX_Check( pDX, IDC_CHK_MERGE_NV, m_bChkMergeNV );
    DDX_Check( pDX, IDC_CHK_ALL, m_bAllFiles );
    DDX_Check( pDX, IDC_MPD_CHK_SN, m_bChkWriteSN );
    
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP( CMainPage, CPropertyPage )
    //{{AFX_MSG_MAP(CMainPage)
    ON_CBN_SELCHANGE( IDC_CMB_PRODUCT, OnSelchangeCmbProduct )
    ON_BN_CLICKED( IDC_CHK_ALL, OnChkAll )
    ON_BN_CLICKED( IDC_CHK_MERGE_NV, OnChkMergeNV )
    ON_NOTIFY( NM_DBLCLK, IDC_LST_PRODUCT_INFO, OnDblclkLstProductInfo )
    ON_NOTIFY( NM_DBLCLK, IDC_LST_MERGE_NV, OnDblclkLstMergeNV )
    ON_NOTIFY( LVN_ENDLABELEDIT, IDC_LST_PRODUCT_INFO, OnEndlabeleditLstProductInfo )
    ON_NOTIFY( LVN_ENDLABELEDIT, IDC_LST_MERGE_NV, OnEndlabeleditLstMergeNV )
    ON_NOTIFY( LVN_ITEMCHANGED, IDC_LST_PRODUCT_INFO, OnItemchangedLstProductInfo )
    ON_WM_CTLCOLOR()
    ON_NOTIFY( NM_RCLICK, IDC_LST_PRODUCT_INFO, OnRclickLstProductInfo )
    ON_NOTIFY( NM_CLICK, IDC_LST_PRODUCT_INFO, OnClickLstProductInfo )
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainPage message handlers

BOOL CMainPage::OnInitDialog()
{
    CPropertyPage::OnInitDialog();
    
    // TODO: Add extra initialization here
    CHKBOOL( LoadConfig() );
    InitPort();
    InitBaudrate();
    InitListCtrl();
    
    int i = 0;
    m_bAllFiles = TRUE;
    
    if ( m_nProductCount > 0 )
    {
        for ( i = 0; i < m_nProductCount; i++ )
        {
            m_cmbProduct.AddString( m_arrProductName[i] );
        }
    }
    
    if ( m_strCurProduct.IsEmpty() && m_arrProductName.GetSize() > 0 )
    {
        m_nCurProduct = 0;
        m_strCurProduct = m_arrProductName.GetAt( 0 );
    }
    
    FillList( m_strCurProduct );
    
    m_cmbProduct.SetCurSel( m_nCurProduct );
#if defined(_SPUPGRADE) || defined(_FACTORY)
    CStatic* pwnd = ( CStatic* )GetDlgItem( IDC_STC_SEL_PRD );
    if ( pwnd != NULL )
    {
        pwnd->SetWindowText( _T( "          Product:" ) );
    }
    GetDlgItem( IDC_CMB_PRODUCT )->EnableWindow( FALSE );
    
#ifdef _FACTORY
    
    if ( !g_theApp.m_bNeedPhaseCheck )
    {
        GetDlgItem( IDC_MPD_CHK_SN )->ShowWindow( SW_HIDE );
    }
#else
    GetDlgItem( IDC_MPD_CHK_SN )->ShowWindow( SW_HIDE );
#endif // _FACTORY
#else
    if ( !g_theApp.m_bNeedPhaseCheck )
    {
        GetDlgItem( IDC_MPD_CHK_SN )->ShowWindow( SW_HIDE );
    }
    GetDlgItem( IDC_EDT_PRD_VERSION )->ShowWindow( SW_HIDE );
#endif // defined(_SPUPGRADE) || defined(_FACTORY)
    
    UpdateData( FALSE );
    
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CMainPage::SaveSettings()
{
    if ( NULL == g_theApp.m_pCfgObject )
    {
        ASSERT( 0 );
        AfxMessageBox( _T( "[DL2082][F]Configure file is empty!" ) );
        return FALSE;
    }
    
    
    g_theApp.m_pCfgObject->SetInt( CFGTYPE_DL, g_sz_SP, g_sz_PORT, m_nComPort );
    g_theApp.m_pCfgObject->SetInt( CFGTYPE_DL, g_sz_SP, g_sz_BR, m_nBaudRate );
    g_theApp.m_pCfgObject->SetString( CFGTYPE_DL, g_sz_SELECT, g_sz_SELECT_PRODUCT, m_strCurProduct );
    
    CMainFrame* pMF = ( CMainFrame* )AfxGetMainWnd();
    if ( m_strCurProduct.Find( _T( "PAC_" ) ) != 0 )
    {
        pMF->m_bPacketOpen = FALSE;
    }
    
    BOOL bPacketMode = FALSE;
    if ( pMF != NULL )
    {
        if ( pMF->m_bPacketOpen )
        {
            bPacketMode = TRUE;
        }
    }
    
    g_theApp.m_pCfgObject->SetInt( CFGTYPE_DL, g_sz_SEC_GUI, _T( "PacketMode" ), bPacketMode );
    
    BOOL bAutoGenerateSN = !m_bChkWriteSN;
    g_theApp.m_pCfgObject->SetInt( CFGTYPE_DL, _T( "SN" ), _T( "AutoGenerateSN" ), bAutoGenerateSN );
    
    // Save download files' names
    // Save omitted file also
    // first delete old files
    CString strFilePath;
    CString strFileID;
    int nItem = m_lstProductInfo.GetItemCount();
    int nItemMergeNV = m_lstMergeNV.GetItemCount();
    CString strCheckFile;
    if ( m_pCurProductInfo != NULL )
    {
        for ( int i = 0; i < nItem && !m_strCurProduct.IsEmpty(); i++ )
        {
            strFileID = m_lstProductInfo.GetItemText( i, 0 );;
            strFilePath = m_lstProductInfo.GetItemText( i, 1 );
            strCheckFile.Format( _T( "%d@%s" ), m_lstProductInfo.GetCheck( i ), strFilePath.operator LPCTSTR() );
            g_theApp.m_pCfgObject->SetString( CFGTYPE_DL, m_strCurProduct, strFileID, strCheckFile );
#if defined(_SPUPGRADE)
            if ( g_theApp.m_bKeepPacNVState )
            {
                if ( strFileID.Find( _T( "NV" ) ) == 0 )
                {
                    g_theApp.m_pCfgObject->SetString( CFGTYPE_DL, strFileID, strFileID, strCheckFile );
                }
            }
#endif
        }
        
        for ( int i = 0; i < nItemMergeNV && !m_strCurProduct.IsEmpty(); i++ )
        {
            strFileID = m_lstMergeNV.GetItemText( i, 0 );
            strFilePath = m_lstMergeNV.GetItemText( i, 1 );
            strCheckFile.Format( _T( "%d@%s" ), m_lstMergeNV.GetCheck( i ), strFilePath.operator LPCTSTR() );
            g_theApp.m_pCfgObject->SetString( CFGTYPE_DL, m_strCurProduct, strFileID, strCheckFile );
#if defined(_SPUPGRADE)
            if ( g_theApp.m_bKeepPacNVState )
            {
                if ( strFileID.Find( _T( "NV" ) ) == 0 )
                {
                    g_theApp.m_pCfgObject->SetString( CFGTYPE_DL, strFileID, strFileID, strCheckFile );
                }
            }
#endif
        }
    }
    
    return TRUE;
}

void CMainPage::OnOK()
{
    // TODO: Add your specialized code here and/or call the base class
    
    CString strText;
    
    m_cmbPort.GetWindowText( strText );
    if ( _tcscmp( strText, ALL_PORT ) == 0 )
    {
        m_nComPort = 0;
    }
    else
    {
        _stscanf( strText, _T( "COM%d" ), &m_nComPort );
    }
    
    m_cmbBaudrate.GetWindowText( strText );
    m_nBaudRate = _ttoi( strText );
    
    m_mapDLFiles.RemoveAll();
    m_mapDLState.RemoveAll();
    m_cmbProduct.GetWindowText( m_strCurProduct );
    m_nCurProduct = m_cmbProduct.GetCurSel();
    
    if ( m_nCurProduct != -1 )
    {
        m_pCurProductInfo = m_arrProductInfo[m_nCurProduct];
    }
    
    int nCount = m_lstProductInfo.GetItemCount();
    if ( m_pCurProductInfo != NULL )
    {
        for ( int i = 0; i < nCount; i++ )
        {
            CString strID = m_lstProductInfo.GetItemText( i, 0 );
            for ( int j = 0; j < ( int )m_pCurProductInfo->dwFileCount; j++ )
            {
                if ( m_pCurProductInfo->pFileInfoArr[j].szIDAlias == strID )
                {
                    strID = m_pCurProductInfo->pFileInfoArr[j].szID;
                    break;
                }
            }
            
            strID.MakeUpper();
            m_mapDLFiles.SetAt( strID, m_lstProductInfo.GetItemText( i, 1 ) );
            m_mapDLState.SetAt( strID, m_lstProductInfo.GetCheck( i ) );
        }
    }
    
    int nCountMergeNV = m_lstMergeNV.GetItemCount();
    if ( m_pCurProductInfo != NULL )
    {
        for ( int i = 0; i < nCountMergeNV; i++ )
        {
            CString strID = m_lstMergeNV.GetItemText( i, 0 );
            for ( int j = 0; j < ( int )m_pCurProductInfo->dwFileCount; j++ )
            {
                if ( m_pCurProductInfo->pFileInfoArr[j].szIDAlias == strID )
                {
                    strID = m_pCurProductInfo->pFileInfoArr[j].szID;
                    break;
                }
            }
            strID.MakeUpper();
            m_mapDLFiles.SetAt( strID, m_lstMergeNV.GetItemText( i, 1 ) );
            m_mapDLState.SetAt( strID, m_lstMergeNV.GetCheck( i ) );
        }
    }
    
    CMainFrame* pMF = ( CMainFrame* )AfxGetMainWnd();
    
    if ( m_strCurProduct.Find( _T( "PAC_" ) ) != 0 )
    {
        CString strTmp;
        strTmp.Format( _T( "%s" ), m_strCurProduct.operator LPCTSTR() );
        if ( pMF != NULL )
        {
            pMF->SetToolBarText( strTmp );
            pMF->m_bPacketOpen = FALSE;
        }
    }
    
    SaveSettings();
    CPropertyPage::OnOK();
}

BOOL CMainPage::OnKillActive()
{
    // TODO: Add your specialized code here and/or call the base class
    this->SetFocus();
    if ( !g_bInitSheet )
    {
        CString strErr;
        int nCount = m_lstProductInfo.GetItemCount();
        int nCheckCount = 0;
        for ( int i = 0; i < nCount; i++ )
        {
            if ( m_lstProductInfo.GetCheck( i ) )
            {
                nCheckCount++;
                if ( DI_IS_NEED_FILE( m_lstProductInfo.GetItemData( i ) ) )
                {
                    if ( m_lstProductInfo.GetItemText( i, 1 ).IsEmpty() )
                    {
                        AfxMessageBox( IDS_ERR_FILE_EMPTY );
                        return FALSE;
                    }
                }
            }
            if ( !m_lstProductInfo.GetCheck( i ) && DI_IS_KEY_ITEM( m_lstProductInfo.GetItemData( i ) ) )
            {
                strErr.Format( _T( "[DL3014]%s must be selected!" ), m_lstProductInfo.GetItemText( i, 0 ).operator LPCTSTR() );
                AfxMessageBox( strErr.operator LPCTSTR()/*IDS_ERR_FDL_NOT_CHECKED*/ );
                m_lstProductInfo.SetCheck( i );
                return FALSE;
            }
        }
        
        if ( nCheckCount == 0 )
        {
            AfxMessageBox( _T( "[DL3015]Must select at least one file!" ) );
            return FALSE;
        }
        
        CSettingsSheet* pParentWnd = NULL;
        pParentWnd = ( CSettingsSheet* )GetParent();
        CString strCurProduct;
        m_cmbProduct.GetWindowText( strCurProduct );
        
        if ( m_pCaliPage != NULL &&
             ( m_pCaliPage->m_strCurProduct.IsEmpty() ||
               m_pCaliPage->m_strCurProduct.CompareNoCase( strCurProduct ) != 0 ) )
        {
            pParentWnd->m_pageCalibration.m_lstBackup.FillList();
            pParentWnd->m_pageCalibration.m_strCurProduct = strCurProduct;
        }
        
#ifdef _RESEARCH
        InitVolFreqPage( TRUE );
        InitUartPortSwitchPage( TRUE );
#endif
        
    }
    
    return CPropertyPage::OnKillActive();
}

void CMainPage::OnSelchangeCmbProduct()
{
    CString strProduct;
    m_cmbProduct.GetWindowText( strProduct );
    GetDlgItem( IDC_CHK_ALL )->EnableWindow( TRUE );
    m_bAllFiles = TRUE;
    FillList( strProduct );
    
    UpdateData( FALSE );
}

BOOL CMainPage::OnSetActive()
{
    return CPropertyPage::OnSetActive();
}

BOOL CMainPage::FillList( LPCTSTR lpcstrProduct )
{
    ASSERT( lpcstrProduct );
    if ( lpcstrProduct == NULL )
    {
        return TRUE;
    }
    
    PPRODUCT_INFO_T pProdInfo = m_pCurProductInfo;
    
    DWORD dwCount = pProdInfo->dwFileCount;
    PFILE_INFO_T pFileInfo = pProdInfo->pFileInfoArr;
    
    
    CString strComment = pProdInfo->szComment;
    m_stcProdComment.SetText( strComment );
    
#if defined(_SPUPGRADE) || defined(_FACTORY)
    CString strVer = m_strPrdVersion;
    strVer.MakeUpper();
    m_edtPrdVersion.SetWindowText( strVer );
#endif
    
    m_lstProductInfo.DeleteAllItems();
    m_lstMergeNV.DeleteAllItems();
    
    UINT i = 0;
    UINT j = 0;
    UINT k = 0;
    UINT l = 0;
    CString str;
    
    m_bListLock = TRUE;
    for ( i = 0; i < dwCount; i++ )
    {
        if ( _tcsicmp( pFileInfo[i].szID, _T( "cust_nv" ) ) == 0 || _tcsicmp( pFileInfo[i].szID, _T( "operator_nv" ) ) == 0 )
        {
            m_lstMergeNV.InsertItem( k, pFileInfo[i].szIDAlias );
            CString strFileID = pFileInfo[i].szID;
            strFileID.MakeUpper();
            CString strFile = _T( "" );
            m_mapDLFiles.Lookup( strFileID, strFile );
            BOOL    bCheck = FALSE;
            m_mapDLState.Lookup( strFileID, bCheck );
            
            m_lstMergeNV.SetItemText( k, 1, strFile );
            m_bChkMergeNV = TRUE;
            m_lstMergeNV.SetCheck( k, bCheck );
            m_lstMergeNV.SetItemData( k, 1 );
            UpdateData( FALSE );
            k++;
        }
        else
        {
            m_lstProductInfo.InsertItem( l, pFileInfo[i].szIDAlias );
            
            CString strID = pFileInfo[i].szID;
            strID.MakeUpper();
            
            UINT nBlock = pFileInfo[i].dwBlockCount;
            if ( nBlock > 5 )
            {
                nBlock = 5;
            }
            
            for ( j = 0; j < nBlock; j++ )
            {
                str.Format( _T( "0x%X" ), pFileInfo[i].arrBlock[j].llBase );
                if ( pProdInfo->dwPartitionCount > 0 && _tcslen( pFileInfo[i].arrBlock[j].szRepID ) != 0 )
                {
                    str = pFileInfo[i].arrBlock[j].szRepID;
                }
                
                m_lstProductInfo.SetItemText( l, 2 + j * 2, str );
                
#if defined(_SPUPGRADE) || defined(_FACTORY)
                __int64 llFileSize;
                if ( j == 0 && pFileInfo[i].dwFlag != 0 && m_mapDLSize.Lookup( strID, llFileSize ) )
                {
                    str.Format( _T( "0x%I64X" ), llFileSize );
                }
                else
#endif
                {
                    if ( 0 == j && pProdInfo->dwPartitionCount > 0 && _tcslen( pFileInfo[i].arrBlock[j].szRepID ) != 0 )
                    {
                        __int64 llPartSize = g_theApp.m_fnOperationHelper.GetPartitionSize( pFileInfo[i].arrBlock[j].szRepID, pProdInfo );
                        if ( 0 != llPartSize )
                        {
                            str.Format( _T( "0x%I64X" ), llPartSize );
                        }
                        else
                        {
                            str.Format( _T( "0x%I64X" ), pFileInfo[i].arrBlock[j].llSize );
                        }
                    }
                    else
                    {
                        str.Format( _T( "0x%I64X" ), pFileInfo[i].arrBlock[j].llSize );
                    }
                    
                }
                
                if ( str.CompareNoCase( _T( "0x0" ) ) == 0 )
                {
                    str = _T( "" );
                }
                m_lstProductInfo.SetItemText( l, 3 + j * 2, str );
            }
            m_lstProductInfo.SetItemText( l, 12, pFileInfo[i].szType );
            DWORD dwItemData = pFileInfo[i].dwFlag;
            if ( pFileInfo[i].dwCheckFlag == 2 )
            {
                dwItemData = DI_SET_OMIT( dwItemData );
            }
            else if ( pFileInfo[i].dwCheckFlag == 1 )
            {
                dwItemData = DI_SET_KEY_ITEM( dwItemData );
            }
            
            m_lstProductInfo.SetItemData( l, dwItemData );
            m_lstProductInfo.SetCheck( l );
            l++;
        }
    }
    m_bListLock = FALSE;
    m_bAllFiles = TRUE;
    UpdateData( FALSE );
    
    CString strFDL2Path;
    strFDL2Path.Empty();
    
    if ( lpcstrProduct != NULL && m_strCurProduct.CompareNoCase( lpcstrProduct ) == 0 ) //lint !e774
    {
        for ( i = 0, l = 0; i < dwCount; i++ )
        {
            if ( _tcsicmp( pFileInfo[i].szID, _T( "cust_nv" ) ) == 0 || _tcsicmp( pFileInfo[i].szID, _T( "operator_nv" ) ) == 0 )
            {
                continue;
            }
            CString strFileID = pFileInfo[i].szID;
            strFileID.MakeUpper();
            CString strFile = _T( "" );
            BOOL    bCheck = FALSE;
            m_mapDLFiles.Lookup( strFileID, strFile );
            m_mapDLState.Lookup( strFileID, bCheck );
            
            m_lstProductInfo.SetItemText( l, 1, strFile );
            
            if ( m_pCurProductInfo->dwOmaDMFlag != 0 )
            {
                DWORD dwData = m_lstProductInfo.GetItemData( l );
                DWORD dwNewData = dwData;
                
                
                if ( strFileID.CompareNoCase( _T( "FDL2" ) ) == 0 )
                {
                    strFDL2Path = strFile;
                    m_bOmaDM = GetFdl2Flag( strFDL2Path, FDL2F_OMADM );
                    m_bPreload = GetFdl2Flag( strFDL2Path, FDL2F_PRELOAD );
                    m_bKernelImg2 = GetFdl2Flag( strFDL2Path, FDL2F_KERNELIMG2 );
                    m_bRomDisk = GetFdl2Flag( strFDL2Path, FDL2F_ROMDISK );
                }
                else if ( strFileID.CompareNoCase( g_szUA ) == 0 || strFileID.CompareNoCase( g_szEDU ) == 0 )
                {
                    if ( !m_bOmaDM )
                    {
                        dwNewData = DI_SET_GRAY( dwData );
                    }
                    else
                    {
                        dwNewData = DI_UNSET_GRAY( dwData );
                    }
                    
                    if ( DI_IS_GRAY( dwData ) != DI_IS_GRAY( dwNewData ) )
                    {
                        m_lstProductInfo.SetItemData( l, dwNewData );
                        m_lstProductInfo.SetCheck( l, FALSE );
                        if ( !DI_IS_GRAY( dwNewData ) )
                        {
                            m_bAllFiles = FALSE;
                        }
                    }
                }
                else if ( strFileID.CompareNoCase( g_szPreload ) == 0 )
                {
                    if ( !m_bPreload )
                    {
                        dwNewData = DI_SET_GRAY( dwData );
                    }
                    else
                    {
                        dwNewData = DI_UNSET_GRAY( dwData );
                    }
                    
                    if ( DI_IS_GRAY( dwData ) != DI_IS_GRAY( dwNewData ) )
                    {
                        m_lstProductInfo.SetItemData( l, dwNewData );
                        m_lstProductInfo.SetCheck( l, FALSE );
                        if ( !DI_IS_GRAY( dwNewData ) )
                        {
                            m_bAllFiles = FALSE;
                        }
                    }
                }
                else if ( strFileID.CompareNoCase( g_szKernelImg2 ) == 0 )
                {
                    if ( !m_bKernelImg2 )
                    {
                        dwNewData = DI_SET_GRAY( dwData );
                    }
                    else
                    {
                        dwNewData = DI_UNSET_GRAY( dwData );
                    }
                    
                    if ( DI_IS_GRAY( dwData ) != DI_IS_GRAY( dwNewData ) )
                    {
                        m_lstProductInfo.SetItemData( l, dwNewData );
                        m_lstProductInfo.SetCheck( l, FALSE );
                        if ( !DI_IS_GRAY( dwNewData ) )
                        {
                            m_bAllFiles = FALSE;
                        }
                    }
                }
                else if ( strFileID.CompareNoCase( g_szRomDisk ) == 0 )
                {
                    if ( !m_bRomDisk )
                    {
                        dwNewData = DI_SET_GRAY( dwData );
                    }
                    else
                    {
                        dwNewData = DI_UNSET_GRAY( dwData );
                    }
                    
                    if ( DI_IS_GRAY( dwData ) != DI_IS_GRAY( dwNewData ) )
                    {
                        m_lstProductInfo.SetItemData( l, dwNewData );
                        m_lstProductInfo.SetCheck( l, FALSE );
                        if ( !DI_IS_GRAY( dwNewData ) )
                        {
                            m_bAllFiles = FALSE;
                        }
                    }
                }
            }
            
            if ( bCheck )
            {
                m_lstProductInfo.SetCheck( l );
            }
            else
            {
                if ( pFileInfo[i].dwCheckFlag == 1 ) // must be selected
                {
                    m_lstProductInfo.SetCheck( l );
                }
                else
                {
                    m_lstProductInfo.SetCheck( l, FALSE );
                    DWORD dwData = m_lstProductInfo.GetItemData( l );
                    if ( !DI_IS_GRAY( dwData ) )
                    {
                        m_bAllFiles = FALSE;
                    }
                }
            }
            l++;
        }
        UpdateData( FALSE );
    }
    return TRUE;
}

CString CMainPage::GetXmlFile()
{
    CFileFind finder;
    CString strXmlFile;
    strXmlFile.Format( _T( "%sApp\\config\\BMAConfig.xml" ), g_theApp.m_strAppPath.operator LPCWSTR() );
    if ( !finder.FindFile( strXmlFile ) )
    {
        strXmlFile.Format( _T( "%sApp\\BMAConfig.xml" ), g_theApp.m_strAppPath.operator LPCWSTR() );
    }
    return strXmlFile;
}

int CMainPage::GetProductIndex( LPCTSTR lpcstrProuct )
{
    int nCount = m_arrProductName.GetSize();
    int i = 0;
    for ( i = 0; i < nCount; i++ )
    {
        if ( m_arrProductName.GetAt( i ).Compare( lpcstrProuct ) == 0 )
        {
            break;
        }
    }
    if ( i >= nCount )
    {
        return -1;
    }
    else
    {
        return i;
    }
}

void CMainPage::OnChkAll()
{
    // TODO: Add your control notification handler code here
    UpdateData();
    
    BOOL bCheck = m_bAllFiles;
    m_bListLock = TRUE;
    int nProduct = GetProductIndex( m_strCurProduct );
    if ( nProduct == -1 )
    {
        return;
    }
    PPRODUCT_INFO_T pProdInfo = m_arrProductInfo[nProduct];
    PFILE_INFO_T pFileInfo = pProdInfo->pFileInfoArr;
    
    int nCount = m_lstProductInfo.GetItemCount();
    CString strID;
    for ( int i = 0; i < nCount; i++ )
    {
        CString strIDAlias = m_lstProductInfo.GetItemText( i, 0 );
        int index = 0;
        for ( int j = 0; j < ( int )m_pCurProductInfo->dwFileCount; j++ )
        {
            if ( m_pCurProductInfo->pFileInfoArr[j].szIDAlias == strIDAlias )
            {
                index = j;
                break;
            }
        }
        
        strID = m_pCurProductInfo->pFileInfoArr[index].szID;
        
        DWORD dwItemData = m_lstProductInfo.GetItemData( i );
        if ( DI_IS_GRAY( dwItemData ) )
        {
            continue;
        }
        
        if ( pFileInfo[index].dwCheckFlag != 1 ) // maybe 0, 2
        {
#ifdef _SPUPGRADE
            if ( strID.CompareNoCase( _T( "FLASH" ) ) == 0 )
            {
                continue;
            }
#endif
            m_lstProductInfo.SetCheck( i, bCheck );
        }
    }
    
    m_bListLock = FALSE;
}

void CMainPage::OnChkMergeNV()
{
    // TODO: Add your control notification handler code here
    UpdateData();
    
    m_bListLock = TRUE;
    
    int nCount = m_lstMergeNV.GetItemCount();
    for ( int i = 0; i < nCount; i++ )
    {
        m_lstMergeNV.SetCheck( i, m_bChkMergeNV );
    }
    
    m_bListLock = FALSE;
}

void CMainPage::OnReadOnly()
{
    UpdateData( TRUE );
    if ( 1 == m_pFlashOptPageEx->m_bActiveReadFlash || 1 == m_pFlashOptPageEx->m_bActiveReadPartitions )
    {
        PPRODUCT_INFO_T pProdInfo = m_pCurProductInfo;

        DWORD dwCount = pProdInfo->dwFileCount;
        PFILE_INFO_T pFileInfo = pProdInfo->pFileInfoArr;

        CString strID;
        for ( int i = 0; i < dwCount; i++ )
        {
            if ( pFileInfo[i].dwCheckFlag != 1 )
            {
                m_lstProductInfo.SetCheck( i, FALSE );
            }
        }
        //Do not repartition on readflash_only
        m_pOptionsPage->m_bTmpRepart = FALSE;
        m_pOptionsPage->m_bRepart = FALSE;
        m_pOptionsPage->UpdateData( FALSE );
    }
    else
    {
        CString strProduct;
        m_cmbProduct.GetWindowText( strProduct );
        FillList( strProduct );
    }
    UpdateData( FALSE );
}

void CMainPage::OnDblclkLstProductInfo( NMHDR* pNMHDR, LRESULT* pResult )
{
    // TODO: Add your control notification handler code here
    if ( !m_bCanEdit )
    {
        *pResult = 0;
        return;
    }
    
    LPNMITEMACTIVATE pnia = ( LPNMITEMACTIVATE )pNMHDR;
    
    CEdtBtnCtrl* pCtrl = NULL;
    int nItem = pnia->iItem;
    int nSubItem = pnia->iSubItem;
    if ( nItem == -1 || nSubItem != 1 )
    {
        return;
    }
    if ( DI_IS_NEED_FILE( m_lstProductInfo.GetItemData( nItem ) ) == 0 )
    {
        return;
    }
    
    pCtrl = new CEdtBtnCtrl();
    
    if ( NULL != pCtrl )
    {
        pCtrl->SetFileFilterString( g_theApp.m_strFileFilter );
        pCtrl->m_nItem = nItem;
        pCtrl->m_nSubItem = nSubItem;
        pCtrl->m_strText = m_lstProductInfo.GetItemText( nItem, nSubItem );
        if ( !pCtrl->Create( IDD_EDT_BTN_DLG, FromHandle( m_lstProductInfo.m_hWnd ) ) )
        {
            AfxMessageBox( _T( "Error to create EdtBtnCtrl" ) );
            return;
        }
    }
    else
    {
        return;
    }
    
    CRect rect;
    m_lstProductInfo.GetSubItemRect( nItem, nSubItem, LVIR_BOUNDS, rect );
    CRect rtList;
    m_lstProductInfo.GetClientRect( &rtList );
    if ( rect.right > rtList.right )
    {
        rect.right = rtList.right;
    }
    if ( rect.left < rtList.left )
    {
        rect.left = rtList.left;
    }
    if ( rect.bottom > rtList.bottom )
    {
        rect.bottom = rtList.bottom;
    }
    if ( rect.top < rtList.top )
    {
        rect.top = rtList.top;
    }
    
    pCtrl->SetWindowPos( &wndTop, rect.left, rect.top - 1, rect.right - rect.left, rect.bottom - rect.top, NULL );
    
    pCtrl->ShowWindow( SW_SHOW );
    
    *pResult = 0;
    
}

void CMainPage::OnDblclkLstMergeNV( NMHDR* pNMHDR, LRESULT* pResult )
{
    // TODO: Add your control notification handler code here
    if ( !m_bCanEdit )
    {
        *pResult = 0;
        return;
    }
    
    LPNMITEMACTIVATE pnia = ( LPNMITEMACTIVATE )pNMHDR;
    
    CEdtBtnCtrl* pCtrl = NULL;
    int nItem = pnia->iItem;
    int nSubItem = pnia->iSubItem;
    if ( nItem == -1 || nSubItem != 1 )
    {
        return;
    }
    if ( DI_IS_NEED_FILE( m_lstMergeNV.GetItemData( nItem ) ) == 0 )
    {
        return;
    }
    
    pCtrl = new CEdtBtnCtrl();
    
    if ( NULL != pCtrl )
    {
        pCtrl->SetFileFilterString( g_theApp.m_strFileFilter );
        pCtrl->m_nItem = nItem;
        pCtrl->m_nSubItem = nSubItem;
        pCtrl->m_strText = m_lstMergeNV.GetItemText( nItem, nSubItem );
        if ( !pCtrl->Create( IDD_EDT_BTN_DLG, FromHandle( m_lstMergeNV.m_hWnd ) ) )
        {
            AfxMessageBox( _T( "Error to create EdtBtnCtrl" ) );
            return;
        }
    }
    else
    {
        return;
    }
    
    CRect rect;
    m_lstMergeNV.GetSubItemRect( nItem, nSubItem, LVIR_BOUNDS, rect );
    CRect rtList;
    m_lstMergeNV.GetClientRect( &rtList );
    if ( rect.right > rtList.right )
    {
        rect.right = rtList.right;
    }
    if ( rect.left < rtList.left )
    {
        rect.left = rtList.left;
    }
    if ( rect.bottom > rtList.bottom )
    {
        rect.bottom = rtList.bottom;
    }
    if ( rect.top < rtList.top )
    {
        rect.top = rtList.top;
    }
    
    pCtrl->SetWindowPos( &wndTop, rect.left, rect.top - 1, rect.right - rect.left, rect.bottom - rect.top, NULL );
    
    pCtrl->ShowWindow( SW_SHOW );
    
    *pResult = 0;
    
}

void CMainPage::OnEndlabeleditLstProductInfo( NMHDR* pNMHDR, LRESULT* pResult )
{
    LV_DISPINFO* pDispInfo = ( LV_DISPINFO* )pNMHDR;
    // TODO: Add your control notification handler code here
    m_bListLock = TRUE;
    m_lstProductInfo.SetItemText( pDispInfo->item.iItem, pDispInfo->item.iSubItem, pDispInfo->item.pszText );
    m_bListLock = FALSE;
    if ( m_pCurProductInfo != NULL && m_pCurProductInfo->dwOmaDMFlag != 0 )
    {
        CString fileID = m_pCurProductInfo->pFileInfoArr[pDispInfo->item.iItem].szID;
        if ( fileID.CompareNoCase( _T( "FDL2" ) ) == 0 )
        {
            m_bOmaDM = GetFdl2Flag( pDispInfo->item.pszText, FDL2F_OMADM );
            m_bPreload = GetFdl2Flag( pDispInfo->item.pszText, FDL2F_PRELOAD );
            m_bKernelImg2 = GetFdl2Flag( pDispInfo->item.pszText, FDL2F_KERNELIMG2 );
            m_bRomDisk = GetFdl2Flag( pDispInfo->item.pszText, FDL2F_ROMDISK );
            ModifyItemGray();
        }
    }
    
    *pResult = 0;
}

void CMainPage::OnEndlabeleditLstMergeNV( NMHDR* pNMHDR, LRESULT* pResult )
{
    LV_DISPINFO* pDispInfo = ( LV_DISPINFO* )pNMHDR;
    // TODO: Add your control notification handler code here
    m_bListLock = TRUE;
    m_lstMergeNV.SetItemText( pDispInfo->item.iItem, pDispInfo->item.iSubItem, pDispInfo->item.pszText );
    
    *pResult = 0;
}

void CMainPage::OnItemchangedLstProductInfo( NMHDR* pNMHDR, LRESULT* pResult )
{
    if ( m_bListLock )
    {
        return;
    }
    
    NM_LISTVIEW* pNMListView = ( NM_LISTVIEW* )pNMHDR;
    if ( pNMListView->iItem < 0 || pNMListView->iSubItem != 0 )
    {
        return;
    }
    // TODO: Add your control notification handler code here
    if ( pNMListView->uNewState == 0x00001000 ) // uncheck
    {
        DWORD dwItemData = m_lstProductInfo.GetItemData( pNMListView->iItem );
        if ( !DI_IS_GRAY( dwItemData ) )
        {
            m_bAllFiles = FALSE;
        }
    }
    else if ( pNMListView->uNewState == 0x00002000 ) // check
    {
        m_bAllFiles = TRUE;
        int nItem = m_lstProductInfo.GetItemCount();
        int i = 0;
        for ( i = 0; i < nItem; i++ )
        {
            if ( !m_lstProductInfo.GetCheck( i ) )
            {
                DWORD dwItemData = m_lstProductInfo.GetItemData( i );
                if ( !DI_IS_GRAY( dwItemData ) )
                {
                    m_bAllFiles = FALSE;
                    break;
                }
            }
        }
    }
    
    GetDlgItem( IDC_CHK_ALL )->UpdateData( FALSE );
    
    if ( m_bCanSel )
    {
        int nProduct = GetProductIndex( m_strCurProduct );
        if ( nProduct != -1 )
        {
            PPRODUCT_INFO_T pProdInfo = m_arrProductInfo[nProduct];
            PFILE_INFO_T pFileInfo = pProdInfo->pFileInfoArr;
            
            CString strFileID;
            strFileID = pFileInfo[pNMListView->iItem].szID;
            if ( pFileInfo[pNMListView->iItem].isGroup == 1 )
            {
                int curFileIndex = pNMListView->iItem;
                BOOL curFileCheck = m_lstProductInfo.GetCheck( curFileIndex );
                int nItem = m_lstProductInfo.GetItemCount();
                int i = 0;
                for ( i = 0; i < nItem; i++ )
                {
                    if ( pFileInfo[i].isGroup == 1 )
                    {
                        m_lstProductInfo.SetCheck( i, curFileCheck );
                    }
                }
                
            }
            CString strText = m_lstProductInfo.GetItemText( pNMListView->iItem, 0 );
            
#ifdef _SPUPGRADE
            if ( strFileID.CompareNoCase( _T( "FLASH" ) ) == 0 && pNMListView->uNewState == 0x00001000 )
            {
                AfxMessageBox( _T( "Must select flash!!!" ) );
                m_lstProductInfo.SetCheck( pNMListView->iItem );
            }
#endif
            if ( pFileInfo[pNMListView->iItem].dwCheckFlag == 1 && pNMListView->uNewState == 0x00001000 )
            {
                CString strWarning;
                strWarning.Format( _T( "Must select %s!!!" ), strText.operator LPCTSTR() );
                AfxMessageBox( strWarning );
                m_lstProductInfo.SetCheck( pNMListView->iItem );
            }
            
            DWORD dwItemData = m_lstProductInfo.GetItemData( pNMListView->iItem );
            if ( DI_IS_GRAY( dwItemData ) )
            {
                m_lstProductInfo.SetCheck( pNMListView->iItem, FALSE );
            }
        }
    }
    
    UpdateData( FALSE );
    *pResult = 0;
}

void CMainPage::InitNandPage( PPRODUCT_INFO_T pCurProduct )
{
    UNUSED_ALWAYS( pCurProduct );
    
    if ( m_pOptionsPage )
    {
        m_pOptionsPage->m_nFlashPageType = m_tBinPacketHeader.dwNandPageType;
#ifdef _FACTORY
        m_pOptionsPage->m_bRepart = ( m_tBinPacketHeader.dwNandStrategy == REPAR_STRATEGY_DO_ALWAYS ) ? TRUE : FALSE;
#endif
    }
}

void CMainPage::OnStorageTypeChangeFunc( int type )
{
    m_nStorageType = type;
    InitVolFreqPage( TRUE );
}

void CMainPage::InitVolFreqPage( BOOL bUpdateWnd )
{
    //BOOL bPS = TRUE;
    VEC_LCD_CFIG vLCDCfig;
    int nSPLItem = g_theApp.m_fnOperationHelper.GetFileInfo( m_pCurProductInfo, _T( "SPLLoader" ), NULL );
    if ( m_nStorageType == 1 )
    {
        nSPLItem = g_theApp.m_fnOperationHelper.GetFileInfo( m_pCurProductInfo, _T( "SPLLoaderEMMC" ), NULL );
    }
    else if ( m_nStorageType == 2 )
    {
        nSPLItem = g_theApp.m_fnOperationHelper.GetFileInfo( m_pCurProductInfo, _T( "SPLLoaderUFS" ), NULL );
    }
    CString strSPLFile;
    BOOL bSPLCheck = TRUE;
    
    if ( nSPLItem != -1 )
    {
        if ( bUpdateWnd )
        {
            if ( nSPLItem < m_lstProductInfo.GetItemCount() )
            {
                bSPLCheck = m_lstProductInfo.GetCheck( nSPLItem );
                if ( bSPLCheck )
                {
                    strSPLFile = m_lstProductInfo.GetItemText( nSPLItem, 1 );
                }
            }
            else
            {
                bSPLCheck = FALSE;
                strSPLFile.Empty();
            }
        }
        else
        {
            CString strID = _T( "SPLLoader" );
            strID.MakeUpper();
            m_mapDLState.Lookup( strID, bSPLCheck );
            if ( bSPLCheck )
            {
                m_mapDLFiles.Lookup( strID, strSPLFile );
            }
        }
    }
    else
    {
        bSPLCheck = FALSE;
        strSPLFile.Empty();
    }
    
    m_pVolFreqPage->SetConfig( bUpdateWnd, strSPLFile );
    
}

void CMainPage::InitUartPortSwitchPage( BOOL bUpdateWnd )
{
    int nUbootItem = g_theApp.m_fnOperationHelper.GetFileInfo( m_pCurProductInfo, _T( "UBOOTLoader" ), NULL );
    
    CString strUbootFile;
    BOOL bUbootCheck = TRUE;
    
    if ( nUbootItem != -1 )
    {
        if ( bUpdateWnd )
        {
            if ( nUbootItem < m_lstProductInfo.GetItemCount() )
            {
                bUbootCheck = m_lstProductInfo.GetCheck( nUbootItem );
                if ( bUbootCheck )
                {
                    strUbootFile = m_lstProductInfo.GetItemText( nUbootItem, 1 );
                }
            }
            else
            {
                bUbootCheck = FALSE;
                strUbootFile.Empty();
            }
        }
        else
        {
            CString strID = _T( "UBOOTLoader" );
            strID.MakeUpper();
            m_mapDLState.Lookup( strID, bUbootCheck );
            if ( bUbootCheck )
            {
                m_mapDLFiles.Lookup( strID, strUbootFile );
            }
        }
        m_pUartPortSwitchPage->SetConfig( bUpdateWnd, strUbootFile );
    }
    else
    {
        bUbootCheck = FALSE;
        strUbootFile.Empty();
    }
}

BOOL CMainPage::LoadConfig()
{
    InitNandPage( m_pCurProductInfo );
    
#ifdef _RESEARCH
    InitVolFreqPage();
    InitUartPortSwitchPage();
#endif
    
#ifdef _SPUPGRADE
    CString strTmpNotChkItemName = m_pCaliPage->m_lstBackup.GetTmpNotChkItemName();
    if ( !strTmpNotChkItemName.IsEmpty() )
    {
        CString strNVItemNames = strTmpNotChkItemName;
        strNVItemNames += _T( "," );
        strNVItemNames.Replace( _T( "Calibration," ), _T( "" ) );
        strNVItemNames.Replace( _T( "TD_Calibration," ), _T( "" ) );
        strNVItemNames.Replace( _T( "IMEI," ), _T( "" ) );
        strNVItemNames.TrimLeft( _T( "," ) );
        if ( strNVItemNames.IsEmpty() )
        {
            return TRUE;
        }
        CString strWarn;
        strWarn.Format( _T( "Not selected \"%s\" to backup in pac file!\nIf continue?" ), strTmpNotChkItemName.operator LPCTSTR() );
        if ( m_bShowChinese )
        {
            CString strTemp;
            strTemp.Format( _T( "\n\n包文件中没有选择 \"%s\" 备份项！\n是否继续？" ), strTmpNotChkItemName.operator LPCTSTR() );
            strWarn += strTemp;
        }
        if ( AfxMessageBox( strWarn, MB_YESNO ) == IDNO )
        {
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }
#endif
    return TRUE;
}

HBRUSH CMainPage::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor )
{
    HBRUSH hbr = CPropertyPage::OnCtlColor( pDC, pWnd, nCtlColor );
    
    // TODO: Change any attributes of the DC here
    
    if ( pWnd->GetDlgCtrlID() == IDC_EDT_PRD_VERSION )
    {
        pDC->SetTextColor( RGB( 0, 128, 64 ) );
    }
    
    // TODO: Return a different brush if the default is not desired
    return hbr;
}

BOOL CMainPage::GetFdl2Flag( LPCTSTR lpszFilePath, UINT nType )
{
    CString strErrMsg = _T( "" );
    BOOL bOK = g_theApp.m_fnOperationHelper.GetFdl2Flag( lpszFilePath, nType, strErrMsg );
    if ( !strErrMsg.IsEmpty() )
    {
        AfxMessageBox( strErrMsg );
    }
    return bOK;
}

void CMainPage::OnRclickLstProductInfo( NMHDR* pNMHDR, LRESULT* pResult )
{
    if ( !m_bCanEdit )
    {
        *pResult = 0;
        return;
    }
    
    LPNMITEMACTIVATE pnia = ( LPNMITEMACTIVATE )pNMHDR;
    
    
    int nItem = pnia->iItem;
    int nSubItem = pnia->iSubItem;
    if ( nItem == -1 || nSubItem != 1 )
    {
        return;
    }
    //LOWORD is file flag
    //HIWORD is gray flag
    if ( DI_IS_NEED_FILE( m_lstProductInfo.GetItemData( nItem ) ) == 0 )
    {
        return;
    }
    
    static _TCHAR BASED_CODE szFilter[] = _T( "All Files(*.*)|*.*|Binary Files (*.bin)|*.bin||" );
    CString strFile = _T( "" );
    CFileDialog dlg( TRUE, NULL, strFile, OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, szFilter, NULL );
    
    if ( dlg.DoModal() == IDOK )
    {
        CString strText = dlg.GetPathName();
        m_lstProductInfo.SetItemText( nItem, nSubItem, strText );
        if ( m_pCurProductInfo != NULL && m_pCurProductInfo->dwOmaDMFlag != 0 )
        {
        
            CString strID = m_lstProductInfo.GetItemText( nItem, 0 );
            int index = 0;
            for ( int j = 0; j < ( int )m_pCurProductInfo->dwFileCount; j++ )
            {
                if ( m_pCurProductInfo->pFileInfoArr[j].szIDAlias == strID )
                {
                    index = j;
                    break;
                }
            }
            
            CString fileID = m_pCurProductInfo->pFileInfoArr[index].szID;
            if ( fileID.CompareNoCase( _T( "FDL2" ) ) == 0 )
            {
                m_bOmaDM = GetFdl2Flag( strText, FDL2F_OMADM );
                m_bPreload = GetFdl2Flag( strText, FDL2F_PRELOAD );
                m_bKernelImg2 = GetFdl2Flag( strText, FDL2F_KERNELIMG2 );
                m_bRomDisk = GetFdl2Flag( strText, FDL2F_ROMDISK );
                ModifyItemGray();
            }
        }
        
    }
    
    *pResult = 0;
}

void CMainPage::OnClickLstProductInfo( NMHDR* pNMHDR, LRESULT* pResult )
{
    // TODO: Add your control notification handler code here
    LPNMITEMACTIVATE pnia = ( LPNMITEMACTIVATE )pNMHDR;
    
    int nItem = pnia->iItem;
    int nSubItem = pnia->iSubItem;
    int index = 0;
    if ( nItem == -1 || nSubItem != 1 )
    {
        if ( m_pCurProductInfo != NULL )
        {
            m_stcProdComment.SetText( m_pCurProductInfo->szComment );
        }
        return;
    }
    
    CString strFileDescript = _T( "" );
    CString strTips;
    CString strID = m_lstProductInfo.GetItemText( nItem, 0 );
    if ( m_pCurProductInfo != NULL )
    {
        for ( int j = 0; j < ( int )m_pCurProductInfo->dwFileCount; j++ )
        {
            if ( m_pCurProductInfo->pFileInfoArr[j].szIDAlias == strID )
            {
                index = j;
                break;
            }
        }
        
        if ( _tcslen( m_pCurProductInfo->pFileInfoArr[index].szFileDescript ) > 0 )
        {
            strFileDescript.Format( _T( "Note: %s" ), m_pCurProductInfo->pFileInfoArr[index].szFileDescript );
        }
    }
    
    CString strFile = m_lstProductInfo.GetItemText( nItem, nSubItem );
    if ( DI_IS_NEED_FILE( m_lstProductInfo.GetItemData( nItem ) ) == 0 || strFile.IsEmpty() )
    {
        strTips = strFileDescript;
        m_stcProdComment.SetText( strTips );
        
    }
    else
    {
        CFileFind finder;
        if ( finder.FindFile( strFile ) )
        {
            finder.FindNextFile();
            CTime lwt;
            finder.GetLastWriteTime( lwt );
            __int64 llFileSize = finder.GetLength();
            if ( IsLoadFromPac( strFile.operator LPCTSTR() ) )
            {
                DATA_INFO_T dataInfo = GetDataInfo( strFile );
                llFileSize = dataInfo.llSize;
            }
            strTips.Format( _T( "Size: %-8I64d [0x%I64X] Modify Time:%04d-%02d-%02d %02d:%02d:%02d\n%s" ),
                            llFileSize,
                            llFileSize,
                            lwt.GetYear(),
                            lwt.GetMonth(),
                            lwt.GetDay(),
                            lwt.GetHour(),
                            lwt.GetMinute(),
                            lwt.GetSecond(),
                            strFileDescript.operator LPCTSTR() );
                            
        }
        else
        {
            strTips.Format( _T( "File Not Found!\n%s" ), strFileDescript.operator LPCTSTR() );
            
        }
        m_stcProdComment.SetText( strTips );
        
    }
    *pResult = 0;
}

void CMainPage::ModifyItemGray()
{
    m_bListLock = TRUE;
    m_bAllFiles = TRUE;
    for ( int i = 0; i < m_lstProductInfo.GetItemCount(); i++ )
    {
        DWORD dwData = m_lstProductInfo.GetItemData( i );
        DWORD dwNewData = dwData;
        CString fileID = m_pCurProductInfo->pFileInfoArr[i].szID;
        if ( fileID.CompareNoCase( g_szUA ) == 0 || fileID.CompareNoCase( g_szEDU ) == 0 )
        {
            if ( !m_bOmaDM )
            {
                dwNewData = DI_SET_GRAY( dwData );
            }
            else
            {
                dwNewData = DI_UNSET_GRAY( dwData );
            }
            
            if ( DI_IS_GRAY( dwData ) != DI_IS_GRAY( dwNewData ) )
            {
                m_lstProductInfo.SetItemData( i, dwNewData );
                m_lstProductInfo.SetCheck( i, FALSE );
                if ( !DI_IS_GRAY( dwNewData ) )
                {
                    m_bAllFiles = FALSE;
                }
            }
        }
        else if ( fileID.CompareNoCase( g_szPreload ) == 0 )
        {
            if ( !m_bPreload )
            {
                dwNewData = DI_SET_GRAY( dwData );
            }
            else
            {
                dwNewData = DI_UNSET_GRAY( dwData );
            }
            
            if ( DI_IS_GRAY( dwData ) != DI_IS_GRAY( dwNewData ) )
            {
                m_lstProductInfo.SetItemData( i, dwNewData );
                m_lstProductInfo.SetCheck( i, FALSE );
                if ( !DI_IS_GRAY( dwNewData ) )
                {
                    m_bAllFiles = FALSE;
                }
            }
            
        }
        else if ( fileID.CompareNoCase( g_szKernelImg2 ) == 0 )
        {
            if ( !m_bKernelImg2 )
            {
                dwNewData = DI_SET_GRAY( dwData );
            }
            else
            {
                dwNewData = DI_UNSET_GRAY( dwData );
            }
            
            if ( DI_IS_GRAY( dwData ) != DI_IS_GRAY( dwNewData ) )
            {
                m_lstProductInfo.SetItemData( i, dwNewData );
                m_lstProductInfo.SetCheck( i, FALSE );
                if ( !DI_IS_GRAY( dwNewData ) )
                {
                    m_bAllFiles = FALSE;
                }
            }
            
        }
        else if ( fileID.CompareNoCase( g_szRomDisk ) == 0 )
        {
            if ( !m_bRomDisk )
            {
                dwNewData = DI_SET_GRAY( dwData );
            }
            else
            {
                dwNewData = DI_UNSET_GRAY( dwData );
            }
            
            if ( DI_IS_GRAY( dwData ) != DI_IS_GRAY( dwNewData ) )
            {
                m_lstProductInfo.SetItemData( i, dwNewData );
                m_lstProductInfo.SetCheck( i, FALSE );
                if ( !DI_IS_GRAY( dwNewData ) )
                {
                    m_bAllFiles = FALSE;
                }
            }
        }
        else
        {
            if ( !m_lstProductInfo.GetCheck( i ) )
            {
                m_bAllFiles = FALSE;
            }
        }
        
    }
    
    m_bListLock = FALSE;
    
    UpdateData( FALSE );
}

void CMainPage::InitBaudrate()
{
    CString strText;
    m_arrBaudRateDefault.RemoveAll();
    
    CStringArray arrBaudRate;
    if ( m_arrBaudRateConfig.GetSize() > 0 )
    {
        arrBaudRate.Copy( m_arrBaudRateConfig );
    }
    else
    {
        CString strBaudRate;
        VERIFY( strBaudRate.LoadString( IDS_BAUDRATE_DEFAULT ) );
        
        INT nCount = 0;
        CUtility utility;
        LPTSTR* lppString = utility.GetTokenString( strBaudRate.operator LPCWSTR(), _T( "," ), nCount );
        if ( NULL != lppString && 0 != nCount )
        {
            for ( int i = 0; i < nCount; i++ )
            {
                m_arrBaudRateDefault.Add( lppString[i] );
            }
        }
        arrBaudRate.Copy( m_arrBaudRateDefault );
    }
    
    for ( int i = 0; i < ( int )arrBaudRate.GetSize(); i++ )
    {
        m_cmbBaudrate.AddString( arrBaudRate.GetAt( i ) );
    }
    
    strText.Format( _T( "%d" ), m_nBaudRate );
    
    int iSelBaudRate = m_cmbBaudrate.FindString( 0, strText );
    if ( iSelBaudRate == CB_ERR )
    {
        iSelBaudRate = 1;// Default is 115200
    }
    m_cmbBaudrate.SetCurSel( iSelBaudRate );
}

void CMainPage::InitPort()
{
    int iSelPort = 0;
    m_cmbPort.AddString( ALL_PORT );
    if ( NULL != m_pDevHound )
    {
        std::vector<DEV_INFO> lstPort;
        DEV_INFO* pDevInfo = NULL;
        m_pDevHound->Start();
        int nCount = m_pDevHound->ScanPort( &pDevInfo );
        m_pDevHound->Stop();
        lstPort.clear();
        for ( int j = 0; j < nCount; ++j )
        {
            lstPort.push_back( pDevInfo[j] );
        }
        for ( UINT i = 0; i < lstPort.size(); i++ )
        {
            if ( g_theApp.m_bFilterPort )
            {
                DWORD dwValue = 0;
                if ( g_theApp.m_mapFilterPort.Lookup( ( DWORD )lstPort[i].nPortNum, dwValue ) )
                {
                    continue;
                }
            }
            
            if ( m_cmbPort.FindString( 0, lstPort[i].szPortName ) == CB_ERR )
            {
                m_cmbPort.AddString( lstPort[i].szPortName );
            }
        }
        m_pDevHound->FreeMemory( pDevInfo );
        
        CString strText;
        if ( m_nComPort != 0 )
        {
            strText.Format( _T( "COM%d" ), m_nComPort );
            iSelPort = m_cmbPort.FindString( 0, strText );
            if ( iSelPort == CB_ERR )
            {
                m_cmbPort.AddString( strText );
                iSelPort = 1;
            }
        }
        
    }
    m_cmbPort.SetCurSel( iSelPort );
}

void CMainPage::InitListCtrl()
{
    //m_lstProduct
    m_lstProductInfo.SetBkColor( RGB( 232, 232, 232 ) );
    m_lstProductInfo.ModifyStyle( 0, LVS_SHOWSELALWAYS );
    
    DWORD dwExStyle = m_lstProductInfo.GetExtendedStyle();
    dwExStyle |= LVS_EX_FULLROWSELECT;
    dwExStyle |= LVS_EX_GRIDLINES;
    if ( m_bCanSel )
    {
        dwExStyle |= LVS_EX_CHECKBOXES;
    }
    else
    {
        GetDlgItem( IDC_CHK_ALL )->ShowWindow( SW_HIDE );
    }
    
    m_lstProductInfo.SetExtendedStyle( dwExStyle );
    
    //Load Column
    CString strClmn;
    VERIFY( strClmn.LoadString( IDS_PRODUCT_INFO_COLUMN ) );
    int nLen = strClmn.GetLength();
    LPTSTR lpBuf = strClmn.GetBuffer( nLen );
    LPTSTR lpFind = _tcschr( lpBuf, _T( ',' ) );
    int     nIndex = 0;
    while ( lpFind != NULL )
    {
        *lpFind = _T( '\0' );
        if ( nIndex == 1 )
        {
            m_lstProductInfo.InsertColumn( nIndex++, lpBuf, LVCFMT_LEFT, 250 );
        }
        else
        {
            m_lstProductInfo.InsertColumn( nIndex++, lpBuf, LVCFMT_LEFT, 110 );
        }
        lpBuf = lpFind + 1;
        lpFind = _tcschr( lpBuf, _T( ',' ) );
    }
    m_lstProductInfo.InsertColumn( nIndex, lpBuf, LVCFMT_LEFT, 100 );
    strClmn.ReleaseBuffer();
    
    m_lstProductInfo.EnableToolTips();
    
    //m_lstMergeNV
    m_lstMergeNV.SetBkColor( RGB( 232, 232, 232 ) );
    m_lstMergeNV.ModifyStyle( 0, LVS_SHOWSELALWAYS );
    
    dwExStyle = m_lstMergeNV.GetExtendedStyle();
    dwExStyle |= LVS_EX_FULLROWSELECT;
    dwExStyle |= LVS_EX_GRIDLINES;
    if ( m_bCanSel )
    {
        dwExStyle |= LVS_EX_CHECKBOXES;
    }
    else
    {
        GetDlgItem( IDC_CHK_MERGE_NV )->ShowWindow( SW_HIDE );
    }
    
    m_lstMergeNV.SetExtendedStyle( dwExStyle );
    
    //Load Column
    CString strClmn2;
    VERIFY( strClmn2.LoadString( IDS_MERGE_NV_COLUMN ) );
    int nLen2 = strClmn2.GetLength();
    LPTSTR lpBuf2 = strClmn2.GetBuffer( nLen2 );
    LPTSTR lpFind2 = _tcschr( lpBuf2, _T( ',' ) );
    int     nIndex2 = 0;
    while ( lpFind2 != NULL )
    {
        *lpFind2 = _T( '\0' );
        if ( nIndex2 == 1 )
        {
            m_lstMergeNV.InsertColumn( nIndex2++, lpBuf2, LVCFMT_LEFT, 350 );
        }
        else
        {
            m_lstMergeNV.InsertColumn( nIndex2++, lpBuf2, LVCFMT_LEFT, 110 );
        }
        lpBuf2 = lpFind2 + 1;
        lpFind2 = _tcschr( lpBuf2, _T( ',' ) );
    }
    m_lstMergeNV.InsertColumn( nIndex2, lpBuf2, LVCFMT_LEFT, 400 );
    strClmn2.ReleaseBuffer();
    
    m_lstMergeNV.EnableToolTips();
}

BOOL CMainPage::LoadSettings()
{
    if ( NULL == g_theApp.m_pCfgObject )
    {
        ASSERT( 0 );
        AfxMessageBox( _T( "[DL2082][F]Configure file is empty!" ) );
        return FALSE;
    }
    
    CString strBaudRate;
    strBaudRate = g_theApp.m_pCfgObject->GetString( CFGTYPE_DL, g_sz_SEC_GUI, g_sz_KEY_BRS, g_sz_BR_DEFAULT );
    if ( !strBaudRate.IsEmpty() )
    {
        m_arrBaudRateConfig.RemoveAll();
        
        INT nCount = 0;
        CUtility utility;
        LPTSTR* lppString = utility.GetTokenString( strBaudRate.operator LPCWSTR(), _T( "," ), nCount );
        if ( NULL != lppString && 0 != nCount )
        {
            for ( int i = 0; i < nCount; i++ )
            {
                m_arrBaudRateConfig.Add( lppString[i] );
            }
        }
    }
    
    // Get baudrate
    m_nBaudRate = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, g_sz_SP, g_sz_BR, 115200 );
    m_nComPort = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, g_sz_SP, g_sz_PORT, 0 );
    
#ifdef _SPUPGRADE
    m_bCanSel = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, g_sz_SEC_GUI, g_sz_KEY_SEL, 0 );
    m_bCanEdit = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, g_sz_SEC_GUI, g_sz_KEY_EDT, 0 );
    m_bShowChinese = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, g_sz_SEC_GUI, _T( "ShowChinese" ), 1 );
#else
    m_bCanSel = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, g_sz_SEC_GUI, g_sz_KEY_SEL, 1 );
    m_bCanEdit = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, g_sz_SEC_GUI, g_sz_KEY_EDT, 1 );
#endif
    
#ifdef _FACTORY
    m_bCanSel = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, g_sz_SEC_GUI, g_sz_KEY_SEL, 0 );
    m_bCanEdit = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, g_sz_SEC_GUI, g_sz_KEY_EDT, 0 );
#endif
    
    m_bEnableChipDspMap = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, g_sz_SEC_CHIPDSPMAP, g_sz_KEY_CDME, FALSE );
    m_wLangNVItemID = ( WORD )g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, g_sz_SEC_NV, g_sz_KEY_LNII, NV_MULTI_LANG_ID );
    m_bChkWriteSN = !( ( BOOL )g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, _T( "SN" ), _T( "AutoGenerateSN" ), 1 ) );
    
    return TRUE;
}

BOOL CMainPage::IsLoadFromPac( LPCTSTR lpDLFile )
{
    BOOL bRet = FALSE;
    CString strFileName( lpDLFile );
    if ( strFileName.GetLength() > MAX_FLAG_LEN && 0 == strFileName.Right( MAX_FLAG_LEN ).Compare( FLAG_FILE_SUFFIX ) )
    {
        bRet = TRUE;
    }
    return bRet;
    
}

DATA_INFO_T CMainPage::GetDataInfo( LPCTSTR lpDLFile )
{
    CString strFile = g_theApp.m_fnOperationHelper.GetStrFileTitle( lpDLFile, TRUE, TRUE );
    DATA_INFO_T dataInfo;
    if ( !m_mapDataInfo.Lookup( strFile, dataInfo ) )
    {
        dataInfo.llOffset = dataInfo.llSize = 0;
    }
    return dataInfo;
    
}

void CMainPage::ReleaseProperty()
{
    m_pDevHound = NULL;
    m_strPrdVersion = _T( "" );
    m_nCurProduct = 0;
    m_mapDLFiles.RemoveAll();
    m_mapDLState.RemoveAll();
    m_mapDLSize.RemoveAll();
    m_mapDataInfo.RemoveAll();
    m_arrProductName.RemoveAll();
    m_arrProductInfo.clear();
}

BOOL CMainPage::SetProperty()
{
    CMainFrame* pMainFrame = ( CMainFrame* )AfxGetMainWnd();
    
    SPRESULT spRlt = SP_OK;
    
    DLFW_ATTRDATA_DLIMAGEINFO data;
    g_theApp.m_fnOperationHelper.CreatDlImageInfo( m_mapDLFiles.GetCount(), &data );
    spRlt &= g_theApp.m_fnOperationHelper.ConfigAttrDataByDlFileName( m_mapDLFiles, &data );
    spRlt &= pMainFrame->m_pFrameHelper->SetProperty( DLFW_ATTR_DL_FILE_NAME, 0, &data );
    g_theApp.m_fnOperationHelper.ReleaseDlImageInfo( &data );
    
    g_theApp.m_fnOperationHelper.CreatDlImageInfo( m_mapDLState.GetCount(), &data );
    spRlt &= g_theApp.m_fnOperationHelper.ConfigAttrDataByDlFileState( m_mapDLState, &data );
    spRlt &= pMainFrame->m_pFrameHelper->SetProperty( DLFW_ATTR_DL_FILE_STATE, 0, &data );
    g_theApp.m_fnOperationHelper.ReleaseDlImageInfo( &data );
    
    spRlt &= pMainFrame->m_pFrameHelper->SetProperty( DLFW_ATTR_PRODUCT_USE, 0, &m_nCurProduct );
    
    if ( SP_OK != spRlt )
    {
        MessageBox( _T( "Set Parameters To DLFramework Fail." ) );
    }
    return SP_OK == spRlt ? TRUE : FALSE;
}

BOOL CMainPage::GetProperty()
{
    CMainFrame* pMainFrame = ( CMainFrame* )AfxGetMainWnd();
    
    SPRESULT spRlt = SP_OK;
    
    spRlt &= pMainFrame->m_pFrameHelper->GetProperty( DLFW_ATTR_DEV_HOUND_OBJECT, 0, &m_pDevHound );
    
    LPTSTR lpString = NULL;
    spRlt &= pMainFrame->m_pFrameHelper->GetProperty( DLFW_ATTR_PRD_VERSION, 0, &lpString );
    m_strPrdVersion = lpString;
    
    spRlt &= pMainFrame->m_pFrameHelper->GetProperty( DLFW_ATTR_PRODUCT_USE, 0, &m_nCurProduct );
    
    DLFW_ATTRDATA_COMMON data;
    g_theApp.m_fnOperationHelper.CreatCommonData( g_theApp.m_attrStackSize.u32ProductName, &data );
    spRlt &= pMainFrame->m_pFrameHelper->GetProperty( DLFW_ATTR_PRODUCT_NAME_ALL, 0, &data );
    spRlt &= g_theApp.m_fnOperationHelper.ConfigProductNameByAttrData( m_arrProductName, &data );
    g_theApp.m_fnOperationHelper.ReleaseCommonData( &data );
    
    g_theApp.m_fnOperationHelper.CreatCommonData( g_theApp.m_attrStackSize.u32ProductInfo, &data );
    spRlt &= pMainFrame->m_pFrameHelper->GetProperty( DLFW_ATTR_PRODUCT_INFO_ALL, 0, &data );
    spRlt &= g_theApp.m_fnOperationHelper.ConfigProductInfoByAttrData( m_arrProductInfo, &data );
    g_theApp.m_fnOperationHelper.ReleaseCommonData( &data );
    
    DLFW_ATTRDATA_DLIMAGEINFO image;
    g_theApp.m_fnOperationHelper.CreatDlImageInfo( g_theApp.m_attrStackSize.u32DlImageInfo, &image );
    spRlt &= pMainFrame->m_pFrameHelper->GetProperty( DLFW_ATTR_DL_FILE_NAME, 0, &image );
    spRlt &= g_theApp.m_fnOperationHelper.ConfigDlFileNameByAttrData( m_mapDLFiles, &image );
    g_theApp.m_fnOperationHelper.ReleaseDlImageInfo( &image );
    
    g_theApp.m_fnOperationHelper.CreatDlImageInfo( g_theApp.m_attrStackSize.u32DlImageInfo, &image );
    spRlt &= pMainFrame->m_pFrameHelper->GetProperty( DLFW_ATTR_DL_FILE_STATE, 0, &image );
    spRlt &= g_theApp.m_fnOperationHelper.ConfigDlFileStateByAttrData( m_mapDLState, &image );
    g_theApp.m_fnOperationHelper.ReleaseDlImageInfo( &image );
    
    g_theApp.m_fnOperationHelper.CreatDlImageInfo( g_theApp.m_attrStackSize.u32DlImageInfo, &image );
    spRlt &= pMainFrame->m_pFrameHelper->GetProperty( DLFW_ATTR_DL_FILE_SIZE, 0, &image );
    spRlt &= g_theApp.m_fnOperationHelper.ConfigDlFileSizeByAttrData( m_mapDLSize, &image );
    g_theApp.m_fnOperationHelper.ReleaseDlImageInfo( &image );
    
    g_theApp.m_fnOperationHelper.CreatDlImageInfo( g_theApp.m_attrStackSize.u32DlImageInfo, &image );
    spRlt &= pMainFrame->m_pFrameHelper->GetProperty( DLFW_ATTR_DL_DATA_INFO, 0, &image );
    spRlt &= g_theApp.m_fnOperationHelper.ConfigDlDataInfoByAttrData( m_mapDataInfo, &image );
    g_theApp.m_fnOperationHelper.ReleaseDlImageInfo( &image );
    
    if ( SP_OK != spRlt )
    {
        MessageBox( _T( "Get Parameters From DLFramework Fail." ) );
    }
    else
    {
        m_nProductCount = m_arrProductName.GetCount();
        if ( m_nCurProduct > -1 && m_nCurProduct < m_nProductCount )
        {
            m_strCurProduct = m_arrProductName[m_nCurProduct];
            m_pCurProductInfo = m_arrProductInfo[m_nCurProduct];
        }
        else
        {
            m_strCurProduct = _T( "" );
            m_pCurProductInfo = NULL;
        }
    }
    
    return SP_OK == spRlt ? TRUE : FALSE;
    
}

BOOL CMainPage::DoPacket()
{
    CWaitCursor wait;
    if ( m_pCurProductInfo == NULL || m_pOptionsPage == NULL || m_pCaliPage == NULL )
    {
        return FALSE;
    }
    
    CSettingsSheet* pParentWnd = NULL;
    pParentWnd = ( CSettingsSheet* )GetParent();
    
    //检查是否选择了文件，并且选择的文件应该输入文件路径的，必须已输入文件路径
    BOOL bNvFileSelected = FALSE;
    BOOL bFlashSelected = FALSE;
    BOOL bFlashIDExist = FALSE;
    BOOL bNVIDExist = FALSE;
    
    BOOL bUDiskImgSelected = FALSE;
    BOOL bUDiskImgIDExist = FALSE;
    CString strUDiskImgFile;
    
    BOOL bDspImgSelected = FALSE;
    BOOL bDspImgIDExist = FALSE;
    CString strDspImgFile;
    
    int nItemCount = m_lstProductInfo.GetItemCount();
    if ( nItemCount == 0 )
    {
        AfxMessageBox( _T( "[DL3023]Please select files to download!" ) );
        return FALSE;
    }
    
    CString strChipPrex;
    BOOL bMultiFiles = FALSE;
    int nStartFile = 1;
    if ( m_pCurProductInfo->dwFlashType == 1 )
    {
        nStartFile = 2;
    }
    if ( m_pCurProductInfo->tChips.bEnable && m_pCurProductInfo->tChips.dwCount != 0 )
    {
        bMultiFiles = TRUE;
        // get chip prefix
        for ( int j = nStartFile; j < m_lstProductInfo.GetItemCount(); j++ )
        {
            if ( !m_lstProductInfo.GetCheck( j ) )
            {
                continue;
            }
            CString strFilePath = m_lstProductInfo.GetItemText( j, 1 );
            if ( strFilePath.IsEmpty() )
            {
                continue;
            }
            
            int nIndx = strFilePath.ReverseFind( '\\' );
            CString strFileName = strFilePath;
            if ( nIndx != -1 )
            {
                strFileName = strFilePath.Right( strFilePath.GetLength() - nIndx - 1 );
                strFilePath = strFilePath.Left( nIndx );
            }
            else
            {
                strFilePath.Empty();
            }
            
            CString strFileTitle = strFileName;
            // Chip category
            for ( UINT i = 0; i < m_pCurProductInfo->tChips.dwCount; i++ )
            {
                CString strChipName = m_pCurProductInfo->tChips.pChips[i].szName;
                strChipName += _T( "_" );
                if ( strFileName.Left( strChipName.GetLength() ).CompareNoCase( strChipName ) == 0 )
                {
                    strFileTitle = strFileName.Right( strFileName.GetLength() - strChipName.GetLength() );
                    strChipPrex = strChipName;
                    strChipPrex.MakeUpper();
                    break;
                }
            }
            break;
        }
        
        if ( strChipPrex.IsEmpty() )
        {
            CString strTmp = _T( "" );
            for ( UINT j = 0; j < m_pCurProductInfo->tChips.dwCount; j++ )
            {
                strTmp += m_pCurProductInfo->tChips.pChips[j].szName;
                strTmp += _T( "\n" );
            }
            CString strErr;
            strErr.Format( _T( "There are no download files matching with chip name prefix:\n%s." ), strTmp.operator LPCWSTR() );
            AfxMessageBox( strErr );
            return FALSE;
        }
    }
    
    CString strTmp = _T( "" );
    CString strTmp2 = _T( "" );
    int i = 0;
    int nRealDownFile = 0;
    int nAllowOmit = 0;
    for ( i = 0; i < nItemCount; i++ )
    {
        CString strIDAlias = m_lstProductInfo.GetItemText( i, 0 );
        int index = 0;
        
        for ( int j = 0; j < ( int )m_pCurProductInfo->dwFileCount; j++ )
        {
            if ( m_pCurProductInfo->pFileInfoArr[j].szIDAlias == strIDAlias )
            {
                index = j;
                break;
            }
        }
        
        CString strFileID = m_pCurProductInfo->pFileInfoArr[index].szID;
        CString strFileIDUpper = strFileID;
        strFileIDUpper.MakeUpper();
        if ( strFileID.Find( _T( "FLASH" ) ) == 0 )
        {
            bFlashIDExist = TRUE;
        }
        else if ( strFileIDUpper.Find( _T( "NV" ) ) == 0 )
        {
            bNVIDExist = TRUE;
        }
        else if ( strFileID.CompareNoCase( _T( "UDISK_IMG" ) ) == 0 )
        {
            bUDiskImgIDExist = TRUE;
        }
        else if ( strFileID.CompareNoCase( _T( "DSPCode" ) ) == 0 )
        {
            bDspImgIDExist = TRUE;
        }
        
        DWORD dwItemData = m_lstProductInfo.GetItemData( i );
        if ( m_lstProductInfo.GetCheck( i ) )
        {
            if ( LOWORD( dwItemData ) != 0 )
            {
                CString strDLFile = m_lstProductInfo.GetItemText( i, 1 );
                
                if ( !CheckFileValidate( strDLFile ) )
                {
                    strTmp += strFileID;
                    strTmp += _T( ": " );
                    strTmp += strDLFile;
                    strTmp += _T( "\n" );
                }
                if ( bMultiFiles && i >= nStartFile && !CheckFileChipPrefix( strDLFile, strChipPrex ) )
                {
                    strTmp2 += strFileID;
                    strTmp2 += _T( ": " );
                    strTmp2 += strDLFile;
                    strTmp2 += _T( "\n" );
                }
                
                //检查是否选择了NV文件
                if ( strFileIDUpper.Find( _T( "NV" ) ) == 0 )
                {
                    bNvFileSelected = TRUE;
                }
                else if ( strFileID.CompareNoCase( _T( "UDISK_IMG" ) ) == 0 )
                {
                    bUDiskImgSelected = TRUE;
                    strUDiskImgFile = m_lstProductInfo.GetItemText( i, 1 );
                }
                else if ( strFileID.CompareNoCase( _T( "DSPCode" ) ) == 0 )
                {
                    bDspImgSelected = TRUE;
                    strDspImgFile = m_lstProductInfo.GetItemText( i, 1 );
                }
            }
            else
            {
                //检查是否选择了擦除Flash
                if ( strFileID.Find( _T( "FLASH" ) ) == 0 )
                {
                    bFlashSelected = TRUE;
                }
            }
            
            nRealDownFile++;
        }
        else
        {
            if ( DI_IS_GRAY( dwItemData ) || DI_IS_OMIT( dwItemData ) )
            {
                nAllowOmit++;
            }
        }
        
    }
    
    if ( !strTmp.IsEmpty() )
    {
        CString strError;
        strError.Format( IDS_ERR_FILES_NOT_EXIST, strTmp.operator LPCTSTR() );
        AfxMessageBox( strError );
        return FALSE;
    }
    
    if ( bMultiFiles && !strTmp2.IsEmpty() )
    {
        CString strError;
        strError.Format( _T( "Following files not match the chip prefix \"%s\":\n%s" ), strChipPrex.operator LPCTSTR(), strTmp2.operator LPCTSTR() );
        AfxMessageBox( strError );
        return FALSE;
    }
    
    CStringArray agMultiFiles;
    int nMultiFiles = 0;
    CString strChipPreErr;
    if ( bMultiFiles )
    {
        nMultiFiles = GetMultiFiles( agMultiFiles, strChipPreErr );
        if ( !strChipPreErr.IsEmpty() )
        {
            CString strError;
            strError.Format( _T( "[DL3134]Follow files not found:\n%s" ), strChipPreErr.operator LPCWSTR() );
            AfxMessageBox( strError );
            return FALSE;
        }
    }
    
    CStringArray agPageBlockFiles;
    if ( IsMapPBFileBuf() )
    {
        if ( GetPageBlockFiles( agPageBlockFiles ) == -1 )
        {
            return FALSE;
        }
    }
    
    BOOL bCaseOK = TRUE;
    
    CString strWarning = _T( "There are warnings:\n" );
    
    
    CStringArray agMultiNVFiles;
    if ( m_pCurProductInfo->tRfChips.bEnable && m_pCurProductInfo->tRfChips.dwCount != 0 )
    {
        CString strTmpWarning;
        if ( GetMultiNVFiles( agMultiNVFiles, strTmpWarning ) == -1 )
        {
            return FALSE;
        }
        if ( !strTmpWarning.IsEmpty() )
        {
            bCaseOK = FALSE;
            strWarning += strTmpWarning;
        }
    }
    
    BOOL bAllFileDown = TRUE;
    if ( nAllowOmit != ( nItemCount - nRealDownFile ) )
    {
        bAllFileDown = FALSE;
    }
    
    //检查NandFlash分区策略
    BOOL bNand = m_pCurProductInfo->dwFlashType;
    
    int nRepStrategy = m_pOptionsPage->GetGUINandRepartStrategy();
    
    BOOL bRepartition = FALSE;
    if ( nRepStrategy == REPAR_STRATEGY_DO_ALWAYS || nRepStrategy == REPAR_STRATEGY_DO )
    {
        bRepartition = TRUE;
    }
    
    
    //not select flash
    if ( bFlashIDExist && !bFlashSelected )
    {
        bCaseOK = FALSE;
        strWarning += _T( "Not selected FLASH to earase flash;\n" );
    }
    
    //not select NV
    if ( bNVIDExist && !bNvFileSelected )
    {
        bCaseOK = FALSE;
        strWarning += _T( "Not selected NV file;\n" );
    }
    
    //检查选择了备份NV
    m_pCaliPage->m_lstBackup.Update();
    BOOL bBackupNV = TRUE;
    bBackupNV = m_pCaliPage->m_lstBackup.m_bBackNV;
    //如果选择了NV文件，但没有选择备份任何NV项
    /*if(bNvFileSelected)
    {
        CString strTmpNotChkItemName = m_pCaliPage->m_lstBackup.GetTmpNotChkItemName();
        if(!bBackupNV)
        {
            bCaseOK = FALSE;
            strWarning += _T("Not selected any NV item to backup;\n");
        }
        else if(!strTmpNotChkItemName.IsEmpty())
        {
            bCaseOK = FALSE;
            CString strWarn;
            strWarn.Format(_T("Not selected \"%s\" to backup;\n"),strTmpNotChkItemName.operator LPCTSTR());
            strWarning += strWarn;
        }
    }*/
    
    //如果没有选择所有文件
    if ( !bAllFileDown )
    {
        bCaseOK = FALSE;
        strWarning += _T( "[DL3024]Not selected all files;\n" );
    }
    
    if ( bNand && bRepartition && !bAllFileDown )
    {
        bCaseOK = FALSE;
        strWarning += _T( "[DL3024]Selected repartition,but not select all files to download;\n" );
    }
    
    if ( bNand && bRepartition && !bBackupNV && bNvFileSelected )
    {
        bCaseOK = FALSE;
        strWarning += _T( "[DL3024]Selected repartition,but not any NV item to backup;\n" );
    }
    
    if ( bNand && bRepartition && bBackupNV && !bNvFileSelected && bNVIDExist )
    {
        bCaseOK = FALSE;
        strWarning += _T( "[DL3024]Selected backup nv items,but not input the NV file;\n" );
    }
    
    if ( !bCaseOK )
    {
        strWarning += _T( "\nIf continue?" );
        if ( AfxMessageBox( strWarning.operator LPCTSTR(), MB_YESNO | MB_ICONWARNING ) == IDNO )
        {
            return FALSE;
        }
    }
    
    CString strPrdName = m_strCurProduct;
    CString strPrdAlias = m_strCurProduct;
    CString strPacPath = _T( "" );
    CString strPrdVersion = m_strPrdVersion;
    
    BOOL    bExistFlagFile = FALSE;
    if ( strPrdName.Find( _T( "PAC_" ) ) == 0 )
    {
        strPrdName.Delete( 0, 4 );
    }
    
    CDlgPacketSetting dlg;
    dlg.m_strPrdName = strPrdName;
    dlg.m_strPrdAlias = strPrdName;
    
    if ( dlg.DoModal() == IDCANCEL )
    {
        return FALSE;
    }
    
    PUMP_MESSAGES();
    
    strPrdName = dlg.m_strPrdName;
    strPrdAlias = dlg.m_strPrdAlias;
    strPacPath = dlg.m_strPath;
    strPrdVersion = dlg.m_strVersion;
    
    int nFileNum = nItemCount;
    
    CStringArray agUDiskFiles;
    int nUDiskFiles = 0;
    if ( bUDiskImgSelected )
    {
        nUDiskFiles = GetUDiskFiles( strUDiskImgFile, agUDiskFiles );
        nFileNum += nUDiskFiles;
    }
    
    CStringArray agDspFiles;
    int nDspFiles = 0;
    if ( bDspImgSelected )
    {
        nDspFiles = GetDspFiles( strDspImgFile, agDspFiles );
        if ( nDspFiles == -1 )
        {
            return FALSE;
        }
        
        nFileNum += nDspFiles;
    }
    
    nFileNum += nMultiFiles;
    nFileNum += agPageBlockFiles.GetSize();
    nFileNum += agMultiNVFiles.GetSize();
    nFileNum += m_lstMergeNV.GetItemCount();
    
    
    int nCurIndex = 0;
    LPFILE_INFO lpAllFile = new FILE_INFO[nFileNum];
    LPFILE_INFO lpOneFile = NULL;
    for ( i = 0; i < nItemCount; i++ )
    {
        lpOneFile = lpAllFile + i;
        lpOneFile->nCheckFlag = m_lstProductInfo.GetCheck( i );
        CString strFileName = m_lstProductInfo.GetItemText( i, 1 );
        CString strIDAlias = m_lstProductInfo.GetItemText( i, 0 );
        int index = 0;
        
        for ( int j = 0; j < ( int )m_pCurProductInfo->dwFileCount; j++ )
        {
            if ( m_pCurProductInfo->pFileInfoArr[j].szIDAlias == strIDAlias )
            {
                index = j;
                break;
            }
        }
        
        CString strFileID = m_pCurProductInfo->pFileInfoArr[index].szID;
        _tcscpy( lpOneFile->szFileID, strFileID );
        if ( lpOneFile->nCheckFlag )
        {
            _tcscpy( lpOneFile->szFileName, strFileName );
            if ( IsLoadFromPac( lpOneFile->szFileName ) )
            {
                DATA_INFO_T dataInfo = GetDataInfo( lpOneFile->szFileName );
                bExistFlagFile = TRUE;
                lpOneFile->dwHiDataOffset = ( dataInfo.llOffset >> 32 ) & 0xFFFFFFFF;
                lpOneFile->dwLoDataOffset = dataInfo.llOffset & 0xFFFFFFFF;
                lpOneFile->dwHiFileSize = ( dataInfo.llSize >> 32 ) & 0xFFFFFFFF;
                lpOneFile->dwLoFileSize = dataInfo.llSize & 0xFFFFFFFF;
            }
        }
        DWORD dwItemData = m_lstProductInfo.GetItemData( i );
        lpOneFile->nFileFlag = LOWORD( dwItemData );
        if ( DI_IS_GRAY( dwItemData ) || DI_IS_OMIT( dwItemData ) )
        {
            lpOneFile->dwCanOmitFlag = 1;
        }
        
        lpOneFile->dwAddrNum = m_pCurProductInfo->pFileInfoArr[index].dwBlockCount;
        for ( UINT k = 0; k < lpOneFile->dwAddrNum; k++ )
        {
            lpOneFile->dwAddr[k] = ( DWORD )( m_pCurProductInfo->pFileInfoArr[index].arrBlock[k].llBase );
        }
        
        nCurIndex++;
        
    }
    
    for ( i = 0; i < m_lstMergeNV.GetItemCount(); i++ )
    {
        lpOneFile = lpAllFile + nCurIndex;
        lpOneFile->nCheckFlag = m_lstMergeNV.GetCheck( i );
        CString strFileName = m_lstMergeNV.GetItemText( i, 1 );
        CString strIDAlias = m_lstMergeNV.GetItemText( i, 0 );
        int index = 0;
        
        for ( int j = 0; j < ( int )m_pCurProductInfo->dwFileCount; j++ )
        {
            if ( m_pCurProductInfo->pFileInfoArr[j].szIDAlias == strIDAlias )
            {
                index = j;
                break;
            }
        }
        
        CString strFileID = m_pCurProductInfo->pFileInfoArr[index].szID;
        _tcscpy( lpOneFile->szFileID, strFileID );
        if ( lpOneFile->nCheckFlag )
        {
            _tcscpy( lpOneFile->szFileName, strFileName );
            if ( IsLoadFromPac( lpOneFile->szFileName ) )
            {
                DATA_INFO_T dataInfo = GetDataInfo( lpOneFile->szFileName );
                bExistFlagFile = TRUE;
                lpOneFile->dwHiDataOffset = ( dataInfo.llOffset >> 32 ) & 0xFFFFFFFF;
                lpOneFile->dwLoDataOffset = dataInfo.llOffset & 0xFFFFFFFF;
                lpOneFile->dwHiFileSize = ( dataInfo.llSize >> 32 ) & 0xFFFFFFFF;
                lpOneFile->dwLoFileSize = dataInfo.llSize & 0xFFFFFFFF;
            }
        }
        DWORD dwItemData = m_lstMergeNV.GetItemData( i );
        lpOneFile->nFileFlag = LOWORD( dwItemData );
        if ( DI_IS_GRAY( dwItemData ) || DI_IS_OMIT( dwItemData ) )
        {
            lpOneFile->dwCanOmitFlag = 1;
        }
        
        lpOneFile->dwAddrNum = m_pCurProductInfo->pFileInfoArr[index].dwBlockCount;
        for ( UINT k = 0; k < lpOneFile->dwAddrNum; k++ )
        {
            lpOneFile->dwAddr[k] = ( DWORD )( m_pCurProductInfo->pFileInfoArr[index].arrBlock[k].llBase );
        }
        
        nCurIndex++;
        
    }
    
    for ( i = 0; i < nMultiFiles; i++ )
    {
        lpOneFile = lpAllFile + nCurIndex;
        lpOneFile->nCheckFlag = 1;
        _tcscpy( lpOneFile->szFileName, agMultiFiles.GetAt( i ) );
        if ( IsLoadFromPac( lpOneFile->szFileName ) )
        {
            DATA_INFO_T dataInfo = GetDataInfo( lpOneFile->szFileName );
            bExistFlagFile = TRUE;
            lpOneFile->dwHiDataOffset = ( dataInfo.llOffset >> 32 ) & 0xFFFFFFFF;
            lpOneFile->dwLoDataOffset = dataInfo.llOffset & 0xFFFFFFFF;
            lpOneFile->dwHiFileSize = ( dataInfo.llSize >> 32 ) & 0xFFFFFFFF;
            lpOneFile->dwLoFileSize = dataInfo.llSize & 0xFFFFFFFF;
        }
        nCurIndex++;
    }
    
    for ( i = 0; i < nUDiskFiles; i++ )
    {
        lpOneFile = lpAllFile + nCurIndex;
        lpOneFile->nCheckFlag = 1;
        _tcscpy( lpOneFile->szFileName, agUDiskFiles.GetAt( i ) );
        if ( IsLoadFromPac( lpOneFile->szFileName ) )
        {
            DATA_INFO_T dataInfo = GetDataInfo( lpOneFile->szFileName );
            bExistFlagFile = TRUE;
            lpOneFile->dwHiDataOffset = ( dataInfo.llOffset >> 32 ) & 0xFFFFFFFF;
            lpOneFile->dwLoDataOffset = dataInfo.llOffset & 0xFFFFFFFF;
            lpOneFile->dwHiFileSize = ( dataInfo.llSize >> 32 ) & 0xFFFFFFFF;
            lpOneFile->dwLoFileSize = dataInfo.llSize & 0xFFFFFFFF;
        }
        nCurIndex++;
    }
    for ( i = 0; i < nDspFiles; i++ )
    {
        lpOneFile = lpAllFile + nCurIndex;
        lpOneFile->nCheckFlag = 1;
        _tcscpy( lpOneFile->szFileName, agDspFiles.GetAt( i ) );
        if ( IsLoadFromPac( lpOneFile->szFileName ) )
        {
            DATA_INFO_T dataInfo = GetDataInfo( lpOneFile->szFileName );
            bExistFlagFile = TRUE;
            lpOneFile->dwHiDataOffset = ( dataInfo.llOffset >> 32 ) & 0xFFFFFFFF;
            lpOneFile->dwLoDataOffset = dataInfo.llOffset & 0xFFFFFFFF;
            lpOneFile->dwHiFileSize = ( dataInfo.llSize >> 32 ) & 0xFFFFFFFF;
            lpOneFile->dwLoFileSize = dataInfo.llSize & 0xFFFFFFFF;
        }
        nCurIndex++;
    }
    
    for ( i = 0; i < agPageBlockFiles.GetSize(); i++ )
    {
        lpOneFile = lpAllFile + nCurIndex;
        lpOneFile->nCheckFlag = 1;
        _tcscpy( lpOneFile->szFileName, agPageBlockFiles.GetAt( i ) );
        
        if ( IsLoadFromPac( lpOneFile->szFileName ) )
        {
            DATA_INFO_T dataInfo = GetDataInfo( lpOneFile->szFileName );
            bExistFlagFile = TRUE;
            lpOneFile->dwHiDataOffset = ( dataInfo.llOffset >> 32 ) & 0xFFFFFFFF;
            lpOneFile->dwLoDataOffset = dataInfo.llOffset & 0xFFFFFFFF;
            lpOneFile->dwHiFileSize = ( dataInfo.llSize >> 32 ) & 0xFFFFFFFF;
            lpOneFile->dwLoFileSize = dataInfo.llSize & 0xFFFFFFFF;
        }
        nCurIndex++;
    }
    
    for ( i = 0; i < agMultiNVFiles.GetSize(); i++ )
    {
        lpOneFile = lpAllFile + nCurIndex;
        lpOneFile->nCheckFlag = 1;
        _tcscpy( lpOneFile->szFileName, agMultiNVFiles.GetAt( i ) );
        
        if ( IsLoadFromPac( lpOneFile->szFileName ) )
        {
            DATA_INFO_T dataInfo = GetDataInfo( lpOneFile->szFileName );
            bExistFlagFile = TRUE;
            lpOneFile->dwHiDataOffset = ( dataInfo.llOffset >> 32 ) & 0xFFFFFFFF;
            lpOneFile->dwLoDataOffset = dataInfo.llOffset & 0xFFFFFFFF;
            lpOneFile->dwHiFileSize = ( dataInfo.llSize >> 32 ) & 0xFFFFFFFF;
            lpOneFile->dwLoFileSize = dataInfo.llSize & 0xFFFFFFFF;
        }
        nCurIndex++;
    }
    CString strCurPactetPath = g_theApp.m_pCfgObject->GetString( CFGTYPE_DL, _T( "Download_Packet" ), _T( "packet" ), _T( "" ) );
    if ( bExistFlagFile && 0 == strCurPactetPath.CompareNoCase( strPacPath ) )
    {
        CString strErr;
        strErr.Format( _T( "[DL3025]Don't allow to replace the original pac file [%s]" ), strCurPactetPath.operator LPCWSTR() );
        AfxMessageBox( strErr );
        SAFE_DELETE_ARRAY( lpAllFile );
        return FALSE;
    }
    
    BIN_PACKET_HEADER tBinPacketHeader;
    _tcsncpy_s( tBinPacketHeader.szPrdName, 256, strPrdName, strPrdName.GetLength() );
    _tcsncpy_s( tBinPacketHeader.szPrdVersion, 256, strPrdVersion, strPrdVersion.GetLength() );
    
    strPrdAlias.TrimLeft();
    strPrdAlias.TrimRight();
    if ( !strPrdAlias.IsEmpty() )
    {
        _tcscpy( tBinPacketHeader.szPrdAlias, strPrdAlias );
    }
    
    tBinPacketHeader.nFileCount = nFileNum;
    tBinPacketHeader.dwFlashType = m_pCurProductInfo->dwFlashType;
    tBinPacketHeader.dwMode = m_pCurProductInfo->dwMode;
    tBinPacketHeader.dwNandStrategy = m_pOptionsPage->GetGUINandRepartStrategy();
    tBinPacketHeader.dwNandPageType = m_pOptionsPage->GetGUIFlashPageType();
    BOOL bBackupNv = m_pCaliPage->m_lstBackup.m_bBackNV;
    tBinPacketHeader.dwIsNvBackup = ( DWORD )bBackupNv;
    tBinPacketHeader.dwOmaDmProductFlag = m_pCurProductInfo->dwOmaDMFlag;
    tBinPacketHeader.dwIsOmaDM = m_bOmaDM;
    tBinPacketHeader.dwIsPreload = m_bPreload;
    
    SPRESULT spRlt = SP_OK;
    CMainFrame* pMainFrame = ( CMainFrame* )AfxGetMainWnd();
    DLFW_DOPACKET_PARAMS dlfwDoPacket;
    dlfwDoPacket.lpszPacketPath = strPacPath.operator LPCWSTR();
    dlfwDoPacket.lpszProduct = m_strCurProduct.operator LPCWSTR();
    dlfwDoPacket.lpPcaketHeader = &tBinPacketHeader;
    dlfwDoPacket.lpAllFile = lpAllFile;
    spRlt = pMainFrame->m_pFrameHelper->DoPacket( &dlfwDoPacket );
    SAFE_DELETE_ARRAY( lpAllFile );
    if ( SP_OK == spRlt )
    {
        AfxMessageBox( _T( "Do Packet Succeed." ), MB_OK | MB_ICONEXCLAMATION );
    }
    else
    {
        AfxMessageBox( _T( "Do Packet Failed." ), MB_OK | MB_ICONSTOP );
    }
    return SP_OK == spRlt ? TRUE : FALSE;
}

int CMainPage::GetUDiskFiles( LPCTSTR lpszBaseUDiskFile, CStringArray& agFiles )
{
    agFiles.RemoveAll();
    
    CString strUDiskImgFile = lpszBaseUDiskFile;
    int nFileNameLen = UDSIK_IMG_NAME_LEN;
    CString strDir = strUDiskImgFile;
    
    int nFind = strDir.ReverseFind( _T( '\\' ) );
    strDir = strDir.Left( nFind );
    
    CString strFind = strDir + _T( "\\udisk_img_*.bin*" );
    
    WIN32_FIND_DATA wfd = { 0 };
    HANDLE hFind = ::FindFirstFile( strFind, &wfd );
    BOOL bFind = TRUE;
    while ( INVALID_HANDLE_VALUE != hFind && bFind )
    {
        CString strExt;
        CStringArray agFilePathInfo;
        g_theApp.m_fnOperationHelper.GetFilePathInfo( wfd.cFileName, agFilePathInfo );
        strExt = agFilePathInfo.GetAt( 2 );
        if ( IsLoadFromPac( wfd.cFileName ) )
        {
            CStringArray agTmpFilePathInfo;
            g_theApp.m_fnOperationHelper.GetFilePathInfo( agFilePathInfo.GetAt( 1 ), agTmpFilePathInfo );
            strExt = agTmpFilePathInfo.GetAt( 2 ) + agFilePathInfo.GetAt( 2 );
            nFileNameLen += MAX_FLAG_LEN;
        }
        CString strFileName = ( ( CString )( wfd.cFileName ) ).Left( ( ( CString )( wfd.cFileName ) ).GetLength() - strExt.GetLength() );
        if ( strFileName.GetLength() == nFileNameLen )
        {
            CString strFile;
            strFile.Format( _T( "%s\\%s" ), strDir.operator LPCTSTR(), wfd.cFileName );
            if ( strFile.CompareNoCase( strUDiskImgFile ) != 0 )
            {
                agFiles.Add( strFile );
            }
        }
        nFileNameLen = UDSIK_IMG_NAME_LEN;
        bFind = ::FindNextFile( hFind, &wfd );
    }
    if ( INVALID_HANDLE_VALUE != hFind )
    {
        ::FindClose( hFind );
    }
    
    return agFiles.GetSize();
}

BOOL CMainPage::CheckFileValidate( LPCTSTR lpszFile )
{
    if ( lpszFile == NULL )
    {
        return FALSE;
    }
    
    BOOL bOK = FALSE;
    
    CFileFind finder;
    if ( finder.FindFile( lpszFile ) )
    {
        finder.FindNextFile();
        if ( finder.GetLength() != 0 || IsLoadFromPac( lpszFile ) )
        {
            bOK = TRUE;
        }
    }
    finder.Close();
    return bOK;
}

int  CMainPage::GetMultiFiles( CStringArray& agFiles, CString& strErr )
{
    agFiles.RemoveAll();
    strErr.Empty();
    
    int nStartFile = 1;
    if ( m_pCurProductInfo->dwFlashType == 1 )
    {
        nStartFile = 2;
    }
    
    CFileFind finder;
    // File number
    for ( int j = nStartFile; j < m_lstProductInfo.GetItemCount(); j++ )
    {
        if ( !m_lstProductInfo.GetCheck( j ) )
        {
            continue;
        }
        CString strFilePath = m_lstProductInfo.GetItemText( j, 1 );
        if ( strFilePath.IsEmpty() )
        {
            continue;
        }
        
        int nIndx = strFilePath.ReverseFind( '\\' );
        CString strFileName = strFilePath;
        if ( nIndx != -1 )
        {
            strFileName = strFilePath.Right( strFilePath.GetLength() - nIndx - 1 );
            strFilePath = strFilePath.Left( nIndx );
        }
        else
        {
            strFilePath.Empty();
        }
        
        CString strFileTitle = strFileName;
        
        UINT nCurChip = ( UINT )( -1 );
        // Chip category
        for ( UINT i = 0; i < m_pCurProductInfo->tChips.dwCount; i++ )
        {
            CString strChipName = m_pCurProductInfo->tChips.pChips[i].szName;
            strChipName += _T( "_" );
            if ( strFileName.Left( strChipName.GetLength() ).CompareNoCase( strChipName ) == 0 )
            {
                strFileTitle = strFileName.Right( strFileName.GetLength() - strChipName.GetLength() );
                nCurChip = i;
                break;
            }
        }
        
        for ( UINT k = 0; k < m_pCurProductInfo->tChips.dwCount; k++ )
        {
            CString strChipName = m_pCurProductInfo->tChips.pChips[k].szName;
            if ( k != nCurChip )
            {
                CString strExtFile;
                strExtFile.Format( _T( "%s\\%s_%s" ), strFilePath.operator LPCWSTR(), strChipName.operator LPCWSTR(), strFileTitle.operator LPCWSTR() );
                if ( finder.FindFile( strExtFile ) )
                {
                    agFiles.Add( strExtFile );
                }
                else
                {
                    strErr += strExtFile;
                    strErr += _T( "\n" );
                }
            }
        }
    }
    
    return agFiles.GetSize();
}

BOOL CMainPage::IsMapPBFileBuf()
{
    int nCount = 0;
    if ( m_pCurProductInfo == NULL )
    {
        return FALSE;
    }
    
    DWORD nFileNum = m_pCurProductInfo->dwFileCount;
    
    for ( UINT i = 0; i < nFileNum; i++ )
    {
        CString strID = m_pCurProductInfo->pFileInfoArr[i].szID;
        strID.MakeUpper();
        BOOL bCheck = FALSE;
        CString strFile;
        m_mapDLState.Lookup( strID, bCheck );
        m_mapDLFiles.Lookup( strID, strFile );
        
        if ( bCheck && m_pCurProductInfo->pFileInfoArr[i].isSelByFlashInfo == 1 )
        {
            nCount++;
        }
    }
    if ( nCount > 0 )
    {
        return TRUE;
    }
    return FALSE;
}

int CMainPage::GetMultiNVFiles( CStringArray& agFiles, CString& strWarning )
{
    agFiles.RemoveAll();
    PFILE_INFO_T pFileInfo = m_pCurProductInfo->pFileInfoArr;
    int nFileCount = ( int )m_pCurProductInfo->dwFileCount;
    std::map<std::pair<CString, CString>, CString> mapCheckNVInfo;
    CMap<CString, LPCTSTR, CString, LPCTSTR> mapMultiNVInfo;
    CString strErr = _T( "" );
    CStringArray agChipName;
    CUIntArray   agChipID;
    for ( int i = 0; i < ( int )m_pCurProductInfo->tRfChips.dwCount; i++ )
    {
        agChipName.Add( m_pCurProductInfo->tRfChips.pChips[i].szName );
        agChipID.Add( m_pCurProductInfo->tRfChips.pChips[i].dwID );
    }
    for ( int i = 0; i < nFileCount; i++ )
    {
        CString strFileType = pFileInfo[i].szType;
        CString strFileID = pFileInfo[i].szID;
        CString strFilePath = m_lstProductInfo.GetItemText( i, 1 );
        if (
            strFilePath.IsEmpty() ||
            !m_lstProductInfo.GetCheck( i ) ||
            pFileInfo[i].isSelByRf != 1
        )
        {
            continue;
        }
        if ( _tcsnicmp( strFileID.operator LPCTSTR(), _T( "NV" ), 2 ) )
        {
            strErr.Format( _T( "This version just support multi nv,But this is %s file." ), strFileID.operator LPCWSTR() );
            MessageBox( strErr );
            return -1;
        }
        
        CString strRFChips;
        CStringArray agFilePathInfo;
        g_theApp.m_fnOperationHelper.GetFilePathInfo( strFilePath, agFilePathInfo );
        CString strFileName = agFilePathInfo[1];
        
        CString strFilePre;     //prefix_RFNAME_*nvitem.bin ->prefix
        CString strCurrentChip; //prefix_RFNAME_*nvitem.bin ->RFNAME
        
        for ( int j = 0; j < ( int )m_pCurProductInfo->tRfChips.dwCount; j++ )
        {
            CString strChipName;
            strChipName.Format( _T( "_%s_" ), m_pCurProductInfo->tRfChips.pChips[j].szName );
            
            int nIndex = strFileName.Find( strChipName );
            if ( nIndex != -1 && -1 != strFileName.Find( g_sz_NVITEM ) )
            {
                strFilePre = strFileName.Left( nIndex );
                strCurrentChip = m_pCurProductInfo->tRfChips.pChips[j].szName;
                break;
            }
        }
        if ( strFilePre.IsEmpty() )
        {
            CString strTmp = _T( "" );
            for ( int j = 0; j < agChipName.GetSize(); j++ )
            {
                strTmp += agChipName.GetAt( j );
                strTmp += _T( "\n" );
            }
            strErr.Format( _T( "The nv file [%s] is invalid! \nFile name must be match with \"prefix_rfname_*nvitem.bin\"\n%s" ), strFilePath.operator LPCWSTR(), strTmp.operator LPCWSTR() );
            MessageBox( strErr );
            agFiles.RemoveAll();
            return -1;
        }
        
        strRFChips = strCurrentChip;
        
        CString strExt = agFilePathInfo[2];
        CString strFind;
        strFind.Format( _T( "%s\\%s_*_*%s%s" ), agFilePathInfo[0].operator LPCWSTR(), strFilePre.operator LPCWSTR(), g_sz_NVITEM, strExt.operator LPCWSTR() );
        
        WIN32_FIND_DATA wfd = { 0 };
        HANDLE hFind = ::FindFirstFile( strFind, &wfd );
        BOOL bFind = TRUE;
        while ( INVALID_HANDLE_VALUE != hFind && bFind )
        {
            //prefix_RFNAME_*nvitem.bin ->RFNAME
            CString strRFName = ( ( CString )( wfd.cFileName ) ).Left( ( ( CString )( wfd.cFileName ) ).GetLength() - strExt.GetLength() - _tcslen( g_sz_NVITEM ) );
            strRFName = strRFName.Right( strRFName.GetLength() - strFilePre.GetLength() - 1 ); //-1 : _
            if ( -1 == strRFName.Find( _T( "_" ) ) )
            {
                continue;
            }
            strRFName = strRFName.Left( strRFName.Find( _T( "_" ) ) );
            
            CString strFile;
            strFile.Format( _T( "%s\\%s" ), agFilePathInfo[0].operator LPCTSTR(), wfd.cFileName );
            
            if ( strFile.CompareNoCase( strFilePath ) != 0 )
            {
                for ( int n = 0; n < ( int )m_pCurProductInfo->tRfChips.dwCount; ++n )
                {
                    if ( 0 == agChipName.GetAt( n ).CompareNoCase( strRFName.operator LPCTSTR() ) )
                    {
                        agFiles.Add( strFile );
                        strRFChips += _T( "," );
                        strRFChips += strRFName;
                        
                        std::map<std::pair<CString, CString>, CString> ::iterator it = mapCheckNVInfo.find( std::make_pair( strRFName, strFileID ) );
                        if ( mapCheckNVInfo.end() != it )
                        {
                            strErr.Format( _T( "%s can't have multiple nv file." ), strRFName.operator LPCTSTR() );
                            MessageBox( strErr );
                            agFiles.RemoveAll();
                            ::FindClose( hFind );
                            return -1;
                        }
                        mapCheckNVInfo[std::make_pair( strRFName, strFileID )] = strFile;
                        break;
                    }
                }
                
            }
            bFind = ::FindNextFile( hFind, &wfd );
            
        }
        
        if ( INVALID_HANDLE_VALUE != hFind )
        {
            ::FindClose( hFind );
        }
        
        mapMultiNVInfo.SetAt( strFileID, strRFChips );
        
    }
    
    POSITION pos = mapMultiNVInfo.GetStartPosition();
    while ( NULL != pos )
    {
        CString strKey;
        CString strRFChips;
        mapMultiNVInfo.GetNextAssoc( pos, strKey, strRFChips );
        
        INT nCount = 0;
        CUtility utility;
        LPTSTR* lppString = utility.GetTokenString( strRFChips.operator LPCWSTR(), _T( "," ), nCount );
        if ( NULL == lppString || ( DWORD )nCount != m_pCurProductInfo->tRfChips.dwCount )
        {
            strWarning.Format( _T( "%s just select %d image files,It should be equal to %d.\n" ),
                               strKey.operator LPCWSTR(), nCount, m_pCurProductInfo->tRfChips.dwCount );
            //g_theApp.MessageBox(strErr.operator LPCTSTR());
            //agFiles.RemoveAll();
            //return -1;
        }
    }
    return agFiles.GetSize();
}

int CMainPage::GetPageBlockFiles( CStringArray& agFiles )
{
    agFiles.RemoveAll();
    
    PFILE_INFO_T pFileInfo = m_pCurProductInfo->pFileInfoArr;
    int nCount = ( int )m_pCurProductInfo->dwFileCount;
    CMap<CString, LPCTSTR, DWORD, DWORD> mapPBInfo;
    
    int i = 0;
    CString strErr = _T( "" );
    int nPBFileCount = 0;
    for ( i = 0; i < nCount; i++ )
    {
        CString strFileType = pFileInfo[i].szType;
        CString strFileID = pFileInfo[i].szID;
        CString strFilePath = m_lstProductInfo.GetItemText( i, 1 );
        if ( strFilePath.IsEmpty() ||
             !m_lstProductInfo.GetCheck( i ) ||
             pFileInfo[i].isSelByFlashInfo != 1 )
        {
            continue;
        }
        
        CStringArray agFilePathInfo;
        g_theApp.m_fnOperationHelper.GetFilePathInfo( strFilePath, agFilePathInfo );
        CString strFileName = agFilePathInfo[1];
        int  nIndex = strFileName.Find( _T( "_b" ) );
        if ( nIndex == -1 )
        {
            strErr.Format( _T( "[DL3028][B]The name of file [%s] is invalid!\nFile name must be \"xxx_bnk_pmk.yyy\", n and m is a number." ), strFilePath.operator LPCWSTR() );
            AfxMessageBox( strErr );
            agFiles.RemoveAll();
            return -1;
        }
        
        nPBFileCount++;
        
        CString strExt = agFilePathInfo[2];
        CString strFilePre = strFileName.Left( nIndex );
        CString strFind;
        strFind.Format( _T( "%s\\%s_b*%s" ), agFilePathInfo[0].operator LPCWSTR(), strFilePre.operator LPCWSTR(), strExt.operator LPCWSTR() );
        
        WIN32_FIND_DATA wfd = { 0 };
        HANDLE hFind = ::FindFirstFile( strFind, &wfd );
        BOOL bFind = TRUE;
        while ( INVALID_HANDLE_VALUE != hFind && bFind )
        {
            CString strPBInfo = ( ( CString )( wfd.cFileName ) ).Left( ( ( CString )( wfd.cFileName ) ).GetLength() - strExt.GetLength() );
            strPBInfo = strPBInfo.Right( strPBInfo.GetLength() - strFilePre.GetLength() );
            if ( IsLoadFromPac( wfd.cFileName ) ) //.flag file
            {
                strPBInfo = g_theApp.m_fnOperationHelper.GetStrFileTitle( strPBInfo.operator LPCTSTR() );
            }
            
            CString strFile;
            strFile.Format( _T( "%s\\%s" ), agFilePathInfo[0].operator LPCTSTR(), wfd.cFileName );
            
            if ( strFile.CompareNoCase( strFilePath ) != 0 )
            {
                agFiles.Add( strFile );
            }
            
            strPBInfo.MakeLower();
            
            bFind = ::FindNextFile( hFind, &wfd );
            
            DWORD dwPBCount = 0;
            
            if ( mapPBInfo.Lookup( strPBInfo, dwPBCount ) )
            {
                dwPBCount += 1;
            }
            else
            {
                dwPBCount = 1;
            }
            mapPBInfo.SetAt( strPBInfo, dwPBCount );
        }
        
        if ( INVALID_HANDLE_VALUE != hFind )
        {
            ::FindClose( hFind );
        }
    }
    
    POSITION pos = mapPBInfo.GetStartPosition();
    while ( NULL != pos )
    {
        CString strKey;
        DWORD dwPBCount = 0;
        
        mapPBInfo.GetNextAssoc( pos, strKey, dwPBCount );
        
        if ( dwPBCount != ( DWORD )nPBFileCount )
        {
            strErr.Format( _T( "Block-Page [%s] only have %d image files.\nIt must be equal to %d." ),
                           strKey.operator LPCWSTR(), dwPBCount, nPBFileCount );
            MessageBox( strErr.operator LPCTSTR() );
            agFiles.RemoveAll();
            return -1;
        }
    }
    return agFiles.GetSize();
}

BOOL CMainPage::CheckFileChipPrefix( LPCTSTR lpszFile, CString& strChipPre )
{
    if ( lpszFile == NULL )
    {
        return FALSE;
    }
    
    BOOL bOK = FALSE;
    CFileFind finder;
    
    if ( !strChipPre.IsEmpty() )
    {
        if ( finder.FindFile( lpszFile ) )
        {
            finder.FindNextFile();
            CString strFileTitle = finder.GetFileTitle();
            strFileTitle.MakeUpper();
            if ( strFileTitle.Find( strChipPre ) == 0 )
            {
                bOK = TRUE;
            }
        }
    }
    
    return bOK;
}

int  CMainPage::GetDspFiles( LPCTSTR lpszBaseDspFile, CStringArray& agFiles )
{
    agFiles.RemoveAll();
    if ( !m_bEnableChipDspMap )
    {
        // need not chip-dsp map
        return 0;
    }
    
    CStringArray arrKeyData;
    if ( !g_theApp.m_fnOperationHelper.GetSection( g_theApp.m_pCfgObject, CFGTYPE_BMFT, _T( "ChipDSPMap" ), arrKeyData ) ||
         arrKeyData.GetSize() == 0 )
    {
        AfxMessageBox( _T( "[PK3106][D]Not found ChipID-DSPName map setting in BMFileType.ini!" ) );
        return -1;
    }
    
    CString strErrMsg = _T( "" );
    
    CString strDspFile = lpszBaseDspFile;
    CString strDir = strDspFile;
    int nFind = strDir.ReverseFind( _T( '\\' ) );
    strDir = strDir.Left( nFind );
    CFileFind finder;
    for ( int i = 0; i < ( int )arrKeyData.GetSize(); i++ )
    {
        CString strFile;
        strFile.Format( _T( "%s\\%s" ), strDir.operator LPCTSTR(), arrKeyData[2 * i + 1].operator LPCTSTR() );
        
        if ( !finder.FindFile( strFile ) )
        {
            CString strFlagFile;
            strFlagFile = strFile + FLAG_FILE_SUFFIX;
            if ( !finder.FindFile( strFlagFile ) )
            {
                strErrMsg += strFile;
                strErrMsg += _T( "\r\n" );
                continue;
            }
            else
            {
                if ( strFlagFile.CompareNoCase( strDspFile ) != 0 )
                {
                    agFiles.Add( strFlagFile );
                }
                finder.Close();
            }
            
        }
        else
        {
            if ( strFile.CompareNoCase( strDspFile ) != 0 )
            {
                agFiles.Add( strFile );
            }
            finder.Close();
        }
    }
    if ( !strErrMsg.IsEmpty() )
    {
        strErrMsg.Insert( 0, _T( "[DL3131][B]Not found bellow DSP file:\r\n" ) );
        strErrMsg += _T( "\r\nPlease check BMFileType.ini, [ChipDspMap]!" );
        AfxMessageBox( strErrMsg );
        return -1;
    }
    return agFiles.GetSize();
}
