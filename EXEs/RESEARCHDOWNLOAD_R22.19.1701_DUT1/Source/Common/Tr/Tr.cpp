#include "StdAfx.h"
#include "Tr.h"
#include <assert.h>

//////////////////////////////////////////////////////////////////////////
///
CTr::CTr( void )
    : m_lpTr( NULL )
    , m_bInnerISpLog( TRUE )
{
    ZeroMemory( ( void* )m_szModule, sizeof( m_szModule ) );
}

CTr::~CTr( void )
{
}

void CTr::BindLog( ISpLog* lpOuter, LPCSTR lpszModule, BOOL bCmdPrintf /*= FALSE*/ )
{
    m_bCmdPrintf = bCmdPrintf;
    m_bInnerISpLog = FALSE;
    m_lpTr = lpOuter;
    strncpy_s( m_szModule, ( NULL != lpszModule ) ? lpszModule : "UNKNOWN", CopySize( m_szModule ) );
} 

#if _ISPLOG_INIT
BOOL CTr::trInit(
    ISpLog* lpOuter,
    LPCSTR  lpszModule/*= NULL*/,
    UINT32  nLv/*= SPLOGLV_ERROR*/,
    BOOL    bRollback/* = TRUE*/,
    UINT32  nOpenFlag/* = defaultFlags*/
)
{
    if ( NULL != lpOuter )
    {
        // Outer trace
        BindLog( lpOuter, lpszModule );
    }
    else
    {
        // Inner trace
        CreateISpLogObject( &m_lpTr );
        if ( NULL == m_lpTr )
        {
            return FALSE;
        }
        
        strncpy_s( m_Args.szModule, ( NULL != lpszModule ) ? lpszModule : "UNKNOWN", CopySize( m_Args.szModule ) );
        strncpy_s( m_szModule, m_Args.szModule, CopySize( m_szModule ) );
        m_Args.nLogLevel = nLv;
        m_Args.bBackTrack = bRollback;
        
        m_Args.Local.bLogToFile = TRUE;
        
        WCHAR szModule[MAX_MODULE_NAME_LEN] = { 0 };
        MultiByteToWideChar( CP_ACP, 0, m_Args.szModule, -1, szModule, MAX_MODULE_NAME_LEN );
        swprintf_s( m_Args.Local.szLogFile, L"%s.Log", szModule );
        m_Args.Local.nOpenFlags = nOpenFlag;
        if ( !m_lpTr->Open( ( LPCVOID )&m_Args ) )
        {
            m_lpTr->Release();
            m_lpTr = NULL;
            
            return FALSE;
        }
        
        m_bInnerISpLog = TRUE;
    }
    
    return TRUE;
}

void CTr::trFree( BOOL bWait/*= TRUE*/ )
{
    if ( m_bInnerISpLog )
    {
        // Inner trace: Close & release object
        if ( NULL != m_lpTr )
        {
            m_lpTr->Close( bWait ? 0 : 1 );
            m_lpTr->Release();
            
            m_lpTr = NULL;
        }
    }
    else
    {
        // Outer trace: free & release is in charge of outer module
        m_lpTr = NULL;
    }
}
#endif // _ISPLOG_INIT

BOOL CTr::Open( void )
{
    if ( !m_bInnerISpLog )
    {
        return TRUE;
    }
    else
    {
        if ( NULL == m_lpTr )
        {
            return FALSE;
        }
        
        UINT32 nState = 0;
        m_lpTr->GetProperty( LogProp_OpenState, 0, ( LPVOID )&nState );
        
        if ( nState > 0 )
        {
            LogRawStrA( SPLOGLV_INFO, "iSpLog object is already open!" );
            return TRUE;
        }
        else
        {
            return m_lpTr->Open( ( LPCVOID )&m_Args );
        }
    }
}

void CTr::Close( BOOL bWait )
{
    if ( m_bInnerISpLog )
    {
        if ( NULL != m_lpTr )
        {
            m_lpTr->Close( bWait ? 0 : 1 );
        }
    }
}

BOOL CTr::LogRawStrA( UINT32 nLv, LPCSTR lpszString )
{
    if ( NULL != m_lpTr )
    {
        if ( m_bCmdPrintf )
        {
            printf( "%s\r\n", lpszString );
            fflush( stdout );
        }
        return m_lpTr->LogRawStrA( nLv, lpszString, m_szModule );
    }
    else
    {
        return FALSE;
    }
}

BOOL CTr::LogRawStrW( UINT32 nLv, LPCWSTR lpszString )
{
    if ( NULL != m_lpTr )
    {
        if ( m_bCmdPrintf )
        {
            wprintf( L"%s\r\n", lpszString );
            fflush( stdout );
        }
        return m_lpTr->LogRawStrW( nLv, lpszString, m_szModule );
    }
    else
    {
        return FALSE;
    }
}

BOOL CTr::LogFmtStrA( UINT32 nLv, LPCSTR lpszFmt, ... )
{
    if ( NULL != m_lpTr && NULL != lpszFmt )
    {
        va_list  args = NULL;
        va_start( args, lpszFmt );
        size_t nLength = _vscprintf( lpszFmt, args ) + 1;
        std::vector<CHAR> vBuff( nLength, '\0' );
        if ( _vsnprintf_s( &vBuff[0], vBuff.size(), nLength, lpszFmt, args ) <= 0 )
        {
            va_end( args );
            return FALSE;
        }
        else
        {
            va_end( args );
            return LogRawStrA( nLv, ( LPCSTR )&vBuff[0] );
        }
    }
    else
    {
        return FALSE;
    }
}

BOOL CTr::LogFmtStrW( UINT32 nLv, LPCWSTR lpszFmt, ... )
{
    if ( NULL != m_lpTr && NULL != lpszFmt )
    {
        va_list  args = NULL;
        va_start( args, lpszFmt );
        size_t nLength = _vscwprintf( lpszFmt, args ) + 1;
        std::vector<WCHAR> vBuff( nLength, L'\0' );
        if ( _vsnwprintf_s( &vBuff[0], vBuff.size(), nLength, lpszFmt, args ) <= 0 )
        {
            va_end( args );
            return FALSE;
        }
        else
        {
            va_end( args );
            return LogRawStrW( nLv, ( LPCWSTR )&vBuff[0] );
        }
    }
    else
    {
        return FALSE;
    }
}

BOOL CTr::LogBufData(
    UINT32 nLv,
    const void* lpData,
    UINT32 nDataSize,
    UINT32 nFlag /*= LOG_WRITE*/,
    const UINT32* lpnExpSize /*= NULL*/
)
{
    if ( NULL != m_lpTr )
    {
        return m_lpTr->LogBufData( nLv, lpData, nDataSize, nFlag, lpnExpSize, m_szModule );
    }
    else
    {
        return FALSE;
    }
}