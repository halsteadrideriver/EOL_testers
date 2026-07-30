#include "StdAfx.h"
#include "UnisocMesHelper.h"
#include "MD5Algo.h"
#include "CLocks.h"
#include <atlconv.h>
#include <assert.h>
#include <shlwapi.h>
#pragma comment(lib,"Shlwapi.lib")
#include <fstream>
using namespace std;

extern std::wstring GetAppPath();

CUnisocMesHelper::CUnisocMesHelper( ISpLog* pLog )
{
    m_uiACP = GetACP();
    BindLog( pLog, "UnisocMES" );
}

CUnisocMesHelper::~CUnisocMesHelper( void )
{

}

BOOL CUnisocMesHelper::Startup()
{
    CTrLog log( this, _T( "Startup" ) );
    wstring strPath = GetAppPath();
    if ( !m_fnMesDrv.Startup( strPath ) )
    {
        LogFmtStr( SPLOGLV_ERROR, _T( "LoadLibrary Fail." ) );
        return FALSE;
    }
    HANDLE hHandle = m_fnMesDrv.UMES_Handle_Create();
    if ( NULL == hHandle )
    {
        LogFmtStr( SPLOGLV_ERROR, _T( "Create MES Handle Fail." ) );
        return FALSE;
    }
    return TRUE;
}

void CUnisocMesHelper::Clearup()
{
    CTrLog log( this, _T( "Clearup" ) );
    m_fnMesDrv.UMES_Handle_Release();
    m_fnMesDrv.Cleanup();
}

MES_RESULT CUnisocMesHelper::MesSnInput( LPCSTR pszSN )
{
    USES_CONVERSION;
    CTrLog log( this, _T( "MesSnInput" ), _T( "%s" ), A2W( pszSN ) );
    CLocks Lock( m_LockMes );
    if ( NULL == pszSN || 0 == strlen( pszSN ) )
    {
        LogFmtStr( SPLOGLV_ERROR, _T( "Invalid SN." ) );
        return MES_FAIL;
    }
    
    MES_RESULT Rlt = MES_SUCCESS;
    
    _UNISOC_MES_CMN_SN_INPUT sn_input;
    sn_input.i_v2_sn_type = 1;
    sn_input.i_v2_label_print_qty = 0;
    strcpy_s( sn_input.sz_cmn_sn, sizeof( sn_input.sz_cmn_sn ), pszSN );
    
    Rlt = m_fnMesDrv.UMES_SnInput( &sn_input );
    m_fnMesDrv.UMES_GetLastError( m_szErrorMsg, sizeof( m_szErrorMsg ) );
    if ( MES_DISABLE_FAIL == Rlt )
    {
        return MES_SUCCESS;
    }
    return Rlt;
}

MES_RESULT CUnisocMesHelper::MesConnect()
{
    CTrLog log( this, _T( "MesConnect" ) );
    CLocks Lock( m_LockMes );
    _UNISOC_MES_CMN_LOGIN mes_login;
    MES_RESULT Rlt = MES_SUCCESS;
    Rlt = m_fnMesDrv.UMES_Login( &mes_login );
    m_fnMesDrv.UMES_GetLastError( m_szErrorMsg, sizeof( m_szErrorMsg ) );
    if ( MES_DISABLE_FAIL == Rlt )
    {
        return MES_SUCCESS;
    }
    else if ( MES_SUCCESS != Rlt )
    {
        return Rlt;
    }
    
    Rlt = m_fnMesDrv.UMES_V2_SendDatabase( NULL );
    m_fnMesDrv.UMES_GetLastError( m_szErrorMsg, sizeof( m_szErrorMsg ) );
    if ( MES_DISABLE_FAIL == Rlt )
    {
        return MES_SUCCESS;
    }
    else if ( MES_SUCCESS != Rlt )
    {
        return Rlt;
    }
    m_fnMesDrv.UMES_New_Guid( m_szGuid, sizeof( m_szGuid ) );
    return MES_SUCCESS;
}

MES_RESULT CUnisocMesHelper::MesCheckFlow( LPCSTR pszSN )
{
    USES_CONVERSION;
    CTrLog log( this, _T( "MesCheckFlow" ), _T( "%s" ), A2W( pszSN ) );
    MES_RESULT Rlt = MES_SUCCESS;
    _UNISOC_MES_CHECK_FLOW check_flow;
    strcpy_s( check_flow.szSN, sizeof( check_flow.szSN ), pszSN );
    Rlt = m_fnMesDrv.UMES_CheckFlow( &check_flow );
    m_fnMesDrv.UMES_GetLastError( m_szErrorMsg, sizeof( m_szErrorMsg ) );
    if ( MES_DISABLE_FAIL == Rlt )
    {
        return MES_SUCCESS;
    }
    return Rlt;
}

