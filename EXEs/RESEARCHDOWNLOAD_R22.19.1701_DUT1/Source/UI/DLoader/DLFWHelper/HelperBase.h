#pragma once
#include "CallbackHelper.h"
#include "DLFWDriver.h"
#include "CLocks.h"
#include "CSimpleLock.h"

class CHelperBase
{
public:
    CHelperBase( CDLFWDriver* pDrv );
    virtual ~CHelperBase( void );
    
    void LoadCfgDLFramework();
    
    LPCTSTR GetLogPath() { return m_strLogPath.operator LPCWSTR(); };
    
protected:
    ISpLog* m_pLog = NULL;
    CCallbackHelper* m_pCbkHelper = NULL;
    CDLFWDriver* m_pDrv = NULL;
    CString m_strCfgLogPath = _T( "" );
    CString m_strLogPath = _T( "" );
    BOOL m_bLogOpen = FALSE;
    BOOL m_bCompressDLFWLog = TRUE;
    UINT m_nLogLevel = SPLOGLV_ERROR;
};

