#include "StdAfx.h"
#include "FlashOperationImpl.h"

HMODULE g_hModule = NULL;

std::string GetAppPath()
{
    CHAR szAppPath[MAX_PATH] = { 0 };
    GetModuleFileNameA( g_hModule, szAppPath, MAX_PATH );
    LPSTR lpChar = strrchr( szAppPath, '\\' );
    if ( NULL != lpChar )
    {
        *lpChar = L'\0';
    }
    return ( std::string )szAppPath + "\\";
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

SP_EXPORT IFlashOperation* SP_API CreateFlashOperationObject( LPVOID pLogUtil )
{
    IFlashOperation* pObject = NULL;
    try
    {
        pObject = new CFlashOperationImpl( pLogUtil );
    }
    catch ( const std::bad_alloc& /*e*/ )
    {
        pObject = NULL;
    }
    
    return pObject;
}
