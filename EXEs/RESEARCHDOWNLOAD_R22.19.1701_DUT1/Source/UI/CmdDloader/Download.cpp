#include "StdAfx.h"
#include "Download.h"

extern CMD_PARAMS g_tCmdParams;

wstring GetAppPath( void )
{
    WCHAR szAppPath[MAX_PATH] = { 0 };
    GetModuleFileNameW( NULL, szAppPath, MAX_PATH );
    LPWSTR lpChar = wcsrchr( szAppPath, L'\\' );
    if ( NULL != lpChar )
    {
        *lpChar = L'\0';
    }
    
    return ( wstring )szAppPath + L"\\";
}

//////////////////////////////////////////////////////////////////////////
CDownload::CDownload( void )
{

}

CDownload::~CDownload( void )
{
}

SPRESULT CDownload::GetConfigObject()
{
    CHKRESULT( m_fnFrameworkHelper.GetProperty( DLFW_ATTR_CONFIG_OBJECT, 0, &m_pCfgObject ) );
    if ( NULL == m_pCfgObject )
    {
        return SP_E_FAIL;
    }
    return SP_OK;
}

SPRESULT CDownload::ReloadParameters()
{
    CHKRESULT( m_fnFrameworkHelper.ReloadSettings() );
    CHKRESULT( m_fnFrameworkHelper.SyncParameters() );
    return SP_OK;
}

SPRESULT CDownload::SetBarcode( int nPort )
{
    DLFW_ATTRDATA_BARCODE dlSN;
    if ( g_tCmdParams.bWriteSN1 )
    {
        dlSN.pszSN1 = g_tCmdParams.strSN1.c_str();
        dlSN.u8SN1Len = ( uint8 )g_tCmdParams.strSN1.length();
    }
    if ( g_tCmdParams.bWriteSN2 )
    {
        dlSN.pszSN2 = g_tCmdParams.strSN2.c_str();
        dlSN.u8SN2Len = ( uint8 )g_tCmdParams.strSN2.length();
    }
    return m_fnFrameworkHelper.TaskSetProperty( DLFW_ATTR_BARCODE_SN, nPort, &dlSN );
}

SPRESULT CDownload::ConfigLoadPacAfter( )
{
    if ( NULL == m_pCfgObject )
    {
        assert( 0 );
        return SP_E_POINTER;
    }
    if ( g_tCmdParams.bMutisoftware )
    {
        PROJECT_CONFIG tProjectCfg;
        tProjectCfg.nCheck = 1;
        wcscpy_s( tProjectCfg.szFileID, g_tCmdParams.strMutiFileID.c_str() );
        wcscpy_s( tProjectCfg.szProjectBase, g_tCmdParams.strMutiProjectBase.c_str() );
        wcscpy_s( tProjectCfg.szProjectID, g_tCmdParams.strMutiProjectID.c_str() );
        CHKRESULT( m_fnFrameworkHelper.SetProperty( DLFW_ATTR_PROJECT_CONFIG, 0, ( LPVOID )&tProjectCfg ) );
    }
    CHKRESULT( ReloadParameters() );
    return SP_OK;
}

