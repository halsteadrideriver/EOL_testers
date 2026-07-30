#pragma once
#include "Unisoc_Solution_MES.h"

typedef struct _tagUNISOC_MES_DRIVER_T
{
	pMES_Handle_Create MES_Handle_Create;
	pMES_Handle_Release MES_Handle_Release;
	pMES_Handle_ReleaseAll MES_Handle_ReleaseAll;

	pMES_EnableCheck MES_EnableCheck;
	pMES_New_Guid MES_New_Guid;
	pMES_Get_Host_MAC MES_Get_Host_MAC;
	pMES_Get_Host_IP MES_Get_Host_IP;
	pMES_Get_Host_PCName MES_Get_Host_PCName;
	pMES_Get_Host_Os MES_Get_Host_Os;
	pMES_Get_Tool_Info MES_Get_Tool_Info;

	pMES_GetLastError MES_GetLastError;
	pMES_GetBatchName MES_GetBatchName;
	pMES_GetStationName MES_GetStationName;
	pMES_GetMesLogPath MES_GetMesLogPath;
	pMES_GetMesIniInfo MES_GetMesIniInfo;

	pMES_Check_Rule_IMEI MES_Check_Rule_IMEI;
	pMES_Check_Rule_MEID MES_Check_Rule_MEID;
	pMES_Check_Rule_BT MES_Check_Rule_BT;
	pMES_Check_Rule_WIFI MES_Check_Rule_WIFI;

	pMES_Login MES_Login;
	pMES_Logout MES_Logout;
	pMES_GetAssignedCodes MES_GetAssignedCodes;
	pMES_GetDeviceCode MES_GetDeviceCode;
	pMES_RecordDeviceInfos MES_RecordDeviceInfos;
	pMES_CheckFlow MES_CheckFlow;
	pMES_SendTestResult MES_SendTestResult;
	pMES_SendTestData MES_SendTestData;
	pMES_SnInput MES_SnInput;
	pMES_CheckSnInput MES_CheckSnInput;
	pMES_RecordAssignedCodes_ExtendedField MES_RecordAssignedCodes_ExtendedField;

	pMES_V2_SendDatabase MES_V2_SendDatabase;
	pMES_V2_CheckTestTool MES_V2_CheckTestTool;
	pMES_V2_SendTestEnvironment MES_V2_SendTestEnvironment;
	pMES_V2_SendTestToolInfo MES_V2_SendTestToolInfo;
	pMES_V2_GetCurrentSeqFileInfo MES_V2_GetCurrentSeqFileInfo;
	pMES_V2_GetSequenceFile MES_V2_GetSequenceFile;
	pMES_V2_GetDocumentListEx MES_V2_GetDocumentListEx;
	pMES_V1_GetBatchInfo MES_V1_GetBatchInfo;

    _tagUNISOC_MES_DRIVER_T(void) {
        Reset();
    };

    void Reset(void) {
		MES_Handle_Create = NULL;
		MES_Handle_Release = NULL;
		MES_Handle_ReleaseAll = NULL;
		MES_EnableCheck = NULL;
		MES_New_Guid = NULL;
		MES_Get_Host_MAC = NULL;
		MES_Get_Host_IP = NULL;
		MES_Get_Host_PCName = NULL;
		MES_Get_Host_Os = NULL;
		MES_Get_Tool_Info = NULL;
		MES_GetLastError = NULL;
		MES_GetBatchName = NULL;
		MES_GetStationName = NULL;
		MES_GetMesLogPath = NULL;
		MES_GetMesIniInfo = NULL;
		MES_Check_Rule_IMEI = NULL;
		MES_Check_Rule_MEID = NULL;
		MES_Check_Rule_BT = NULL;
		MES_Check_Rule_WIFI = NULL;
		MES_Login = NULL;
		MES_Logout = NULL;
		MES_GetAssignedCodes = NULL;
		MES_GetDeviceCode = NULL;
		MES_RecordDeviceInfos = NULL;
		MES_CheckFlow = NULL;
		MES_SendTestResult = NULL;
		MES_SendTestData = NULL;
		MES_SnInput = NULL;
		MES_CheckSnInput = NULL;
		MES_RecordAssignedCodes_ExtendedField = NULL;
		MES_V2_SendDatabase = NULL;
		MES_V2_CheckTestTool = NULL;
		MES_V2_SendTestEnvironment = NULL;
		MES_V2_SendTestToolInfo = NULL;
		MES_V2_GetCurrentSeqFileInfo = NULL;
		MES_V2_GetSequenceFile = NULL;
		MES_V1_GetBatchInfo = NULL;
		MES_V2_GetDocumentListEx = NULL;
    };

    BOOL IsValid(void) {
		return ((NULL != MES_Handle_Create) &&
			(NULL != MES_Handle_Release) &&
			(NULL != MES_Handle_ReleaseAll) &&
			(NULL != MES_EnableCheck) &&
			(NULL != MES_New_Guid) &&
			(NULL != MES_Get_Host_MAC) &&
			(NULL != MES_Get_Host_IP) &&
			(NULL != MES_Get_Host_PCName) &&
			(NULL != MES_Get_Host_Os) &&
			(NULL != MES_Get_Tool_Info) &&
			(NULL != MES_GetLastError) &&
			(NULL != MES_GetBatchName) &&
			(NULL != MES_GetStationName) &&
			(NULL != MES_GetMesLogPath) &&
			(NULL != MES_GetMesIniInfo) &&
			(NULL != MES_Check_Rule_IMEI) &&
			(NULL != MES_Check_Rule_MEID) &&
			(NULL != MES_Check_Rule_BT) &&
			(NULL != MES_Check_Rule_WIFI) &&
			(NULL != MES_Login) &&
			(NULL != MES_Logout) &&
			(NULL != MES_GetAssignedCodes) &&
			(NULL != MES_GetDeviceCode) &&
			(NULL != MES_RecordDeviceInfos) &&
			(NULL != MES_CheckFlow) &&
			(NULL != MES_SendTestResult) &&
			(NULL != MES_SendTestData) &&
			(NULL != MES_SnInput) &&
			(NULL != MES_CheckSnInput) &&
			(NULL != MES_RecordAssignedCodes_ExtendedField) &&
			(NULL != MES_V2_SendDatabase) &&
			(NULL != MES_V2_CheckTestTool) &&
			(NULL != MES_V2_SendTestEnvironment) &&
			(NULL != MES_V2_SendTestToolInfo) &&
			(NULL != MES_V2_GetCurrentSeqFileInfo) &&
			(NULL != MES_V2_GetSequenceFile) &&
			(NULL != MES_V1_GetBatchInfo) &&
			(NULL != MES_V2_GetDocumentListEx));};

}UNISOC_MES_DRIVER_T;

