/******************************************************************************
*  @Copyright:     (C) 2019 UNISOC Technologies Co.,Ltd. All Rights Reserved.
*  @Filename:      Unisoc_Solution_MES.h
*  @Author:        MES Team
*  @Date:          2019/6/3
*  @Description:
*****************************************************************************/
#define __EXPORTS 1
#ifdef __cplusplus
#ifdef __EXPORTS
#define UNISOC_EXPORT extern "C" __declspec (dllexport)
#else
#define UNISOC_EXPORT extern "C" __declspec (dllimport)
#endif
#else
#ifdef __EXPORTS
#define UNISOC_EXPORT __declspec (dllexport)
#else
#define UNISOC_EXPORT __declspec (dllimport)
#endif
#endif

#define     UNISOC_API                      __stdcall
//*******************************************************************************/
/*                                                      返回值宏定义                                                                         */
//*******************************************************************************/
typedef enum
{
    MES_SUCCESS = 0,            //SQL执行成功
    MES_FAIL,                   //SQL执行失败
    MES_DISABLE_FAIL,           //MES离线状态
    MES_UNCONNECT_FAIL,         //SQL未连接
    MES_EOF_FAIL,               //记录集结束
    MES_OPENRS_FAIL,            //打开Recordset失败
    MES_ADDNEW_FAIL,            //添加记录失败
    MES_PUTCOLLECT_FAIL,        //设置列数据失败
    MES_GETCOLLECT_FAIL,        //获得列数据失败
    MES_UPDATE_FAIL,            //更新记录集失败
    MES_REPEATEDRS_FAIL,        //重复的记录
    MES_CHECKDONE_FAIL,         //检查前面是否做过失败
    MES_CHECKOVERRUN_FAIL,      //检查超出失败，比如装箱大于设定值
    MES_CHECKNULLPARAM_FAIL,    //检查输入的参数为空
    MES_NetCode_REPEATEDRS_FAIL,    //网标重复
    MES_SN1_REPEATEDRS_FAIL,        //SN1重复
    MES_SN2_REPEATEDRS_FAIL,        //SN1重复
    MES_IMEI1_REPEATEDRS_FAIL,      //IMEI重复
    MES_IMEI2_REPEATEDRS_FAIL,      //IMEI2重复
    MES_IMEI3_REPEATEDRS_FAIL,      //IMEI3重复
    MES_IMEI4_REPEATEDRS_FAIL,      //IMEI4重复
    MES_BTMAC_REPEATEDRS_FAIL,      //蓝牙地址重复
    MES_WIFIMAC_REPEATEDRS_FAIL,        //Wifi地址重复
    MES_WRITE_BINDING_FAIL,         //写绑定表失败
    MES_CURSTATION_NOT_EXIST_FAIL,      //检查站位时，发现当前站位不存在
    MES_CURSTATION_PASSED_FAIL,     //当前站位已测试pass
    MES_PRESTATION_NOTPASSED_FAIL,  //前一站未测试pass
    MES_TEST_LOCKED_FAIL,           //该SN已被锁定
    MES_CHECK_LOCKED_FAIL,          //检查MES锁定失败
    MES_CHECK_PRE_SN_FAIL,          //SN前缀不匹配
    MES_CHECK_SN_RANGE_FAIL,        //SN流水号超出设定范围
    MES_CHECK_SN_LENGTH_FAIL,       //SN长度不匹配
    MES_INVALID_HANDLE,             //INVALID HANDLE
    MES_INVALID_SESSION,            //INVALID SESSION
    MES_INVALID_BATCH,              //INVALID BATCH
    MES_SN_IS_BOUND,
    MES_EXCEPTION_FAIL,             //程序异常
    MES_NOT_SN_INPUT,               //不是投板工站
    MES_RESULT_SUM,                 //返回值类型总数
    MES_CLIENT_TYPE_NULL,           //客户端类型为空 binglin.wen
    MES_SEQFILE_NULL,                   //未上传Seq文件 binglin.wen
} MES_RESULT;

