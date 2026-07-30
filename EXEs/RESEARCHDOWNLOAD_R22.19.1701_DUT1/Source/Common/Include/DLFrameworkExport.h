/*****************************************************************************
** File Name:      DLFrameworkExport.h                                       *
** Author:         UNISOC                                                    *
** DATE:           05/18/2022                                                *
** Copyright:      UNISOC Technologies Co., Ltd.                             *
** Description:    This files contains dll interface.                        *
******************************************************************************

******************************************************************************
**                        Edit History                                       *
** ------------------------------------------------------------------------- *
** DATE               DESCRIPTION                                            *
** 05/18/2022         Create.                                                *
******************************************************************************/

#pragma once
#include "def\global_err.h"
#include "def\global_def.h"
#include "def\callback_def.h"
#include "def\packet_def.h"
#include "def\framework_def.h"

/**
 * @brief Creating System Resources
 *
 * @param [in] eDLType: Download type                   @see DL_TYPE_ENUM
 *            -# RESEARCH_TYPE : ResearchDownload
 *            -# FACTORY_TYPE : FactoryDownload
 *            -# UPGRADE_TYPE : UpgradeDownload
 *
 * @param [in] lpCallBack: Used to register Callback    @see SPCALLBACK_PARAM
 *
 * @param [in] pLogUtil: ISpLog Handle                  @see class ISpLog
 *
 * @return return SP_OK if successful, return other values if failure;
 */
SP_EXPORT SPRESULT  SP_API DLFW_Startup( DL_TYPE_ENUM eDLType, LPCSPCALLBACK_PARAM lpCallBack, LPVOID pLogUtil );

/**
 * @brief Releasing System Resources
 *
 * @param none
 *
 * @return none
 */
SP_EXPORT void      SP_API DLFW_Cleanup( void );

/**
 * @brief Reload ini configuration file parameters
 *
 * @param none
 *
 * @return return SP_OK if successful, return other values if failure;
 */
SP_EXPORT SPRESULT  SP_API DLFW_ReloadSettings();


/**
 * @brief Synchronize the frame parameters to all task
 *
 * @param none
 *
 * @return return SP_OK if successful, return other values if failure;
 */
SP_EXPORT SPRESULT  SP_API DLFW_SyncParameters();


/**
 * @brief Uncompress packet to the specified path and get packet information
 *
 * @param [in] lpszPacketPath: the path of the packet
 *
 * @param [in] lpszImageFilesDir: the path of uncompress packet
 *
 * @param [in] u32Timeout: Timeout of uncompress packet
 *
 * @return return SP_OK if successful, return other values if failure;
 */
SP_EXPORT SPRESULT  SP_API DLFW_LoadPacket( LPCWSTR lpszPacketPath, LPCWSTR lpszImageFilesDir, uint32 u32Timeout = INFINITE );


/**
 * @brief Uncompress packet to the specified path and get packet information
 *
 * @param [in] lpszImagesDir: the dir of the Images
 *
 * @param [in] u32Timeout: Timeout of uncompress packet
 *
 * @return return SP_OK if successful, return other values if failure;
 */
SP_EXPORT SPRESULT  SP_API DLFW_LoadFiles( LPCWSTR lpszImagesDir, uint32 u32Timeout = INFINITE );

/**
 * @brief Information used to generate Packet
 */
typedef struct _tagDLFW_DOPACKET_PARAMS
{
    LPCWSTR                 lpszPacketPath;       /*Specific Packet generation path*/
    LPCWSTR                 lpszProduct;          /*Product name*/
    LPBIN_PACKET_HEADER     lpPcaketHeader;       /*Packet header information*/
    LPFILE_INFO             lpAllFile;            /*Packet all files information*/
    uint32                  u32Timeout;           /*Timeout of generation packet*/
    STRUCT_INITIALIZE( _tagDLFW_DOPACKET_PARAMS );
} DLFW_DOPACKET_PARAMS, * LPDLFW_DOPACKET_PARAMS;
typedef const DLFW_DOPACKET_PARAMS* LPCDLFW_DOPACKET_PARAMS;

/**
 * @brief Generation packet to the specified path
 *
 * @param [in] lpParams: Refer to the definition of DLFW_DOPACKET_PARAMS @see DLFW_DOPACKET_PARAMS
 *
 */
SP_EXPORT SPRESULT  SP_API DLFW_DoPacket( LPCDLFW_DOPACKET_PARAMS lpParams );

/**
 * @brief Start or end the USB device plug monitoring
 *
 * @param [in] bStart: Start or end
 *               - TRUE : Start the USB device plug monitoring
 *               - FALSE : End the USB device plug monitoring
 *
 */
