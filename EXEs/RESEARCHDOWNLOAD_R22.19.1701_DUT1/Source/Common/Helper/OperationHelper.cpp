#include "stdafx.h"
#include "OperationHelper.h"
#include "IConfig.h"
#include "Utility.h"
#include <algorithm>
#define MAX_BUFF_SIZE         (1024*20)

#if _MSC_VER >= 1300    // for VC 7.0 
// from ATL 7.0 sources 
#ifndef _delayimp_h
extern "C" IMAGE_DOS_HEADER __ImageBase;
#endif 
#endif  

COperationHelper::COperationHelper()
{

}

COperationHelper::~COperationHelper()
{

}

HMODULE COperationHelper::GetCurrentModule()
{
#if _MSC_VER < 1300    // earlier than .NET compiler (VC 6.0)           
    // Here's a trick that will get you the handle of the module
    // you're running in without any a-priori knowledge:
    MEMORY_BASIC_INFORMATION mbi;
    static int dummy;
    VirtualQuery( &dummy, &mbi, sizeof( mbi ) );
    
    return reinterpret_cast<HMODULE>( mbi.AllocationBase );
#else // VC 7.0     
    // from ATL 7.0 sources
    return reinterpret_cast<HMODULE>( &__ImageBase );
#endif
}

BOOL COperationHelper::GetModuleFilePath( HMODULE hModule, LPTSTR lpszAppPath, DWORD dwSize )
{
    TCHAR szPath[_MAX_PATH] = { 0 };
    TCHAR szDrive[_MAX_DRIVE] = { 0 };
    TCHAR szDir[_MAX_DIR] = { 0 };
    TCHAR szFname[_MAX_FNAME] = { 0 };
    TCHAR szExt[_MAX_EXT] = { 0 };
    if ( 0 == GetModuleFileName( hModule, szPath, _countof( szPath ) ) )
    {
        return FALSE;
    }
    _tsplitpath_s( szPath, szDrive, szDir, szFname, szExt );
    _stprintf_s( lpszAppPath, dwSize, L"%s%s", szDrive, szDir );
    return TRUE;
}

BOOL COperationHelper::GetFilePathInfo( LPCTSTR lpszFile, CStringArray& agInfo )
{
    agInfo.RemoveAll();
    if ( lpszFile == NULL )
    {
        return FALSE;
    }
    
    CString strFilePath = lpszFile;
    int nIndx = strFilePath.ReverseFind( '\\' );
    CString strFileName = strFilePath;
    CString strExt = _T( "" );
    if ( nIndx != -1 )
    {
        strFileName = strFilePath.Right( strFilePath.GetLength() - nIndx - 1 );
        strFilePath = strFilePath.Left( nIndx );
    }
    else
    {
        strFilePath.Empty();
    }
    
    nIndx = strFileName.ReverseFind( '.' );
    if ( nIndx != -1 )
    {
        strExt = strFileName.Right( strFileName.GetLength() - nIndx );
        strFileName = strFileName.Left( nIndx );
    }
    
    agInfo.Add( strFilePath );
    agInfo.Add( strFileName );
    agInfo.Add( strExt );
    
    return TRUE;
}

BOOL COperationHelper::CreateDeepDirectory( LPCTSTR lpszDir )
{
    if ( lpszDir == NULL )
    {
        return FALSE;
    }
    
    CString         strDir = lpszDir;
    HANDLE          hFile;
    WIN32_FIND_DATA fileinfo;
    CStringArray    agDir;
    BOOL            bOK;
    int             nCount = 0;
    CString         strTemp = _T( "" );
    
    hFile = FindFirstFile( strDir, &fileinfo );
    
    // if the file exists and it is a directory
    if ( fileinfo.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY )
    {
        //     Directory   Exists   close   file   and   return
        FindClose( hFile );
        return   TRUE;
    }
    agDir.RemoveAll();
    for ( nCount = 0; nCount < strDir.GetLength(); nCount++ )
    {
        if ( strDir.GetAt( nCount ) != _T( '\\' ) )
        {
            strTemp += strDir.GetAt( nCount );
        }
        else
        {
            agDir.Add( strTemp );
            strTemp += _T( "\\" );
        }
        if ( nCount == strDir.GetLength() - 1 )
        {
            agDir.Add( strTemp );
        }
    }
    
    
    //   Close   the   file
    FindClose( hFile );
    
    
    // Now lets cycle through the String Array and create each directory in turn
    for ( nCount = 1; nCount < agDir.GetSize(); nCount++ )
    {
        strTemp = agDir.GetAt( nCount );
        bOK = CreateDirectory( strTemp, NULL );
        
        // If the Directory exists it will return a false
        if ( bOK )
        {
            // If we were successful we set the attributes to normal
            SetFileAttributes( strTemp, FILE_ATTRIBUTE_NORMAL );
        }
        else
        {
            if ( GetLastError() != ERROR_ALREADY_EXISTS )
            {
                agDir.RemoveAll();
                return FALSE;
            }
        }
        
    }
    // Now lets see if the directory was successfully created
    hFile = FindFirstFile( strDir, &fileinfo );
    
    agDir.RemoveAll();
    if ( fileinfo.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY )
    {
        // Directory Exists close file and return
        FindClose( hFile );
        return   TRUE;
    }
    else
    {
        // For Some reason the Function Failed Return FALSE
        FindClose( hFile );
        return   FALSE;
    }
}

