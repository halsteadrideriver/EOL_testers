#include "StdAfx.h"
#include "Utility.h"
#include <assert.h>
#include <tchar.h>
#include <regex>
#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

///
LPCSTR  DEFAULT_DELIMITER_A = ",";
LPCWSTR DEFAULT_DELIMITER_W = L",";

//////////////////////////////////////////////////////////////////////////
CUtility::CUtility( void )
    : m_lpOutputStrA( NULL )
    , m_lpOutputStrW( NULL )
{
}

CUtility::~CUtility( void )
{
    if ( NULL != m_lpOutputStrA )
    {
        delete[]m_lpOutputStrA;
        m_lpOutputStrA = NULL;
    }
    
    if ( NULL != m_lpOutputStrW )
    {
        delete[]m_lpOutputStrW;
        m_lpOutputStrW = NULL;
    }
}

LPCSTR CUtility::_W2CA( LPCWSTR lpszInput )
{
    if ( NULL != lpszInput )
    {
        size_t  nInputLen = wcslen( lpszInput );
        size_t nOutputLen = WideCharToMultiByte( CP_ACP, 0, lpszInput, nInputLen, NULL, 0, 0, 0 ) + 2;
        
        if ( NULL != m_lpOutputStrA )
        {
            delete[]m_lpOutputStrA;
            m_lpOutputStrA = NULL;
        }
        
        try
        {
            m_lpOutputStrA = new CHAR[nOutputLen];
        }
        catch ( const std::bad_alloc& /*e*/ )
        {
            m_lpOutputStrA = NULL;
        }
        
        if ( NULL != m_lpOutputStrA )
        {
            memset( ( void* )m_lpOutputStrA, 0x0, sizeof( CHAR ) * nOutputLen );
            WideCharToMultiByte( CP_ACP, 0, lpszInput, nInputLen, m_lpOutputStrA, nOutputLen, 0, 0 );
            return m_lpOutputStrA;
        }
    }
    
    return NULL;
}

LPCWSTR CUtility::_A2CW( LPCSTR lpszInput )
{
    if ( NULL != lpszInput )
    {
        size_t  nInputLen = strlen( lpszInput );
        size_t nOutputLen = MultiByteToWideChar( CP_ACP, 0, lpszInput, nInputLen, NULL, 0 ) + 2;
        
        if ( NULL != m_lpOutputStrW )
        {
            delete[]m_lpOutputStrW;
            m_lpOutputStrW = NULL;
        }
        
        try
        {
            m_lpOutputStrW = new WCHAR[nOutputLen];
        }
        catch ( const std::bad_alloc& /*e*/ )
        {
            m_lpOutputStrW = NULL;
        }
        
        if ( NULL != m_lpOutputStrW )
        {
            memset( ( void* )m_lpOutputStrW, 0x0, sizeof( WCHAR ) * nOutputLen );
            MultiByteToWideChar( CP_ACP, 0, lpszInput, nInputLen, m_lpOutputStrW, nOutputLen );
            return m_lpOutputStrW;
        }
    }
    
    return NULL;
}

LPSTR* CUtility::GetSegStringA( LPCSTR lpszOrg, LPCSTR lpsczDelimit, INT& nCount )
{
    if ( NULL == lpszOrg || NULL == lpsczDelimit )
    {
        assert( 0 );
        return NULL;
    }
    
    size_t nLen = strlen( lpszOrg );
    m_vecStringA.resize( nLen + 1/*'\0'*/ );
    strcpy_s( &m_vecStringA[0], nLen + 1, lpszOrg );
    m_vecStringA[nLen] = '\0';
    
    m_vecTokenA.clear();
    nCount = 0;
    LPSTR lpszToken = &m_vecStringA[0];
    LPSTR lpNext = NULL;
    do
    {
        lpNext = strstr( lpszToken, lpsczDelimit );
        if ( NULL != lpNext )
        {
            *( ( char* )lpNext ) = '\0';
        }
        m_vecTokenA.push_back( lpszToken );
        nCount++;
        lpszToken = lpNext + 1;
    } while ( NULL != lpNext );
    
    return ( LPSTR* )&m_vecTokenA[0];
}

