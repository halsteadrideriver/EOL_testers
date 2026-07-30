#pragma once
#include <map>
#include <vector>
#include "Tr.h"
#include "CallbackHelper.h"
#include "DLFrameworkExport.h"
using namespace std;

class CDLFWDriver;
class CFrameHelper
{
public:
    CFrameHelper( CDLFWDriver* pDLFWDriver );
    virtual ~CFrameHelper( void );
    
    SPRESULT Startup( UINT nLv );
    void Cleanup();
    SPRESULT LoadPacket( LPCTSTR lpPacket, LPCTSTR lpImageDir );
    SPRESULT DeviceMoniter( BOOL bStart );
private:
    ISpLog* m_pLogFrame = NULL;
    CCallbackHelper* m_pCbkFrame = NULL;
    CDLFWDriver* m_pDLFWDriver = NULL;
};