enum _ENUM_UNISOC_MES_LANGUAGE
{
    _E_LANGUAGE_NOSETTING,
    _E_LANGUAGE_ENGLISH,
    _E_LANGUAGE_CHINESE,
};

enum _ENUM_UNISOC_MES_LOGIN_TYPE
{
    _E_MES_LOGIN_TEST,
    _E_MES_LOGIN_LOGIN,
    _E_MES_LOGIN_UPLOAD,
    _E_MES_LOGIN_OTHER,
    NUM_OF_LOGIN_TYPE
};

enum _ENUM_UNISOC_MES_TYPE
{
    _E_UNISOC_MES_V2,
    _E_UNISOC_MES_V1
};

enum _ENUM_BARCODE_TYPE
{
    _E_BARCODE_SN1,
    _E_BARCODE_SN2,
    _E_BARCODE_IMEI1,
    _E_BARCODE_IMEI2,
    _E_BARCODE_IMEI3,
    _E_BARCODE_IMEI4,
    _E_BARCODE_WIFI,
    _E_BARCODE_BT,
    _E_BARCODE_NET_CODE,
    _E_BARCODE_NFC_CPLC,
    _E_BARCODE_SIM_ICCID,
    _E_BARCODE_SW_VER,
    _E_BARCODE_LCD,
    _E_BARCODE_CODE1,
    _E_BARCODE_CODE2,
    _E_BARCODE_CODE3,
    _E_BARCODE_CODE4,
    _E_BARCODE_CODE5,
    _E_BARCODE_MEID1,
    _E_BARCODE_MEID2,
    NUM_OF_BARCODE_TYPE
};

enum _ENUM_MES_V1_SERVER_TYPE
{
    _E_UNISOC_MES_V1_SERVER_SQL = 0,
    _E_UNISOC_MES_V1_SERVER_MYSQL,
    _E_UNISOC_MES_V1_SERVER_MDB
};

enum _ENUM_MES_V2_FLOW_CONTROL
{
    _E_FLOW_CONTROL_SN1,
    _E_FLOW_CONTROL_SN2
};

enum _ENUM_MES_UPLOAD_METHOD
{
    _E_UPLOAD_METHOD_FTP_UNISOC = 1,
    _E_UPLOAD_METHOD_FTP_LOCALLY = 2,
    _E_UPLOAD_METHOD_WEB_API = 3
};
/************************************************************************/
/*                                      Common Parameters                                                               */
/************************************************************************/
typedef struct _STEUCT_UNISOC_CMN_MES_LOGIN
{
    //Common
    char sz_cmn_mes_user[32];
    char sz_cmn_mes_pwd[32];
    //MES V2
    char sz_v2_url[512];
    _ENUM_UNISOC_MES_LANGUAGE e_v2_langure;
    _ENUM_UNISOC_MES_LOGIN_TYPE e_v2_login_type;
    //MES V1
    char sz_v1_server[512];
    char sz_v1_mysql_driver[64];
    char sz_v1_db[64];
    char sz_v1_db_user[64];
    char sz_v1_db_pwd[64];
    _ENUM_MES_V1_SERVER_TYPE e_v1_server_type;
    //Init
    _STEUCT_UNISOC_CMN_MES_LOGIN()
    {
        ZeroMemory( this, sizeof( *this ) );
    }
} _UNISOC_MES_CMN_LOGIN, *_pUNISOC_MES_CMN_LOGIN;

typedef struct _STEUCT_UNISOC_MES_CMN_TEST_RESULT
{
    //Common
    char sz_cmn_result_id[64];
    char sz_cmn_sn[64];
    char sz_cmn_batch[64];
    char sz_cmn_station[64];
    char sz_cmn_err_code[32];
    char sz_cmn_err_type[32];
    char sz_cmn_err_msg[512];
    int i_cmn_result;
    char sz_cmn_elapsed_time[64];
    //MES V2.0
    char sz_v2_log_path[512];
    //MES V1.0
    char sz_v1_tool_version[32];
    char sz_v1_information[128];
    //SoftwareVersion
    char sz_SoftwareVersionAP[500 - 1];
    char sz_SoftwareVersionCP[500 - 1];
    //Init
    _STEUCT_UNISOC_MES_CMN_TEST_RESULT()
    {
        ZeroMemory( this, sizeof( *this ) );
    }
} _UNISOC_MES_CMN_TEST_RESULT, *_pUNISOC_MES_CMN_TEST_RESULT;

