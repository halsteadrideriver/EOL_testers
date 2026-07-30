#pragma once
#include <iostream>
#include "global_err.h"
#include "Thread.h"
using namespace std;

//////////////////////////////////////////////////////////////////////////
class CDevMoniter : public CThread
{
    // TODO: To simply the invoke flow, here only support single thread.
    // Framework can definitely support multi-threads.
    //
public:
    CDevMoniter( void );
    virtual ~CDevMoniter( void );
    
    BOOL StartMoniter( BOOL bStart, DWORD dwTimeout = INFINITE );
    void run( void );
    
    BOOL GetRunResult();
    
private:
    BOOL m_bOK = FALSE;
    uint32 m_u32Start = 0;
    uint32 m_u32Timeout;
};
