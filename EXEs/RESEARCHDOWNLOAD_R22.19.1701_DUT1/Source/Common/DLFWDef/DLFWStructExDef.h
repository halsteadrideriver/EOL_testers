#pragma once

/////////////////////////////////////////////////////////////////////////////////
/// PORT_DATA
/////////////////////////////////////////////////////////////////////////////////
#define X_SN_LEN                (64)

struct PORT_DATA
{
    HANDLE hSNEvent;
    LPBYTE lpPhaseCheck;
    char   szSN[X_SN_LEN + 1];
    char   szSN2[X_SN_LEN + 1];
    PORT_DATA()
    {
        memset( this, 0, sizeof( PORT_DATA ) );
    }
};


/////////////////////////////////////////////////////////////////////////////////
/// _SOFTSIM_INFO
/////////////////////////////////////////////////////////////////////////////////
#define CUST_MISCDATA_OFFSET        (0xC0000)   //768K
#define SOFT_VERSION_LEN            (100)
#define TCT_CU_REF_LEN              (20)
#pragma pack(push, 1)
struct MISCDATA_TCL
{
    CHAR szCU[TCT_CU_REF_LEN];
    CHAR szReserved[4];
    INT  nRootFlag; // 0x52: root   0x4E: un-root
    CHAR szSoftVersion[SOFT_VERSION_LEN];
    INT  nInproduction;
    MISCDATA_TCL()
    {
        memset( this, 0, sizeof( MISCDATA_TCL ) );
    }
};
#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////////
/// _SOFTSIM_INFO
/////////////////////////////////////////////////////////////////////////////////
typedef struct tagSOFTSIM_INFO
{
    TCHAR szFile[MAX_PATH];
    BYTE* pBuf;
    DWORD dwSize;
    tagSOFTSIM_INFO()
    {
        memset( this, 0, sizeof( tagSOFTSIM_INFO ) );
    }
    void Clear()
    {
        SAFE_DELETE_ARRAY( pBuf );
        memset( this, 0, sizeof( tagSOFTSIM_INFO ) );
    }
} SOFTSIM_INFO, *LPSOFTSIM_INFO;

/////////////////////////////////////////////////////////////////////////////////
/// _BACKUP_INFO
/////////////////////////////////////////////////////////////////////////////////
typedef struct _BACKUP_INFO
{
    TCHAR szNVFile[MAX_PATH];
    BYTE* pBuf;
    DWORD dwSize;
    _BACKUP_INFO()
    {
        memset( this, 0, sizeof( _BACKUP_INFO ) );
    }
    void Clear()
    {
        SAFE_DELETE_ARRAY( pBuf );
        memset( this, 0, sizeof( _BACKUP_INFO ) );
    }
    
} BACKUP_INFO, *BACKUP_INFO_PTR;

/////////////////////////////////////////////////////////////////////////////////
/// _DATA_INFO
/////////////////////////////////////////////////////////////////////////////////
typedef struct _DATA_INFO
{
    BYTE* pBuf;
    DWORD dwSize;
    _DATA_INFO()
    {
        memset( this, 0, sizeof( _DATA_INFO ) );
    }
    void Clear()
    {
        SAFE_DELETE_ARRAY( pBuf );
        memset( this, 0, sizeof( _DATA_INFO ) );
    }
    
} DATA_INFO, *PDATA_INFO_PTR;

/////////////////////////////////////////////////////////////////////////////////
/// _DLUID_INFO
/////////////////////////////////////////////////////////////////////////////////
#define DLUID_SECTION    _T("kpanic")
#define DLUID_SIZE       (256*1024)
#define DLUID_LEN        8

typedef struct _tagDLUID_INFO
{
    BYTE*  pBuf;
    DWORD  dwSize;
    TCHAR  szUID[20];
    
} DLUID_INFO, *LPDLUID_INFO;


/////////////////////////////////////////////////////////////////////////////////
/// _REF_INFO_HEADER_T_
/////////////////////////////////////////////////////////////////////////////////
#define REFT_MAGIC_NUMBER       (0x54464552)    //"REFT"

typedef struct _REF_INFO_HEADER_T_
{
    _REF_INFO_HEADER_T_()
    {
        memset( this, 0, sizeof( _REF_INFO_HEADER_T_ ) );
        nMagic = REFT_MAGIC_NUMBER;
    }
    unsigned int nMagic;        //"REFT"
    WORD wYear;
    WORD wMonth;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    unsigned int nReserved[4];
} REF_INFO_HEADER_T, *REF_INFO_HEADER_PTR;

