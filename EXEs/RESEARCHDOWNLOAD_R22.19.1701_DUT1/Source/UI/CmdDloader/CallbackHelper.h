#pragma once
#include "global_err.h"
#include "callback_def.h"
#include <vector>

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
    BOOL __cbHandleProgressInfo( LPCCALLBACKDATA_PROGRESS_INFO lpcbData );
    BOOL __cbHandleStepInfo( LPCCALLBACKDATA_STEP_INFO lpcbData );
    BOOL __cbHandleDlEnd( LPCCALLBACKDATA_DL_END lpcbData );
    BOOL __cbHandleStringInfo( LPCCALLBACKDATA_STRING_INFO lpcbData );
    
public:
    INT m_nPort = 0;
    string_t m_strSN1 = _T( "" );
    string_t m_strSN2 = _T( "" );
    string_t m_strIMEI = _T( "" );
    CALLBACKDATA_DL_END m_tDlEnd;
    
private:
    vector<string_t> m_arrFileID;
    string_t m_strStep = _T( "" );
    string_t m_strPacName = _T( "" );
    string_t m_strPacVersion = _T( "" );
    uint32 m_u32Step = 0;
    uint32 m_u32CurProgress = 0;
    double m_dImageSize = 0.0;
    uint32 m_u32StepStartTime = 0;
    uint32 m_u32StartTime = 0;
    uint64 m_u64TotalSize = 0;
    uint32 m_u32CurTime = 0;
};
