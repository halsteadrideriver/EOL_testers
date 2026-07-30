#include "StdAfx.h"
#include "CallbackHelper.h"
#include "GlobalHelper.h"
#include "Download.h"

extern CConsoleHelper g_fnConsoleHelper;
extern CEvnetHelper g_fnEventHelper;
extern CDownload g_fnDownload;
extern CMD_PARAMS g_tCmdParams;

//////////////////////////////////////////////////////////////////////////
CCallbackHelper::CCallbackHelper( void )
{
}

CCallbackHelper::~CCallbackHelper( void )
{
}

BOOL CCallbackHelper::HandleCallbackData( LPCSPCALLBACK_DATA lpcbData )
{
    if ( NULL == lpcbData )
    {
        assert( 0 );
        return FALSE;
    }
    
    switch ( lpcbData->eType )
    {
    case CALLBACK_LOAD_PACKET_PROGRESS:
        return __cbHandleLoadPacketProgress( LPCCALLBACKDATA_LOAD_PACKET_PROGRESS( lpcbData->lpData ) );
    case CALLBACK_PACKET_INFO:
        return __cbHandlePacketInfo( LPCCALLBACKDATA_PACKET_INFO( lpcbData->lpData ) );
    case CALLBACK_STEP_INFO:
        return __cbHandleStepInfo( LPCCALLBACKDATA_STEP_INFO( lpcbData->lpData ) );
    case CALLBACK_STRING_INFO:
        return __cbHandleStringInfo( LPCCALLBACKDATA_STRING_INFO( lpcbData->lpData ) );
    case CALLBACK_DL_END:
        return __cbHandleDlEnd( LPCCALLBACKDATA_DL_END( lpcbData->lpData ) );
    case CALLBACK_DL_IMAGE_BEGIN:
        return __cbHandleDlImageBegin( LPCCALLBACKDATA_DL_IMAGE_SIZE( lpcbData->lpData ) );
    case CALLBACK_PROGRESS_INFO:
        return __cbHandleProgressInfo( LPCCALLBACKDATA_PROGRESS_INFO( lpcbData->lpData ) );
    case CALLBACK_DL_STEP_DESCRIPTION:
        return __cbHandleDlStepDescription( LPCCALLBACKDATA_DL_STEP_DESCRIPTION( lpcbData->lpData ) );
    default:
        break;
    }
    
    return TRUE;
}

BOOL CCallbackHelper::__cbHandleStringInfo( LPCCALLBACKDATA_STRING_INFO lpcbData )
{
    if ( NULL == lpcbData )
    {
        assert( 0 );
        return FALSE;
    }
    switch ( lpcbData->eType )
    {
    case CBK_STRING_ERROR_MSG:
        {
            if ( !g_tCmdParams.bEZMode )
            {
                _tprintf( _T( "%s %s\n" ), KEY_ERROR, lpcbData->lpInfo );
            }
        }
        break;
    case CBK_STRING_WARN_MSG:
        {
            if ( !g_tCmdParams.bEZMode )
            {
                _tprintf( _T( "%s %s\n" ), KEY_WARNING, lpcbData->lpInfo );
            }
        }
        break;
    case CBK_STRING_USB_INSERT:
        {
            m_nPort = lpcbData->u32Port;
            if ( !g_tCmdParams.bEZMode )
            {
                _tprintf( DEVICE_FROMART_ALL, KEY_INSERT_DEV, lpcbData->u32Port, lpcbData->lpInfo );
            }
            SetEvent( g_fnEventHelper.m_hEventDevInsert );
        }
        break;
    case CBK_STRING_USB_REMOVE:
        {
            if ( !g_tCmdParams.bEZMode )
            {
                _tprintf( DEVICE_FROMART_ALL, KEY_REMOVE_DEV, lpcbData->u32Port, lpcbData->lpInfo );
            }
            SetEvent( g_fnEventHelper.m_hEventDevRemove );
        }
        break;
    case CBK_STRING_IMEI:
        {
            m_strIMEI.clear();
            m_strIMEI = lpcbData->lpInfo;
        }
        break;
    case CBK_STRING_SN1:
        {
            m_strSN1.clear();
            m_strSN1 = lpcbData->lpInfo;
        }
        break;
    case CBK_STRING_SN2:
        {
            m_strSN2.clear();
            m_strSN2 = lpcbData->lpInfo;
        }
        break;
    default:
        break;
    }
    fflush( stdout );
    return TRUE;
}

BOOL CCallbackHelper::__cbHandleDlImageBegin( LPCCALLBACKDATA_DL_IMAGE_SIZE lpcbData )
{
    if ( NULL == lpcbData )
    {
        assert( 0 );
        return FALSE;
    }
    
    m_u32StepStartTime = GetTickCount();
    m_dImageSize = *lpcbData->pu64Size / 1024.0 / 1024.0;
    m_u64TotalSize += *lpcbData->pu64Size;
    m_u32CurProgress = START_PROGRESS;
    m_u32CurTime = 0;
    if ( m_u32Step < m_arrFileID.size() )
    {
        m_strStep = m_arrFileID[m_u32Step];
        m_u32Step++;
        if ( !g_tCmdParams.bEZMode )
        {
            TCHAR szBuff[256] = { 0 };
            _stprintf_s( szBuff, DLIMAGE_FROMART_MS, m_u32Step - 1, m_strStep.c_str(), 1, m_dImageSize, 0 );
            _tprintf( szBuff );
            fflush( stdout );
        }
    }
    
    return TRUE;
}