/////////////////////////////////////////////////////////////////////////////////
/// _BMOBJ
/////////////////////////////////////////////////////////////////////////////////
#define CHIP_UID_LEN_16         (16)
#define CHIP_UID_LEN_64         (64)
#define FLASH_UID_LEN           (16)
#define MAX_BACKUP_FILE_NUM     (10)
#define CUST_REF_INFO_LEN       (MAX_REF_INFO_LEN + 64 )

typedef struct tagBOOT_MODE_PARAMS
{
    tagBOOT_MODE_PARAMS()
    {
        memset( this, 0, sizeof( tagBOOT_MODE_PARAMS ) );
    }
    
    void Clear()
    {
        for ( int i = 0; i < MAX_BACKUP_FILE_NUM; i++ )
        {
            SAFE_DELETE_ARRAY( tFileBackup[i].pBuf );
            SAFE_DELETE_ARRAY( tNVBackup[i].pBuf );
        }
        SAFE_DELETE_ARRAY( lpRefData );
        SAFE_DELETE_ARRAY( tDLUIDInfo.pBuf );
        tSoftSim.Clear();
        memset( this, 0, sizeof( tagBOOT_MODE_PARAMS ) );
    }
    
    void InitRefInfo()
    {
        REF_INFO_HEADER_T stRefHeader;
        SYSTEMTIME  time ;
        GetLocalTime( &time );
        stRefHeader.wYear = time.wYear;
        stRefHeader.wMonth = time.wMonth;
        stRefHeader.wDay = time.wDay;
        stRefHeader.wHour = time.wHour;
        stRefHeader.wMinute = time.wMinute;
        stRefHeader.wSecond = time.wSecond;
        
        if ( NULL == lpRefData )
        {
            lpRefData = new BYTE[CUST_REF_INFO_LEN];
        }
        if ( lpRefData )
        {
            ZeroMemory( lpRefData, CUST_REF_INFO_LEN );
            memcpy( lpRefData, &stRefHeader, sizeof( REF_INFO_HEADER_T ) );
        }
    }
    BOOL  bResult;
    DWORD dwIsUart;
    DWORD dwChipID;
    DWORD aFlashType[4];
    char  szSN[X_SN_LEN + 1];
    char  szSN2[X_SN_LEN + 1];
    char  szIMEI[X_SN_LEN + 1];
    TCHAR szErrorMsg[_MAX_PATH * 2];
    TCHAR szMcpInfo[_MAX_PATH];
    TCHAR szBlockPageSize[64];
    BACKUP_INFO tFileBackup[MAX_BACKUP_FILE_NUM];
    BACKUP_INFO tNVBackup[MAX_BACKUP_FILE_NUM];
    DLUID_INFO tDLUIDInfo;
    BOOL    bStoping;
    BOOL    bRMDev;
    char    szFlashUID[FLASH_UID_LEN + 1];
    SOFTSIM_INFO tSoftSim;
    int nStage;//1: need second enum, 0/2: done!
    char    szChipUID[CHIP_UID_LEN_64 + 1];
    char    szTestResultGUID[50];
    LPBYTE  lpRefData;   // CUST_REF_INFO_LEN
    __int64 llCodeSize;
    DWORD   dwRFChipType;
    DWORD   dwOldMemoryType;
    DWORD   nState;
    
    /// 20220729 hong.yang
    __int64 llDownloadedSize;
    DWORD dwStartTime;
    DWORD dwEndTime;
    TCHAR szImageName[_MAX_PATH];
    
} BOOT_MODE_PARAMS, *LPBOOT_MODE_PARAMS;

/////////////////////////////////////////////////////////////////////////////////
/// _EXT_IMG_INFO
/////////////////////////////////////////////////////////////////////////////////
typedef struct _EXT_IMG_INFO
{
    _EXT_IMG_INFO()
    {
        memset( this, 0, sizeof( _EXT_IMG_INFO ) );
    }
    DWORD dwSize;
    BYTE* pBuf;
    TCHAR szFilePath[MAX_PATH];
    BOOL   bIsFileMap;
    HANDLE hFile;
    HANDLE hFileMap;
    DWORD  dwFirstMapSize;
    
    void clear()
    {
        if ( !bIsFileMap )
        {
            if ( pBuf != NULL )
            {
                delete [] pBuf;
            }
        }
        else
        {
            if ( pBuf != NULL )
            {
                ::UnmapViewOfFile( pBuf );
            }
            if ( hFile != NULL )
            {
                ::CloseHandle( hFile );
            }
            if ( hFileMap != NULL )
            {
                ::CloseHandle( hFileMap );
            }
        }
        
        memset( this, 0, sizeof( _EXT_IMG_INFO ) );
    }
} EXT_IMG_INFO, *EXT_IMG_INFO_PTR;


