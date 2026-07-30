#include "stdafx.h"
#include <iostream>
#include "CallbackHelper.h"
using namespace std;

extern HWND g_hHwndDidg;

BOOL CALLBACK MyCallBack( LPCSPCALLBACK_DATA lpcbData, LPCVOID pParam )
{
    if ( NULL == lpcbData )
    {
        ASSERT( 0 );
        return FALSE;
    }
    
    CCallbackHelper* pThis = ( CCallbackHelper* )pParam;
    ASSERT( NULL != pThis );
    
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
BOOL CCallbackHelper::__cbHandleStepInfo( LPCCALLBACKDATA_STEP_INFO lpcbData )
{
    if ( NULL == lpcbData )
    {
        ASSERT( 0 );
        return FALSE;
    }
    switch ( lpcbData->eType )
    {
    case CBK_STEP_DL_BEGIN:
        {
            m_u32Step = 0;
            m_u32StartTime = GetTickCount();
            ::PostMessage( g_hHwndDidg, WM_DL_PROGRESS, lpcbData->u32Port, 0 );
            ::PostMessage( g_hHwndDidg, WM_DL_RESULT, lpcbData->u32Port, ( LPARAM )_T( "" ) );
        }
        break;
    default:
        break;
    }
    return TRUE;
}

BOOL CCallbackHelper::__cbHandleDlImageBegin( LPCCALLBACKDATA_DL_IMAGE_SIZE lpcbData )
{
    if ( NULL == lpcbData )
    {
        ASSERT( 0 );
        return FALSE;
    }
    //m_u32StepStartTime = GetTickCount();
    if ( m_u32Step <= m_vFileID.size() )
    {
        ::PostMessage( g_hHwndDidg, WM_DL_STEP, lpcbData->u32Port, ( LPARAM )m_vFileID[m_u32Step].c_str() );
        m_dSize = ( *lpcbData->pu64Size ) / 1024.0 / 1024.0;
        ::PostMessage( g_hHwndDidg, WM_DL_SIZE, lpcbData->u32Port, ( LPARAM )&m_dSize );
        m_u32Step++;
    }
    return TRUE;
}

BOOL CCallbackHelper::__cbHandleDlEnd( LPCCALLBACKDATA_DL_END lpcbData )
{
    if ( NULL == lpcbData )
    {
        ASSERT( 0 );
        return FALSE;
    }
    uint32 u32Time = ( uint32 )( GetTickCount() - m_u32StartTime );
    ::PostMessage( g_hHwndDidg, WM_DL_TIME, lpcbData->u32Port, u32Time );
    ::PostMessage( g_hHwndDidg, WM_STOP_DOWNLOAD, lpcbData->u32Port, NULL );
    if ( 0 == lpcbData->u32ErrCode )
    {
        ::PostMessage( g_hHwndDidg, WM_DL_RESULT, lpcbData->u32Port, ( LPARAM )KEY_PASS );
    }
    else
    {
    
        _stprintf_s( m_szMsg, L"%s ErrCode: %d; ErrMsg: %s;", KEY_FAIL, lpcbData->u32ErrCode, lpcbData->lpErrMsg );
        ::PostMessage( g_hHwndDidg, WM_DL_RESULT, lpcbData->u32Port, ( LPARAM )m_szMsg );
    }
    
    return TRUE;
}

BOOL CCallbackHelper::__cbHandleDlStepDescription( LPCCALLBACKDATA_DL_STEP_DESCRIPTION lpcbData )
{
    if ( NULL == lpcbData )
    {
        ASSERT( 0 );
        return FALSE;
    }
    m_u32Step++;
    m_vFileID.push_back( lpcbData->lpFileID );
    return TRUE;
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
    case CBK_STRING_USB_INSERT:
        {
            ::SendMessage( g_hHwndDidg, WM_DL_PORT, lpcbData->u32Port, NULL );
            ::PostMessage( g_hHwndDidg, WM_START_DOWNLOAD, lpcbData->u32Port, NULL );
        } break;
    case CBK_STRING_USB_REMOVE:
        {
            ::PostMessage( g_hHwndDidg, WM_STOP_DOWNLOAD, lpcbData->u32Port, NULL );
        } break;
    default:
        break;
    }
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
    case CBK_PROGRESS_DL_IMAGE:
        {
            ::PostMessage( g_hHwndDidg, WM_DL_PROGRESS, lpcbData->u32Port, lpcbData->u32Progress );
            
            //uint32 u32Time = ( uint32 )( GetTickCount() - m_u32StepStartTime );
            uint32 u32Time = ( uint32 )( GetTickCount() - m_u32StartTime );
            ::PostMessage( g_hHwndDidg, WM_DL_TIME, lpcbData->u32Port, u32Time );
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
        ASSERT( 0 );
        return FALSE;
    }
    
    _stprintf_s( m_szMsg, _T( "PACKET: %s; Version: %s; Packet Size: %0.3fMB; LoadTime = %s" ),
                 lpcbData->lpName,
                 lpcbData->lpVersion,
                 ( *lpcbData->pu64Size ) / 1024.0 / 1024.0,
                 lpcbData->lpLoadTime );
                 
    ::PostMessage( g_hHwndDidg, WM_DL_TITLE, 0, ( LPARAM )m_szMsg );
    
    return TRUE;
}

BOOL CCallbackHelper::__cbHandleLoadPacketProgress( LPCCALLBACKDATA_LOAD_PACKET_PROGRESS lpcbData )
{
    if ( NULL == lpcbData )
    {
        ASSERT( 0 );
        return FALSE;
    }
    ::PostMessage( g_hHwndDidg, WM_LOAD_PACKET, lpcbData->eType, lpcbData->u32Progress );
    
    return TRUE;
}

