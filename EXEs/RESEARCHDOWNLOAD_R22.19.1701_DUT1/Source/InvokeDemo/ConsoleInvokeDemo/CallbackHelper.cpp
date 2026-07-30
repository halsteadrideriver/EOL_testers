#include "StdAfx.h"
#include <iostream>
#include "CallbackHelper.h"
#include "ConsoleHelper.h"
using namespace std;
extern CConsoleHelper g_fnConsole;
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
            _tprintf( _T( "%s %s\r\n" ), KEY_ERROR, lpcbData->lpInfo );
        } break;
    case CBK_STRING_WARN_MSG:
        {
            _tprintf( _T( "%s %s\r\n" ), KEY_WARNING, lpcbData->lpInfo );
        } break;
    case CBK_STRING_USB_INSERT:
        {
            m_nPort = lpcbData->u32Port;
            _tprintf( _T( "%s\r\n" ), KEY_START_DL );
            _tprintf( _T( "%s\r\n" ), KEY_START_DL );
            _tprintf( _T( "%s\r\n" ), KEY_START_DL );
        } break;
    default:
        break;
    }
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
    if ( m_u32Step <= m_vFileID.size() )
    {
        m_strStep = m_vFileID[m_u32Step];
        m_u32Step++;
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
            printf( "Start Downloading...\r\n" );
            m_u32Step = 0;
            m_u32StartTime = GetTickCount();
        }
        break;
    case CBK_STEP_REQUIRE_SN:
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
    uint32 u32Time = ( uint32 )( GetTickCount() - m_u32StartTime );
    printf( "Elapsed Time: %.2fs.\r\n", u32Time / 1000.0 );
    
    if ( 0 == lpcbData->u32ErrCode )
    {
        _tprintf( _T( "%s\r\n" ), KEY_PASS );
    }
    else
    {
        _tprintf( _T( "%s%s\r\n" ), KEY_FAIL, lpcbData->lpErrMsg );
    }
    
    return TRUE;
}

BOOL CCallbackHelper::__cbHandleDlStepDescription( LPCCALLBACKDATA_DL_STEP_DESCRIPTION lpcbData )
{
    if ( NULL == lpcbData )
    {
        assert( 0 );
        return FALSE;
    }
    _tprintf( _T( "No.%-03d FileID: %-30s; FileType: %-30s;\r\n" ),
              m_u32Step,
              lpcbData->lpFileID,
              lpcbData->lpFileType );
    m_u32Step++;
    m_vFileID.push_back( lpcbData->lpFileID );
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
            wstring strBuff = g_fnConsole.GetConsoleOutput();
            if ( strBuff.size() > 0 &&
                 wstring::npos != strBuff.find( _T( "Downloading image" ) ) &&
                 wstring::npos != strBuff.find( _T( "progress is" ) ) &&
                 wstring::npos == strBuff.find( _T( "100%" ) ) )
            {
                g_fnConsole.SetConsole();
            }
            
            uint32 u32Time = ( uint32 )( GetTickCount() - m_u32StepStartTime );
            _tprintf( _T( "No.%-03d Downloading image %-30s progress is %3d%%; Size: %.3fM; Time:%dms\r\n" ),
                      m_u32Step - 1,
                      m_strStep.c_str(),
                      lpcbData->u32Progress,
                      m_dImageSize,
                      u32Time );
        }
        break;
    case CBK_PROGRESS_READ_FLASH:
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
    double dSize = ( *lpcbData->pu64Size ) / 1024.0 / 1024.0;
    _tprintf( _T( "PACKET: %s; Version: %s; Packet Size: %.2fMB; LoadTime = %s\r\n" ),
              lpcbData->lpName,
              lpcbData->lpVersion,
              dSize,
              lpcbData->lpLoadTime );
    return TRUE;
}

BOOL CCallbackHelper::__cbHandleLoadPacketProgress( LPCCALLBACKDATA_LOAD_PACKET_PROGRESS lpcbData )
{
    if ( NULL == lpcbData )
    {
        assert( 0 );
        return FALSE;
    }
    wstring strBuff = g_fnConsole.GetConsoleOutput();
    switch ( lpcbData->eType )
    {
    case CBK_PROG_BEGIN:
        cout << "Start loading packet... " << endl;
        break;
    case CBK_PROG_PROCEED:
        if ( strBuff.size() > 0 && wstring::npos != strBuff.find( _T( "Loading packet progress is" ) ) )
        {
            g_fnConsole.SetConsole();
        }
        printf( "Loading packet progress is %3d%%\r\n", lpcbData->u32Progress );
        break;
    case CBK_PROG_END:
        cout << "End loading packet" << endl;
        break;
    default:
        break;
    }
    
    return TRUE;
}

