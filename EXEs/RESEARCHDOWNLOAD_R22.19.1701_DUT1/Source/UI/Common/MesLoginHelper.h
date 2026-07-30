#pragma once

class CMesLoginHelper
{
public:
    CMesLoginHelper( void );
    virtual ~CMesLoginHelper( void );
    
public:
    BOOL MesEnable();
    void MesGetInfo( LPWSTR lpMesInfo, DWORD dwSize );
    void MesLogin();
};

