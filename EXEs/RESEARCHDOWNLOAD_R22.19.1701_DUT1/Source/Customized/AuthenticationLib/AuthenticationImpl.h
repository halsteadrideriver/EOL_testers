#pragma once
#include "Tr.h"
#include "IAuthentication.h"

class CAuthenticationImpl : public IAuthentication, public CTr
{
public:
    CAuthenticationImpl( LPVOID pLogUtil );
    virtual ~CAuthenticationImpl();
    
public:
    virtual void Release( void );
    virtual SPRESULT EnableAuth( LPCRANDOM_DATA lpRandomData, AUTH_MODE& eMode );
    virtual SPRESULT ServerAuth( LPAUTH_DUT lpDut, LPAUTH_SERVER lpServer, DWORD& dwLength );
    virtual SPRESULT GetSecureKey( LPSECURE_KEY lpKey );
};