LPWSTR* CUtility::GetSegStringW( LPWSTR lpszOrg, LPWSTR lpsczDelimit, INT& nCount )
{
    if ( NULL == lpszOrg || NULL == lpsczDelimit )
    {
        assert( 0 );
        return NULL;
    }
    
    size_t nLen = wcslen( lpszOrg );
    m_vecStringW.resize( nLen + 1/*'\0'*/ );
    wcscpy_s( &m_vecStringW[0], nLen + 1, lpszOrg );
    m_vecStringW[nLen] = '\0';
    
    m_vecTokenW.clear();
    nCount = 0;
    LPWSTR lpszToken = &m_vecStringW[0];
    LPWSTR lpNext = NULL;
    do
    {
        lpNext = wcsstr( lpszToken, lpsczDelimit );
        if ( NULL != lpNext )
        {
            *( ( WCHAR* )lpNext ) = L'\0';
        }
        m_vecTokenW.push_back( lpszToken );
        nCount++;
        lpszToken = lpNext + 1;
    } while ( NULL != lpNext );
    
    return ( LPWSTR* )&m_vecTokenW[0];
}

LPSTR* CUtility::GetTokenStringA( LPCSTR lpszOrg, LPCSTR lpsczDelimit, INT& nCount )
{
    if ( NULL == lpszOrg || NULL == lpsczDelimit )
    {
        assert( 0 );
        return NULL;
    }
    
    size_t nLen = strlen( lpszOrg );
    m_vecStringA.resize( nLen + 1/*'\0'*/ );
    strcpy_s( &m_vecStringA[0], nLen + 1, lpszOrg );
    m_vecStringA[nLen] = '\0';
    
    m_vecTokenA.clear();
    nCount = 0;
    
    LPSTR    lpNext = NULL;
    LPSTR lpszToken = strtok_s( ( char* )&m_vecStringA[0], lpsczDelimit, &lpNext );
    if ( NULL == lpszToken )
    {
        return NULL;
    }
    
    while ( NULL != lpszToken )
    {
        if ( ';' == *lpszToken || '#' == *lpszToken )
        {
            //  Ignore comment
            break;
        }
        else
        {
            m_vecTokenA.push_back( lpszToken );
            nCount++;
            
            lpszToken = strtok_s( NULL, lpsczDelimit, &lpNext );
        }
    }
    
    return ( LPSTR* )&m_vecTokenA[0];
}

LPWSTR* CUtility::GetTokenStringW( LPCWSTR lpszOrg, LPCWSTR lpsczDelimit, INT& nCount )
{
    if ( NULL == lpszOrg || NULL == lpsczDelimit )
    {
        assert( 0 );
        return NULL;
    }
    
    size_t nLen = wcslen( lpszOrg );
    m_vecStringW.resize( nLen + 1/*'\0'*/ );
    wcscpy_s( &m_vecStringW[0], nLen + 1, lpszOrg );
    m_vecStringW[nLen] = L'\0';
    
    m_vecTokenW.clear();
    nCount = 0;
    
    LPWSTR    lpNext = NULL;
    LPWSTR lpszToken = wcstok_s( ( wchar_t* )&m_vecStringW[0], lpsczDelimit, &lpNext );
    if ( NULL == lpszToken )
    {
        return NULL;
    }
    
    while ( NULL != lpszToken )
    {
        if ( L';' == *lpszToken || L'#' == *lpszToken )
        {
            //  Ignore comment
            break;
        }
        else
        {
            m_vecTokenW.push_back( lpszToken );
            nCount++;
            
            lpszToken = wcstok_s( NULL, lpsczDelimit, &lpNext );
        }
    }
    
    return ( LPWSTR* )&m_vecTokenW[0];
}

