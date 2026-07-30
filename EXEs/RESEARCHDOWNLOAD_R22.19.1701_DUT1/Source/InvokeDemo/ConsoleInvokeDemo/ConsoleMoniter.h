#pragma once
#include <iostream>
#include <vector>
#include "global_err.h"
#include "Thread.h"
using namespace std;

//////////////////////////////////////////////////////////////////////////
class CConsoleMoniter : public CThread
{
    // TODO: To simply the invoke flow, here only support single thread.
    // Framework can definitely support multi-threads.
    //
public:
    CConsoleMoniter( void );
    virtual ~CConsoleMoniter( void );
    void operator()( LPCTSTR lpszkey );
    BOOL StartMoniter( BOOL bStart, DWORD dwTimeout = INFINITE );
    void run( void );
    BOOL GetRunResult();
    
private:
    vector<string_t> m_vKey;
};