/////////////////////////////////////////////////////////////////////////////////
/// _DUT_KEY_T
/////////////////////////////////////////////////////////////////////////////////
typedef struct _DUT_KEY_T
{
    _DUT_KEY_T()
    {
        memset( this, 0, sizeof( _DUT_KEY_T ) );
        ver[0] = '1';
    }
    BYTE  ver[4];
    char  szDUTKey[MAX_PATH];   // key
    BYTE  Reserved[760];        // unused
} DUT_KEY_T, *PDUT_KEY_T;

/////////////////////////////////////////////////////////////////////////////////
/// _COMPARE_FILE_INFO_
/////////////////////////////////////////////////////////////////////////////////
typedef struct _COMPARE_FILE_INFO_
{
    _COMPARE_FILE_INFO_()
    {
        memset( this, 0, sizeof( _COMPARE_FILE_INFO_ ) );
    }
    TCHAR  szFile1[MAX_PATH];   //org file
    TCHAR  szFile2[MAX_PATH];   //read back file
} COMPARE_FILE_INFO, *PCOMPARE_FILE_INFO_PTR;


/////////////////////////////////////////////////////////////////////////////////
/// _DATA_INFO_T
/////////////////////////////////////////////////////////////////////////////////
typedef struct _DATA_INFO_T
{
    unsigned __int64 llSize;
    unsigned __int64 llOffset;
    _DATA_INFO_T()
    {
        memset( this, 0, sizeof( _DATA_INFO_T ) );
    }
} DATA_INFO_T;


/////////////////////////////////////////////////////////////////////////////////
/// _NV_BACKUP_INFO_T
/////////////////////////////////////////////////////////////////////////////////
typedef struct _NV_BACKUP_INFO_T
{
    PNV_BACKUP_ITEM_T   pNvBkpItmArray;
    int                 nNvBkpItmCount;
    BOOL                bBackNV;
    int                 nNvBkpItmChkCount;
    TCHAR               szNotChkItemName[MAX_PATH * 4];
    _NV_BACKUP_INFO_T()
    {
        memset( this, 0, sizeof( _NV_BACKUP_INFO_T ) );
    }
    void Clear()
    {
        if ( NULL != pNvBkpItmArray )
        {
            delete[]pNvBkpItmArray;
            pNvBkpItmArray = NULL;
        }
        memset( this, 0, sizeof( _NV_BACKUP_INFO_T ) );
    }
} NV_BACKUP_INFO_T, * PNV_BACKUP_INFO_T;


/////////////////////////////////////////////////////////////////////////////////
/// _SPPT_HEADER_T
/// _SPPT_TABLE_T
/////////////////////////////////////////////////////////////////////////////////

/*
    magic(4Byte) | Version(1Byte) | Unit(1Byte) | table count(1Byte)|Reserved(1Byte) | table tag(4) | table offset(2)| table size(2)|
    Magic:          0x3A726170,  字符串"par:"
    Version:        0x01, 当前版本0x01
    Unit:           0x0,  1M Byte; 0x1, 512K Byte; 0x2, 1K Byte; 0x3, 1 Byte; 0x4, 1 Sector
    table count:    分区表的个数
    Table tag:      0x3A6C6274，字符串"tbl:", 分区表的标签
    table offset:   从magic开始的偏移，当前应该是0x10
    table size:     总的分区表占用的字节数
*/

#define SPPT_HEADER_MAGIC   0x3A726170  //"par:"
#define SPPT_TABLE_TAG      0x3A6C6274  //"tbl:"

typedef struct _SPPT_HEADER_T
{
    _SPPT_HEADER_T()
    {
        memset( this, 0, sizeof( _SPPT_HEADER_T ) );
        dwMagic = SPPT_HEADER_MAGIC;
        byteVersion = 1;
        byteUnit = 0x03; //1 Byte
    }
    DWORD   dwMagic;
    BYTE    byteVersion;
    BYTE    byteUnit;
    BYTE    byteTblCount;
    BYTE    Reserved;
} SPPT_HEADER_T, * PSPPT_HEADER_T;

typedef struct _SPPT_TABLE_T
{
    _SPPT_TABLE_T()
    {
        memset( this, 0, sizeof( _SPPT_TABLE_T ) );
        dwTag = SPPT_TABLE_TAG;
    }
    DWORD   dwTag;
    WORD    wOffset;
    WORD    wSize;
} SPPT_TABLE_T, * PSPPT_TABLE_T;
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