double* CUtility::GetSimpleTokenDoubleA( LPCSTR lpszOrg, LPCSTR lpsczDelimit, INT& nCount )
{
    LPSTR* lppToken = GetTokenStringA( lpszOrg, lpsczDelimit, nCount );
    if ( NULL == lppToken || nCount < 1 )
    {
        return NULL;
    }
    std::vector<std::string> vstr;
    vstr.clear();
    for ( int i = 0; i < nCount; i++ )
    {
        vstr.push_back( lppToken[i] );
    }
    m_vecDouble.clear();
    size_t nPosition1 = 0;
    size_t nPosition2 = 0;
    size_t nPosition3 = 0;
    size_t nPosition4 = 0;
    // Simple Input format :  [start~end:step] [4/14/2017 jian.zhong]
    for ( INT i = 0; i < nCount; i++ )
    {
        std::string strTmp = vstr[i];
        trimA( strTmp );
        nPosition1 = strTmp.find( '[' );
        nPosition2 = strTmp.find( '~' );
        nPosition3 = strTmp.find( ':' );
        nPosition4 = strTmp.find( ']' );
        if ( ( nPosition1 == std::string::npos )
             && ( nPosition2 == std::string::npos )
             && ( nPosition3 == std::string::npos )
             && ( nPosition4 == std::string::npos ) )
        {
            m_vecDouble.push_back( strtod( &strTmp[0], NULL ) );
        }
        else if ( ( nPosition1 != std::string::npos )
                  && ( nPosition2 != std::string::npos )
                  && ( nPosition3 != std::string::npos )
                  && ( nPosition4 != std::string::npos ) )
        {
            double dStart = strtod( &strTmp.substr( nPosition1 + 1, nPosition2 )[0], NULL );
            double dEnd = strtod( &strTmp.substr( nPosition2 + 1, nPosition3 )[0], NULL );
            double dStep = strtod( &strTmp.substr( nPosition3 + 1, nPosition4 )[0], NULL );
            if ( IS_EQUAL( dStep, 0 ) )
            {
                return NULL;
            }
            if ( dStep > 0 )
            {
                while ( dStart <= dEnd )
                {
                    m_vecDouble.push_back( dStart );
                    dStart += dStep;
                }
            }
            else
            {
                while ( dStart >= dEnd )
                {
                    m_vecDouble.push_back( dStart );
                    dStart += dStep;
                }
            }
        }
        else
        {
            return NULL;
        }
    }
    
    nCount = m_vecDouble.size();
    return &m_vecDouble[0];
}

double* CUtility::GetTokenDoubleA( LPCSTR lpszOrg, LPCSTR lpsczDelimit, INT& nCount, double defValue/* = INVALID_POSITIVE_DOUBLE_VALUE*/ )
{
    LPSTR* lppToken = GetTokenStringA( lpszOrg, lpsczDelimit, nCount );
    if ( NULL == lppToken || nCount < 1 )
    {
        return NULL;
    }
    
    m_vecDouble.resize( nCount );
    for ( INT i = 0; i < nCount; i++ )
    {
        m_vecDouble[i] = safe_strtod( lppToken[i], defValue );
        //  m_vecDouble[i] = strtod(lppToken[i], NULL);
    }
    
    return &m_vecDouble[0];
}

double* CUtility::GetSimpleTokenDoubleW( LPCWSTR lpszOrg, LPCWSTR lpsczDelimit, INT& nCount )
{
    LPWSTR* lppToken = GetTokenStringW( lpszOrg, lpsczDelimit, nCount );
    if ( NULL == lppToken || nCount < 1 )
    {
        return NULL;
    }
    std::vector<std::wstring> vWstr;
    vWstr.clear();
    for ( int i = 0; i < nCount; i++ )
    {
        vWstr.push_back( lppToken[i] );
    }
    
    m_vecDouble.clear();
    size_t nPosition1 = 0;
    size_t nPosition2 = 0;
    size_t nPosition3 = 0;
    size_t nPosition4 = 0;
    // Simple Input format :  [start~end:step] [4/14/2017 jian.zhong]
    for ( INT i = 0; i < nCount; i++ )
    {
        std::wstring wstrTmp = vWstr[i];
        trimW( wstrTmp );
        nPosition1 = wstrTmp.find( '[' );
        nPosition2 = wstrTmp.find( '~' );
        nPosition3 = wstrTmp.find( ':' );
        nPosition4 = wstrTmp.find( ']' );
        if ( ( nPosition1 == std::wstring::npos )
             && ( nPosition2 == std::wstring::npos )
             && ( nPosition3 == std::wstring::npos )
             && ( nPosition4 == std::wstring::npos ) )
        {
            m_vecDouble.push_back( wcstod( &wstrTmp[0], NULL ) );
        }
        else if ( ( nPosition1 != std::wstring::npos )
                  && ( nPosition2 != std::wstring::npos )
                  && ( nPosition3 != std::wstring::npos )
                  && ( nPosition4 != std::wstring::npos ) )
        {
            double dStart = wcstod( &wstrTmp.substr( nPosition1 + 1, nPosition2 )[0], NULL );
            double dEnd = wcstod( &wstrTmp.substr( nPosition2 + 1, nPosition3 )[0], NULL );
            double dStep = wcstod( &wstrTmp.substr( nPosition3 + 1, nPosition4 )[0], NULL );
            if ( IS_EQUAL( dStep, 0 ) )
            {
                return NULL;
            }
            if ( dStep > 0 )
            {
                while ( dStart <= dEnd )
                {
                    m_vecDouble.push_back( dStart );
                    dStart += dStep;
                }
            }
            else
            {
                while ( dStart >= dEnd )
                {
                    m_vecDouble.push_back( dStart );
                    dStart += dStep;
                }
            }
        }
        else
        {
            return NULL;
        }
    }
    
    nCount = m_vecDouble.size();
    return &m_vecDouble[0];
}

