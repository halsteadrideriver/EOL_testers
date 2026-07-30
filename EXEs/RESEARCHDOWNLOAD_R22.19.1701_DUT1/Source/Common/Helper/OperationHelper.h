#pragma once
#include "BMAGlobal.h"
#include "DLFWDef.h"
#include "IConfig.h"
#include "LcdHelper.h"
#include "DLFrameworkExport.h"

class COperationHelper
{
public:
    COperationHelper();
    ~COperationHelper();
    
public:
    HMODULE     GetCurrentModule();
    BOOL        GetModuleFilePath( HMODULE hModule, LPTSTR lpszAppPath, DWORD dwSize );
    BOOL        CreateDeepDirectory( LPCTSTR lpszDir );
    BOOL        GetFilePathInfo( LPCTSTR lpszFile, CStringArray& agInfo );
    BOOL        SaveFileToLocal( LPCTSTR pszFileName, LPBYTE pBuf, DWORD dwSize );
    BOOL        DeleteDirectory( LPCTSTR lpszDirName, BOOL bRmFolder = TRUE );
    BOOL        GetAbsolutePath( HMODULE hModule, LPCTSTR lpszFilePath, CString& strAbsoluteFilePath );
    CString     GetStrFileTitle( LPCTSTR lpFile, BOOL bMakeUpper = FALSE, BOOL bPostfix = FALSE );
    uint64      GetFileSizeEx( LPCTSTR lpFileName );
    void        GetLastModifyTime( LPCTSTR lpszFile, FILETIME& ft );
    CString     GetStrFilePath( LPCTSTR lpFile, BOOL bMakeUpper = FALSE, BOOL bPostfix = FALSE );
    LPBYTE      GetKeyDataFromString( LPCTSTR pszBuf, DWORD dwCount, int& nLen );
    int         GetDLFiles( BOOL bCheckMatch, PPRODUCT_INFO_T pProductInfo, MAP_DLFILE* pMapDLFiles, MAP_DLSTATE* pMapDLState, CStringArray& agFiles );
    int         GetAllFileInfo( PPRODUCT_INFO_T pProductInfo, LPDWORD ppFileInfo );
    int         GetFileInfo( PPRODUCT_INFO_T pProductInfo, LPCTSTR lpszFileID, LPDWORD ppFileInfo );
    CString     GetFilePostfix( LPCTSTR lpFile, BOOL bMakeUpper = FALSE );
    BOOL        GetFdl2Flag( LPCTSTR lpszFilePath, UINT nType, CString& strErrMsg );
    __int64     GetPartitionSize( LPCTSTR lpPartName, PPRODUCT_INFO_T pProdInfo );
    BOOL        GetSection( IConfig* pConfig, DLFW_CFGTYPE_ENUM eType, LPCTSTR lpSection, CStringArray& arrString, BOOL bAddKey = FALSE );
    BOOL        GetSectionMcp( IConfig* pConfig, CStringArray& arrString, MAP_MCP& mapMcp );
    BOOL        GetSectionImeiId( IConfig* pConfig, CUIntArray& arrUInt );
    int         GetNvBkpItemCount( MAP_NV_BAK& mapNvBkpItm, LPCTSTR lpNvID );
    PNV_BACKUP_ITEM_T   GetNvBkpItemInfo( MAP_NV_BAK& mapNvBkpItm, int nIndex, LPCTSTR lpNvID );
    
    void        CreatCommonData( uint32 u32Size, LPDLFW_ATTRDATA_COMMON lpData );
    void        CreatDlImageInfo( uint32 u32Size, LPDLFW_ATTRDATA_DLIMAGEINFO lpData );
    
    void        ReleaseCommonData( LPDLFW_ATTRDATA_COMMON lpData );
    void        ReleaseDlImageInfo( LPDLFW_ATTRDATA_DLIMAGEINFO lpData );
    
