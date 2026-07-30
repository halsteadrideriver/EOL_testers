#include "StdAfx.h"
#include "FlashOperationImpl.h"

#define FLASH_NAME "FlashOperat"

extern std::string GetAppPath();

CFlashOperationImpl::CFlashOperationImpl( LPVOID pLogUtil )
{
    BindLog( ( ISpLog* )pLogUtil, FLASH_NAME );
}

CFlashOperationImpl::~CFlashOperationImpl()
{

}

void CFlashOperationImpl::Release( void )
{
    delete this;
}

void CFlashOperationImpl::GetErrorMsg( LPTSTR lpErrMsg, DWORD dwCount )
{
    if ( NULL != lpErrMsg && dwCount > 0 )
    {
        _tcsnccpy_s( lpErrMsg, dwCount, m_szErrorMsg, _tcslen( m_szErrorMsg ) > dwCount ? dwCount : _tcslen( m_szErrorMsg ) );
    }
}

SPRESULT CFlashOperationImpl::SetProperty( INT nProperty, INT nFlags, LPCVOID lpValue )
{
    SPRESULT spRlt = SP_OK;
    switch ( nProperty )
    {
    case FLASH_OP_WRITE_FDL2:
        {
            LPFLASH_WRITE_FILE_VALUE lpWtire = ( LPFLASH_WRITE_FILE_VALUE )lpValue;
            m_arrWriteFdl2.PushBack( lpWtire );
        }
        break;
    case FLASH_OP_WRITE_DLEND:
        {
            LPFLASH_WRITE_FILE_VALUE lpWtire = ( LPFLASH_WRITE_FILE_VALUE )lpValue;
            m_arrWriteDlEnd.PushBack( lpWtire );
        }
        break;
    default:
        break;
    }
    return spRlt;
}

SPRESULT CFlashOperationImpl::GetProperty( INT nProperty, INT nFlags, LPVOID lpValue )
{
    return SP_OK;
}

SPRESULT CFlashOperationImpl::EnableOperation( DL_TYPE_ENUM eType, LPFLASH_OPERATION_SWITCH lpOperation )
{
    CTrLog log( this, _T( "EnableOperation" ) );
    if ( RESEARCH_TYPE == eType )
    {
        lpOperation->u8ReadFlashAfterFDL2 = 0;
    }
    lpOperation->u8EraseFlashAfterFDL2 = 0;
    lpOperation->u8WriteFlashAfterFDL2 = 0;
    lpOperation->u8WriteFlashAfterDlEnd = 0;
    return SP_OK;
}

SPRESULT CFlashOperationImpl::PrepareReadFlashAfterFDL2( LPFLASH_READ_FILE lpReadFile, INT& nCount )
{
    CTrLog log( this, _T( "ReadFlashAfterFDL2" ) );
    if ( 2 > nCount )
    {
        LogFmtStr( SPLOGLV_ERROR, _T( "The operation cannot be larger than %d" ), nCount );
        return SP_E_FAIL;
    }
    nCount = 0;
    wcscpy_s( lpReadFile[nCount].szFileId, L"miscdata" );
    lpReadFile[nCount].dwFileOffset = 0x00;
    lpReadFile[nCount].dwFileSize = 0x10000;
    nCount++;
    
    wcscpy_s( lpReadFile[nCount].szFileId, L"uboot_log" );
    lpReadFile[nCount].dwFileOffset = 0x00;
    lpReadFile[nCount].dwFileSize = 0x40000;
    nCount++;
    
    return SP_OK;
}