SP_EXPORT SPRESULT  SP_API DLFW_DeviceMoniter( BOOL bStart = TRUE );

/**
 * @brief Creating Task Resources
 *
 * @param [in] lpCallBack: Used to register Callback    @see SPCALLBACK_PARAM
 *
 * @param [in] pLogUtil: ISpLog Handle                  @see class ISpLog
 *
 * @param [in] bUart: USB or UART communication
 *               - TRUE : UART communication
 *               - FALSE : USB communication
 *
 * @return return non-null if successful, return null if failure;
 */
SP_EXPORT SP_HANDLE SP_API DLFW_CreateTask( LPCSPCALLBACK_PARAM lpCallBack, LPVOID pLogUtil, BOOL bUart = FALSE );

/**
 * @brief Releasing Task Resources
 *
 * @param [in] hTask: Handle created by DLFW_CreateTask
 *
 * @return none;
 */
SP_EXPORT void      SP_API DLFW_FreeTask( SP_HANDLE hTask );

/**
 * @brief Run the downloaded
 *
 * @param [in] hTask: Handle created by DLFW_CreateTask
 *
 * @param [in] dwPort: Device port number
 *
 * @return return SP_OK if successful, return other values if failure;
 */
SP_EXPORT SPRESULT  SP_API DLFW_RunTask( SP_HANDLE hTask, DWORD dwPort );

/**
 * @brief Stop the downloaded
 *
 * @param [in] hTask: Handle created by DLFW_CreateTask
 *
 * @return return SP_OK if successful, return other values if failure;
 */
SP_EXPORT SPRESULT  SP_API DLFW_StopTask( SP_HANDLE hTask );


/**
* @brief DLFW_PROPERTY_TYPE_ENUM    : - Parameter  < Transition Type Description >
*
* @param DLFW_ATTR_PRODUCT_NAME_ALL : - lppData    < LPWSTR* >
                                      - u32Size    < uint32 >
* @param DLFW_ATTR_PRODUCT_INFO_ALL : - lppData    < PPRODUCT_INFO_T* >
                                      - u32Size    < uint32 >
* @param DLFW_ATTR_BACKUP_FILES     : - lppData    < PFILE_INFO_T* >
                                      - u32Size    < uint32 >
* @param DLFW_ATTR_LCD_CONFIG       : - lppData    < LPLCD_ITEM* >
                                      - u32Size    < uint32 >
* @param DLFW_ATTR_BACKUP_NV_ITEMS  : - lppData    < PNV_BACKUP_INFO_T* >
                                      - lpReserved < LPWSTR* >
                                      - u32Size    < uint32 >
* @param DLFW_ATTR_MULTI_LANG_FLAG  : - lppData    < UINT* >
                                      - u32Size    < uint32 >
* @param DLFW_ATTR_LCD_CONFIG       : - lppData    < UINT* >
                                      - u32Size    < uint32 >
* @param DLFW_ATTR_FLASH_OPERATION  : - lppData    < PFILE_INFO_T >
                                      - u32Size    < uint32 >
* @param DLFW_ATTR_PROJECT_CONFIG  : - lppData    < LPPROJECT_CONFIG >
                                      - u32Size    < uint32 >
                                      
*/
typedef struct _tagDLFW_ATTRDATA_COMMON
{
    PVOID* lppData;
    PVOID* lppReserved;
    uint32 u32Size;
    
    STRUCT_INITIALIZE( _tagDLFW_ATTRDATA_COMMON );
} DLFW_ATTRDATA_COMMON, * LPDLFW_ATTRDATA_COMMON;
typedef const DLFW_ATTRDATA_COMMON* LPCDLFW_ATTRDATA_COMMON;

/**
* @brief DLFW_PROPERTY_TYPE_ENUM    : - Parameter  < Type Description >
*
* @param DLFW_ATTR_DL_FILE_NAME     : - lppID      < LPWSTR* >
                                      - lppName    < LPWSTR* >
                                      - u32Size    < uint32 >
* @param DLFW_ATTR_DL_FILE_STATE    : - lppID      < LPWSTR* >
                                      - pn32Flag   < int32* >
                                      - u32Size    < uint32 >
* @param DLFW_ATTR_DL_FILE_SIZE     : - lppID      < LPWSTR* >
                                      - pu64Size    < uint64* >
                                      - pu64Offset   < uint32 >
                                      - u32Size    < uint32 >
* @param DLFW_ATTR_DL_DATA_INFO     : - lppName    < LPWSTR* >
                                      - pu64Size    < uint64* >
                                      - pu64Offset   < uint32 >
                                      - u32Size    < uint32 >
*/
typedef struct _tagDLFW_ATTRDATA_DLIMAGEINFO
{
    LPWSTR* lppID;
    LPWSTR* lppName;
    int32*  pn32Flag;
    uint64* pu64Size;
    uint64* pu64Offset;
    uint32  u32Size;
    
    STRUCT_INITIALIZE( _tagDLFW_ATTRDATA_DLIMAGEINFO );
} DLFW_ATTRDATA_DLIMAGEINFO, * LPDLFW_ATTRDATA_DLIMAGEINFO;
typedef const DLFW_ATTRDATA_DLIMAGEINFO* LPCDLFW_ATTRDATA_DLIMAGEINFO;