    SPRESULT    ConfigAttrDataByBackupNvItems( MAP_NV_BAK& mapNvBkpItm, LPDLFW_ATTRDATA_COMMON lpData );
    SPRESULT    ConfigAttrDataByBackupFiles( VEC_FILE_INFO& arrBackFiles, LPDLFW_ATTRDATA_COMMON lpData );
    SPRESULT    ConfigAttrDataByProductInfo( VEC_PRODUCT_INFO& arrProductInfo, LPDLFW_ATTRDATA_COMMON lpData );
    SPRESULT    ConfigAttrDataByProductName( CStringArray& arrProductName, LPDLFW_ATTRDATA_COMMON lpData );
    SPRESULT    ConfigAttrDataByDlFileName( MAP_DLFILE& mapDLFiles, LPDLFW_ATTRDATA_DLIMAGEINFO lpData );
    SPRESULT    ConfigAttrDataByDlFileState( MAP_DLSTATE& mapDLState, LPDLFW_ATTRDATA_DLIMAGEINFO lpData );
    SPRESULT    ConfigAttrDataByDlFileSize( MAP_DLSIZE& mapDLSize, LPDLFW_ATTRDATA_DLIMAGEINFO lpData );
    SPRESULT    ConfigAttrDataByDlDataInfo( MAP_DATAINFO& mapDataInfo, LPDLFW_ATTRDATA_DLIMAGEINFO lpData );
    SPRESULT    ConfigAttrDataByLcdConfig( VEC_LCD_CFIG& arrLCDCfig, LPDLFW_ATTRDATA_COMMON lpData );
    SPRESULT    ConfigAttrDataByCUIntArray( CUIntArray& arrUInt, LPDLFW_ATTRDATA_COMMON lpData );
    SPRESULT    ConfigAttrDataByProjectConfig( VEC_PROJECT_CONFIG& arrProjectCfg, LPDLFW_ATTRDATA_COMMON lpData );
    SPRESULT    ConfigAttrDataByPreloadConfig( VEC_PRELOAD_CONFIG& arrProjectCfg, LPDLFW_ATTRDATA_COMMON lpData );
    
    SPRESULT    ConfigProductNameByAttrData( CStringArray& arrProductName, LPDLFW_ATTRDATA_COMMON lpData );
    SPRESULT    ConfigProductInfoByAttrData( VEC_PRODUCT_INFO& arrProductInfo, LPDLFW_ATTRDATA_COMMON lpData );
    SPRESULT    ConfigDlFileNameByAttrData( MAP_DLFILE& mapDLFiles, LPDLFW_ATTRDATA_DLIMAGEINFO lpData );
    SPRESULT    ConfigDlFileStateByAttrData( MAP_DLSTATE& mapDLState, LPDLFW_ATTRDATA_DLIMAGEINFO lpData );
    SPRESULT    ConfigDlFileSizeByAttrData( MAP_DLSIZE& mapDLSize, LPDLFW_ATTRDATA_DLIMAGEINFO lpData );
    SPRESULT    ConfigDlDataInfoByAttrData( MAP_DATAINFO& mapDataInfo, LPDLFW_ATTRDATA_DLIMAGEINFO lpData );
    SPRESULT    ConfigBackupNvItemsByAttrData( MAP_NV_BAK& mapNvBkpItm, LPDLFW_ATTRDATA_COMMON lpData );
    SPRESULT    ConfigBackupFilesByAttrData( VEC_FILE_INFO& arrBackFiles, LPDLFW_ATTRDATA_COMMON lpData );
    SPRESULT    ConfigLcdConfigByAttrData( VEC_LCD_CFIG& arrLCDCfig, LPDLFW_ATTRDATA_COMMON lpData );
    SPRESULT    ConfigProjectConfigByAttrData( VEC_PROJECT_CONFIG& arrProjectCfg, LPDLFW_ATTRDATA_COMMON lpData );
    SPRESULT    ConfigPreloadConfigByAttrData( VEC_PRELOAD_CONFIG& arrPreloadCfg, LPDLFW_ATTRDATA_COMMON lpData );
    SPRESULT    ConfigCUIntArrayByAttrData( CUIntArray& arrUInt, LPDLFW_ATTRDATA_COMMON lpData );
    SPRESULT    ClearMapNvBakItems( MAP_NV_BAK& mapNvBkpItm );
    SPRESULT    UpdatedNvBkpMap( MAP_NV_BAK& mapNvBkpItm, LPCTSTR lpszID, PNV_BACKUP_INFO_T pNVBakItemInfo );
};