/// Include files
#include "global_err.h"
#include "callback_def.h"

#ifdef CALLBACK_EXPORT
#undef CALLBACK_EXPORT
#endif
#ifdef CALLBACK_EXPORTS
#define CALLBACK_EXPORT EXTERN_C __declspec (dllexport)
#else
#define CALLBACK_EXPORT EXTERN_C __declspec (dllimport)
#endif


//////////////////////////////////////////////////////////////////////////
class ICallback
{

public:

    virtual void Release( void ) = 0;
    
    //CALLBACK_LOAD_PACKET_PROGRESS
    virtual BOOL UiMsgLoadPacketProgress( LOAD_PACKET_PROGRESS_E eType, uint32 u32Progress ) = 0;
    //CALLBACK_PACKET_INFO
    virtual BOOL UiMsgPacketInfo( LPCTSTR lpName, LPCTSTR lpVersion, LPCTSTR lpLoadTime, uint64* pu64Size ) = 0;
    //CALLBACK_DL_STEP_DESCRIPTION
    virtual BOOL UiMsgDlStepDescription( LPCTSTR lpFileID, LPCTSTR lpFileType = NULL ) = 0;
    //CALLBACK_DL_IMAGE_BEGIN
    virtual BOOL UiMsgDlImageBegin( uint32 u32Port, uint64* pu64Size ) = 0;
    //CALLBACK_DL_END
    virtual BOOL UiMsgDlEnd( uint32 u32Port, uint32 u32Stage, uint32 u32ErrCode, LPCTSTR lpErrMsg ) = 0;
    
    //CALLBACK_STRING_INFO
    virtual BOOL UiMsgStringInfo( uint32 u32Port, STRING_INFO_TYPE_E eType, LPCTSTR lpInfo = NULL ) = 0;
    
    //CALLBACK_STEP_INFO
    virtual BOOL UiMsgStepInfo( uint32 u32Port, STEP_INFO_TYPE_E eType ) = 0;
    
    //CALLBACK_PROGRESS_INFO,
    virtual BOOL UiMsgProgressInfo( uint32 u32Port, PROGRESS_INFO_TYPE_E eType, uint32 u32Progress ) = 0;
    
protected:
    virtual ~ICallback( void ) { };
};

//////////////////////////////////////////////////////////////////////////
/// Export API
CALLBACK_EXPORT ICallback* SP_API CreateCallbackObject( LPCSPCALLBACK_PARAM lpCallBack, LPVOID pLogUtil );