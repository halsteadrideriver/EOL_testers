#pragma once
#include "ISpLogExport.h"

//////////////////////////////////////////////////////////////////////////
///
class CTr
{
    CLASS_UNCOPYABLE( CTr );
    
public:
    CTr( void );
    virtual ~CTr( void );
    
    ///
    /// Get ISpLog object
    ///
    ISpLog* GetISpLogObject( void )const { return m_lpTr; };
    
    /// Binding ISpLog Object
    void    BindLog( ISpLog* lpOuter, LPCSTR lpszModule, BOOL bCmdPrintf = FALSE );
    
    ///
    /// Initialize & Free
    ///
#if _ISPLOG_INIT
    BOOL    trInit(
        ISpLog* lpOuter,
        LPCSTR  lpszModule = NULL,
        UINT32  nLv = SPLOGLV_ERROR,
        BOOL    bRollback = TRUE,
        UINT32  nOpenFlag = splog::defaultFlags );
        
    void    trFree( BOOL bWait = TRUE/*Wait all logs flush first and then free*/ );
#endif // _ISPLOG_INIT
    
    BOOL    Open( void );
    void    Close( BOOL bWait = TRUE );
    
    ///
    /// Tr Interface: LogRawStrA/LogRawStrW
    ///
    BOOL    LogRawStrA( UINT32 nLv, LPCSTR  lpszString );
    BOOL    LogRawStrW( UINT32 nLv, LPCWSTR lpszString );
    
    ///
    /// Tr Interface: LogFmtStrA/LogFmtStrW
    ///
    BOOL    LogFmtStrA( UINT32 nLv, LPCSTR  lpszFmt, ... );
    BOOL    LogFmtStrW( UINT32 nLv, LPCWSTR lpszFmt, ... );
    
    ///
    /// Tr Interface: LogBufData
    ///
    BOOL    LogBufData( UINT32 nLv, const void* lpData, UINT32 nDataSize, UINT32 nFlag = LOG_WRITE, const UINT32* lpnExpSize = NULL );
    
private:
    ISpLog*    m_lpTr;
    BOOL       m_bInnerISpLog;
    CHAR       m_szModule[MAX_MODULE_NAME_LEN];
    OpenArgs_T m_Args;
    BOOL       m_bCmdPrintf = FALSE;
};


//////////////////////////////////////////////////////////////////////////

#include <TCHAR.h>
#include <vector>

class CTrLog sealed
{
public:
    CTrLog( CTr* lpTr, LPCTSTR lpszFnName, LPCTSTR lpszFmt = NULL, ... )
        : m_lpTr( lpTr )
        , m_sFnName( lpszFnName )
    {
        if ( NULL != m_lpTr )
        {
            if ( NULL == lpszFmt )
            {
                m_lpTr->LogFmtStr( SPLOGLV_INFO, _T( "[ ENTER ] -> %s( )" ), m_sFnName.c_str() );
            }
            else
            {
                va_list args = NULL;
                va_start( args, lpszFmt );
                size_t nLength = _vsctprintf( lpszFmt, args ) + 1;
                std::vector<TCHAR> vBuff( nLength, _T( '\0' ) );
                if ( _vsntprintf_s( &vBuff[0], vBuff.size(), nLength, lpszFmt, args ) > 0 )
                {
                    m_lpTr->LogFmtStr( SPLOGLV_INFO, _T( "[ ENTER ] -> %s( %s )" ), m_sFnName.c_str(), ( LPCTSTR )&vBuff[0] );
                }
                va_end( args );
            }
        }
    }
    
    ~CTrLog( void )
    {
        if ( NULL != m_lpTr )
        {
            m_lpTr->LogFmtStr( SPLOGLV_INFO, _T( "[ LEAVE ] <- %s( )" ), m_sFnName.c_str() );
            m_lpTr = NULL;
        }
    }
    
private:
    CTr* m_lpTr;
    string_t m_sFnName;
};

