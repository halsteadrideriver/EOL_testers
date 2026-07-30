#include "StdAfx.h"
#include "AppVer.h"
#include <exception>
#include <wchar.h>
#include <assert.h>
#pragma comment(lib, "version.lib")

//////////////////////////////////////////////////////////////////////////
CAppVer::CAppVer( void )
    : m_lpVerInfo( NULL )
    , m_lpAppVer( NULL )
    , m_lpVendor( NULL )
    , m_lpFileName( NULL )
    , m_lpProVer( NULL )
{
}

CAppVer::~CAppVer( void )
{
    if ( NULL != m_lpVerInfo )
    {
        delete []m_lpVerInfo;
        m_lpVerInfo = NULL;
    }
}

BOOL CAppVer::Init( HMODULE hApp )
{
    WCHAR szAppPath[MAX_PATH] = {0};
    GetModuleFileNameW( hApp, szAppPath, MAX_PATH );
    return InitEx( szAppPath );
}

BOOL CAppVer::InitEx( LPCWSTR lpszFileName )
{
    if ( NULL == lpszFileName )
    {
        assert( 0 );
        return FALSE;
    }
    
    DWORD dwSize    = 0;
    DWORD dwHandle  = 0;
    dwSize = ::GetFileVersionInfoSizeW( lpszFileName, &dwHandle );
    if ( 0 == dwSize )
    {
        return FALSE;
    }
    
    if ( NULL !=  m_lpVerInfo )
    {
        delete []m_lpVerInfo;
        m_lpVerInfo = NULL;
    }
    m_lpVerInfo = new WCHAR[dwSize + 1];
    
    BOOL bRet = ::GetFileVersionInfoW( lpszFileName, dwHandle, dwSize, m_lpVerInfo );
    if ( !bRet )
    {
        delete []m_lpVerInfo;
        m_lpVerInfo = NULL;
        return FALSE;
    }
    
    struct LANGANDCODEPAGE
    {
        WORD wLanguage;
        WORD wCodePage;
    } *lpTranslate;
    
    
    UINT cbTranslate = 0;
    
    // Read the list of languages and code pages.
    VerQueryValueW( m_lpVerInfo,  L"\\VarFileInfo\\Translation", ( LPVOID* )&lpTranslate, &cbTranslate );
    
    wchar_t    szSubBlock[MAX_PATH] = {0};
    swprintf_s( szSubBlock, MAX_PATH, L"\\StringFileInfo\\%04x%04x\\FileVersion", \
                lpTranslate[0].wLanguage, lpTranslate[0].wCodePage );
                
    // Retrieve file description for language and code page "i".
    VerQueryValueW( m_lpVerInfo, szSubBlock, ( LPVOID* )&m_lpAppVer, &cbTranslate );
    
    swprintf_s( szSubBlock, MAX_PATH, L"\\StringFileInfo\\%04x%04x\\ProductVersion", \
                lpTranslate[0].wLanguage, lpTranslate[0].wCodePage );
    VerQueryValueW( m_lpVerInfo, szSubBlock, ( LPVOID* )&m_lpProVer, &cbTranslate );
    
    swprintf_s( szSubBlock, MAX_PATH, L"\\StringFileInfo\\%04x%04x\\CompanyName", \
                lpTranslate[0].wLanguage, lpTranslate[0].wCodePage );
    VerQueryValueW( m_lpVerInfo, szSubBlock, ( LPVOID* )&m_lpVendor, &cbTranslate );
    
    swprintf_s( szSubBlock, MAX_PATH, L"\\StringFileInfo\\%04x%04x\\OriginalFilename", \
                lpTranslate[0].wLanguage, lpTranslate[0].wCodePage );
    VerQueryValueW( m_lpVerInfo, szSubBlock, ( LPVOID* )&m_lpFileName, &cbTranslate );
    
    WCHAR* lpChar = NULL;
    while ( NULL != ( lpChar = wcsstr( m_lpAppVer, L"," ) ) )
    {
        *lpChar = L'.';
    }
    
    return TRUE;
}

