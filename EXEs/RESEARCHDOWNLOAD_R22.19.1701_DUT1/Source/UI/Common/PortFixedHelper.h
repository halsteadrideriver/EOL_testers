#pragma once
#include<set>

class CDLoaderView;
class CPortFixedHelper
{
public:
    CPortFixedHelper( CDLoaderView* pView );
    ~CPortFixedHelper();
    
public:
    void LoadSettings();
    void SaveSettings();
    void AddFixedProg( int nPort );
    void InitPortList( );
    
private:
    BOOL m_bEnable = FALSE;
    TCHAR m_szPortList[512] = {0};
    CDLoaderView* m_pView = NULL;
    std::set<INT> m_setPort;
};