double* CUtility::GetTokenDoubleW( LPCWSTR lpszOrg, LPCWSTR lpsczDelimit, INT& nCount, double defValue/* = INVALID_POSITIVE_DOUBLE_VALUE*/ )
{
    LPWSTR* lppToken = GetTokenStringW( lpszOrg, lpsczDelimit, nCount );
    if ( NULL == lppToken || nCount < 1 )
    {
        return NULL;
    }
    
    m_vecDouble.resize( nCount );
    for ( INT i = 0; i < nCount; i++ )
    {
        m_vecDouble[i] = safe_wcstod( lppToken[i], defValue );
        //  m_vecDouble[i] = wcstod(lppToken[i], NULL);
    }
    
    return &m_vecDouble[0];
}

INT* CUtility::GetTokenIntegerA( LPCSTR lpszOrg, LPCSTR lpsczDelimit, INT& nCount, INT nBase /* = 10 */, INT defValue/*  = INVALID_POSITIVE_INTEGER_VALUE*/ )
{
    LPSTR* lppToken = GetTokenStringA( lpszOrg, lpsczDelimit, nCount );
    if ( NULL == lppToken || nCount < 1 )
    {
        return NULL;
    }
    
    m_vecInteger.resize( nCount );
    for ( INT i = 0; i < nCount; i++ )
    {
        m_vecInteger[i] = static_cast<INT>( safe_strtol( lppToken[i], nBase, defValue ) );
        //  m_vecInteger[i] = static_cast<INT>(strtol(lppToken[i], NULL, nBase));
    }
    
    return &m_vecInteger[0];
}

INT* CUtility::GetSimpleTokenIntegerA( LPCSTR lpszOrg, LPCSTR lpsczDelimit, INT& nCount, INT nBase /* = 10 */ )
{
    LPSTR* lppToken = GetTokenStringA( lpszOrg, lpsczDelimit, nCount );
    if ( NULL == lppToken || nCount < 1 )
    {
        return NULL;
    }
    std::vector<std::string> vstr;
    vstr.clear();
    for ( int i = 0; i < nCount; i++ )
    {
        vstr.push_back( lppToken[i] );
    }
    
    m_vecInteger.clear();
    size_t nPosition1 = 0;
    size_t nPosition2 = 0;
    size_t nPosition3 = 0;
    size_t nPosition4 = 0;
    // Simple Input format :  [start~end:step] [4/14/2017 jian.zhong]
    for ( INT i = 0; i < nCount; i++ )
    {
        std::string strTmp = vstr[i];
        trimA( strTmp );
        nPosition1 = strTmp.find( '[' );
        nPosition2 = strTmp.find( '~' );
        nPosition3 = strTmp.find( ':' );
        nPosition4 = strTmp.find( ']' );
        if ( ( nPosition1 == std::string::npos )
             && ( nPosition2 == std::string::npos )
             && ( nPosition3 == std::string::npos )
             && ( nPosition4 == std::string::npos ) )
        {
            m_vecInteger.push_back( static_cast<INT>( strtol( &strTmp[0], NULL, nBase ) ) );
        }
        else if ( ( nPosition1 != std::string::npos )
                  && ( nPosition2 != std::string::npos )
                  && ( nPosition3 != std::string::npos )
                  && ( nPosition4 != std::string::npos ) )
        {
            INT nStart = strtol( &strTmp.substr( nPosition1 + 1, nPosition2 )[0], NULL, nBase );
            INT nEnd = strtol( &strTmp.substr( nPosition2 + 1, nPosition3 )[0], NULL, nBase );
            INT nStep = strtol( &strTmp.substr( nPosition3 + 1, nPosition4 )[0], NULL, nBase );
            if ( nStep == 0 )
            {
                return NULL;
            }
            if ( nStep > 0 )
            {
                while ( nStart <= nEnd )
                {
                    m_vecInteger.push_back( nStart );
                    nStart += nStep;
                }
            }
            else
            {
                while ( nStart >= nEnd )
                {
                    m_vecInteger.push_back( nStart );
                    nStart += nStep;
                }
            }
        }
        else
        {
            return NULL;
        }
    }
    
    nCount = m_vecInteger.size();
    return &m_vecInteger[0];
}

