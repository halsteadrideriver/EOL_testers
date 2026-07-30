#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct _tagPROJECT_CONFIG
{
    int nCheck;
    WCHAR szDisplayText[64];
    WCHAR szProjectID[64];
    WCHAR szProjectBase[64];
    WCHAR szFileID[64];
    
    _tagPROJECT_CONFIG()
    {
        memset( this, 0, sizeof( _tagPROJECT_CONFIG ) );
    }
} PROJECT_CONFIG, * LPPROJECT_CONFIG;
typedef const PROJECT_CONFIG* LPCPROJECT_CONFIG;

typedef struct _tagPRELOAD_CONFIG
{
    int nCheck;
    WCHAR szImageNameCode[64];
    WCHAR szImageNameBase[64];
    WCHAR szCountryID[64];
    WCHAR szCountryName[64];
    WCHAR szFileID[64];

    _tagPRELOAD_CONFIG()
    {
        memset(this, 0, sizeof(_tagPRELOAD_CONFIG));
    }
} PRELOAD_CONFIG, * LPPRELOAD_CONFIG;
typedef const PRELOAD_CONFIG* LPCPRELOAD_CONFIG;