typedef struct _STEUCT_UNISOC_MES_CMN_TEST_DATA
{
    char sz_cmn_batch[64];
    char sz_cmn_sn[64];
    char sz_cmn_item[128];
    char sz_cmn_sub_item[256];
    char sz_cmn_test_condition[128];
    char sz_cmn_test_value[32];
    char sz_cmn_low_limit[32];
    char sz_cmn_high_limit[32];
    char sz_cmn_test_info[128];
    char sz_cmn_result_id[64];
    int i_cmn_result;
    char sz_cmn_elapsed_time[64];
    //MES V2.0
    char sz_v2_station[64];
    //MES V1.0
    char sz_v1_end_time[32];
    //Init
    _STEUCT_UNISOC_MES_CMN_TEST_DATA()
    {
        ZeroMemory( this, sizeof( *this ) );
    }
} _UNISOC_MES_CMN_TEST_DATA, *_pUNISOC_MES_CMN_TEST_DATA;

typedef struct _STEUCT_UNISOC_MES_CMN_CODES
{
    //GUID
    char sz_cmn_guid[128];
    //Common
    char sz_cmn_batch[64];
    char sz_cmn_sn1[64];
    char sz_cmn_sn2[64];
    char sz_cmn_imei1[17];
    char sz_cmn_imei2[17];
    char sz_cmn_imei3[17];
    char sz_cmn_imei4[17];
    char sz_cmn_wifi[13];
    char sz_cmn_bt[13];
    char sz_cmn_net_code[128];
    char sz_cmn_nfc_cplc[256];
    char sz_cmn_sim_iccid[32];
    char sz_cmn_sw_ver[256];
    int i_cmn_label_print_qty;
    //扩展字段
    char sz_cmn_code1[128];
    char sz_cmn_code2[128];
    char sz_cmn_code3[128];
    char sz_cmn_code4[128];
    char sz_cmn_code5[128];
    //MEID
    char sz_cmn_meid1[16];
    char sz_cmn_meid2[16];
    //MES V2.0
    char sz_v2_key_sn[64];
    char sz_v2_station[64];
    //MES V1.0
    char sz_v1_project[64];
    char sz_v1_product[64];
    char sz_v1_dl_sn[64];
    char sz_v1_result_guid[64];
    //Init
    _STEUCT_UNISOC_MES_CMN_CODES()
    {
        ZeroMemory( this, sizeof( *this ) );
    }
} _UNISOC_MES_CMN_CODES, *_pUNISOC_MES_CMN_CODES;

typedef struct _STEUCT_UNISOC_MES_CMN_SN_INPUT
{
    //Common
    char sz_cmn_sn[64];
    char sz_cmn_batch[64];
    //MES V2.0
    char sz_v2_station[64];
    int i_v2_sn_type;
    int i_v2_label_print_qty;
    
    _STEUCT_UNISOC_MES_CMN_SN_INPUT()
    {
        ZeroMemory( this, sizeof( *this ) );
    }
} _UNISOC_MES_CMN_SN_INPUT, *_pUNISOC_MES_CMN_SN_INPUT;
/************************************************************************/
/*                                        MES V1.0 Parameters                                                           */
/************************************************************************/
typedef struct _STRUCT_UNISOC_MES_V1_BATCH_INFO
{
    int iBatchID;
    char szBatchName[64];
    char szProjectName[64];
    char szPreSN1[64];
    char szPreSN2[64];
    int iSN1_Length;
    int iSN2_Length;
    int iStatus;
    char szChipName[64];
    char szSWVer[256];
    char szMiniVer[256];
    char szCustVer[256];
    char szPcbVersion[64];
    char szElecBomName[256];
    char szMechBomName[256];
    char szScheduledDate[256];
    int iScheduledQuantity;
    char szManufactureDate[128];
    int iManufactureQuantity;
    char szSN1Start[32];
    char szSN1End[32];
    char szSN2Start[32];
    char szSN2End[32];
    int iIMEINumber;
    int iOperator;
    BOOL bFlowControl;
    int iLogLevel;
    int iFactoryID;
    int iBatchType;
    char szSemiProductBatchList[256];
    char szProductCode[128];
    char szSemiProductCodeList[256];
    BOOL bCheckPreBatchFlow;
    
    _STRUCT_UNISOC_MES_V1_BATCH_INFO()
    {
        ZeroMemory( this, sizeof( *this ) );
    }
} _UNISOC_MES_V1_BATCH_INFO, *_pUNISOC_MES_V1_BATCH_INFO;

