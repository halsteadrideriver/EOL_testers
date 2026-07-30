#include "StdAfx.h"
#include "CallbackHelper.h"


BOOL CALLBACK MyCallBack( LPCSPCALLBACK_DATA lpcbData, LPCVOID pParam )
{
    if ( NULL == lpcbData || NULL == pParam )
    {
        ASSERT( 0 );
        return FALSE;
    }
    
    CCallbackHelper* pThis = ( CCallbackHelper* )pParam;
    return pThis->HandleCallbackData( lpcbData );
}

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
        ASSERT( 0 );
        return FALSE;
    }
    
    switch ( lpcbData->eType )
    {
    case CALLBACK_LOAD_PACKET_PROGRESS:
        return __cbHandleLoadPacketProgress( LPCCALLBACKDATA_LOAD_PACKET_PROGRESS( lpcbData->lpData ) );
    case CALLBACK_PACKET_INFO:
        return __cbHandlePacketInfo( LPCCALLBACKDATA_PACKET_INFO( lpcbData->lpData ) );
    case CALLBACK_DL_END:
        return __cbHandleDlEnd( LPCCALLBACKDATA_DL_END( lpcbData->lpData ) );
    case CALLBACK_DL_IMAGE_BEGIN:
        return __cbHandleDlImageBegin( LPCCALLBACKDATA_DL_IMAGE_SIZE( lpcbData->lpData ) );
    case CALLBACK_DL_STEP_DESCRIPTION:
        return __cbHandleDlStepDescription( LPCCALLBACKDATA_DL_STEP_DESCRIPTION( lpcbData->lpData ) );
    case CALLBACK_STRING_INFO:
        return __cbHandleStringInfo( LPCCALLBACKDATA_STRING_INFO( lpcbData->lpData ) );
    case CALLBACK_STEP_INFO:
        return __cbHandleStepInfo( LPCCALLBACKDATA_STEP_INFO( lpcbData->lpData ) );
    case CALLBACK_PROGRESS_INFO:
        return __cbHandleProgressInfo( LPCCALLBACKDATA_PROGRESS_INFO( lpcbData->lpData ) );
    default: return FALSE;
    }
}

