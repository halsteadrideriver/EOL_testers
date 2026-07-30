#pragma once
#include "DevMoniter.h"
#include "DownloadMoniter.h"
#include "FrameworkHelper.h"

//////////////////////////////////////////////////////////////////////////
class CDownload
{
    // TODO: To simply the invoke flow, here only support single thread.
    // Framework can definitely support multi-threads.
    //
public:
    CDownload( void );
    virtual ~CDownload( void );
    
    SPRESULT Download( );
    SPRESULT SetBarcode( int nPort );
    
private:
    SPRESULT ConfigLoadPacBefore( );
    SPRESULT ConfigLoadPacAfter( );
    SPRESULT ConfigLoadPacPreloadAfter( );
    SPRESULT GetConfigObject();
    SPRESULT ReloadParameters();
    void     SetDlEndData( SPRESULT spRlt );
private:
    CFrameworkHelper m_fnFrameworkHelper;
    CDevMoniter m_fnDevMoniter;
    CDownloadMoniter m_fnDownloadMoniter;
    IConfig* m_pCfgObject = NULL;
    string_t m_strErrMsg = _T( "" );
};