MES_RESULT CUnisocMesHelper::MesSendTestResult( LPCDLMES_RESULT lpResult )
{
    CTrLog log( this, _T( "MesSendTestResult" ) );
    if ( NULL == lpResult )
    {
        LogFmtStr( SPLOGLV_ERROR, _T( "Invalid DLMES_RESULT Param." ) );
        return MES_FAIL;
    }
    
    MES_RESULT Rlt = MES_SUCCESS;
    _UNISOC_MES_TEST_TOOL test_tool;
    sprintf_s( test_tool.szTestToolName, "%s", lpResult->lpszToolName );
    sprintf_s( test_tool.szTestToolVersion, "R%s", lpResult->lpszToolVer );
    Rlt = m_fnMesDrv.UMES_V2_SendTestToolInfo( &test_tool );
    m_fnMesDrv.UMES_GetLastError( m_szErrorMsg, sizeof( m_szErrorMsg ) );
    if ( MES_DISABLE_FAIL == Rlt )
    {
        return MES_SUCCESS;
    }
    else if ( MES_SUCCESS != Rlt )
    {
        return Rlt;
    }
    
    _UNISOC_MES_TEST_ENV test_env;
    m_fnMesDrv.UMES_Get_Host_IP( test_env.szProductStationIP, sizeof( test_env.szProductStationIP ) );
    m_fnMesDrv.UMES_Get_Host_MAC( test_env.szProductStationMac, sizeof( test_env.szProductStationMac ) );
    m_fnMesDrv.UMES_Get_Host_Os( test_env.szOs, sizeof( test_env.szOs ) );
    m_fnMesDrv.UMES_Get_Host_PCName( test_env.szProductStationName, sizeof( test_env.szProductStationName ) );
    Rlt = m_fnMesDrv.UMES_V2_SendTestEnvironment( &test_env );
    m_fnMesDrv.UMES_GetLastError( m_szErrorMsg, sizeof( m_szErrorMsg ) );
    if ( MES_DISABLE_FAIL == Rlt )
    {
        return MES_SUCCESS;
    }
    else if ( MES_SUCCESS != Rlt )
    {
        return Rlt;
    }
    
    _UNISOC_MES_CMN_TEST_RESULT test_result;
    test_result.i_cmn_result = lpResult->nResult;
    strcpy_s( test_result.sz_cmn_elapsed_time,  lpResult->lpszTime );
    strcpy_s( test_result.sz_cmn_result_id, m_szGuid );
    strcpy_s( test_result.sz_cmn_sn, lpResult->lpszSN );
    strcpy_s( test_result.sz_cmn_err_msg, lpResult->lpszError );
    Rlt = m_fnMesDrv.UMES_SendTestResult( &test_result );
    m_fnMesDrv.UMES_GetLastError( m_szErrorMsg, sizeof( m_szErrorMsg ) );
    return Rlt;
}

MES_RESULT CUnisocMesHelper::MesCheckCRC( LPCTSTR lpPacPath )
{
    CHAR szPathIni[512] = { 0 };
    CHAR szPacPath[512] = { 0 };
    wstring strAppPath = GetAppPath();
    sprintf_s( szPathIni, "%lsPhaseCheck.ini", strAppPath.c_str() );
    if ( !PathFileExistsA( szPathIni ) )
    {
        strcpy_s( m_szErrorMsg, sizeof( m_szErrorMsg ), "PhaseCheck.ini does not exist." );
        return MES_FAIL;
    }
    
    _UNISOC_MES_SEQ_FILEINFO_REQ seq_req;
    _UNISOC_MES_SEQ_FILEINFO seq_info;
    MES_RESULT mes_result = MES_SUCCESS;
    mes_result = m_fnMesDrv.UMES_V2_GetCurrentSeqFileInfo( &seq_req, &seq_info );
    if ( MES_SUCCESS == mes_result && 1 == seq_info.iIsCheck && 32 == strlen( seq_info.szSeqCRC ) )
    {
        if ( !CheckCRC( szPathIni, seq_info.szSeqCRC ) )
        {
            strcpy_s( m_szErrorMsg, sizeof( m_szErrorMsg ), "PhaseCheck.ini CRC Fail." );
            return MES_FAIL;
        }
        
        CHAR szPacketCRC[64] = {0};
        GetPrivateProfileStringA( "Packet", "CRC", "", szPacketCRC, sizeof( szPacketCRC ), szPathIni );
        
        if ( 32 == strlen( szPacketCRC ) )
        {
            int nMemory = 200;
            sprintf_s( szPathIni, "%lsUnisocMES\\Unisoc_Solution_MES.ini", strAppPath.c_str() );
            if ( PathFileExistsA( szPathIni ) )
            {
                nMemory = GetPrivateProfileIntA( "Setting", "MES DL CRC Memory", 200, szPathIni );
            }
            
            int iLen = WideCharToMultiByte( CP_ACP, 0, lpPacPath, -1, NULL, 0, NULL, NULL );
            WideCharToMultiByte( CP_ACP, 0, lpPacPath, -1, szPacPath, iLen, NULL, NULL );
            
            if ( !CheckPacCRC( nMemory, szPacPath, szPacketCRC ) )
            {
                strcpy_s( m_szErrorMsg, sizeof( m_szErrorMsg ), "Packet CRC Fail." );
                return MES_FAIL;
            }
        }
    }
    return MES_SUCCESS;
}