BOOL CCallbackHelper::__cbHandleStringInfo( LPCCALLBACKDATA_STRING_INFO lpcbData )
{
    if ( NULL == lpcbData )
    {
        ASSERT( 0 );
        return FALSE;
    }
    
    switch ( lpcbData->eType )
    {
    case CBK_STRING_POWER_MANAGE:
        PostMessage( g_hWndFrame, WM_POWER_MANAGE, lpcbData->u32Port, ( LPARAM )lpcbData->lpInfo );
        break;
    case CBK_STRING_WARN_MSG:
        PostMessage( g_hWndFrame, WM_WARN_MESSAGEBOX, lpcbData->u32Port, ( LPARAM )lpcbData->lpInfo );
        break;
    case CBK_STRING_WARN_ERASE:
        PostMessage( g_hWndFrame, WM_WARN_MESSAGEBOX, CBK_STRING_WARN_ERASE, ( LPARAM )lpcbData->lpInfo );
        break;
    case CBK_STRING_ERROR_MSG:
        {
#ifdef _RESEARCH
            MessageBox( g_hWndFrame, lpcbData->lpInfo, _T( "Error Message" ), MB_OK | MB_ICONERROR );
#else
            if ( 0 == lpcbData->u32Port )
            {
                MessageBox( g_hWndFrame, lpcbData->lpInfo, _T( "Error Message" ), MB_OK | MB_ICONERROR );
            }
#endif // _RESEARCH   
        }
        break;
    case CBK_STRING_IMEI:
        {
#ifdef _RESEARCH
            m_strIMEI.clear();
            m_strIMEI = lpcbData->lpInfo;
            PostMessage( g_hWndView, BM_SET_IMEI, lpcbData->u32Port, ( LPARAM )m_strIMEI.c_str() );
#endif // _RESEARCH
        }
        break;
    case CBK_STRING_MCP:
        {
            m_strMcpType.clear();
            m_strMcpType = lpcbData->lpInfo;
            PostMessage( g_hWndView, BM_SET_MCP_TYPE, lpcbData->u32Port, ( LPARAM )m_strMcpType.c_str() );
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
    case CBK_STRING_USB_INSERT:
        {
            SendMessage( g_hWndView, BM_SET_PORT, lpcbData->u32Port, TRUE );
            PostMessage( g_hWndFrame, WM_DWONLOAD_START, lpcbData->u32Port, NULL );
        } break;
    case CBK_STRING_USB_REMOVE:
        {
            memset( &m_tBmDlEnd, 0, sizeof( m_tBmDlEnd ) );
            m_tBmDlEnd.bDLEnd = FALSE;
            PostMessage( g_hWndView, BM_END, lpcbData->u32Port, ( LPARAM )&m_tBmDlEnd );
            PostMessage( g_hWndFrame, WM_STOP_ONE_PORT, lpcbData->u32Port, NULL );
            SendMessage( g_hWndView, BM_SET_PORT, lpcbData->u32Port, TRUE );
        } break;
    default: break;
    }
    
    return TRUE;
}

BOOL CCallbackHelper::__cbHandleStepInfo( LPCCALLBACKDATA_STEP_INFO lpcbData )
{
    if ( NULL == lpcbData )
    {
        ASSERT( 0 );
        return FALSE;
    }
    
    switch ( lpcbData->eType )
    {
    case CBK_STEP_CONNECT:
        PostMessage( g_hWndView, BM_CONNECT, lpcbData->u32Port, 100 );
        break;
    case CBK_STEP_CHECK_BAUDRATE:
        PostMessage( g_hWndView, BM_CHANGE_BAUD, lpcbData->u32Port, 100 );
        break;
    case CBK_STEP_DL_BEGIN:
        PostMessage( g_hWndView, BM_BEGIN, lpcbData->u32Port, 0 );
        break;
    case CBK_STEP_DL_IMAGE:
        PostMessage( g_hWndView, BM_DOWNLOAD, lpcbData->u32Port, 100 );
        break;
    case CBK_STEP_REQUIRE_SN:
        PostMessage( g_hWndView, WM_REQUIRE_SN, lpcbData->u32Port, NULL );
        break;
    case CBK_STEP_MUTI_SOFTWARE:
        SendMessage( g_hWndFrame, WM_MUTI_SOFTWARE_CONFIG, NULL, NULL );
        break; 
    case CBK_STEP_PRELOAD:
        SendMessage( g_hWndFrame, VM_PRELOAD_CONFIG, NULL, NULL );
        break;
    case CBK_STEP_STOP_AUTODLOADER:
        PostMessage( g_hWndFrame, WM_STOP_AUTODLOADER, lpcbData->u32Port, NULL );
        break;
    default: break;
    }
    
    return TRUE;
}

BOOL CCallbackHelper::__cbHandleLoadPacketProgress( LPCCALLBACKDATA_LOAD_PACKET_PROGRESS lpcbData )
{
    if ( NULL == lpcbData )
    {
        ASSERT( 0 );
        return FALSE;
    }
    PostMessage( g_hWndFrame, WM_BINPAC_PROG_MSG, lpcbData->eType, lpcbData->u32Progress );
    
    return TRUE;
}


BOOL CCallbackHelper::__cbHandleDlImageBegin( LPCCALLBACKDATA_DL_IMAGE_SIZE lpcbData )
{
    if ( NULL == lpcbData )
    {
        ASSERT( 0 );
        return FALSE;
    }
    PostMessage( g_hWndView, BM_FILE_BEGIN, lpcbData->u32Port, ( LPARAM )lpcbData->pu64Size );
    return TRUE;
}

BOOL CCallbackHelper::__cbHandleDlEnd( LPCCALLBACKDATA_DL_END lpcbData )
{
    if ( NULL == lpcbData )
    {
        ASSERT( 0 );
        return FALSE;
    }
    
    m_tBmDlEnd.lpErrMsg = lpcbData->lpErrMsg;
    m_tBmDlEnd.dwErrCode = lpcbData->u32ErrCode;
    m_tBmDlEnd.u32Stage = lpcbData->u32Stage;
    m_tBmDlEnd.bDLEnd = TRUE;
    
    PostMessage( g_hWndView, BM_END, lpcbData->u32Port, ( LPARAM )&m_tBmDlEnd );
    PostMessage( g_hWndFrame, WM_STOP_ONE_PORT, lpcbData->u32Port, NULL );
    return TRUE;
}

BOOL CCallbackHelper::__cbHandleDlStepDescription( LPCCALLBACKDATA_DL_STEP_DESCRIPTION lpcbData )
{
    if ( NULL == lpcbData )
    {
        ASSERT( 0 );
        return FALSE;
    }
    SendMessage( g_hWndView, BM_STEP_DESCRIPTION, 0, ( LPARAM )lpcbData->lpFileID );
    return TRUE;
}

BOOL CCallbackHelper::__cbHandleProgressInfo( LPCCALLBACKDATA_PROGRESS_INFO lpcbData )
{
    if ( NULL == lpcbData )
    {
        ASSERT( 0 );
        return FALSE;
    }
    switch ( lpcbData->eType )
    {
    case CBK_PROGRESS_DL_IMAGE: PostMessage( g_hWndView, BM_DOWNLOAD_PROCESS, lpcbData->u32Port, lpcbData->u32Progress ); break;
    case CBK_PROGRESS_READ_FLASH: PostMessage( g_hWndView, BM_READ_FLASH_PROCESS, lpcbData->u32Port, lpcbData->u32Progress ); break;
    default: break;
    }
    
    return TRUE;
}

BOOL CCallbackHelper::__cbHandlePacketInfo( LPCCALLBACKDATA_PACKET_INFO lpcbData )
{
    if ( NULL == lpcbData )
    {
        ASSERT( 0 );
        return FALSE;
    }
    if ( NULL != lpcbData->lpVersion && NULL != lpcbData->pu64Size )
    {
        _stprintf_s( m_szMsg, _T( "PACKET: %s  --  Version: %s \nPacket Size: %0.3fMB  --  LoadTime = %s" ),
                     lpcbData->lpName,
                     lpcbData->lpVersion,
                     ( *lpcbData->pu64Size ) / 1024.0 / 1024.0,
                     lpcbData->lpLoadTime );
    }
    else
    {
        _stprintf_s( m_szMsg, _T( "FileMode: %s \nLoadTime = %s" ),
                     lpcbData->lpName,
                     lpcbData->lpLoadTime );
    }
    
    PostMessage( g_hWndFrame, WM_TOOL_BAR_TEXT, 0, ( LPARAM )m_szMsg );
    
    return TRUE;
}