/**
* @brief Used to transmit scan SN information
*
* @param DLFW_PROPERTY_TYPE_ENUM::nProperty = DLFW_ATTR_BARCODE_SN;
*/
typedef struct _tagDLFW_ATTRDATA_BARCODE
{
    uint8 u8SN1Len;
    uint8 u8SN2Len;
    LPCWSTR pszSN1;
    LPCWSTR pszSN2;
    
    STRUCT_INITIALIZE( _tagDLFW_ATTRDATA_BARCODE );
} DLFW_ATTRDATA_BARCODE, * LPDLFW_ATTRDATA_BARCODE;
typedef const DLFW_ATTRDATA_BARCODE* LPCDLFW_ATTRDATA_BARCODE;

/**
* @brief Property
*/
enum DLFW_PROPERTY_TYPE_ENUM
{
    /// [DLFW][GET]      - Pointer to " IConfig** "
    DLFW_ATTR_CONFIG_OBJECT = 0,
    
    /// [DLFW][GET]      - Pointer to " IDevHound** "
    DLFW_ATTR_DEV_HOUND_OBJECT,
    
    /// [DLFW][SET]      - Pointer to " LPCWSTR "
    DLFW_ATTR_START_PATH,
    
    /// [DLFW][GET]      - Pointer to " LPWSTR* "
    DLFW_ATTR_PRD_VERSION,
    
    /// [DLFW][GET][SET] - Pointer to " PINT "
    DLFW_ATTR_PRODUCT_USE,
    
    /// [DLFW][GET]      - Pointer to " DLFW_ATTRDATA_COMMON - lppData"
    DLFW_ATTR_PRODUCT_NAME_ALL,
    
    /// [DLFW][GET]      - Pointer to " DLFW_ATTRDATA_COMMON - lppData "
    DLFW_ATTR_PRODUCT_INFO_ALL,
    
    /// [DLFW][SET]      - Pointer to " PINT "
    DLFW_ATTR_CHECK_DL_FILES,
    
    /// [DLFW][GET]      - Pointer to " LPDLFW_ATTRDATA_IMAGEINFO - lpID - lpName "
    DLFW_ATTR_DL_FILE_NAME,
    
    /// [DLFW][GET]      - Pointer to " LPDLFW_ATTRDATA_IMAGEINFO - lpID - u8Flag "
    DLFW_ATTR_DL_FILE_STATE,
    
    /// [DLFW][GET]      - Pointer to " LPDLFW_ATTRDATA_IMAGEINFO - lpID - llSize "
    DLFW_ATTR_DL_FILE_SIZE,
    
    /// [DLFW][GET]      - Pointer to " LPDLFW_ATTRDATA_IMAGEINFO - lpName - llSize - llOffset "
    DLFW_ATTR_DL_DATA_INFO,
    
    /// [DLFW][GET][SET]  - Pointer to " BOOL* "
    DLFW_ATTR_MULTI_LANG_RIGHTNV,
    
    /// [DLFW][GET][SET]  - Pointer to " BOOL* "
    DLFW_ATTR_MULTI_LANG_BACKUP,
    
    /// [DLFW][GET][SET]  - Pointer to " LPDLFW_ATTRDATA_COMMON - lppData "
    DLFW_ATTR_MULTI_LANG_FLAG,
    
    /// [DLFW][GET][SET]  - Pointer to " DLFW_ATTRDATA_COMMON - lppData "
    DLFW_ATTR_BACKUP_FILES,
    
    /// [DLFW][GET][SET]  - Pointer to " BOOL* "
    DLFW_ATTR_BACKUP_NV_ENABLE,
    
    /// [DLFW][GET][SET]  - Pointer to " DLFW_ATTRDATA_COMMON - lppData - lppReserved "
    DLFW_ATTR_BACKUP_NV_ITEMS,
    
    /// [DLFW][SET]       - Pointer to " LPDLFW_ATTRDATA_COMMON - lppData "
    DLFW_ATTR_FLASH_OPERATION,