BOOL CCallbackHelper::__cbHandleStepInfo( LPCCALLBACKDATA_STEP_INFO lpcbData )
{
    if ( NULL == lpcbData )
    {
        assert( 0 );
        return FALSE;
    }
    switch ( lpcbData->eType )
    {
    case CBK_STEP_DL_BEGIN:
        {
            if ( !g_tCmdParams.bEZMode )
            {
                printf( "Start Downloading...\n" );
                fflush( stdout );
            }
            m_u32Step = 0;
            m_u64TotalSize = 0;
            m_strSN1.clear();
            m_strSN2.clear();
            m_strIMEI.clear();
            m_u32StartTime = GetTickCount();
        }
        break;
    case CBK_STEP_DL_IMAGE_END:
        {
            if ( !g_tCmdParams.bEZMode )
            {
                uint32 u32WaitTime = ( uint32 )( GetTickCount() - m_u32StepStartTime );
                if ( m_u32CurProgress != 100 )
                {
                    TCHAR szBuff[256] = { 0 };
                    _stprintf_s( szBuff, u32WaitTime > 1000 ? DLIMAGE_FROMART_S : DLIMAGE_FROMART_MS, m_u32Step - 1, m_strStep.c_str(), 100, m_dImageSize, u32WaitTime > 1000 ? ( uint32 )( u32WaitTime / 1000 ) : u32WaitTime );
                    wstring strBuff = g_fnConsoleHelper.GetConsoleOutput();
                    if ( strBuff.length() > _tcsclen( szBuff ) )
                    {
                        g_fnConsoleHelper.SetConsole();
                        printf( "%-100s\n", " " );
                    }
                    
                    g_fnConsoleHelper.SetConsole();
                    m_u32CurProgress = 100;
                    _tprintf( szBuff );
                    fflush( stdout );
                }
            }
            else
            {
                printf( "[Status] %d%%\n", ( m_u32Step * 100 ) / m_arrFileID.size() );
                fflush( stdout );
            }
        }
        break;
    case CBK_STEP_REQUIRE_SN:
        {
            g_fnDownload.SetBarcode( lpcbData->u32Port );
        }
        break;
    default:
        break;
    }
    
    return TRUE;
}

BOOL CCallbackHelper::__cbHandleDlEnd( LPCCALLBACKDATA_DL_END lpcbData )
{
    if ( NULL == lpcbData )
    {
        assert( 0 );
        return FALSE;
    }
    
    memcpy_s( &m_tDlEnd, sizeof( m_tDlEnd ), lpcbData, sizeof( m_tDlEnd ) );
    
    if ( !g_tCmdParams.bEZMode )
    {
        printf( "End Download.\n\n" );
    }
    
    if ( 0 != m_nPort )
    {
        _tprintf( _T( "PORT = COM%d\n" ), m_nPort );
    }
    if ( !m_strSN1.empty() )
    {
        _tprintf( _T( "SN1 = %s\n" ), m_strSN1.c_str() );
    }
    if ( !m_strSN2.empty() )
    {
        _tprintf( _T( "SN2 = %s\n" ), m_strSN2.c_str() );
    }
    if ( !m_strIMEI.empty() )
    {
        _tprintf( _T( "IMEI = %s\n" ), m_strIMEI.c_str() );
    }
    if ( !m_strPacName.empty() )
    {
        _tprintf( _T( "Product Name = %s\n" ), m_strPacName.c_str() );
    }
    if ( !m_strPacVersion.empty() )
    {
        _tprintf( _T( "Software Version = %s\n" ), m_strPacVersion.c_str() );
    }
    
    uint32 u32Time = ( uint32 )( GetTickCount() - m_u32StartTime );
    double dElapsedTime = u32Time / 1000.0;
    double dTotalSize = m_u64TotalSize / 1024.0 / 1024.0;
    printf( "Total Size = %.3fM\n", dTotalSize );
    printf( "Elapsed Time = %.2fs\n", dElapsedTime );
    if ( dElapsedTime > 0 )
    {
        double dRate = dTotalSize / dElapsedTime;
        printf( "Average Rate = %.3fM/s\n", dRate );
    }
    
    if ( 0 == lpcbData->u32ErrCode )
    {
        _tprintf( _T( "%s\n" ), KEY_PASS );
    }
    else
    {
        _tprintf( _T( "%s%s;\n" ), KEY_FAIL, lpcbData->lpErrMsg );
    }
    fflush( stdout );
    SetEvent( g_fnEventHelper.m_hEventDownloadEnd );
    
    return TRUE;
}

