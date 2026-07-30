#pragma once
#include "Tr.h"
#include "ICallback.h"
#include "IConfig.h"
#include "IDevMoniter.h"
#include "IAuthentication.h"

struct BM_INIT_PARAMS
{
    HANDLE              hStopEvent      = NULL;    /// Event to indicate stop, if signaled, task has been stopped.
    IConfig*            pConfigObject   = NULL;    /// IConfig
    ISpLog*             pSpLogObject    = NULL;    /// ISpLog
    ICallback*          pCallbackObject = NULL;    /// ICallback
    IAuthentication*    pAuthentication = NULL;    /// IAuthentication
    IDevMoniter*        pDevMoniter     = NULL;
    LPCWSTR             lpszAppPath     = NULL;    /// The path of DLFramework
    LPCWSTR             lpszLogPath     = NULL;    /// The path to generate the Trace Log
    
};
typedef BM_INIT_PARAMS* LPBM_INIT_PARAMS;
typedef const BM_INIT_PARAMS* LPCBM_INIT_PARAMS;


struct BM_INIT_PARAMS_EX
{
public:
    BM_INIT_PARAMS_EX( LPCBM_INIT_PARAMS lpParams )
    {
        if ( NULL != lpParams )
        {
            m_lpParams = lpParams;
        }
        else
        {
            m_lpParams = NULL;
        }
    }
    
    BOOL IsInvalidObject()
    {
        return ( NULL == m_lpParams ||
                 NULL == m_lpParams->hStopEvent ||
                 NULL == m_lpParams->pConfigObject ||
                 NULL == m_lpParams->pSpLogObject ||
                 NULL == m_lpParams->pCallbackObject ||
                 NULL == m_lpParams->pDevMoniter ||
                 NULL == m_lpParams->lpszAppPath
               );
    }
    
public:
    LPCBM_INIT_PARAMS m_lpParams;
};


