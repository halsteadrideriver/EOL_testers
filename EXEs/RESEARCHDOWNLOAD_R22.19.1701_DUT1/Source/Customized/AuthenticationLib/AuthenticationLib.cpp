#include "StdAfx.h"
#include "AuthenticationImpl.h"

HMODULE g_hModule = NULL;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch ( ul_reason_for_call )
    {
    case DLL_PROCESS_ATTACH: g_hModule = hModule;
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

SP_EXPORT IAuthentication* SP_API CreateAuthenticationObject( LPVOID pLogUtil )
{
    IAuthentication* pObject = NULL;
    try
    {
        pObject = new CAuthenticationImpl( pLogUtil );
    }
    catch ( const std::bad_alloc& /*e*/ )
    {
        pObject = NULL;
    }
    
    return pObject;
}