INT* CUtility::GetSimpleTokenIntegerW( LPCWSTR lpszOrg, LPCWSTR lpsczDelimit, INT& nCount, INT nBase /* = 10 */ )
{
    LPWSTR* lppToken = GetTokenStringW( lpszOrg, lpsczDelimit, nCount );
    if ( NULL == lppToken || nCount < 1 )
    {
        return NULL;
    }
    std::vector<std::wstring> vWstr;
    vWstr.clear();
    for ( int i = 0; i < nCount; i++ )
    {
        vWstr.push_back( lppToken[i] );
    }
    
    m_vecInteger.clear();
    size_t nPosition1 = 0;
    size_t nPosition2 = 0;
    size_t nPosition3 = 0;
    size_t nPosition4 = 0;
    // Simple Input format :  [start~end:step] [4/14/2017 jian.zhong]
    for ( INT i = 0; i < nCount; i++ )
    {
        std::wstring wstrTmp = vWstr[i];
        trimW( wstrTmp );
        nPosition1 = wstrTmp.find( '[' );
        nPosition2 = wstrTmp.find( '~' );
        nPosition3 = wstrTmp.find( ':' );
        nPosition4 = wstrTmp.find( ']' );
        if ( ( nPosition1 == std::wstring::npos )
             && ( nPosition2 == std::wstring::npos )
             && ( nPosition3 == std::wstring::npos )
             && ( nPosition4 == std::wstring::npos ) )
        {
            m_vecInteger.push_back( static_cast<INT>( wcstol( &wstrTmp[0], NULL, nBase ) ) );
        }
        else if ( ( nPosition1 != std::wstring::npos )
                  && ( nPosition2 != std::wstring::npos )
                  && ( nPosition3 != std::wstring::npos )
                  && ( nPosition4 != std::wstring::npos ) )
        {
            INT nStart = wcstol( &wstrTmp.substr( nPosition1 + 1, nPosition2 )[0], NULL, nBase );
            INT nEnd = wcstol( &wstrTmp.substr( nPosition2 + 1, nPosition3 )[0], NULL, nBase );
            INT nStep = wcstol( &wstrTmp.substr( nPosition3 + 1, nPosition4 )[0], NULL, nBase );
            if ( nStep == 0 )
            {
                return NULL;
            }
            if ( nStep > 0 )
            {
                while ( nStart <= nEnd )
                {
                    m_vecInteger.push_back( nStart );
                    nStart += nStep;
                }
            }
            else
            {
                while ( nStart >= nEnd )
                {
                    m_vecInteger.push_back( nStart );
                    nStart += nStep;
                }
            }
        }
        else
        {
            return NULL;
        }
    }
    
    nCount = m_vecInteger.size();
    return &m_vecInteger[0];
}

INT* CUtility::GetTokenIntegerW( LPCWSTR lpszOrg, LPCWSTR lpsczDelimit, INT& nCount, INT nBase /* = 10 */, INT defValue/*  = INVALID_POSITIVE_INTEGER_VALUE*/ )
{
    LPWSTR* lppToken = GetTokenStringW( lpszOrg, lpsczDelimit, nCount );
    if ( NULL == lppToken || nCount < 1 )
    {
        return NULL;
    }
    
    m_vecInteger.resize( nCount );
    for ( INT i = 0; i < nCount; i++ )
    {
        m_vecInteger[i] = static_cast<INT>( safe_wcstol( lppToken[i], nBase, defValue ) );
        //  m_vecInteger[i] = static_cast<INT>(wcstol(lppToken[i], NULL, nBase));
    }
    
    return &m_vecInteger[0];
}

std::string& CUtility::trimA( std::string& str )
{
    std::string::size_type pos = str.find_last_not_of( ' ' );
    if ( pos != std::string::npos )
    {
        str.erase( pos + 1 );
        pos = str.find_first_not_of( ' ' );
        if ( pos != std::string::npos )
        {
            str.erase( 0, pos );
        }
    }
    else
    {
        str.erase( str.begin(), str.end() );
    }
    
    return str;
}

