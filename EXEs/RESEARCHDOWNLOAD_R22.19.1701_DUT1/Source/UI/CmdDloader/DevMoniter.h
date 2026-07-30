#pragma once
#include "Thread.h"
//////////////////////////////////////////////////////////////////////////
class CDevMoniter : public CThread
{
public:
    BOOL StartMoniter( BOOL bStart, DWORD dwTimeout = INFINITE );
    void run( void );
    
    BOOL GetRunResult();
    
private:
    BOOL m_bOK = FALSE;
    DWORD m_dwStartTime = 0;
    DWORD m_dwTimeout = 0;
    DWORD m_dwCurTime = 0;
};