typedef struct _STRUCT_UNISOC_MES_V1_ACCOUNT
{
    int iAccountID;
    char szAccountName[64];
    char szPassword[64];
    char szPrivilege[128];
    int iStatus;
    int iFactoryID;
    int iLineID;
    int iBenchID;
    _STRUCT_UNISOC_MES_V1_ACCOUNT()
    {
        ZeroMemory( this, sizeof( *this ) );
    }
} _UNISOC_MES_V1_ACCOUNT, *_pUNISOC_MES_V1_ACCOUNT;

typedef struct _STRUCT_UNISOC_MES_V1_BATCH_STATION_CONFIG
{
    char szBatch[64];
    int iStationID;
    int iMaxFailCount;
    _STRUCT_UNISOC_MES_V1_BATCH_STATION_CONFIG()
    {
        ZeroMemory( this, sizeof( *this ) );
    }
} _UNISOC_MES_V1_BATCH_STATION_CONFIG, *_pUNISOC_MES_V1_BATCH_STATION_CONFIG;

typedef struct _STRUCT_UNISOC_MES_V1_STATION_INFO
{
    char szStation[64];
    int iStationID;
    int iStatus;
    _STRUCT_UNISOC_MES_V1_STATION_INFO()
    {
        ZeroMemory( this, sizeof( *this ) );
    }
} _UNISOC_MES_V1_STATION_INFO, *_pUNISOC_MES_V1_STATION_INFO;

typedef struct _STRUCT_UNISOC_MES_V1_MDB_TEST_DATA
{
    int iTestValueID;
    int iStationID;
    
    char szSN[64];
    char szTestItem[64];
    char szSubTestItem[256];
    float fTestValue;
    float fLowSpec;
    float fHighSpec;
    int iResult;
    int iTestTime;
    char szEndTestTime[32];
    char szTestInfo[258];
    char szTestResultGUID[64];
    
    _STRUCT_UNISOC_MES_V1_MDB_TEST_DATA()
    {
        ZeroMemory( this, sizeof( *this ) );
    }
} _UNISOC_MES_V1_MDB_TEST_DATA, *_pUNISOC_MES_V1_MDB_TEST_DATA;

typedef struct _STRUCT_UNISOC_MES_V1_STATION_RESULT
{
    char szStationName[64];
    int iStationID;
    int iLastTestResult;
    _STRUCT_UNISOC_MES_V1_STATION_RESULT()
    {
        ZeroMemory( this, sizeof( *this ) );
    }
} _UNISOC_MES_V1_STATION_RESULT, *_pUNISOC_MES_V1_STATION_RESULT;
/************************************************************************/
/*                                        MES V2.0 Parameters                                                           */
/************************************************************************/
typedef struct _STEUCT_UNISOC_MES_UPLOAD_DATA_LOG_CONFIG
{
    int iUploadType;
    int iPort;
    char szUrl[128];
    char szAccount[64];
    char szPassword[64];
    char szFtpLogDir[512];
    //Init
    _STEUCT_UNISOC_MES_UPLOAD_DATA_LOG_CONFIG()
    {
        ZeroMemory( this, sizeof( *this ) );
    }
} _UNISOC_MES_UPLOAD_DATA_LOG_CONFIG, *_pUNISOC_MES_UPLOAD_DATA_LOG_CONFIG;

