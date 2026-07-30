#include "StdAfx.h"
#include "MesImpl.h"

HMODULE g_hModule = NULL;

std::wstring GetAppPath( )
{
    WCHAR szAppPath[MAX_PATH] = { 0 };
    GetModuleFileNameW( g_hModule, szAppPath, MAX_PATH );
    LPWSTR lpChar = wcsrchr( szAppPath, L'\\' );
    if ( NULL != lpChar )
    {
        *lpChar = L'\0';
    }
    lpChar = wcsrchr( szAppPath, L'\\' );
    if ( NULL != lpChar )
    {
        *lpChar = L'\0';
    }
    return ( std::wstring )szAppPath + L"\\";
}

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

SP_EXPORT IMes* SP_API CreateMesObject( LPVOID pLogUtil )
{
    IMes* pObject = NULL;
    try
    {
        pObject = new CMesImpl( pLogUtil );
        if ( !pObject->Startup() )
        {
            pObject->Release();
            pObject = NULL;
        }
    }
    catch ( const std::bad_alloc& /*e*/ )
    {
        pObject = NULL;
    }
    
    return pObject;
}