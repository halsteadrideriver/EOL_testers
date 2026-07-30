#pragma once
#pragma warning(push,2)
#include <vector>
#include <algorithm>
#pragma warning(pop)

struct LCD_ITEM
{
    DWORD dwFlag;
    DWORD dwFlagOffset;
    DWORD dwNameLen;
    TCHAR szName[128];
};
typedef LCD_ITEM* LPLCD_ITEM;

typedef std::vector<LCD_ITEM> VEC_LCD_CFIG;

class CLcdHelper
{
public:
    static BOOL FindLCDItem( LPCTSTR lpszFilePath, VEC_LCD_CFIG& vLcdCfig, FILETIME* pLastWriteTime );
    static BOOL CheckLCDConfig( VEC_LCD_CFIG& vLcdCfig1, VEC_LCD_CFIG& vLcdCfig2 );
};

