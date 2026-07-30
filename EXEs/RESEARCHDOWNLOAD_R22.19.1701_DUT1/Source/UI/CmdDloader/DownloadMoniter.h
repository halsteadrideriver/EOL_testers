#pragma once
#include "Thread.h"

//////////////////////////////////////////////////////////////////////////
class CDownloadMoniter : public CThread
{
public:
    CDownloadMoniter( void );
    virtual ~CDownloadMoniter( void );
    
    BOOL StartMoniter( BOOL bStart, DWORD dwTimeout = INFINITE );
    void run( void );
    
private:
    DWORD m_dwStartTime = 0;
    DWORD m_dwTimeout = 0;
};