SPRESULT CFlashOperationImpl::CheckReadBuffer( LPCSTR lpReadBuffer, LPFLASH_READ_FILE lpReadFile )
{
    CTrLog log( this, _T( "CheckReadBuffer" ) );
    DWORD dwFileSize = lpReadFile->dwFileSize;
    DWORD dwFileOffset = lpReadFile->dwFileOffset;
    DWORD dwToatlSize = lpReadFile->dwToatlSize;
    LPWSTR lpFileID = lpReadFile->szFileId;
    CHAR szPath[512] = { 0 };
    CHAR szInfo[128] = { 0 };
    
    if ( 0 == dwFileOffset )
    {
        m_nCount = 1;
    }
    else
    {
        m_nCount++;
    }
    
    sprintf_s( szInfo, "No.%d, FileId:%ls, FileSize:0x%x, FileOffset:0x%x", m_nCount, lpFileID, dwFileSize, dwFileOffset );
    LogFmtStrA( SPLOGLV_INFO, szInfo );
    
    // 实现修改从UE侧读取的数据，再写入UE测;
    // 注意：根据判断条件确认修改的位置
    if (dwToatlSize == (dwFileSize + dwFileOffset))
    {
        LPFLASH_WRITE_FILE_VALUE lpValue = m_arrWriteFdl2.Find( L"miscdata" );
        if ( NULL != lpValue )
        {
            memcpy_s( lpValue->pFileValue, lpValue->dwFileValueSize, lpReadBuffer, ( *lpValue->pFileSize ) );
        }
    }

    int nCheckOffset = 0;
    if ( 0 == _tcscmp( lpFileID, L"miscdata" ) )
    {
        nCheckOffset = m_nCheckOffset1;
    }
    else
    {
        nCheckOffset = m_nCheckOffset2;
    }
    
    if ( dwFileOffset < ( DWORD )nCheckOffset && ( DWORD )nCheckOffset < ( dwFileOffset + dwFileSize ) )
    {
        sprintf_s( szInfo, "No.%d, FileId:%ls, CheckOffset:0x%x, ReadBuffer:%d", m_nCount, lpFileID, nCheckOffset, lpReadBuffer[nCheckOffset - dwFileOffset] );
        LogFmtStrA( SPLOGLV_INFO, szInfo );
        //The data is passed in in a segmented loop. When the required data is obtained,
        //return SP_E_USER_ABORT to terminate the loop in advance
        return SP_E_USER_ABORT;
    }

    return SP_OK;
}

SPRESULT CFlashOperationImpl::PrepareWriteFlashAfterFDL2( LPFLASH_WRITE_FILE lpWriteFile, INT& nCount )
{
    CTrLog log( this, _T( "WriteFlashAfterFDL2" ) );
    INT nOperation = 1;
    if ( nOperation > nCount )
    {
        LogFmtStr( SPLOGLV_ERROR, _T( "The operation cannot be larger than %d" ), nCount );
        return SP_E_FAIL;
    }
    m_arrWriteFdl2.Clear();
    
    nCount = 0;
    wcscpy_s( lpWriteFile[nCount].szFileId, L"miscdata" );
    lpWriteFile[nCount].dwFileOffset = 0x00;
    lpWriteFile[nCount].dwFileSize = 0x0E;
    memcpy( lpWriteFile[nCount].u8FileValue, "123456789012345", lpWriteFile[nCount].dwFileSize );
    nCount++;
    
    return SP_OK;
    
}

SPRESULT CFlashOperationImpl::PrepareWriteFlashAfterDlEnd( LPFLASH_WRITE_FILE lpWriteFile, INT& nCount )
{
    CTrLog log( this, _T( "WriteFlashAfterDlEnd" ) );
    INT nOperation = 1;
    if ( nOperation > nCount )
    {
        LogFmtStr( SPLOGLV_ERROR, _T( "The operation cannot be larger than %d" ), nCount );
        return SP_E_FAIL;
    }
    m_arrWriteDlEnd.Clear();
    
    nCount = 0;
    wcscpy_s( lpWriteFile[nCount].szFileId, L"miscdata" );
    lpWriteFile[nCount].dwFileOffset = 0x2000;
    lpWriteFile[nCount].dwFileSize = 0x01;
    memcpy( lpWriteFile[nCount].u8FileValue, "123456789012345", lpWriteFile[nCount].dwFileSize );
    nCount++;
    
    return SP_OK;
}

SPRESULT CFlashOperationImpl::PrepareEraseFlashAfterFDL2( LPFLASH_ERASE_FILE lpEraseFile, INT& nCount )
{
    CTrLog log( this, _T( "EraseFlashAfterFDL2" ) );
    /*
    INT nOperation = 1;
    if ( nOperation > nCount )
    {
        LogFmtStr( SPLOGLV_ERROR, _T( "The operation cannot be larger than %d" ), nCount );
        return SP_E_FAIL;
    }
    
    nCount = 0;
    wcscpy_s( lpEraseFile[nCount].szFileId, L"miscdata" );
    nCount++;
    */
    return SP_OK;
}

BOOL CFlashOperationImpl::SaveFileToLocal( LPCSTR pszFileName, LPBYTE pBuf, DWORD dwSize )
{
    FILE* pFile = NULL;
    fopen_s( &pFile, pszFileName, "wt" );
    if ( pFile == NULL )
    {
        return FALSE;
    }
    DWORD dwWrite = fwrite( pBuf, 1, dwSize, pFile );
    fclose( pFile );
    if ( dwWrite != dwSize )
    {
        return FALSE;
    }
    return TRUE;
}