typedef struct _STEUCT_UNISOC_MES_CHECK_FLOW
{
    char szSN[64];
    char szBatch[64];
    char szStation[64];
    int iSNType;
    
    _STEUCT_UNISOC_MES_CHECK_FLOW()
    {
        ZeroMemory( this, sizeof( *this ) );
    }
} _UNISOC_MES_CHECK_FLOW, *_pUNISOC_MES_CHECK_FLOW;

typedef struct _STEUCT_UNISOC_MES_TEST_ENV
{
    char szProductHouseID[64];
    char szProductHouseName[64];
    char szProductLineID[64];
    char szProductLineName[64];
    char szProductStationName[64];
    char szProductStationIP[32];
    char szProductStationMac[32];
    char szOs[32];
    char szInstrument1[256];
    char szFixture[128];
    char szCableLoss[512];
    char szLine[512];
    int iSlot;
    _STEUCT_UNISOC_MES_TEST_ENV()
    {
        ZeroMemory( this, sizeof( *this ) );
    }
} _UNISOC_MES_TEST_ENV, *_pUNISOC_MES_TEST_ENV;

typedef struct _STEUCT_UNISOC_MES_BARCODE_INFO
{
    char szBatch[64];
    char szBarcode[256];
    _ENUM_BARCODE_TYPE e_barcode_type;
    
    _STEUCT_UNISOC_MES_BARCODE_INFO()
    {
        ZeroMemory( this, sizeof( *this ) );
    }
} _UNISOC_MES_BARCODE_INFO, *_pUNISOC_MES_BARCODE_INFO;

typedef struct _STEUCT_UNISOC_MES_SEQ_FILEINFO
{
    char szId[128];
    char szToolName[256];
    char szToolVersion[64];
    char szSeqCRC[64];
    char szSeqName[256];
    char szSeqVersion[64];
    int iIsCheck;
    
    _STEUCT_UNISOC_MES_SEQ_FILEINFO()
    {
        ZeroMemory( this, sizeof( *this ) );
    }
} _UNISOC_MES_SEQ_FILEINFO, *_pUNISOC_MES_SEQ_FILEINFO;

typedef struct _STEUCT_UNISOC_MES_DOC_INFO
{
    int iSortNo;
    char szId[128];
    char szTypeCode[256];
    char szFileTitle[256];
    char szOriFileName[256];
    char szFileVersion[64];
    char szFileCRC[64];
    char szDocumentId[64];
    
    _STEUCT_UNISOC_MES_DOC_INFO()
    {
        ZeroMemory( this, sizeof( *this ) );
    }
} _UNISOC_MES_DOC_INFO, *_pUNISOC_MES_DOC_INFO;

typedef struct _STEUCT_UNISOC_MES_SEQ_FILEINFO_REQ
{
    char szBatch[64];
    char szStation[256];
    
    _STEUCT_UNISOC_MES_SEQ_FILEINFO_REQ()
    {
        ZeroMemory( this, sizeof( *this ) );
    }
} _UNISOC_MES_SEQ_FILEINFO_REQ, *_pUNISOC_MES_SEQ_FILEINFO_REQ;
//
typedef struct _STEUCT_UNISOC_MES_TEST_TOOL
{
    char szBatch[64];
    char szStation[64];
    char szTestToolName[64];
    char szTestToolVersion[64];
    char szTestSeqName[64];
    char szTestSeqVersion[64];
    _STEUCT_UNISOC_MES_TEST_TOOL()
    {
        ZeroMemory( this, sizeof( *this ) );
    }
} _UNISOC_MES_TEST_TOOL, *_pUNISOC_MES_TEST_TOOL;

