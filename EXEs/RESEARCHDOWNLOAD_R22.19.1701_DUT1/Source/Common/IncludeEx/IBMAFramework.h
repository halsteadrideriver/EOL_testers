#ifndef _IBMAFRAMEWORK_H__
#define _IBMAFRAMEWORK_H__

#include "BMAFExport.h"
#include "BootModeitf.h"

class IBMAFramework
{
public:
    virtual HRESULT BMAF_Startup( LPCTSTR lpszProductName, LPCTSTR* ppszFileList, DWORD dwFileCount ) = 0;
    virtual HRESULT BMAF_Clearup() = 0;
    virtual HRESULT BMAF_StartOneWork( LPVOID  pOpenArgument, BOOL bBigEndian, LPCWSTR lpbstrProgID ) = 0;
    virtual HRESULT BMAF_StopOneWork( ) = 0;
    virtual HRESULT BMAF_GetBootModeObjInfo( LPVOID* ppBootModeObjT ) = 0;
    virtual HRESULT BMAF_SubscribeObserver( IBMOprObserver* pSink ) = 0;
    virtual HRESULT BMAF_SetProperty( DWORD dwPropertyID, LPCVOID pPropertyValue ) = 0;
    virtual HRESULT BMAF_GetProperty( DWORD dwPropertyID, LPVOID*  lpPropertyValue ) = 0;
    virtual HRESULT BMAF_SetCommunicateChannelPtr( LPVOID pCommunicateChannel ) = 0;
    virtual HRESULT BMAF_GetCommunicateChannelPtr( LPVOID* ppCommunicateChannel ) = 0 ;
    virtual HRESULT BMAF_Release() = 0;
    
};

BMAF_EXPORT_API BOOL CreateBMAFramework( IBMAFramework** pBMAFramework, LPCVOID lpBmInitParams );

#endif //_IBMAFRAMEWORK_H__