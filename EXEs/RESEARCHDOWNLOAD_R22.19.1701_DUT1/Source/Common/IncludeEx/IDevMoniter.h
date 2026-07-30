#pragma once
#include "global_err.h"

class IDevMoniter
{
public:
    virtual SPRESULT LoadSetting() = 0;
    virtual SPRESULT Start() = 0;
    virtual void Stop() = 0;
    virtual void SetDevStatus( LPCWSTR lpszLocationPath, BOOL bStatus ) = 0;
    virtual void SetDevPort( LPCWSTR lpszLocationPath, UINT nPort ) = 0;
    virtual void SetDevPath( UINT nPort, LPCWSTR lpszLocationPath ) = 0;
    virtual BOOL GetDevStatus( LPCWSTR lpszLocationPath ) = 0;
    virtual UINT GetDevPort( LPCWSTR lpszLocationPath ) = 0;
    virtual LPCWSTR GetDevPath( UINT nPort ) = 0;
    virtual BOOL GetDownloadByPoweroff() = 0;
};