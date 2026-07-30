#include "StdAfx.h"
#include "MESDriver.h"
#include <assert.h>

#ifdef _DEBUG
//#define _DUMMY_UNISOC_MES_
#endif

#ifdef _DUMMY_UNISOC_MES_
#pragma warning(disable:4100)
#endif

//////////////////////////////////////////////////////////////////////////
CMESDriver::CMESDriver( void )
    : m_hDLL( NULL )
    , m_hHandle( NULL )
{
}

CMESDriver::~CMESDriver( void )
{
}

void CMESDriver::Cleanup( void )
{
    if ( NULL != m_hDLL )
    {
        FreeLibrary( m_hDLL );
        m_hDLL = NULL;
        m_drv.Reset();
    }
}

#define GET_PROC_ADD(name) m_drv.##name = ( p##name )GetProcAddress( m_hDLL, #name )
BOOL CMESDriver::Startup( std::wstring& strPath )
{
    std::wstring strMES = strPath + L"UnisocMES\\Unisoc_Solution_MES.dll";
    m_hDLL = LoadLibraryExW( strMES.c_str(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH );
    if ( NULL == m_hDLL )
    {
        return FALSE;
    }
    
    GET_PROC_ADD( MES_Handle_Create );
    GET_PROC_ADD( MES_Handle_Release );
    GET_PROC_ADD( MES_Handle_ReleaseAll );
    GET_PROC_ADD( MES_EnableCheck );
    GET_PROC_ADD( MES_New_Guid );
    GET_PROC_ADD( MES_Get_Host_MAC );
    GET_PROC_ADD( MES_Get_Host_IP );
    GET_PROC_ADD( MES_Get_Host_PCName );
    GET_PROC_ADD( MES_Get_Host_Os );
    GET_PROC_ADD( MES_Get_Tool_Info );
    GET_PROC_ADD( MES_GetLastError );
    GET_PROC_ADD( MES_GetBatchName );
    GET_PROC_ADD( MES_GetStationName );
    GET_PROC_ADD( MES_GetMesLogPath );
    GET_PROC_ADD( MES_GetMesIniInfo );
    GET_PROC_ADD( MES_Check_Rule_IMEI );
    GET_PROC_ADD( MES_Check_Rule_BT );
    GET_PROC_ADD( MES_Check_Rule_WIFI );
    GET_PROC_ADD( MES_Check_Rule_MEID );
    GET_PROC_ADD( MES_Login );
    GET_PROC_ADD( MES_Logout );
    GET_PROC_ADD( MES_GetAssignedCodes );
    GET_PROC_ADD( MES_GetDeviceCode );
    GET_PROC_ADD( MES_RecordDeviceInfos );
    GET_PROC_ADD( MES_CheckFlow );
    GET_PROC_ADD( MES_SendTestResult );
    GET_PROC_ADD( MES_SendTestData );
    GET_PROC_ADD( MES_SnInput );
    GET_PROC_ADD( MES_CheckSnInput );
    GET_PROC_ADD( MES_RecordAssignedCodes_ExtendedField );
    GET_PROC_ADD( MES_V2_SendDatabase );
    GET_PROC_ADD( MES_V2_CheckTestTool );
    GET_PROC_ADD( MES_V2_SendTestEnvironment );
    GET_PROC_ADD( MES_V2_SendTestToolInfo );
    GET_PROC_ADD( MES_V2_GetCurrentSeqFileInfo );
    GET_PROC_ADD( MES_V2_GetDocumentListEx );
    GET_PROC_ADD( MES_V2_GetSequenceFile );
    GET_PROC_ADD( MES_V1_GetBatchInfo );
    
    if ( !m_drv.IsValid() )
    {
        Cleanup();
        return FALSE;
    }
    
    return TRUE;
}

HANDLE CMESDriver::UMES_Handle_Create( INT nSlot, HANDLE m_hHandleLog )
{
    if ( NULL != m_drv.MES_Handle_Create )
    {
#ifdef _DUMMY_UNISOC_MES_
        return NULL;
#else
        m_hHandle = m_drv.MES_Handle_Create( nSlot, m_hHandleLog );
        return m_hHandle;
#endif
    }
    else
    {
        return NULL;
    }
}
void CMESDriver::UMES_Handle_ReleaseAll()
{
    if ( NULL != m_drv.MES_Handle_ReleaseAll )
    {
#ifdef _DUMMY_UNISOC_MES_
#else
        m_drv.MES_Handle_ReleaseAll();
#endif
    }
}
void CMESDriver::UMES_Handle_Release( )
{
    if ( NULL != m_drv.MES_Handle_Release )
    {
#ifdef _DUMMY_UNISOC_MES_
#else
        return m_drv.MES_Handle_Release( m_hHandle );
#endif
    }
}
MES_RESULT CMESDriver::UMES_EnableCheck( bool& bEnable )
{
    if ( NULL != m_drv.MES_EnableCheck )
    {
#ifdef _DUMMY_UNISOC_MES_
        return MES_SUCCESS;
#else
        return m_drv.MES_EnableCheck( m_hHandle, bEnable );
#endif
    }
    else
    {
        return MES_FAIL;
    }
}
MES_RESULT CMESDriver::UMES_New_Guid( LPSTR pszGuid, INT nSize )
{
    if ( NULL != m_drv.MES_New_Guid )
    {
#ifdef _DUMMY_UNISOC_MES_
        return MES_SUCCESS;
#else
        return m_drv.MES_New_Guid( m_hHandle, pszGuid, nSize );
#endif
    }
    else
    {
        return MES_FAIL;
    }
}
MES_RESULT CMESDriver::UMES_Get_Host_MAC( LPSTR pszMac, INT nSize )
{
    if ( NULL != m_drv.MES_Get_Host_MAC )
    {
#ifdef _DUMMY_UNISOC_MES_
        return MES_SUCCESS;
#else
        return m_drv.MES_Get_Host_MAC( m_hHandle, pszMac, nSize );
#endif
    }
    else
    {
        return MES_FAIL;
    }
}
MES_RESULT CMESDriver::UMES_Get_Host_IP( LPSTR pszIP, INT nSize )
{
    if ( NULL != m_drv.MES_Get_Host_IP )
    {
#ifdef _DUMMY_UNISOC_MES_
        return MES_SUCCESS;
#else
        return m_drv.MES_Get_Host_IP( m_hHandle, pszIP, nSize );
#endif
    }
    else
    {
        return MES_FAIL;
    }
}
MES_RESULT CMESDriver::UMES_Get_Host_PCName( LPSTR pszName, INT nSize )
{
    if ( NULL != m_drv.MES_Get_Host_PCName )
    {
#ifdef _DUMMY_UNISOC_MES_
        return MES_SUCCESS;
#else
        return m_drv.MES_Get_Host_PCName( m_hHandle, pszName, nSize );
#endif
    }
    else
    {
        return MES_FAIL;
    }
}
MES_RESULT CMESDriver::UMES_Get_Host_Os( LPSTR pszOs, INT nSize )
{
    if ( NULL != m_drv.MES_Get_Host_Os )
    {
#ifdef _DUMMY_UNISOC_MES_
        return MES_SUCCESS;
#else
        return m_drv.MES_Get_Host_Os( m_hHandle, pszOs, nSize );
#endif
    }
    else
    {
        return MES_FAIL;
    }
}
MES_RESULT CMESDriver::UMES_Get_Tool_Info( LPSTR pszToolName, LPSTR pszToolVersion )
{
    if ( NULL != m_drv.MES_Get_Tool_Info )
    {
#ifdef _DUMMY_UNISOC_MES_
        return MES_SUCCESS;
#else
        return m_drv.MES_Get_Tool_Info( m_hHandle, pszToolName, pszToolVersion );
#endif
    }
    else
    {
        return MES_FAIL;
    }
}
MES_RESULT CMESDriver::UMES_GetLastError( LPSTR pszError, INT nSize )
{
    if ( NULL != m_drv.MES_GetLastError )
    {
#ifdef _DUMMY_UNISOC_MES_
        return MES_SUCCESS;
#else
        return m_drv.MES_GetLastError( m_hHandle, pszError, nSize );
#endif
    }
    else
    {
        return MES_FAIL;
    }
}
MES_RESULT CMESDriver::UMES_GetBatchName( LPSTR pszBatch, INT nSize )
{
    if ( NULL != m_drv.MES_GetBatchName )
    {
#ifdef _DUMMY_UNISOC_MES_
        return MES_SUCCESS;
#else
        return m_drv.MES_GetBatchName( m_hHandle, pszBatch, nSize );
#endif
    }
    else
    {
        return MES_FAIL;
    }
}
MES_RESULT CMESDriver::UMES_GetStationName( LPSTR pszStaion, INT nSize )
{
    if ( NULL != m_drv.MES_GetStationName )
    {
#ifdef _DUMMY_UNISOC_MES_
        return MES_SUCCESS;
#else
        return m_drv.MES_GetStationName( m_hHandle, pszStaion, nSize );
#endif
    }
    else
    {
        return MES_FAIL;
    }
}
MES_RESULT CMESDriver::UMES_GetMesLogPath( LPSTR pszPath, INT nSize )
{
    if ( NULL != m_drv.MES_GetMesLogPath )
    {
#ifdef _DUMMY_UNISOC_MES_
        return MES_SUCCESS;
#else
        return m_drv.MES_GetMesLogPath( m_hHandle, pszPath, nSize );
#endif
    }
    else
    {
        return MES_FAIL;
    }
}

MES_RESULT CMESDriver::UMES_GetMesIniInfo( LPSTR pszNode, LPSTR pszKey, LPSTR psValue, INT nSize )
{
    if ( NULL != m_drv.MES_GetMesIniInfo )
    {
#ifdef _DUMMY_UNISOC_MES_
        return MES_SUCCESS;
#else
        return m_drv.MES_GetMesIniInfo( m_hHandle, pszNode, pszKey, psValue, nSize );
#endif
    }
    else
    {
        return MES_FAIL;
    }
}
MES_RESULT CMESDriver::UMES_Check_Rule_MEID( LPSTR pszMEID )
{
    if ( NULL != m_drv.MES_Check_Rule_MEID )
    {
#ifdef _DUMMY_UNISOC_MES_
        return MES_SUCCESS;
#else
        return m_drv.MES_Check_Rule_MEID( m_hHandle, pszMEID );
#endif
    }
    else
    {
        return MES_FAIL;
    }
}
MES_RESULT CMESDriver::UMES_Check_Rule_IMEI( LPSTR pszIMEI )
{
    if ( NULL != m_drv.MES_Check_Rule_IMEI )
    {
#ifdef _DUMMY_UNISOC_MES_
        return MES_SUCCESS;
#else
        return m_drv.MES_Check_Rule_IMEI( m_hHandle, pszIMEI );
#endif
    }
    else
    {
        return MES_FAIL;
    }
}
MES_RESULT CMESDriver::UMES_Check_Rule_BT( LPSTR pszBT )
{
    if ( NULL != m_drv.MES_Check_Rule_BT )
    {
#ifdef _DUMMY_UNISOC_MES_
        return MES_SUCCESS;
#else
        return m_drv.MES_Check_Rule_BT( m_hHandle, pszBT );
#endif
    }
    else
    {
        return MES_FAIL;
    }
}
MES_RESULT CMESDriver::UMES_Check_Rule_WIFI( LPSTR pszWIFI )
{
    if ( NULL != m_drv.MES_Check_Rule_WIFI )
    {
#ifdef _DUMMY_UNISOC_MES_
        return MES_SUCCESS;
#else
        return m_drv.MES_Check_Rule_WIFI( m_hHandle, pszWIFI );
#endif
    }
    else
    {
        return MES_FAIL;
    }
}
MES_RESULT CMESDriver::UMES_Login( _pUNISOC_MES_CMN_LOGIN p_login )
{
    if ( NULL != m_drv.MES_Login )
    {
#ifdef _DUMMY_UNISOC_MES_
        return MES_SUCCESS;
#else
        return m_drv.MES_Login( m_hHandle, p_login );
#endif
    }
    else
    {
        return MES_FAIL;
    }
}
MES_RESULT CMESDriver::UMES_Logout( )
{
    if ( NULL != m_drv.MES_Logout )
    {
#ifdef _DUMMY_UNISOC_MES_
        return MES_SUCCESS;
#else
        return m_drv.MES_Logout( m_hHandle );
#endif
    }
    else
    {
        return MES_FAIL;
    }
}
MES_RESULT CMESDriver::UMES_GetAssignedCodes( _pUNISOC_MES_BARCODE_INFO p_barcode_info, _pUNISOC_MES_CMN_CODES p_codes )
{
    if ( NULL != m_drv.MES_GetAssignedCodes )
    {
#ifdef _DUMMY_UNISOC_MES_
        return MES_SUCCESS;
#else
        return m_drv.MES_GetAssignedCodes( m_hHandle, p_barcode_info, p_codes );
#endif
    }
    else
    {
        return MES_FAIL;
    }
}
MES_RESULT CMESDriver::UMES_GetDeviceCode( _pUNISOC_MES_BARCODE_INFO p_barcode_info, _pUNISOC_MES_CMN_CODES p_codes )
{
    if ( NULL != m_drv.MES_GetDeviceCode )
    {
#ifdef _DUMMY_UNISOC_MES_
        return MES_SUCCESS;
#else
        return m_drv.MES_GetDeviceCode( m_hHandle, p_barcode_info, p_codes );
#endif
    }
    else
    {
        return MES_FAIL;
    }
}
MES_RESULT CMESDriver::UMES_RecordDeviceInfos( _pUNISOC_MES_CMN_CODES p_codes )
{
    if ( NULL != m_drv.MES_RecordDeviceInfos )
    {
#ifdef _DUMMY_UNISOC_MES_
        return MES_SUCCESS;
#else
        return m_drv.MES_RecordDeviceInfos( m_hHandle, p_codes );
#endif
    }
    else
    {
        return MES_FAIL;
    }
}
MES_RESULT CMESDriver::UMES_CheckFlow( _pUNISOC_MES_CHECK_FLOW p_check_flow )
{
    if ( NULL != m_drv.MES_CheckFlow )
    {
#ifdef _DUMMY_UNISOC_MES_
        return MES_SUCCESS;
#else
        return m_drv.MES_CheckFlow( m_hHandle, p_check_flow );
#endif
    }
    else
    {
        return MES_FAIL;
    }
}
MES_RESULT CMESDriver::UMES_SendTestResult( _pUNISOC_MES_CMN_TEST_RESULT p_test_result )
{
    if ( NULL != m_drv.MES_SendTestResult )
    {
#ifdef _DUMMY_UNISOC_MES_
        return MES_SUCCESS;
#else
        return m_drv.MES_SendTestResult( m_hHandle, p_test_result );
#endif
    }
    else
    {
        return MES_FAIL;
    }
}
MES_RESULT CMESDriver::UMES_SendTestData( _pUNISOC_MES_CMN_TEST_DATA p_test_data )
{
    if ( NULL != m_drv.MES_SendTestData )
    {
#ifdef _DUMMY_UNISOC_MES_
        return MES_SUCCESS;
#else
        return m_drv.MES_SendTestData( m_hHandle, p_test_data );
#endif
    }
    else
    {
        return MES_FAIL;
    }
}
MES_RESULT CMESDriver::UMES_SnInput( _pUNISOC_MES_CMN_SN_INPUT p_sn_input )
{
    if ( NULL != m_drv.MES_SnInput )
    {
#ifdef _DUMMY_UNISOC_MES_
        return MES_SUCCESS;
#else
        return m_drv.MES_SnInput( m_hHandle, p_sn_input );
#endif
    }
    else
    {
        return MES_FAIL;
    }
}
MES_RESULT CMESDriver::UMES_CheckSnInput( _pUNISOC_MES_CMN_SN_INPUT p_sn_input )
{
    if ( NULL != m_drv.MES_CheckSnInput )
    {
#ifdef _DUMMY_UNISOC_MES_
        return MES_SUCCESS;
#else
        return m_drv.MES_CheckSnInput( m_hHandle, p_sn_input );
#endif
    }
    else
    {
        return MES_FAIL;
    }
}
MES_RESULT CMESDriver::UMES_RecordAssignedCodes_ExtendedField( _pUNISOC_MES_CMN_CODES p_codes )
{
    if ( NULL != m_drv.MES_RecordAssignedCodes_ExtendedField )
    {
#ifdef _DUMMY_UNISOC_MES_
        return MES_SUCCESS;
#else
        return m_drv.MES_RecordAssignedCodes_ExtendedField( m_hHandle, p_codes );
#endif
    }
    else
    {
        return MES_FAIL;
    }
}
MES_RESULT CMESDriver::UMES_V2_SendDatabase( LPSTR pszDatabase )
{
    if ( NULL != m_drv.MES_V2_SendDatabase )
    {
#ifdef _DUMMY_UNISOC_MES_
        return MES_SUCCESS;
#else
        return m_drv.MES_V2_SendDatabase( m_hHandle, pszDatabase );
#endif
    }
    else
    {
        return MES_FAIL;
    }
}
MES_RESULT CMESDriver::UMES_V2_CheckTestTool( _pUNISOC_MES_TEST_TOOL p_test_tool )
{
    if ( NULL != m_drv.MES_V2_CheckTestTool )
    {
#ifdef _DUMMY_UNISOC_MES_
        return MES_SUCCESS;
#else
        return m_drv.MES_V2_CheckTestTool( m_hHandle, p_test_tool );
#endif
    }
    else
    {
        return MES_FAIL;
    }
}
MES_RESULT CMESDriver::UMES_V2_SendTestEnvironment( _pUNISOC_MES_TEST_ENV p_test_env )
{
    if ( NULL != m_drv.MES_V2_SendTestEnvironment )
    {
#ifdef _DUMMY_UNISOC_MES_
        return MES_SUCCESS;
#else
        return m_drv.MES_V2_SendTestEnvironment( m_hHandle, p_test_env );
#endif
    }
    else
    {
        return MES_FAIL;
    }
}
MES_RESULT CMESDriver::UMES_V2_SendTestToolInfo( _pUNISOC_MES_TEST_TOOL p_test_tool )
{
    if ( NULL != m_drv.MES_V2_SendTestToolInfo )
    {
#ifdef _DUMMY_UNISOC_MES_
        return MES_SUCCESS;
#else
        return m_drv.MES_V2_SendTestToolInfo( m_hHandle, p_test_tool );
#endif
    }
    else
    {
        return MES_FAIL;
    }
}
MES_RESULT CMESDriver::UMES_V2_GetDocumentListEx( _pUNISOC_MES_SEQ_FILEINFO_REQ p_seq_req, _pUNISOC_MES_DOC_INFO p_seq_info )
{
    if ( NULL != m_drv.MES_V2_GetDocumentListEx )
    {
#ifdef _DUMMY_UNISOC_MES_
        return MES_SUCCESS;
#else
        return m_drv.MES_V2_GetDocumentListEx( m_hHandle, p_seq_req, p_seq_info );
#endif
    }
    else
    {
        return MES_FAIL;
    }
}
MES_RESULT CMESDriver::UMES_V2_GetCurrentSeqFileInfo( _pUNISOC_MES_SEQ_FILEINFO_REQ p_seq_req, _pUNISOC_MES_SEQ_FILEINFO p_seq_info )
{
    if ( NULL != m_drv.MES_V2_GetCurrentSeqFileInfo )
    {
#ifdef _DUMMY_UNISOC_MES_
        return MES_SUCCESS;
#else
        return m_drv.MES_V2_GetCurrentSeqFileInfo( m_hHandle, p_seq_req, p_seq_info );
#endif
    }
    else
    {
        return MES_FAIL;
    }
}
MES_RESULT CMESDriver::UMES_V2_GetSequenceFile( LPSTR pszDirSeq )
{
    if ( NULL != m_drv.MES_V2_GetSequenceFile )
    {
#ifdef _DUMMY_UNISOC_MES_
        return MES_SUCCESS;
#else
        return m_drv.MES_V2_GetSequenceFile( m_hHandle, pszDirSeq );
#endif
    }
    else
    {
        return MES_FAIL;
    }
}
MES_RESULT CMESDriver::UMES_V1_GetBatchInfo( LPSTR pszBatch, _pUNISOC_MES_V1_BATCH_INFO p_batch )
{
    if ( NULL != m_drv.MES_V1_GetBatchInfo )
    {
#ifdef _DUMMY_UNISOC_MES_
        return MES_SUCCESS;
#else
        return m_drv.MES_V1_GetBatchInfo( m_hHandle, pszBatch, p_batch );
#endif
    }
    else
    {
        return MES_FAIL;
    }
}