SPRESULT CDownload::ConfigLoadPacBefore( )
{
    if ( NULL == m_pCfgObject )
    {
        assert( 0 );
        return SP_E_POINTER;
    }
    
    /// When the default configuration of PhaseCheck is 1 and the SN is not entered in the command line,
    /// AutoGenerateSN needs to be configured to 1 to ensure that DLFramework has an SN that can be used;
    int nNeedPhaseCheck = m_pCfgObject->GetInt( CFGTYPE_DL, _T( "SN" ), _T( "NeedPhaseCheck" ), 0 );
    if ( 1 == nNeedPhaseCheck && ( !g_tCmdParams.bWriteSN1 || !g_tCmdParams.bWriteSN2 ) )
    {
        CHKRESULT( m_pCfgObject->SetInt( CFGTYPE_DL, _T( "SN" ), _T( "AutoGenerateSN" ), 1 ) );
    }
    
    if ( g_tCmdParams.bWriteSN1 )
    {
        CHKRESULT( m_pCfgObject->SetInt( CFGTYPE_DL, _T( "SN" ), _T( "NeedPhaseCheck" ), 1 ) );
        CHKRESULT( m_pCfgObject->SetInt( CFGTYPE_DL, _T( "SN" ), _T( "AutoGenerateSN" ), 0 ) );
    }
    
    if ( g_tCmdParams.bWriteSN2 )
    {
        CHKRESULT( m_pCfgObject->SetInt( CFGTYPE_DL, _T( "SN" ), _T( "NeedPhaseCheck" ), 1 ) );
        CHKRESULT( m_pCfgObject->SetInt( CFGTYPE_DL, _T( "SN" ), _T( "AutoGenerateSN" ), 0 ) );
        CHKRESULT( m_pCfgObject->SetInt( CFGTYPE_DL, _T( "SN" ), _T( "WriteSN2Enable" ), 1 ) );
    }
    
    CHKRESULT( m_pCfgObject->SetString( CFGTYPE_DL, _T( "Settings" ), _T( "SkipFileID" ), g_tCmdParams.strSkipFileID.c_str() ) );
    CHKRESULT( m_pCfgObject->SetInt( CFGTYPE_DL, _T( "Serial port" ), _T( "Port" ), g_tCmdParams.nPort ) );
    CHKRESULT( m_pCfgObject->SetInt( CFGTYPE_DL, _T( "Serial port" ), _T( "Baud rate" ), g_tCmdParams.nBaudRate ) );
    
    if ( -1 != g_tCmdParams.nWriteEfuse )
    {
        CHKRESULT( m_pCfgObject->SetInt( CFGTYPE_DL, _T( "Options" ), _T( "WriteEfuse" ), g_tCmdParams.nWriteEfuse ) );
    }
    
    if ( -1 != g_tCmdParams.nWriteTimeStamp )
    {
        CHKRESULT( m_pCfgObject->SetInt( CFGTYPE_DL, _T( "Options" ), _T( "WriteTimestamp" ), g_tCmdParams.nWriteTimeStamp ) );
    }
    if ( -1 != g_tCmdParams.nSendFlag )
    {
        CHKRESULT( m_pCfgObject->SetInt( CFGTYPE_DL, _T( "Options" ), _T( "SendFlag" ), g_tCmdParams.nSendFlag ) );
    }
    if ( -1 != g_tCmdParams.nLoadPolicy )
    {
        CHKRESULT( m_pCfgObject->SetInt( CFGTYPE_BINP, _T( "Setting" ), _T( "LoadPolicy" ), g_tCmdParams.nLoadPolicy ) );
    }
    if ( -1 != g_tCmdParams.nPoweroff )
    {
        CHKRESULT( m_pCfgObject->SetInt( CFGTYPE_DL, _T( "Options" ), _T( "PowerOff" ), g_tCmdParams.nPoweroff ) );
    }
    if ( -1 != g_tCmdParams.nReset )
    {
        CHKRESULT( m_pCfgObject->SetInt( CFGTYPE_DL, _T( "Options" ), _T( "Reset" ), g_tCmdParams.nReset ) );
    }
    
    if ( g_tCmdParams.bReadFixNV )
    {
        CHKRESULT( m_pCfgObject->SetInt( CFGTYPE_DL, _T( "NV" ), _T( "EnableReadNV" ), 1 ) );
        TCHAR szkey[64] = { 0 };
        _stprintf_s( szkey, _T( "PORT%d" ), g_tCmdParams.nPort );
        CHKRESULT( m_pCfgObject->SetString( CFGTYPE_DL, _T( "NV" ), szkey, g_tCmdParams.strSaveFixNV.c_str() ) );
    }
    else
    {
        CHKRESULT( m_pCfgObject->SetInt( CFGTYPE_DL, _T( "NV" ), _T( "EnableReadNV" ), 0 ) );
    }
    
    CHKRESULT( ReloadParameters() );
    return SP_OK;
}