std::wstring& CUtility::trimW( std::wstring& str )
{
    std::wstring::size_type pos = str.find_last_not_of( L' ' );
    if ( pos != std::wstring::npos )
    {
        str.erase( pos + 1 );
        pos = str.find_first_not_of( L' ' );
        if ( pos != std::wstring::npos )
        {
            str.erase( 0, pos );
        }
    }
    else
    {
        str.erase( str.begin(), str.end() );
    }
    
    return str;
}

std::string CUtility::GetAbsoluteFilePathA( LPCSTR lpszFilePath )
{
    std::string strAbsPath = "";
    if ( NULL == lpszFilePath || '\0' == lpszFilePath[0] )
    {
        return strAbsPath;
    }
    
    if ( PathIsRelativeA( lpszFilePath ) )
    {
        CHAR szAppPath[_MAX_PATH] = { 0 };
        GetModuleFileNameA( NULL, szAppPath, _MAX_PATH );
        LPSTR lpChar = strrchr( szAppPath, '\\' );
        if ( NULL != lpChar )
        {
            *lpChar = '\0';
        }
        
        strAbsPath = ( std::string )szAppPath + "\\" + lpszFilePath;
    }
    else
    {
        strAbsPath = lpszFilePath;
    }
    
    replace_all( strAbsPath, "/", "\\" );
    CONST INT SIZE = strAbsPath.length() + 1;
    LPSTR  pszFile = new CHAR[SIZE];
    memset( pszFile, 0, SIZE );
    strcpy_s( pszFile, SIZE, strAbsPath.c_str() );
    PathCanonicalizeA( pszFile, strAbsPath.c_str() ); // Replace .  or ... in path
    strAbsPath = pszFile;
    delete[]pszFile;
    
    return strAbsPath;
}

std::wstring CUtility::GetAbsoluteFilePathW( LPCWSTR lpszFilePath )
{
    std::wstring strAbsPath = L"";
    if ( NULL == lpszFilePath || L'\0' == lpszFilePath[0] )
    {
        return strAbsPath;
    }
    
    if ( PathIsRelativeW( lpszFilePath ) )
    {
        WCHAR szAppPath[_MAX_PATH] = { 0 };
        GetModuleFileNameW( NULL, szAppPath, _MAX_PATH );
        LPWSTR lpChar = wcsrchr( szAppPath, L'\\' );
        if ( NULL != lpChar )
        {
            *lpChar = L'\0';
        }
        
        strAbsPath = ( std::wstring )szAppPath + L"\\" + lpszFilePath;
    }
    else
    {
        strAbsPath = lpszFilePath;
    }
    
    replace_all_w( strAbsPath, L"/", L"\\" );
    CONST INT SIZE = strAbsPath.length() + 1;
    LPWSTR pszFile = new WCHAR[SIZE];
    memset( pszFile, 0, SIZE * sizeof( WCHAR ) );
    wcscpy_s( pszFile, SIZE, strAbsPath.c_str() );
    PathCanonicalizeW( pszFile, strAbsPath.c_str() ); // Replace .  or ... in path
    strAbsPath = pszFile;
    delete[]pszFile;
    
    return strAbsPath;
}

