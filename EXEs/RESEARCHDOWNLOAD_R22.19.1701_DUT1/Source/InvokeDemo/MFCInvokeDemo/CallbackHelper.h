#pragma once
#include "global_err.h"
#include "callback_def.h"
#include <vector>

#define KEY_PASS _T("Download Successful.")
#define KEY_FAIL _T("Download Failed.")

BOOL CALLBACK MyCallBack( LPCSPCALLBACK_DATA lpcbData, LPCVOID pParam );
//////////////////////////////////////////////////////////////////////////
class CCallbackHelper
{
    // TODO: To simply the invoke flow, here only support single thread.
    // Framework can definitely support multi-threads.
    //
public:
    CCallbackHelper( void );
    virtual ~CCallbackHelper( void );
    
public:
    BOOL HandleCallbackData( LPCSPCALLBACK_DATA lpcbData );
    
private:
    BOOL __cbHandleLoadPacketProgress( LPCCALLBACKDATA_LOAD_PACKET_PROGRESS lpcbData );
    BOOL __cbHandlePacketInfo( LPCCALLBACKDATA_PACKET_INFO lpcbData );
    BOOL __cbHandleDlImageBegin( LPCCALLBACKDATA_DL_IMAGE_SIZE lpcbData );
    BOOL __cbHandleProgressInfo( LPCCALLBACKDATA_PROGRESS_INFO lpcbData );
    BOOL __cbHandleDlStepDescription( LPCCALLBACKDATA_DL_STEP_DESCRIPTION lpcbData );
    BOOL __cbHandleDlEnd( LPCCALLBACKDATA_DL_END lpcbData );
    BOOL __cbHandleStepInfo( LPCCALLBACKDATA_STEP_INFO lpcbData );
    BOOL __cbHandleStringInfo( LPCCALLBACKDATA_STRING_INFO lpcbData );

private:
    std::vector<string_t> m_vFileID;
    uint32 m_u32Step = 0;
    double m_dSize = 0.0;
    uint32 m_u32StepStartTime = 0;
    uint32 m_u32StartTime = 0;
    TCHAR m_szMsg[256] = { 0 };
};