typedef struct _STEUCT_UNISOC_MES_STATION_INFO
{
    char szFactory[64];
    char szHouse[64];
    char szLine[64];
    char szMachine[64];
    char szMac[32];
    char szIp[32];
    char szDescription[128];
    _STEUCT_UNISOC_MES_STATION_INFO()
    {
        ZeroMemory( this, sizeof( *this ) );
    }
} _UNISOC_MES_STATION_INFO, *_pUNISOC_MES_STATION_INFO;

typedef struct _STEUCT_UNISOC_MES_HOUSE_INFO
{
    char szHouseId[64];
    char szHouseName[128];
    _STEUCT_UNISOC_MES_HOUSE_INFO()
    {
        ZeroMemory( this, sizeof( *this ) );
    }
} _UNISOC_MES_HOUSE_INFO, *_pUNISOC_MES_HOUSE_INFO;

typedef struct _STEUCT_UNISOC_MES_BATCH_INFO
{
    char szProjectName[128];
    char szProductCode[128];
    _STEUCT_UNISOC_MES_BATCH_INFO()
    {
        ZeroMemory( this, sizeof( *this ) );
    }
} _UNISOC_MES_BATCH_INFO, *_pUNISOC_MES_BATCH_INFO;

typedef struct _STEUCT_UNISOC_MES_UPLOAD_LOG_URL
{
    int iLevel;
    char szStorageMode[64];
    char szUrl[512];
    char szPrompt[512];
    _STEUCT_UNISOC_MES_UPLOAD_LOG_URL()
    {
        ZeroMemory( this, sizeof( *this ) );
    }
} _UNISOC_MES_UPLOAD_LOG_URL, *_pUNISOC_MES_UPLOAD_LOG_URL;