BOOL CUtility::CreateMultiDirectoryA( LPCSTR lpPath, LPSECURITY_ATTRIBUTES lpSecurityAttributes /*= NULL*/ )
{
    if ( NULL == lpPath || 0 == strlen( lpPath ) )
    {
        return FALSE;
    }
    
    if ( ::PathFileExistsA( lpPath ) || ::PathIsRootA( lpPath ) )
    {
        return TRUE;
    }
    
    CHAR szParent[MAX_PATH] = { 0 };
    strncpy_s( szParent, lpPath, MAX_PATH - 1 );
    ::PathRemoveBackslashA( szParent );
    if ( !::PathRemoveFileSpecA( szParent ) )
    {
        return FALSE;
    }
    
    if ( CreateMultiDirectoryA( szParent, lpSecurityAttributes ) )
    {
        if ( !::CreateDirectoryA( lpPath, lpSecurityAttributes ) )
        {
            if ( ::GetLastError() != ERROR_ALREADY_EXISTS )
            {
                return FALSE;
            }
        }
        
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL CUtility::CreateMultiDirectoryW( LPCWSTR lpPath, LPSECURITY_ATTRIBUTES lpSecurityAttributes /*= NULL*/ )
{
    if ( NULL == lpPath || 0 == wcslen( lpPath ) )
    {
        return FALSE;
    }
    
    if ( ::PathFileExistsW( lpPath ) || ::PathIsRootW( lpPath ) )
    {
        return TRUE;
    }
    
    WCHAR szParent[MAX_PATH] = { 0 };
    wcsncpy_s( szParent, lpPath, MAX_PATH - 1 );
    ::PathRemoveBackslashW( szParent );
    if ( !::PathRemoveFileSpecW( szParent ) )
    {
        return FALSE;
    }
    
    if ( CreateMultiDirectoryW( szParent, lpSecurityAttributes ) )
    {
        if ( !::CreateDirectoryW( lpPath, lpSecurityAttributes ) )
        {
            if ( ::GetLastError() != ERROR_ALREADY_EXISTS )
            {
                return FALSE;
            }
        }
        
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

unsigned short CUtility::Convert16( unsigned short src ) const
{
    return MAKEWORD( HIBYTE( src ), LOBYTE( src ) );
}

unsigned int CUtility::Convert32( unsigned int Src ) const
{
    return MAKELONG( MAKEWORD( HIBYTE( HIWORD( Src ) ), LOBYTE( HIWORD( Src ) ) ), MAKEWORD( HIBYTE( LOWORD( Src ) ), LOBYTE( LOWORD( Src ) ) ) );
}

std::string& CUtility::replace_all( std::string& str, const std::string& old_value, const std::string& new_value )
{
    for ( ; ; )
    {
        std::string::size_type pos( 0 );
        if ( ( pos = str.find( old_value ) ) != std::string::npos )
        {
            str.replace( pos, old_value.length(), new_value );
        }
        else
        {
            break;
        }
    }
    
    return str;
}

std::wstring& CUtility::replace_all_w( std::wstring& str, const std::wstring& old_value, const std::wstring& new_value )
{
    for ( ; ; )
    {
        std::wstring::size_type pos( 0 );
        if ( ( pos = str.find( old_value ) ) != std::wstring::npos )
        {
            str.replace( pos, old_value.length(), new_value );
        }
        else
        {
            break;
        }
    }
    
    return str;
}

std::string& CUtility::replace_all_distinct( std::string& str, const std::string& old_value, const std::string& new_value )
{
    for ( std::string::size_type pos( 0 ); pos != std::string::npos; pos += new_value.length() )
    {
        if ( ( pos = str.find( old_value, pos ) ) != std::string::npos )
        {
            str.replace( pos, old_value.length(), new_value );
        }
        else
        {
            break;
        }
    }
    
    return str;
}

std::wstring& CUtility::replace_all_distinct_w( std::wstring& str, const std::wstring& old_value, const std::wstring& new_value )
{
    for ( std::wstring::size_type pos( 0 ); pos != std::wstring::npos; pos += new_value.length() )
    {
        if ( ( pos = str.find( old_value, pos ) ) != std::wstring::npos )
        {
            str.replace( pos, old_value.length(), new_value );
        }
        else
        {
            break;
        }
    }
    
    return str;
}

double CUtility::safe_strtod( const char* nptr, double defValue/* = INVALID_POSITIVE_DOUBLE_VALUE*/ )
{
    // 以+、-、空白符或者数字开头的字符串
    std::string strTest = nptr;
    std::smatch m;
    std::regex rx( "^\\s*[+-]?[\\d]+" );
    
    if ( NULL != nptr && std::regex_search( strTest, m, rx ) )
    {
        return strtod( nptr, NULL );
    }
    
    return defValue;
}

double CUtility::safe_wcstod( const wchar_t* nptr, double defValue/* = INVALID_POSITIVE_DOUBLE_VALUE*/ )
{
    return safe_strtod( _W2CA( nptr ), defValue );
}

long CUtility::safe_strtol( const char* nptr, int base, long defValue/* = INVALID_POSITIVE_INTEGER_VALUE*/ )
{
    // 以+、-、空白符或者数字开头的字符串
    std::string strTest = nptr;
    std::smatch m;
    std::regex rx( "^\\s*[+-]?[\\d]+" );
    
    if ( NULL != nptr && std::regex_search( strTest, m, rx ) )
    {
        return strtol( nptr, NULL, base );
    }
    
    return defValue;
}

long CUtility::safe_wcstol( const wchar_t* nptr, int base, long defValue/* = INVALID_POSITIVE_INTEGER_VALUE*/ )
{
    return safe_strtol( _W2CA( nptr ), base, defValue );
}

BOOL CUtility::ASCII2Bin(
    const BYTE* pBuffer,//source buffer
    BYTE* pResultBuffer,//dest buffer
    unsigned long ulSize //source buffer length
)
{
    int iCount = 0;
    BYTE l, h;
    while ( ( unsigned long )iCount < ulSize )
    {
        if ( *pBuffer >= '0' && *pBuffer <= '9' )
        {
            h = ( BYTE )( *pBuffer - '0' );
        }
        else
        {
            h = ( BYTE )( *pBuffer - 'a' + 10 );
        }
        h = ( BYTE )( h << 4 );
        pBuffer++;
        if ( *pBuffer >= '0' && *pBuffer <= '9' )
        {
            l = ( BYTE )( *pBuffer - '0' );
        }
        else
        {
            l = ( BYTE )( *pBuffer - 'a' + 10 );
        }
        *pResultBuffer = ( BYTE )( h | l );
        pResultBuffer++;
        pBuffer++;
        iCount += 2;
    }
    return TRUE;
}

BOOL CUtility::Bin2ASCII(
    const BYTE* pBuffer,//source buffer
    BYTE* pResultBuffer,//dest buffer
    unsigned long ulSize //source buffer length
)
{
    const BYTE* pCurBype = pBuffer;
    const BYTE* pEnd = pBuffer + ulSize;
    BYTE* pCurDestBuf = pResultBuffer;
    int iCount = 0;
    while ( pCurBype != pEnd )
    {
        BYTE bCurByte = *pCurBype;
        unsigned char lHigh = ( BYTE )( bCurByte >> 4 );
        if ( lHigh <= 9 )
        {
            *pCurDestBuf = ( BYTE )( lHigh + '0' );
        }
        else
        {
            *pCurDestBuf = ( BYTE )( lHigh - 10 + 'a' );
        }
        ++pCurDestBuf;
        
        bCurByte = *pCurBype;
        
        unsigned char lLow = ( unsigned char )( bCurByte << 4 );
        lLow = ( unsigned char )( lLow >> 4 );
        if ( lLow <= 9 )
        {
            *pCurDestBuf = ( BYTE )( lLow + '0' );
        }
        else
        {
            *pCurDestBuf = ( BYTE )( lLow - 10 + 'a' );
        }
        ++pCurDestBuf;
        
        ++pCurBype;
        ++iCount;
    }
    //    int iOff = pCurDestBuf - pResultBuffer;
    return TRUE;
}

CLinearEquation::CLinearEquation( void )
    : k( INVALID_POSITIVE_DOUBLE_VALUE )
    , b( INVALID_POSITIVE_DOUBLE_VALUE )
{
}

CLinearEquation::~CLinearEquation( void )
{
}

CLinearEquation::CLinearEquation( double k, double b )
{
    this->k = k;
    this->b = b;
}

CLinearEquation::CLinearEquation( double x1, double y1, double x2, double y2 )
{
    if ( x1 != x2 )
    {
        k = ( y1 - y2 ) / ( x1 - x2 );
        b = y1 - k * x1;
    }
    else
    {
        k = INVALID_POSITIVE_DOUBLE_VALUE;
        b = INVALID_POSITIVE_DOUBLE_VALUE;
    }
}

CLinearEquation::CLinearEquation( double k, double x1, double y1 )
{
    this->k = k;
    this->b = y1 - k * x1;
}

double CLinearEquation::operator()( double x )
{
    return x2y( x );
}

double CLinearEquation::x2y( double x )
{
    if ( k == INVALID_POSITIVE_DOUBLE_VALUE )
    {
        return INVALID_POSITIVE_DOUBLE_VALUE;
    }
    else
    {
        return ( k * x + b );
    }
}

double CLinearEquation::y2x( double y )
{
    if ( k == 0 )
    {
        return INVALID_POSITIVE_DOUBLE_VALUE;
    }
    else
    {
        return ( ( y - b ) / k );
    }
}

BOOL CUtility::IsExistFileA( LPCSTR lpszFileFullPath )
{
    DWORD dwAttributes = ::GetFileAttributesA( lpszFileFullPath );
    return ( dwAttributes != ( ( DWORD ) -1 ) ) && ( ( dwAttributes & FILE_ATTRIBUTE_DIRECTORY ) == 0 );
}