BOOL CUnisocMesHelper::CheckPacCRC( INT nMemory, LPSTR pszPath, LPSTR pszCRC )
{
    std::locale loc1;
    if ( 936 == m_uiACP )
    {
        loc1 = std::locale::global( std::locale( ".936" ) );
    }
    
    ifstream fin( pszPath, std::ios::binary );
    LPSTR pszBuffer = NULL;
    SIZE_T size_buff = ( ULONG_PTR )fin.seekg( 0, std::ios::end ).tellg();
    
    INT nLoop = 0;
    BOOL bLoop = TRUE;
    SIZE_T sizeMemory = size_buff;
    SIZE_T uiMemory = nMemory * 1024 * 1024;
    SIZE_T uiLength = 0;
    
    CHAR szCode[16] = {0};
    CHAR szMd5Code[64] = {0};
    
    CMD5Algo md5_algo;
    
    while ( bLoop )
    {
        if ( sizeMemory > uiMemory )
        {
            pszBuffer = new char[uiMemory + 16];
            memset( pszBuffer, 0, uiMemory );
            if ( 0 == nLoop )
            {
                uiLength = uiMemory;
                fin.seekg( nLoop * uiMemory, std::ios::beg ).read( pszBuffer, static_cast<std::streamsize>( uiMemory ) );
            }
            else
            {
                uiLength = uiMemory + 16;
                strcat_s( pszBuffer, uiLength, szMd5Code );
                fin.seekg( nLoop * uiMemory, std::ios::beg ).read( pszBuffer + 16, static_cast<std::streamsize>( uiMemory ) );
            }
            if ( ( size_buff - nLoop * uiMemory ) > uiMemory )
            {
                nLoop++;
            }
        }
        else
        {
            pszBuffer = new char[sizeMemory + 16];
            memset( pszBuffer, 0, sizeMemory );
            uiLength = sizeMemory + 16;
            strcat_s( pszBuffer, uiLength, szMd5Code );
            fin.seekg( nLoop * uiMemory + sizeMemory, std::ios::beg ).read( pszBuffer + 16, static_cast<std::streamsize>( sizeMemory ) );
            bLoop = FALSE;
        }
        
        unsigned char byCode[16] = {0};
        MD5_CTX md5;
        md5_algo.MD5_Init( &md5 );
        md5_algo.MD5_Update( &md5, ( unsigned char* )pszBuffer, uiLength );
        md5_algo.MD5_Final( byCode, &md5 );
        
        ZeroMemory( szMd5Code, sizeof( szMd5Code ) );
        for ( int i = 0; i < 16; i++ )
        {
            sprintf_s( szCode, sizeof( szCode ), "%02x", byCode[i] );
            strcat_s( szMd5Code, szCode );
        }
        
        delete []pszBuffer;
        sizeMemory -= uiMemory;
    };
    fin.close();
    if ( 936 == m_uiACP )
    {
        std::locale::global( std::locale( loc1 ) );
    }
    if ( 0 != strcmp( pszCRC, szMd5Code ) )
    {
        return FALSE;
    }
    return TRUE;
}

BOOL CUnisocMesHelper::CheckCRC( LPSTR pszPath, LPSTR pszCRC )
{
    std::locale loc1;
    if ( 936 == m_uiACP )
    {
        loc1 = std::locale::global( std::locale( ".936" ) );
    }
    ifstream fin( pszPath, std::ios::binary );
    LPSTR pszBuffer = NULL;
    SIZE_T size_buff = ( ULONG_PTR )fin.seekg( 0, std::ios::end ).tellg();
    pszBuffer = new char[size_buff];
    memset( pszBuffer, 0, size_buff );
    fin.seekg( 0, std::ios::beg ).read( pszBuffer, static_cast<std::streamsize>( size_buff ) );
    fin.close();
    if ( 936 == m_uiACP )
    {
        std::locale::global( std::locale( loc1 ) );
    }
    
    CMD5Algo md5_algo;
    MD5_CTX md5;
    unsigned char byCode[16] = {0};
    
    md5_algo.MD5_Init( &md5 );
    md5_algo.MD5_Update( &md5, ( unsigned char* )pszBuffer, size_buff );
    md5_algo.MD5_Final( byCode, &md5 );
    
    char szCode[16] = {0};
    char szMd5Code[64] = {0};
    for ( int i = 0; i < 16; i++ )
    {
        sprintf_s( szCode, sizeof( szCode ), "%02x", byCode[i] );
        strcat_s( szMd5Code, szCode );
    }
    delete []pszBuffer;
    
    if ( 0 != strcmp( pszCRC, szMd5Code ) )
    {
        return FALSE;
    }
    return TRUE;
}

void CUnisocMesHelper::MesGetErrMsg( LPSTR pszErrMsg, DWORD dwSize )
{
    if ( strlen( m_szErrorMsg ) > 0 )
    {
        strcpy_s( pszErrMsg, dwSize, m_szErrorMsg );
        LogFmtStrA( SPLOGLV_ERROR, m_szErrorMsg );
    }
};
