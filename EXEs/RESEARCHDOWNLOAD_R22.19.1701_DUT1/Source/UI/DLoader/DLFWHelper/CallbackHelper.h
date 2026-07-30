#pragma once
#include "global_err.h"
#include "callback_def.h"
#include <vector>

BOOL CALLBACK MyCallBack( LPCSPCALLBACK_DATA lpcbData, LPCVOID pParam );
//////////////////////////////////////////////////////////////////////////
class CCallbackHelper
{
public:
    CCallbackHelper( void );
    virtual ~CCallbackHelper( void );
    
public:
    BOOL HandleCallbackData( LPCSPCALLBACK_DATA lpcbData );
    
private:
    BOOL __cbHandleLoadPacketProgress( LPCCALLBACKDATA_LOAD_PACKET_PROGRESS lpcbData );
    BOOL __cbHandlePacketInfo( LPCCALLBACKDATA_PACKET_INFO lpcbData );
    BOOL __cbHandleDlImageBegin( LPCCALLBACKDATA_DL_IMAGE_SIZE lpcbData );
    BOOL __cbHandleDlStepDescription( LPCCALLBACKDATA_DL_STEP_DESCRIPTION lpcbData );
    BOOL __cbHandleDlEnd( LPCCALLBACKDATA_DL_END lpcbData );
    
    BOOL __cbHandleStepInfo( LPCCALLBACKDATA_STEP_INFO lpcbData );
    BOOL __cbHandleProgressInfo( LPCCALLBACKDATA_PROGRESS_INFO lpcbData );
    BOOL __cbHandleStringInfo( LPCCALLBACKDATA_STRING_INFO lpcbData );
    
public:
    BM_DL_END m_tBmDlEnd;
    TCHAR m_szMsg[256] = { 0 };
    string_t m_strIMEI = _T( "" );
    string_t m_strMcpType = _T( "" );
    string_t m_strSN1 = _T( "" );
    string_t m_strSN2 = _T( "" );
};