SPRESULT CDownload::Download( )
{
    SPRESULT spFramRlt = SP_OK;
    SPRESULT spTaskRlt = SP_OK;
    m_strErrMsg = _T( "" );
    string_t strAppPath = GetAppPath();
    wstring strIniPath = strAppPath + L"App\\DLFramework.ini";
    UINT nLv = GetPrivateProfileInt( _T( "Log" ), _T( "Level" ), SPLOGLV_ERROR, strIniPath.c_str() );
    wstring strImageDir = strAppPath + L"ImageFiles";
    do
    {
        spFramRlt = m_fnFrameworkHelper.Startup( g_tCmdParams.eDlType, nLv );
        if ( SP_OK != spFramRlt )
        {
            m_strErrMsg = _T( "Startup Fail." );
            break;
        }
        spFramRlt = GetConfigObject();
        if ( SP_OK != spFramRlt )
        {
            m_strErrMsg = _T( "GetConfigObject Fail." );
            break;
        }
        spFramRlt = ConfigLoadPacBefore( );
        if ( SP_OK != spFramRlt )
        {
            m_strErrMsg = _T( "ConfigLoadPacBefore Fail." );
            break;
        }
        spFramRlt = m_fnFrameworkHelper.DeviceMoniter( TRUE );
        if ( SP_OK != spFramRlt )
        {
            m_strErrMsg = _T( "StartDeviceMoniter Fail." );
            break;
        }
        spFramRlt = m_fnFrameworkHelper.LoadPacket( g_tCmdParams.strPacFile.c_str(), strImageDir.c_str() );
        if ( SP_OK != spFramRlt )
        {
            m_strErrMsg = _T( "LoadPacket Fail." );
            break;
        }
        
        do
        {
            spTaskRlt = ConfigLoadPacAfter( );
            if ( SP_OK != spTaskRlt )
            {
                m_strErrMsg = _T( "ConfigLoadPacAfter Fail." );
                break;
            }
            spTaskRlt = m_fnFrameworkHelper.CreatTask( nLv ) ;
            if ( SP_OK != spTaskRlt )
            {
                m_strErrMsg = _T( "CreatTask Fail." );
                break;
            }
            m_fnDevMoniter.StartMoniter( TRUE, g_tCmdParams.nTimeout );
            m_fnDevMoniter.StartMoniter( FALSE );
            if ( m_fnDevMoniter.GetRunResult() )
            {
                do
                {
                    CHKRESULTBRK( m_fnFrameworkHelper.RunTask() );
                    m_fnDownloadMoniter.StartMoniter( TRUE, g_tCmdParams.nDLTimeout );
                    m_fnDownloadMoniter.StartMoniter( FALSE );
                } while ( 0 );
                m_fnFrameworkHelper.StopTask();
            }
            else
            {
                spTaskRlt = SP_E_TIMEOUT;
                TCHAR szBuff[256] = { 0 };
                _stprintf_s( szBuff, _T( "Connect DUT timeout( %dms )" ), g_tCmdParams.nTimeout );
                m_strErrMsg = szBuff;
            }
        } while ( 0 );
        SetDlEndData( spTaskRlt );
        m_fnFrameworkHelper.FreeTask();
    } while ( 0 );
    SetDlEndData( spFramRlt );
    m_fnFrameworkHelper.DeviceMoniter( FALSE );
    m_fnFrameworkHelper.Cleanup();
    
    return ( ( SP_OK != spFramRlt ) ? spFramRlt : ( ( SP_OK != spTaskRlt ) ? spTaskRlt : SP_OK ) );
}

void CDownload::SetDlEndData( SPRESULT spRlt )
{
    LPCALLBACKDATA_DL_END lpData = m_fnFrameworkHelper.GetDlEndData();
    if ( SP_OK != spRlt )
    {
        lpData->u32ErrCode = spRlt;
        lpData->lpErrMsg = m_strErrMsg.c_str();
        lpData->u32Port = ( 0 != g_tCmdParams.nPort ) ? g_tCmdParams.nPort : 0;
        _tprintf( _T( "%s%s;\n" ), KEY_FAIL, lpData->lpErrMsg );
        fflush( stdout );
    }
    else if ( SP_OK == spRlt )
    {
        lpData->lpErrMsg = NULL;
    }
}