BOOL CCallbackHelper::__cbHandleDlStepDescription( LPCCALLBACKDATA_DL_STEP_DESCRIPTION lpcbData )
{
    if ( NULL == lpcbData )
    {
        assert( 0 );
        return FALSE;
    }
    
#ifdef _DEBUG
    _tprintf( _T( "No.%-03d FileID: %-30s; FileType: %-30s;\n" ),
              m_u32Step,
              lpcbData->lpFileID,
              lpcbData->lpFileType );
    fflush( stdout );
#endif // _DEBUG
    
    m_u32Step++;
    m_arrFileID.push_back( lpcbData->lpFileID );
    return TRUE;
}

BOOL CCallbackHelper::__cbHandleProgressInfo( LPCCALLBACKDATA_PROGRESS_INFO lpcbData )
{
    if ( NULL == lpcbData )
    {
        assert( 0 );
        return FALSE;
    }
    switch ( lpcbData->eType )
    {
    case CBK_PROGRESS_DL_IMAGE:
        {
            if ( g_tCmdParams.bDetailDlProcess && !g_tCmdParams.bEZMode )
            {
                uint32 u32WaitTime = ( uint32 )( GetTickCount() - m_u32StepStartTime );
                if ( m_u32CurProgress != lpcbData->u32Progress || u32WaitTime > ( m_u32CurTime + 999 ) || START_PROGRESS == m_u32CurProgress )
                {
                    m_u32CurProgress = lpcbData->u32Progress;
                    m_u32CurTime = u32WaitTime;
                    
                    TCHAR szBuff[256] = { 0 };
                    _stprintf_s( szBuff, u32WaitTime > 1000 ? DLIMAGE_FROMART_S : DLIMAGE_FROMART_MS, m_u32Step - 1, m_strStep.c_str(), lpcbData->u32Progress, m_dImageSize, u32WaitTime > 1000 ? ( uint32 )( u32WaitTime / 1000 ) : u32WaitTime );
                    wstring strBuff = g_fnConsoleHelper.GetConsoleOutput();
                    if ( strBuff.size() > 0 &&
                         wstring::npos != strBuff.find( m_strStep.c_str() ) &&
                         wstring::npos != strBuff.find( _T( "Progress:" ) ) )
                    {
                        if ( strBuff.length() > _tcsclen( szBuff ) )
                        {
                            g_fnConsoleHelper.SetConsole();
                            printf( "%-100s\n", " " );
                        }
                        g_fnConsoleHelper.SetConsole();
                    }
                    _tprintf( szBuff );
                    fflush( stdout );
                }
            }
        }
        break;
    default:
        break;
    }
    
    return TRUE;
}

BOOL CCallbackHelper::__cbHandlePacketInfo( LPCCALLBACKDATA_PACKET_INFO lpcbData )
{
    if ( NULL == lpcbData )
    {
        assert( 0 );
        return FALSE;
    }
    
    m_strPacName = lpcbData->lpName;
    m_strPacVersion = lpcbData->lpVersion;
    
    if ( !g_tCmdParams.bEZMode )
    {
        double dSize = ( *lpcbData->pu64Size ) / 1024.0 / 1024.0;
        _tprintf( _T( "PACKET: %s; Version: %s; Packet Size: %.2fMB; LoadTime = %s\n" ),
                  lpcbData->lpName,
                  lpcbData->lpVersion,
                  dSize,
                  lpcbData->lpLoadTime );
        fflush( stdout );
    }
    
    return TRUE;
}

BOOL CCallbackHelper::__cbHandleLoadPacketProgress( LPCCALLBACKDATA_LOAD_PACKET_PROGRESS lpcbData )
{
    if ( NULL == lpcbData )
    {
        assert( 0 );
        return FALSE;
    }
    wstring strBuff = g_fnConsoleHelper.GetConsoleOutput();
    switch ( lpcbData->eType )
    {
    case CBK_PROG_BEGIN:
        if ( !g_tCmdParams.bEZMode )
        {
            printf( "Start loading packet... \n" );
            m_u32CurProgress = START_PROGRESS;
        }
        break;
    case CBK_PROG_PROCEED:
        if ( !g_tCmdParams.bEZMode )
        {
            if ( START_PROGRESS == m_u32CurProgress || m_u32CurProgress != lpcbData->u32Progress )
            {
                m_u32CurProgress = lpcbData->u32Progress;
                if ( strBuff.size() > 0 && wstring::npos != strBuff.find( _T( "Loading packet progress is" ) ) )
                {
                    g_fnConsoleHelper.SetConsole();
                }
                printf( "Loading packet progress is %3d%%\n", lpcbData->u32Progress );
            }
        }
        
        break;
    case CBK_PROG_END:
        if ( !g_tCmdParams.bEZMode )
        {
            g_fnConsoleHelper.SetConsole();
            printf( "Loading packet progress is %3d%%\n", 100 );
            printf( "End loading packet\n" );
        }
        break;
    default:
        break;
    }
    fflush( stdout );
    return TRUE;
}

