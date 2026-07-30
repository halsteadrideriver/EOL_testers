#pragma once

class CAppVer
{
public:
    CAppVer( void );
    virtual ~CAppVer( void );
    
    BOOL    Init( HMODULE hApp );
    BOOL    InitEx( LPCWSTR lpszFileName );
    LPCWSTR GetVersion( void )const  { return m_lpAppVer;   };
    LPCWSTR GetVendor( void )const   { return m_lpVendor;   };
    LPCWSTR GetFileName( void )const { return m_lpFileName; };
    LPCWSTR GetProVer( void )const { return m_lpProVer; };
    
private:
    LPWSTR m_lpVerInfo;
    LPWSTR m_lpAppVer;
    LPWSTR m_lpVendor;
    LPWSTR m_lpFileName;
    LPWSTR m_lpProVer;
};