	/// [DLFW][SET]       - Pointer to " LPDLFW_ATTRDATA_COMMON - lppData "
	DLFW_ATTR_FLASH_OPERATION_EX,
    
    /// [DLFW][SET]       - Pointer to " BOOL* "
    DLFW_ATTR_FLASH_ERASE_ALL,
    
    /// [DLFW][GET][SET]  - Pointer to " BOOL* "
    DLFW_ATTR_LCD_HAS,
    
    /// [DLFW][GET][SET]  - Pointer to " DLFW_ATTRDATA_COMMON - lppData "
    DLFW_ATTR_LCD_CONFIG,
    
    /// [DLFW][GET][SET]  - Pointer to " LPCWSTR "
    DLFW_ATTR_LCD_FILE,
    
    /// [DLFW][GET]       - Pointer to " PLONG* "
    DLFW_ATTR_START_NV_REF,
    
    /// [DLFW][GET][SET]  - Pointer to " BOOL* "
    DLFW_ATTR_POWER_OFF,
    
    /// [TASK][SET]       - Pointer to " LPCWSTR "
    DLFW_ATTR_LOG_PATH,
    
    /// [TASK][SET]       - Pointer to " LPDLFW_BARCODE_SN "
    DLFW_ATTR_BARCODE_SN,
    
    /// [DLFW][GET]       - Pointer to " LPPROJECT_CONFIG ", Reference framework_def.h
    DLFW_ATTR_PROJECT_CONFIG_ALL,

    /// [DLFW][GET]       - Pointer to " LPPPRELOAD_CONFIG ", Reference framework_def.h
    DLFW_ATTR_PRELOAD_CONFIG_ALL,
    
    /// [DLFW][SET]       - Pointer to " LPCWSTR "
    DLFW_ATTR_PROJECT_CONFIG,

    /// [DLFW][SET]       - Pointer to " LPCWSTR "
    DLFW_ATTR_PRELOAD_CONFIG,
    
    /// [TASK][SET]
    DLFW_ATTR_NO_SN,

    DLFW_MAX_ATTR
} ;

/**
 * @brief Set Property to system resources
 *
 * @param [in] nProperty: Refer to the definition of DLFW_PROPERTY_TYPE_ENUM
 *
 * @param [in] nFlags: Reserved parameter
 *
 * @param [in] lpValue: Refer to the definition of DLFW_ATTRDATA_COMMON or DLFW_ATTRDATA_DLIMAGEINFO
 * @see Please refer to DLFW_ATTRDATA_COMMON or DLFW_ATTRDATA_DLIMAGEINFO for details
 *
 * @return return SP_OK if successful, return other values if failure;
 */
SP_EXPORT SPRESULT SP_API DLFW_SetProperty( INT nProperty, INT nFlags, LPCVOID lpValue );

/**
 * @brief Get Property to system resources
 *
 * @param [in] nProperty: Refer to the definition of DLFW_PROPERTY_TYPE_ENUM
 *
 * @param [in] nFlags: Reserved parameter
 *
 * @param [out] lpValue: Refer to the definition of DLFW_ATTRDATA_COMMON or DLFW_ATTRDATA_DLIMAGEINFO
 * @see Please refer to DLFW_ATTRDATA_COMMON or DLFW_ATTRDATA_DLIMAGEINFO for details
 *
 * @return return SP_OK if successful, return other values if failure;
 */
SP_EXPORT SPRESULT SP_API DLFW_GetProperty( INT nProperty, INT nFlags, LPVOID lpValue );

/**
 * @brief Set Property to task resources
 *
 * @param [in] hTask: Handle created by DLFW_CreateTask
 *
 * @param [in] nProperty: Refer to the definition of DLFW_PROPERTY_TYPE_ENUM
 *
 * @param [in] nFlags: Reserved parameter
 *
 * @param [in] lpValue: Refer to the definition of DLFW_ATTRDATA_BARCODE
 * @see Please refer to DLFW_ATTRDATA_BARCODE for details
 *
 * @return return SP_OK if successful, return other values if failure;
 */
SP_EXPORT SPRESULT SP_API DLFW_TaskSetProperty( SP_HANDLE hTask, INT nProperty, INT nFlags, LPCVOID lpValue );

/**
 * @brief Get Property to task resources; Reserved API;
 *
 * @param [in] hTask: Handle created by DLFW_CreateTask
 *
 * @param [in] nProperty:
 *
 * @param [in] nFlags: Reserved parameter
 *
 * @param [out] lpValue:
 *
 * @return return SP_OK if successful, return other values if failure;
 */
SP_EXPORT SPRESULT SP_API DLFW_TaskGetProperty( SP_HANDLE hTask, INT nProperty, INT nFlags, LPVOID lpValue );