BOOL COperationHelper::SaveFileToLocal( LPCTSTR pszFileName, LPBYTE pBuf, DWORD dwSize )
{
    CString strFilePath( pszFileName );
    FILE* pFile = NULL;
    _tfopen_s( &pFile, strFilePath, _T( "wb" ) );
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

BOOL COperationHelper::DeleteDirectory( LPCTSTR lpszDirName, BOOL bRmFolder )
{
    if ( lpszDirName == NULL )
    {
        ASSERT( 0 );
        return FALSE;
    }
    
    CFileFind tempFind;
    BOOL IsFinded = TRUE;
    IsFinded = ( BOOL )tempFind.FindFile( lpszDirName );
    if ( !IsFinded )
    {
        tempFind.Close();
        return TRUE;
    }
    tempFind.Close();
    
    DWORD dwAtt = 0;
    _TCHAR tempFileFind[MAX_PATH] = { 0 };
    _stprintf_s( tempFileFind, MAX_PATH, _T( "%s\\*.*" ), lpszDirName );
    IsFinded = ( BOOL )tempFind.FindFile( tempFileFind );
    while ( IsFinded )
    {
        IsFinded = ( BOOL )tempFind.FindNextFile();
        if ( !tempFind.IsDots() )
        {
            CString foundFileName;
            foundFileName = tempFind.GetFileName();
            if ( tempFind.IsDirectory() )
            {
                _TCHAR tempDir[MAX_PATH] = { 0 };
                _stprintf_s( tempDir, MAX_PATH, _T( "%s\\%s" ), lpszDirName, foundFileName.operator LPCWSTR() );
                DeleteDirectory( tempDir );
            }
            else
            {
                _TCHAR tempFileName[MAX_PATH] = { 0 };
                _stprintf_s( tempFileName, MAX_PATH, _T( "%s\\%s" ), lpszDirName, foundFileName.operator LPCWSTR() );
                dwAtt = 0;
                dwAtt = ::GetFileAttributes( tempFileName );
                dwAtt &= ~FILE_ATTRIBUTE_READONLY;
                ::SetFileAttributes( tempFileName, dwAtt );
                DeleteFile( tempFileName );
            }
        }
    }
    tempFind.Close();
    dwAtt = 0;
    dwAtt = ::GetFileAttributes( lpszDirName );
    dwAtt &= ~FILE_ATTRIBUTE_READONLY;
    ::SetFileAttributes( lpszDirName, dwAtt );
    if ( bRmFolder )
    {
        RemoveDirectory( lpszDirName );
    }
    return TRUE;
}

BOOL COperationHelper::GetAbsolutePath( HMODULE hModule, LPCTSTR lpszFilePath, CString& strAbsoluteFilePath )
{
    ASSERT( lpszFilePath );
    if ( lpszFilePath == NULL )
    {
        return FALSE;
    }
    
    TCHAR szExt[_MAX_EXT] = { 0 };
    TCHAR szFileName[_MAX_FNAME] = { 0 };
    TCHAR szDir[_MAX_DIR] = { 0 };
    TCHAR szDirve[_MAX_DRIVE] = { 0 };
    
    _tsplitpath_s( lpszFilePath, szDirve, _MAX_DRIVE, szDir, _MAX_DIR, szFileName, _MAX_FNAME, szExt, _MAX_EXT );
    if ( szDirve[0] != _T( '\0' ) || ( _tcslen( lpszFilePath ) > 2 && lpszFilePath[0] == _T( '\\' ) && lpszFilePath[1] == _T( '\\' ) ) )
    {
        strAbsoluteFilePath = lpszFilePath;
    }
    else
    {
        //do it if strHelpTopic is ralatively
        GetModuleFileName( hModule, szFileName, _MAX_FNAME );
        _tsplitpath_s( szFileName, szDirve, _MAX_DRIVE, szDir, _MAX_DIR, szFileName, _MAX_FNAME, szExt, _MAX_EXT );
        strAbsoluteFilePath = szDirve;
        strAbsoluteFilePath += szDir;
        if ( lpszFilePath[0] == _T( '\\' ) || lpszFilePath[0] == _T( '/' ) )
        {
            lpszFilePath++;
        }
        
        strAbsoluteFilePath += lpszFilePath;
        
    }
    return TRUE;
}

CString COperationHelper::GetStrFileTitle( LPCTSTR lpFile, BOOL bMakeUpper, BOOL bPostfix )
{
    CString str( lpFile );
    
    if ( str.Find( _T( '\\' ) ) != -1 )
    {
        str = str.Right( str.GetLength() - str.ReverseFind( _T( '\\' ) ) - 1 );
    }
    else if ( str.Find( _T( '/' ) ) != -1 )
    {
        str = str.Right( str.GetLength() - str.ReverseFind( _T( '/' ) ) - 1 );
    }
    
    if ( !bPostfix )
    {
        if ( str.ReverseFind( _T( '.' ) ) != -1 )
        {
            str = str.Left( str.ReverseFind( _T( '.' ) ) );
        }
    }
    
    if ( bMakeUpper )
    {
        str.MakeUpper();
    }
    return str;
}

CString COperationHelper::GetStrFilePath( LPCTSTR lpFile, BOOL bMakeUpper, BOOL bPostfix )
{
    CString str( lpFile );
    if ( str.Find( _T( '\\' ) ) != -1 )
    {
        str = bPostfix ? str.Left( str.ReverseFind( _T( '\\' ) ) + 1 ) : str.Left( str.ReverseFind( _T( '\\' ) ) );
    }
    else if ( str.Find( _T( '/' ) ) != -1 )
    {
        str = bPostfix ? str.Left( str.ReverseFind( _T( '/' ) ) + 1 ) : str.Left( str.ReverseFind( _T( '/' ) ) );
    }
    if ( bMakeUpper )
    {
        str.MakeUpper();
    }
    return str;
}

LPBYTE COperationHelper::GetKeyDataFromString( LPCTSTR pszBuf, DWORD dwCount, int& nLen )
{
    TCHAR szBuf[MAX_BUF_SIZE * 4] = { 0 };
    int nCount = ( int )dwCount;
    while ( nCount > 0 )
    {
        _tcscpy_s( szBuf, MAX_BUF_SIZE * 4, pszBuf );
        UINT nCurCount = _tcslen( pszBuf );
        pszBuf += nCurCount + 1;
        nCount -= nCurCount + 1;
    }
    CString strBuf = szBuf;
    strBuf.MakeLower();
    strBuf.Replace( _T( "\r" ), _T( "" ) );
    strBuf.Replace( _T( "\n" ), _T( "" ) );
    strBuf.Replace( _T( " " ), _T( "" ) );
    strBuf.Replace( _T( "," ), _T( "" ) );
    strBuf.Replace( _T( "\t" ), _T( "" ) );
    strBuf.Replace( _T( "0x" ), _T( "" ) );
    nLen = strBuf.GetLength() / 2;
    
    if ( nLen == 0 )
    {
        return NULL;
    }
    
    LPBYTE pData = new BYTE[nLen];
    memset( pData, 0, sizeof( BYTE )*nLen );
    
    TCHAR szByte[3] = { 0 };
    for ( int i = 0; i < nLen; i++ )
    {
        szByte[0] = strBuf.GetAt( 2 * i );
        szByte[1] = strBuf.GetAt( 2 * i + 1 );
        
        DWORD dwData = 0;
        
        _stscanf_s( szByte, _T( "%02x" ), &dwData );
        pData[i] = ( BYTE )dwData;
    }
    
    return pData;
    
}


uint64 COperationHelper::GetFileSizeEx( LPCTSTR lpFileName )
{
    if ( NULL == lpFileName )
    {
        ASSERT( 0 );
        return 0;
    }
    
    HANDLE           hResult = NULL;
    WIN32_FIND_DATA  FindFileData;
    uint64           llSize = 0;
    FindFileData.nFileSizeHigh = FindFileData.nFileSizeLow = 0;
    
    hResult = FindFirstFile( lpFileName, &FindFileData );
    if ( hResult != INVALID_HANDLE_VALUE )
    {
        llSize = FindFileData.nFileSizeHigh;
        llSize = llSize << 32;
        llSize |= FindFileData.nFileSizeLow;
        FindClose( hResult );
        hResult = NULL;
    }
    return llSize;
}

void COperationHelper::GetLastModifyTime( LPCTSTR lpszFile, FILETIME& ft )
{
    ZeroMemory( &ft, sizeof( ft ) );
    if ( lpszFile && _tcslen( lpszFile ) )
    {
        WIN32_FIND_DATA fd = { 0 };
        HANDLE hFind = NULL;
        hFind = FindFirstFile( lpszFile, &fd );
        if ( hFind )
        {
            FindClose( hFind );
            ft = fd.ftLastWriteTime;
        }
    }
}

int COperationHelper::GetDLFiles( BOOL bCheckMatch, PPRODUCT_INFO_T pProductInfo, MAP_DLFILE* pMapDLFiles, MAP_DLSTATE* pMapDLState, CStringArray& agFiles )
{
    agFiles.RemoveAll();
    
    if ( pProductInfo == NULL )
    {
        return 0;
    }
    
    UINT nCount = pProductInfo->dwFileCount;
    for ( UINT i = 0; i < nCount; i++ )
    {
        CString strID = pProductInfo->pFileInfoArr[i].szID;
        strID.MakeUpper();
        BOOL bCheck = FALSE;
        CString strFile;
        pMapDLFiles->Lookup( strID, strFile );
        pMapDLState->Lookup( strID, bCheck );
        
        if ( bCheckMatch && 0 == strID.CompareNoCase( _T( "EraseUBOOT" ) ) ) //need to skip EraseUBOOT when Check matching
        {
            bCheck = FALSE;
        }
        
        if ( bCheck )
        {
            agFiles.Add( strFile );
        }
        else
        {
            agFiles.Add( FILE_OMIT );
        }
    }
    return nCount;
}

int COperationHelper::GetAllFileInfo( PPRODUCT_INFO_T pProductInfo, LPDWORD ppFileInfo )
{
    int nCount = 0;
    PFILE_INFO_T pFileInfo = NULL;
    
    if ( pProductInfo == NULL )
    {
        return -1;
    }
    
    nCount = pProductInfo->dwFileCount;
    pFileInfo = pProductInfo->pFileInfoArr;
    
    if ( ppFileInfo != NULL )
    {
        *ppFileInfo = ( DWORD )pFileInfo;
    }
    
    return nCount;
}

int COperationHelper::GetFileInfo( PPRODUCT_INFO_T pProductInfo, LPCTSTR lpszFileID, LPDWORD ppFileInfo )
{
    if ( lpszFileID == NULL || pProductInfo == NULL )
    {
        return -1;
    }
    
    int nCount = 0;
    PFILE_INFO_T pFileInfo = NULL;
    nCount = pProductInfo->dwFileCount;
    pFileInfo = pProductInfo->pFileInfoArr;
    
    int i = 0;
    CString strID;
    for ( i = 0; i < nCount; i++ )
    {
        strID = ( pFileInfo + i )->szID;
        if ( strID.CompareNoCase( lpszFileID ) == 0 )
        {
            if ( ppFileInfo != NULL )
            {
                *ppFileInfo = ( DWORD )( pFileInfo + i );
            }
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

CString COperationHelper::GetFilePostfix( LPCTSTR lpFile, BOOL bMakeUpper )
{
    CString str( lpFile );
    
    if ( str.ReverseFind( _T( '.' ) ) != -1 )
    {
        str = str.Right( str.GetLength() - str.ReverseFind( _T( '.' ) ) - 1 );
    }
    
    if ( bMakeUpper )
    {
        str.MakeUpper();
    }
    return str;
}

BOOL COperationHelper::GetFdl2Flag( LPCTSTR lpszFilePath, UINT nType, CString& strErrMsg )
{
    if ( lpszFilePath == NULL || _tcslen( lpszFilePath ) == 0 )
    {
        return TRUE;
    }
    strErrMsg = _T( "" );
    
    CFileFind finder;
    if ( !finder.FindFile( lpszFilePath ) )
    {
        return FALSE;
    }
    
    CFile file;
    CFileException fe;
    BOOL bRet = file.Open( lpszFilePath, CFile::modeRead | CFile::shareDenyWrite, &fe );
    
    if ( !bRet )
    {
        // Can not open file
        CString strTmp;
        LPTSTR p = strTmp.GetBuffer( _MAX_PATH );
        fe.GetErrorMessage( p, _MAX_PATH );
        strTmp.ReleaseBuffer();
        strErrMsg.Format( _T( "[DL3026][E]Can not open file: %s; ErrMsg: %s" ), lpszFilePath, strTmp.operator LPCTSTR() );
        return FALSE;
    }
    
    LPBYTE lpContent = NULL;
    int nLen = 0;
    
    try
    {
        nLen = ( int )file.GetLength();
        lpContent = new BYTE[nLen];
        file.Read( lpContent, nLen );
    }
    catch ( CFileException& ex )
    {
        CString strTmp;
        LPTSTR p = strTmp.GetBuffer( _MAX_PATH );
        ex.GetErrorMessage( p, _MAX_PATH );
        strTmp.ReleaseBuffer();
        if ( lpContent != NULL )
        {
            delete[]lpContent;
        }
        strErrMsg.Format( _T( "[DL3026][E]Can not open file: %s; ErrMsg: %s" ), lpszFilePath, strTmp.operator LPCTSTR() );
        return FALSE;
    }
    
    const char szOmadm[] = "#$DEVICE_MANAGER$#";
    const char szPreload[] = "#*PRELOADSUPPORT*#";
    const char szKernelImg2[] = "#*DEMANDPAGING*#";
    const char szRomDisk[] = "#*USBCOMAUTORUN*#";
    
    BYTE* lpPos = NULL;
    if ( nType == FDL2F_OMADM )
    {
        lpPos = std::search( lpContent, lpContent + nLen, szOmadm, szOmadm + strlen( szOmadm ) - 1 );
    }
    else if ( nType == FDL2F_PRELOAD )
    {
        lpPos = std::search( lpContent, lpContent + nLen, szPreload, szPreload + strlen( szPreload ) - 1 );
    }
    else if ( nType == FDL2F_KERNELIMG2 )
    {
        lpPos = std::search( lpContent, lpContent + nLen, szKernelImg2, szKernelImg2 + strlen( szKernelImg2 ) - 1 );
    }
    else if ( nType == FDL2F_ROMDISK )
    {
        lpPos = std::search( lpContent, lpContent + nLen, szRomDisk, szRomDisk + strlen( szRomDisk ) - 1 );
    }
    else
    {
        delete[]lpContent;
        return FALSE;
    }
    
    if ( lpPos == lpContent + nLen )
    {
        // not find the version string
        delete[]lpContent;
        return FALSE;
    }
    else
    {
        delete[]lpContent;
        return TRUE;
    }
}

__int64 COperationHelper::GetPartitionSize( LPCTSTR lpPartName, PPRODUCT_INFO_T pProdInfo )
{
    if ( NULL == lpPartName || NULL == pProdInfo || NULL == pProdInfo->pPartitions )
    {
        ASSERT( 0 );
        return 0;
    }
    __int64 llPartSize = 0;
    PARTITION_T* pPartitions = pProdInfo->pPartitions;
    for ( DWORD i = 0; i < pProdInfo->dwPartitionCount; ++i )
    {
        PARTITION_T* pPartItem = pPartitions + i;
        if ( pPartItem && 0 == _tcscmp( lpPartName, pPartItem->szID ) )
        {
        
            if ( 0xFFFFFFFF == ( 0xFFFFFFFF & pPartItem->llSize ) )
            {
                llPartSize = pPartItem->llSize;
                
            }
            else
            {
                llPartSize = pPartItem->llSize * 1024 * 1024;
            }
            
            break;
        }
        
    }
    return llPartSize;
    
}

BOOL COperationHelper::GetSection( IConfig* pConfig, DLFW_CFGTYPE_ENUM eType, LPCTSTR lpSection, CStringArray& arrString, BOOL bAddKey/* = FALSE*/ )
{
    if ( NULL == pConfig || NULL == lpSection )
    {
        ASSERT( 0 );
        return FALSE;
    }
    arrString.RemoveAll();
    
    INT nCount = 0;
    CUtility utility;
    PTCHAR pszValue = new TCHAR[MAX_BUFF_SIZE];
    PTCHAR pszToken = pszValue;
    memset( pszValue, 0, sizeof( TCHAR )*MAX_BUFF_SIZE );
    pConfig->GetSection( eType, lpSection, pszValue, MAX_BUFF_SIZE );
    while ( true )
    {
        size_t len = _tcsclen( pszToken );
        if ( 0 == len )
        {
            break;
        }
        
        LPTSTR* lppString = utility.GetTokenString( pszToken, _T( "=" ), nCount );
        if ( NULL != lppString && 2 == nCount )
        {
            CString strText = lppString[1];
            if ( !strText.IsEmpty() )
            {
                if ( bAddKey )
                {
                    arrString.Add( lppString[0] );
                }
                arrString.Add( strText );
            }
        }
        pszToken += len + 1;
    }
    delete[]pszValue;
    return TRUE;
}

BOOL COperationHelper::GetSectionMcp( IConfig* pConfig, CStringArray& arrString, MAP_MCP& mapMcp )
{
    if ( NULL == pConfig )
    {
        ASSERT( 0 );
        return FALSE;
    }
    
    mapMcp.RemoveAll();
    arrString.RemoveAll();
    
    INT nCount = 0;
    CUtility utility;
    PTCHAR pszValue = new TCHAR[MAX_BUFF_SIZE];
    PTCHAR pszToken = pszValue;
    memset( pszValue, 0, sizeof( TCHAR ) * MAX_BUFF_SIZE );
    pConfig->GetSection( CFGTYPE_MCPT, _T( "MCPTypeList" ), pszValue, MAX_BUFF_SIZE );
    while ( true )
    {
        size_t len = _tcsclen( pszToken );
        if ( 0 == len )
        {
            break;
        }
        
        LPTSTR* lppString = utility.GetTokenString( pszToken, _T( "=" ), nCount );
        if ( NULL != lppString && 2 == nCount )
        {
            CString strTmp = _T( "" );
            if ( !mapMcp.Lookup( lppString[0], strTmp ) )
            {
                arrString.Add( lppString[0] );
            }
            mapMcp.SetAt( lppString[0], lppString[1] );
        }
        pszToken += len + 1;
    }
    delete[]pszValue;
    
    return TRUE;
}

BOOL COperationHelper::GetSectionImeiId( IConfig* pConfig, CUIntArray& arrUInt )
{
    if ( NULL == pConfig )
    {
        ASSERT( 0 );
        return FALSE;
    }
    
    INT nCount = 0;
    CUtility utility;
    PTCHAR pszValue = new TCHAR[MAX_BUF_SIZE];
    PTCHAR pszToken = pszValue;
    memset( pszValue, 0, sizeof( TCHAR )*MAX_BUF_SIZE );
    pConfig->GetSection( CFGTYPE_BMFT, _T( "IMEI_ID" ), pszValue, MAX_BUF_SIZE );
    while ( true )
    {
        size_t len = _tcsclen( pszToken );
        if ( 0 == len )
        {
            break;
        }
        
        LPTSTR* lppString = utility.GetTokenString( pszToken, _T( "=" ), nCount );
        if ( NULL != lppString && 2 == nCount )
        {
            CString strTmp = lppString[1];
            UINT uID = 0;
            strTmp.MakeLower();
            if ( strTmp.Find( _T( "0x" ) ) == 0 )
            {
                _stscanf_s( strTmp.operator LPCTSTR(), _T( "0x%X" ), &uID );
            }
            else
            {
                uID = _ttoi( strTmp.operator LPCTSTR() );
            }
            if ( uID > 0 && uID < 0xFFFF )
            {
                arrUInt.Add( uID );
            }
        }
        pszToken += len + 1;
    }
    delete[]pszValue;
    
    return TRUE;
}

SPRESULT COperationHelper::ConfigAttrDataByBackupNvItems( MAP_NV_BAK& mapNvBkpItm, LPDLFW_ATTRDATA_COMMON lpData )
{
    uint32 u32Count = mapNvBkpItm.GetCount();
    if ( NULL == lpData || u32Count > lpData->u32Size )
    {
        ASSERT( 0 );
        return SP_E_INVALID_PARAMETER;
    }
    
    lpData->u32Size = u32Count;
    uint32 i = 0;
    MAP_NV_BAK::CPair* pCurVal;
    pCurVal = mapNvBkpItm.PGetFirstAssoc();
    while ( pCurVal )
    {
        lpData->lppData[i] = ( PVOID )pCurVal->value;
        lpData->lppReserved[i] = ( PVOID )pCurVal->key.operator LPCWSTR();
        pCurVal = mapNvBkpItm.PGetNextAssoc( pCurVal );
        i++;
    }
    return SP_OK;
}

SPRESULT COperationHelper::ConfigAttrDataByBackupFiles( VEC_FILE_INFO& arrBackFiles, LPDLFW_ATTRDATA_COMMON lpData )
{
    uint32 u32Count = arrBackFiles.size();
    if ( NULL == lpData || u32Count > lpData->u32Size )
    {
        ASSERT( 0 );
        return SP_E_INVALID_PARAMETER;
    }
    
    lpData->u32Size = u32Count;
    for ( size_t i = 0; i < u32Count; i++ )
    {
        lpData->lppData[i] = ( PVOID )&arrBackFiles[i];
    }
    return SP_OK;
    
}

SPRESULT COperationHelper::ConfigAttrDataByProductInfo( VEC_PRODUCT_INFO& arrProductInfo, LPDLFW_ATTRDATA_COMMON lpData )
{
    uint32 u32Count = arrProductInfo.size();
    if ( NULL == lpData || u32Count > lpData->u32Size )
    {
        ASSERT( 0 );
        return SP_E_INVALID_PARAMETER;
    }
    lpData->u32Size = u32Count;
    for ( size_t i = 0; i < u32Count; i++ )
    {
        lpData->lppData[i] = ( PVOID )arrProductInfo[i];
    }
    return SP_OK;
}

SPRESULT COperationHelper::ConfigAttrDataByProductName( CStringArray& arrProductName, LPDLFW_ATTRDATA_COMMON lpData )
{
    uint32 u32Count = arrProductName.GetCount();
    if ( NULL == lpData || u32Count > lpData->u32Size )
    {
        ASSERT( 0 );
        return SP_E_INVALID_PARAMETER;
    }
    
    lpData->u32Size = u32Count;
    for ( size_t i = 0; i < u32Count; i++ )
    {
        lpData->lppData[i] = ( PVOID )arrProductName.GetAt( i ).operator LPCWSTR();
    }
    return SP_OK;
}

SPRESULT COperationHelper::ConfigAttrDataByDlFileName( MAP_DLFILE& mapDLFiles, LPDLFW_ATTRDATA_DLIMAGEINFO lpData )
{
    uint32 u32Count = mapDLFiles.GetCount();
    if ( NULL == lpData || u32Count > lpData->u32Size )
    {
        ASSERT( 0 );
        return SP_E_INVALID_PARAMETER;
    }
    
    lpData->u32Size = u32Count;
    uint32 i = 0;
    MAP_DLFILE::CPair* pCurVal;
    pCurVal = mapDLFiles.PGetFirstAssoc();
    while ( pCurVal )
    {
        lpData->lppID[i] = ( LPTSTR )pCurVal->key.operator LPCWSTR();
        lpData->lppName[i] = ( LPTSTR )pCurVal->value.operator LPCWSTR();
        pCurVal = mapDLFiles.PGetNextAssoc( pCurVal );
        i++;
    }
    return SP_OK;
}

SPRESULT COperationHelper::ConfigAttrDataByDlFileState( MAP_DLSTATE& mapDLState, LPDLFW_ATTRDATA_DLIMAGEINFO lpData )
{
    uint32 u32Count = mapDLState.GetCount();
    if ( NULL == lpData || u32Count > lpData->u32Size )
    {
        ASSERT( 0 );
        return SP_E_INVALID_PARAMETER;
    }
    
    lpData->u32Size = u32Count;
    uint32 i = 0;
    MAP_DLSTATE::CPair* pCurVal;
    pCurVal = mapDLState.PGetFirstAssoc();
    while ( pCurVal )
    {
        lpData->lppID[i] = ( LPTSTR )pCurVal->key.operator LPCWSTR();
        lpData->pn32Flag[i] = pCurVal->value;
        pCurVal = mapDLState.PGetNextAssoc( pCurVal );
        i++;
    }
    return SP_OK;
}

SPRESULT COperationHelper::ConfigAttrDataByDlFileSize( MAP_DLSIZE& mapDLSize, LPDLFW_ATTRDATA_DLIMAGEINFO lpData )
{
    uint32 u32Count = mapDLSize.GetCount();
    if ( NULL == lpData || u32Count > lpData->u32Size )
    {
        ASSERT( 0 );
        return SP_E_INVALID_PARAMETER;
    }
    
    lpData->u32Size = u32Count;
    uint32 i = 0;
    MAP_DLSIZE::CPair* pCurVal;
    pCurVal = mapDLSize.PGetFirstAssoc();
    while ( pCurVal )
    {
        lpData->lppID[i] = ( LPTSTR )pCurVal->key.operator LPCWSTR();
        lpData->pu64Size[i] = pCurVal->value;
        pCurVal = mapDLSize.PGetNextAssoc( pCurVal );
        i++;
    }
    return SP_OK;
}

SPRESULT COperationHelper::ConfigAttrDataByDlDataInfo( MAP_DATAINFO& mapDataInfo, LPDLFW_ATTRDATA_DLIMAGEINFO lpData )
{
    uint32 u32Count = mapDataInfo.GetCount();
    if ( NULL == lpData || u32Count > lpData->u32Size )
    {
        ASSERT( 0 );
        return SP_E_INVALID_PARAMETER;
    }
    
    lpData->u32Size = u32Count;
    uint32 i = 0;
    MAP_DATAINFO::CPair* pCurVal;
    pCurVal = mapDataInfo.PGetFirstAssoc();
    while ( pCurVal )
    {
        lpData->lppName[i] = ( LPTSTR )pCurVal->key.operator LPCWSTR();
        lpData->pu64Size[i] = pCurVal->value.llSize;
        lpData->pu64Offset[i] = pCurVal->value.llOffset;
        pCurVal = mapDataInfo.PGetNextAssoc( pCurVal );
        i++;
    }
    return SP_OK;
}

SPRESULT COperationHelper::ConfigAttrDataByCUIntArray( CUIntArray& arrUInt, LPDLFW_ATTRDATA_COMMON lpData )
{
    uint32 u32Count = arrUInt.GetCount();
    if ( NULL == lpData || u32Count > lpData->u32Size )
    {
        ASSERT( 0 );
        return SP_E_INVALID_PARAMETER;
    }
    
    lpData->u32Size = u32Count;
    for ( size_t i = 0; i < u32Count; i++ )
    {
        lpData->lppData[i] = ( PVOID )( arrUInt[i] );
    }
    return SP_OK;
}

SPRESULT COperationHelper::ConfigAttrDataByLcdConfig( VEC_LCD_CFIG& arrLCDCfig, LPDLFW_ATTRDATA_COMMON lpData )
{
    uint32 u32Count = arrLCDCfig.size();
    if ( NULL == lpData || u32Count > lpData->u32Size )
    {
        ASSERT( 0 );
        return SP_E_INVALID_PARAMETER;
    }
    
    lpData->u32Size = u32Count;
    for ( size_t i = 0; i < u32Count; i++ )
    {
        lpData->lppData[i] = ( PVOID )&arrLCDCfig[i];
    }
    return SP_OK;
}

SPRESULT COperationHelper::ConfigAttrDataByProjectConfig( VEC_PROJECT_CONFIG& arrProjectCfg, LPDLFW_ATTRDATA_COMMON lpData )
{
    uint32 u32Count = arrProjectCfg.size();
    if ( NULL == lpData || u32Count > lpData->u32Size )
    {
        ASSERT( 0 );
        return SP_E_INVALID_PARAMETER;
    }
    
    lpData->u32Size = u32Count;
    for ( size_t i = 0; i < u32Count; i++ )
    {
        lpData->lppData[i] = ( PVOID )&arrProjectCfg[i];
    }
    return SP_OK;
}

SPRESULT COperationHelper::ConfigAttrDataByPreloadConfig( VEC_PRELOAD_CONFIG& arrPreloadCfg, LPDLFW_ATTRDATA_COMMON lpData )
{
    uint32 u32Count = arrPreloadCfg.size();
    if ( NULL == lpData || u32Count > lpData->u32Size )
    {
        ASSERT( 0 );
        return SP_E_INVALID_PARAMETER;
    }

    lpData->u32Size = u32Count;
    for ( size_t i = 0; i < u32Count; i++ )
    {
        lpData->lppData[i] = ( PVOID )&arrPreloadCfg[i];
    }
    return SP_OK;
}

SPRESULT COperationHelper::ConfigProductNameByAttrData( CStringArray& arrProductName, LPDLFW_ATTRDATA_COMMON lpData )
{
    if ( NULL == lpData ||  NULL == lpData->lppData )
    {
        ASSERT( 0 );
        return SP_E_INVALID_PARAMETER;
    }
    
    arrProductName.RemoveAll();
    for ( size_t i = 0; i < lpData->u32Size; i++ )
    {
        arrProductName.Add( ( LPTSTR )lpData->lppData[i] );
    }
    return SP_OK;
}

SPRESULT COperationHelper::ConfigProductInfoByAttrData( VEC_PRODUCT_INFO& arrProductInfo, LPDLFW_ATTRDATA_COMMON lpData )
{
    if ( NULL == lpData || NULL == lpData->lppData )
    {
        ASSERT( 0 );
        return SP_E_INVALID_PARAMETER;
    }
    
    arrProductInfo.clear();
    for ( size_t i = 0; i < lpData->u32Size; i++ )
    {
        arrProductInfo.push_back( ( PPRODUCT_INFO_T )lpData->lppData[i] );
    }
    return SP_OK;
}

SPRESULT COperationHelper::ConfigDlFileNameByAttrData( MAP_DLFILE& mapDLFiles, LPDLFW_ATTRDATA_DLIMAGEINFO lpData )
{
    if ( NULL == lpData ||  NULL == lpData->lppID || NULL == lpData->lppName )
    {
        ASSERT( 0 );
        return SP_E_INVALID_PARAMETER;
    }
    
    mapDLFiles.RemoveAll();
    for ( size_t i = 0; i < lpData->u32Size; i++ )
    {
        mapDLFiles.SetAt( lpData->lppID[i], lpData->lppName[i] );
    }
    return SP_OK;
}

SPRESULT COperationHelper::ConfigDlFileStateByAttrData( MAP_DLSTATE& mapDLState, LPDLFW_ATTRDATA_DLIMAGEINFO lpData )
{
    if ( NULL == lpData ||  NULL == lpData->lppID || NULL == lpData->pn32Flag )
    {
        ASSERT( 0 );
        return SP_E_INVALID_PARAMETER;
    }
    
    mapDLState.RemoveAll();
    for ( size_t i = 0; i < lpData->u32Size; i++ )
    {
        mapDLState.SetAt( lpData->lppID[i], lpData->pn32Flag[i] );
    }
    return SP_OK;
}

SPRESULT COperationHelper::ConfigDlFileSizeByAttrData( MAP_DLSIZE& mapDLSize, LPDLFW_ATTRDATA_DLIMAGEINFO lpData )
{
    if ( NULL == lpData || NULL == lpData->lppID || NULL == lpData->pu64Size )
    {
        ASSERT( 0 );
        return SP_E_INVALID_PARAMETER;
    }
    
    mapDLSize.RemoveAll();
    for ( size_t i = 0; i < lpData->u32Size; i++ )
    {
        mapDLSize.SetAt( lpData->lppID[i], lpData->pu64Size[i] );
    }
    return SP_OK;
}

SPRESULT COperationHelper::ConfigDlDataInfoByAttrData( MAP_DATAINFO& mapDataInfo, LPDLFW_ATTRDATA_DLIMAGEINFO lpData )
{
    if ( NULL == lpData || NULL == lpData->pu64Size || NULL == lpData->pu64Offset || NULL == lpData->lppName )
    {
        ASSERT( 0 );
        return SP_E_INVALID_PARAMETER;
    }
    
    mapDataInfo.RemoveAll();
    for ( size_t i = 0; i < lpData->u32Size; i++ )
    {
        DATA_INFO_T info;
        info.llSize = lpData->pu64Size[i];
        info.llOffset = lpData->pu64Offset[i];
        mapDataInfo.SetAt( lpData->lppName[i], info );
    }
    return SP_OK;
}

void COperationHelper::CreatCommonData( uint32 u32Size, LPDLFW_ATTRDATA_COMMON lpData )
{
    if ( NULL != lpData )
    {
        lpData->u32Size = u32Size;
        lpData->lppData = new PVOID[u32Size];
        lpData->lppReserved = new PVOID[u32Size];
    }
}

void COperationHelper::CreatDlImageInfo( uint32 u32Size, LPDLFW_ATTRDATA_DLIMAGEINFO lpData )
{
    if ( NULL != lpData )
    {
        lpData->u32Size = u32Size;
        lpData->lppID = new LPTSTR[u32Size];
        lpData->lppName = new LPTSTR[u32Size];
        lpData->pn32Flag = new BOOL[u32Size];
        lpData->pu64Size = new uint64[u32Size];
        lpData->pu64Offset = new uint64[u32Size];
    }
}

void COperationHelper::ReleaseCommonData( LPDLFW_ATTRDATA_COMMON lpData )
{
    if ( NULL != lpData )
    {
        if ( NULL != lpData->lppData )
        {
            delete[]lpData->lppData;
            lpData->lppData = NULL;
        }
        if ( NULL != lpData->lppReserved )
        {
            delete[]lpData->lppReserved;
            lpData->lppReserved = NULL;
        }
    }
}

void COperationHelper::ReleaseDlImageInfo( LPDLFW_ATTRDATA_DLIMAGEINFO lpData )
{
    if ( NULL != lpData )
    {
        if ( NULL != lpData->lppID )
        {
            delete[]lpData->lppID;
            lpData->lppID = NULL;
        }
        if ( NULL != lpData->lppName )
        {
            delete[]lpData->lppName;
            lpData->lppName = NULL;
        }
        if ( NULL != lpData->pn32Flag )
        {
            delete[]lpData->pn32Flag;
            lpData->pn32Flag = NULL;
        }
        if ( NULL != lpData->pu64Size )
        {
            delete[]lpData->pu64Size;
            lpData->pu64Size = NULL;
        }
        if ( NULL != lpData->pu64Offset )
        {
            delete[]lpData->pu64Offset;
            lpData->pu64Offset = NULL;
        }
    }
}

SPRESULT COperationHelper::ConfigBackupNvItemsByAttrData( MAP_NV_BAK& mapNvBkpItm, LPDLFW_ATTRDATA_COMMON lpData )
{
    if ( NULL == lpData || NULL == lpData->lppData || NULL == lpData->lppReserved )
    {
        ASSERT( 0 );
        return SP_E_INVALID_PARAMETER;
    }
    
    mapNvBkpItm.RemoveAll();
    for ( size_t i = 0; i < lpData->u32Size; i++ )
    {
        mapNvBkpItm.SetAt( ( LPCTSTR )lpData->lppReserved[i], ( PNV_BACKUP_INFO_T )lpData->lppData[i] );
    }
    return SP_OK;
}

SPRESULT COperationHelper::ConfigBackupFilesByAttrData( VEC_FILE_INFO& arrBackFiles, LPDLFW_ATTRDATA_COMMON lpData )
{
    if ( NULL == lpData || NULL == lpData->lppData )
    {
        ASSERT( 0 );
        return SP_E_INVALID_PARAMETER;
    }
    
    arrBackFiles.clear();
    for ( size_t i = 0; i < lpData->u32Size; i++ )
    {
        FILE_INFO_T info;
        memcpy( &info, ( PFILE_INFO_T )lpData->lppData[i], sizeof( FILE_INFO_T ) );
        arrBackFiles.push_back( info );
    }
    return SP_OK;
}


SPRESULT COperationHelper::ConfigCUIntArrayByAttrData( CUIntArray& arrUInt, LPDLFW_ATTRDATA_COMMON lpData )
{
    if ( NULL == lpData ||  NULL == lpData->lppData )
    {
        ASSERT( 0 );
        return SP_E_INVALID_PARAMETER;
    }
    
    arrUInt.RemoveAll();
    for ( size_t i = 0; i < lpData->u32Size; i++ )
    {
        arrUInt.Add( ( UINT )lpData->lppData[i] );
    }
    return SP_OK;
}

SPRESULT COperationHelper::ConfigLcdConfigByAttrData( VEC_LCD_CFIG& arrLCDCfig, LPDLFW_ATTRDATA_COMMON lpData )
{
    if ( NULL == lpData || NULL == lpData->lppData )
    {
        ASSERT( 0 );
        return SP_E_INVALID_PARAMETER;
    }
    
    arrLCDCfig.clear();
    for ( size_t i = 0; i < lpData->u32Size; i++ )
    {
        LCD_ITEM lcd;
        memcpy( &lcd, ( LPLCD_ITEM )lpData->lppData[i], sizeof( LCD_ITEM ) );
        arrLCDCfig.push_back( lcd );
    }
    return SP_OK;
}

SPRESULT COperationHelper::ConfigProjectConfigByAttrData( VEC_PROJECT_CONFIG& arrPorjectCfg, LPDLFW_ATTRDATA_COMMON lpData )
{
    if ( NULL == lpData || NULL == lpData->lppData )
    {
        ASSERT( 0 );
        return SP_E_INVALID_PARAMETER;
    }
    
    arrPorjectCfg.clear();
    for ( size_t i = 0; i < lpData->u32Size; i++ )
    {
        PROJECT_CONFIG cfg;
        memcpy( &cfg, ( LPPROJECT_CONFIG )lpData->lppData[i], sizeof( PROJECT_CONFIG ) );
        arrPorjectCfg.push_back( cfg );
    }
    return SP_OK;
}

SPRESULT COperationHelper::ConfigPreloadConfigByAttrData( VEC_PRELOAD_CONFIG& arrPreloadCfg, LPDLFW_ATTRDATA_COMMON lpData )
{
    if ( NULL == lpData || NULL == lpData->lppData )
    {
        ASSERT( 0 );
        return SP_E_INVALID_PARAMETER;
    }

    arrPreloadCfg.clear();
    for ( size_t i = 0; i < lpData->u32Size; i++ )
    {
        PRELOAD_CONFIG cfg;
        memcpy( &cfg, ( LPPROJECT_CONFIG )lpData->lppData[i], sizeof( PRELOAD_CONFIG ) );
        arrPreloadCfg.push_back( cfg );
    }
    return SP_OK;
}

SPRESULT COperationHelper::ClearMapNvBakItems( MAP_NV_BAK& mapNvBkpItm )
{
    CString strID;
    PNV_BACKUP_INFO_T  pStruct = NULL;
    POSITION pos = mapNvBkpItm.GetStartPosition();
    while ( pos )
    {
        mapNvBkpItm.GetNextAssoc( pos, strID, pStruct );
        if ( pStruct )
        {
            pStruct->Clear();
            SAFE_DELETE( pStruct );
        }
    }
    mapNvBkpItm.RemoveAll();
    return SP_OK;
}

SPRESULT COperationHelper::UpdatedNvBkpMap( MAP_NV_BAK& mapNvBkpItm, LPCTSTR lpID, PNV_BACKUP_INFO_T pNVBakItemInfo )
{
    if ( NULL == pNVBakItemInfo || NULL == lpID || 0 == _tcslen( lpID ) )
    {
        return SP_E_INVALID_PARAMETER;
    }
    NV_BACKUP_INFO_T* pTempNVBakInfo;
    CString strID( lpID );
    if ( mapNvBkpItm.Lookup( strID, pTempNVBakInfo ) && pTempNVBakInfo )
    {
        pTempNVBakInfo->Clear();
        SAFE_DELETE( pTempNVBakInfo );
        mapNvBkpItm.RemoveKey( strID );
    }
    
    NV_BACKUP_INFO_T* pNVBakInfo = new NV_BACKUP_INFO_T;
    if ( pNVBakInfo )
    {
        memcpy( pNVBakInfo, pNVBakItemInfo, sizeof( NV_BACKUP_INFO_T ) );
        pNVBakInfo->pNvBkpItmArray = new NV_BACKUP_ITEM_T[pNVBakItemInfo->nNvBkpItmCount];
    }
    
    if ( NULL == pNVBakInfo || NULL == pNVBakInfo->pNvBkpItmArray )
    {
        SAFE_DELETE( pNVBakInfo );
        return SP_E_INVALID_PARAMETER;
    }
    
    memcpy( pNVBakInfo->pNvBkpItmArray, pNVBakItemInfo->pNvBkpItmArray, sizeof( NV_BACKUP_ITEM_T ) * ( pNVBakItemInfo->nNvBkpItmCount ) );
    mapNvBkpItm.SetAt( strID, pNVBakInfo );
    return SP_OK;
}

int COperationHelper::GetNvBkpItemCount( MAP_NV_BAK& mapNvBkpItm, LPCTSTR lpNvID )
{
    int nCount = 0;
    NV_BACKUP_INFO_T* pNvBkpInfo = NULL;
    if ( mapNvBkpItm.Lookup( lpNvID, pNvBkpInfo ) && pNvBkpInfo )
    {
        nCount = pNvBkpInfo->nNvBkpItmCount;
    }
    return nCount;
}

PNV_BACKUP_ITEM_T COperationHelper::GetNvBkpItemInfo( MAP_NV_BAK& mapNvBkpItm, int nIndex, LPCTSTR lpNvID )
{
    int nUper = GetNvBkpItemCount( mapNvBkpItm, lpNvID );
    if ( nIndex < 0 || nIndex >= nUper )
    {
        ASSERT( 0 );
        return NULL;
    }
    PNV_BACKUP_ITEM_T pNvBkpItm = NULL;
    NV_BACKUP_INFO_T* pNvBkpInfo = NULL;
    if ( mapNvBkpItm.Lookup( lpNvID, pNvBkpInfo ) && pNvBkpInfo )
    {
        pNvBkpItm = pNvBkpInfo->pNvBkpItmArray + nIndex;
    }
    return pNvBkpItm;
}