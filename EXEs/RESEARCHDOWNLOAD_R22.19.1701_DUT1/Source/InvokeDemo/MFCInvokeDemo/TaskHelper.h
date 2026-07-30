#pragma once
#include <map>
#include <vector>
#include "Tr.h"

#include "CallbackHelper.h"
#include "DLFrameworkExport.h"
using namespace std;



//////////////////////////////////////////////////////////////////////////
// Callback function

class CDLFWDriver;
//////////////////////////////////////////////////////////////////////////
class CTaskHelper
{
    // TODO: To simply the invoke flow, here only support single thread.
    // Framework can definitely support multi-threads.
    //
public:
    CTaskHelper( CDLFWDriver* pDLFWDriver );
    virtual ~CTaskHelper( void );
    
    SPRESULT CreatTask( int nPort, UINT nLv );
    SPRESULT RunTask( int nPort );
    SPRESULT StopTask( );
    
    SPRESULT OpenLog( int nPort, UINT nLv );
    void FreeTask();
    
private:
    void FreeCallback();
    void FreeLog();
private:
    ISpLog* m_pLogTask = NULL;
    SP_HANDLE m_hTask = NULL;
    CCallbackHelper* m_pCbkTask = NULL;
    CDLFWDriver* m_pDLFWDriver = NULL;
};
