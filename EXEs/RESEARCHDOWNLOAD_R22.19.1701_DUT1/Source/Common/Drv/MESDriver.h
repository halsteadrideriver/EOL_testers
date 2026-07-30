#pragma once
#include <Windows.h>
#include <string>
#include "MESDrvDef.h"

class CMESDriver sealed
{
public:
    CMESDriver( void );
    ~CMESDriver( void );
    
    // Load dll
    BOOL Startup( std::wstring& strPath );
    void Cleanup( void );
    
    // Invoke interfaces of dll
    HANDLE UMES_Handle_Create( INT nSlot = 0, HANDLE hHandleLog = NULL );
    void UMES_Handle_ReleaseAll();
    void UMES_Handle_Release( );
    
    MES_RESULT UMES_EnableCheck( bool& bEnable );
    MES_RESULT UMES_New_Guid( LPSTR pszGuid, INT nSize );
    MES_RESULT UMES_Get_Host_MAC( LPSTR pszMac, INT nSize );
    MES_RESULT UMES_Get_Host_IP( LPSTR pszIP, INT nSize );
    MES_RESULT UMES_Get_Host_PCName( LPSTR pszName, INT nSize );
    MES_RESULT UMES_Get_Host_Os( LPSTR pszOs, INT nSize );
    MES_RESULT UMES_Get_Tool_Info( LPSTR pszToolName, LPSTR pszToolVersion );
    MES_RESULT UMES_GetLastError( LPSTR pszError, INT nSize );
    MES_RESULT UMES_GetBatchName( LPSTR pszBatch, INT nSize );
    MES_RESULT UMES_GetStationName( LPSTR pszStaion, INT nSize );
    MES_RESULT UMES_GetMesLogPath( LPSTR pszPath, INT nSize );
    MES_RESULT UMES_GetMesIniInfo( LPSTR pszNode, LPSTR pszKey, LPSTR psValue, INT nSize );
    MES_RESULT UMES_Check_Rule_IMEI( LPSTR pszIMEI );
    MES_RESULT UMES_Check_Rule_MEID( LPSTR pszMEID );
    MES_RESULT UMES_Check_Rule_BT( LPSTR pszBT );
    MES_RESULT UMES_Check_Rule_WIFI( LPSTR pszWIFI );
    MES_RESULT UMES_Login( _pUNISOC_MES_CMN_LOGIN p_login = NULL );
    MES_RESULT UMES_Logout( );
    MES_RESULT UMES_GetAssignedCodes( _pUNISOC_MES_BARCODE_INFO p_barcode_info, _pUNISOC_MES_CMN_CODES p_codes );
    MES_RESULT UMES_GetDeviceCode( _pUNISOC_MES_BARCODE_INFO p_barcode_info, _pUNISOC_MES_CMN_CODES p_codes );
    MES_RESULT UMES_RecordDeviceInfos( _pUNISOC_MES_CMN_CODES p_codes );
    MES_RESULT UMES_CheckFlow( _pUNISOC_MES_CHECK_FLOW p_check_flow );
    MES_RESULT UMES_SendTestResult( _pUNISOC_MES_CMN_TEST_RESULT p_test_result );
    MES_RESULT UMES_SendTestData( _pUNISOC_MES_CMN_TEST_DATA p_test_data );
    MES_RESULT UMES_SnInput( _pUNISOC_MES_CMN_SN_INPUT p_sn_input );
    MES_RESULT UMES_CheckSnInput( _pUNISOC_MES_CMN_SN_INPUT p_sn_input );
    MES_RESULT UMES_RecordAssignedCodes_ExtendedField( _pUNISOC_MES_CMN_CODES p_codes );
    MES_RESULT UMES_V2_SendDatabase( LPSTR pszDatabase );
    MES_RESULT UMES_V2_CheckTestTool( _pUNISOC_MES_TEST_TOOL p_test_tool );
    MES_RESULT UMES_V2_SendTestEnvironment( _pUNISOC_MES_TEST_ENV p_test_env );
    MES_RESULT UMES_V2_SendTestToolInfo( _pUNISOC_MES_TEST_TOOL p_test_tool );
    MES_RESULT UMES_V2_GetCurrentSeqFileInfo( _pUNISOC_MES_SEQ_FILEINFO_REQ p_seq_req, _pUNISOC_MES_SEQ_FILEINFO p_seq_info );
    MES_RESULT UMES_V2_GetDocumentListEx( _pUNISOC_MES_SEQ_FILEINFO_REQ p_seq_req, _pUNISOC_MES_DOC_INFO p_seq_info );
    MES_RESULT UMES_V2_GetSequenceFile( LPSTR pszDirSeq );
    MES_RESULT UMES_V1_GetBatchInfo( LPSTR pszBatch, _pUNISOC_MES_V1_BATCH_INFO p_batch );
    
private:
    HMODULE m_hDLL;
    HANDLE m_hHandle;
    UNISOC_MES_DRIVER_T m_drv;
};