typedef HANDLE( UNISOC_API* pMES_Handle_Create )( int iSlot, HANDLE hHandleLog );
typedef void ( UNISOC_API* pMES_Handle_ReleaseAll )( void );
typedef void ( UNISOC_API* pMES_Handle_Release )( HANDLE hHandle );
typedef MES_RESULT( UNISOC_API* pMES_EnableCheck )( HANDLE hHandle, bool& bEnable );
typedef MES_RESULT( UNISOC_API* pMES_New_Guid )( HANDLE hHandle, char* pszGuid, int iSize );
typedef MES_RESULT( UNISOC_API* pMES_Get_Host_MAC )( HANDLE hHandle, char* pszMac, int iSize );
typedef MES_RESULT( UNISOC_API* pMES_Get_Host_IP )( HANDLE hHandle, char* pszIP, int iSize );
typedef MES_RESULT( UNISOC_API* pMES_Get_Host_PCName )( HANDLE hHandle, char* pszName, int iSize );
typedef MES_RESULT( UNISOC_API* pMES_Get_Host_Os )( HANDLE hHandle, char* pszOs, int iSize );
typedef MES_RESULT( UNISOC_API* pMES_Get_Tool_Info )( HANDLE hHandle, char* pszToolName, char* pszToolVersion );
typedef MES_RESULT( UNISOC_API* pMES_GetLastError )( HANDLE hHandle, char* pszError, int iSize );
typedef MES_RESULT( UNISOC_API* pMES_GetBatchName )( HANDLE hHandle, char* pszBatch, int iSize );
typedef MES_RESULT( UNISOC_API* pMES_GetStationName )( HANDLE hHandle, char* pszStaion, int iSize );
typedef MES_RESULT( UNISOC_API* pMES_GetMesLogPath )( HANDLE hHandle, char* pszPath, int iSize );
typedef MES_RESULT( UNISOC_API* pMES_GetMesType )( HANDLE hHandle, _ENUM_UNISOC_MES_TYPE& e_mes_type );
typedef MES_RESULT( UNISOC_API* pMES_GetMesIniInfo )( HANDLE hHandle, char* pszNode, char* pszKey, char* psValue, int iSize );
typedef MES_RESULT( UNISOC_API* pMES_Check_Rule_IMEI )( HANDLE hHandle, char* pszIMEI );
typedef MES_RESULT( UNISOC_API* pMES_Check_Rule_BT )( HANDLE hHandle, char* pszBT );
typedef MES_RESULT( UNISOC_API* pMES_Check_Rule_WIFI )( HANDLE hHandle, char* pszWIFI );
typedef MES_RESULT( UNISOC_API* pMES_Check_Rule_MEID )( HANDLE hHandle, char* pszMEID );
typedef MES_RESULT( UNISOC_API* pMES_Login )( HANDLE hHandle, _pUNISOC_MES_CMN_LOGIN p_login );
typedef MES_RESULT( UNISOC_API* pMES_Logout )( HANDLE hHandle );
typedef MES_RESULT( UNISOC_API* pMES_GetAssignedCodes )( HANDLE hHandle, _pUNISOC_MES_BARCODE_INFO p_barcode_info, _pUNISOC_MES_CMN_CODES p_codes );
typedef MES_RESULT( UNISOC_API* pMES_GetDeviceCode )( HANDLE hHandle, _pUNISOC_MES_BARCODE_INFO p_barcode_info, _pUNISOC_MES_CMN_CODES p_codes );
typedef MES_RESULT( UNISOC_API* pMES_RecordDeviceInfos )( HANDLE hHandle, _pUNISOC_MES_CMN_CODES p_codes );
typedef MES_RESULT( UNISOC_API* pMES_CheckFlow )( HANDLE hHandle, _pUNISOC_MES_CHECK_FLOW p_check_flow );
typedef MES_RESULT( UNISOC_API* pMES_SendTestResult )( HANDLE hHandle, _pUNISOC_MES_CMN_TEST_RESULT p_test_result );
typedef MES_RESULT( UNISOC_API* pMES_SendTestData )( HANDLE hHandle, _pUNISOC_MES_CMN_TEST_DATA p_test_data );
typedef MES_RESULT( UNISOC_API* pMES_SnInput )( HANDLE hHandle, _pUNISOC_MES_CMN_SN_INPUT p_sn_input );
typedef MES_RESULT( UNISOC_API* pMES_CheckSnInput )( HANDLE hHandle, _pUNISOC_MES_CMN_SN_INPUT p_sn_input );
typedef MES_RESULT( UNISOC_API* pMES_RecordAssignedCodes_ExtendedField )( HANDLE hHandle, _pUNISOC_MES_CMN_CODES p_codes );
typedef MES_RESULT( UNISOC_API* pMES_V2_SendDatabase )( HANDLE hHandle, char* pszDatabase );
typedef MES_RESULT( UNISOC_API* pMES_V2_CheckTestTool )( HANDLE hHandle, _pUNISOC_MES_TEST_TOOL p_test_tool );
typedef MES_RESULT( UNISOC_API* pMES_V2_SendTestEnvironment )( HANDLE hHandle, _pUNISOC_MES_TEST_ENV p_test_env );
typedef MES_RESULT( UNISOC_API* pMES_V2_SendTestToolInfo )( HANDLE hHandle, _pUNISOC_MES_TEST_TOOL p_test_tool );
typedef MES_RESULT( UNISOC_API* pMES_V2_GetCurrentSeqFileInfo )( HANDLE hHandle, _pUNISOC_MES_SEQ_FILEINFO_REQ p_seq_req, _pUNISOC_MES_SEQ_FILEINFO p_seq_info );
typedef MES_RESULT( UNISOC_API* pMES_V2_GetSequenceFile )( HANDLE hHandle, char* pszDirSeq );
typedef MES_RESULT( UNISOC_API* pMES_V2_GetDocumentListEx )( HANDLE hHandle, _pUNISOC_MES_SEQ_FILEINFO_REQ p_seq_req, _pUNISOC_MES_DOC_INFO p_seq_info );
typedef MES_RESULT( UNISOC_API* pMES_V1_GetBatchInfo )( HANDLE hHandle, char* pszBatch, _pUNISOC_MES_V1_BATCH_INFO p_batch );
