#include <Windows.h>
#include "global_err.h"

#pragma once
#define KEY_LENGTH			256
#define RSA_N_LENGTH			256
#define RSA_E_LEN			    6
//#define T(x) (((x&0xff)<<24)|((x&0xff00)<<8)|((x&0xff0000)>>8)|((x&0xff000000)>>24)) 
#define MAX_UID_LENGTH				32
#define MAX_UID_ASCII_LENGTH	    64
#define SIMLOCK_ENCRYPT_KEYS  (320 + 32)	// (sizeof(simlock_encrypt_keys_t) + 32)
#define MAX_RAND_CHAR_LENGTH 4
#define ENG_HUK_STR_LENGTH			(8+1)
#define ENG_HUK_HEX_LENGTH			(ENG_HUK_STR_LENGTH >> 1)
#define MAX_IMEI_STR_LENGTH  15
#define ENG_SSK_STR_LENGTH  18
//#define SIZE_1_K  1024
//#define SIZE_4_K  4 * 1024
//#define SIZE_64_K  64 * 1024
#define SHA1_DATA_LENGTH  20	//160bits
#define SHA256_DATA_LENGTH  32	//256bits
#define AES_KEY_DATA_LENGTH  32
#define AES_KEY_STR_LENGTH (AES_KEY_DATA_LENGTH* 2 + 1)
#define AES_RANDOM_DATA_LENGTH ( ENG_HUK_HEX_LENGTH + ENG_SSK_STR_LENGTH)
#define ENG_SSK_STRING_T   "Secure Storage Key"
#define IMEI_LENGTH      15
#define ENG_HUK_STR_LENGTH			(8+1)
#define ENG_HUK_HEX_LENGTH			(ENG_HUK_STR_LENGTH >> 1)
#define ENG_SSK_STR_LENGTH  18
#define FLAG_IMEI1 0x01 //BIT0
#define FLAG_IMEI2 0x02 //BIT1
#define FLAG_IMEI3 0x04 //BIT2
#define FLAG_IMEI4 0x08 //BIT3
#define  COMMDATA_UID_LEN           (31)
#define  ROTPK_HASH_LENGTH          (32)

#define  BUFFER_SOLID_LEN           25	    // ID(2) + FLAG(1) + UUID(16) + COMMANDID(4) + VERSION(1) + XOR(1)
#define  BUFFER_SOLID_RESLEN        8		// ID(2) + FLAG(1) + RETURNCODE(4)  + XOR(1)

#define MAX_SIMLOCK_KEY_LEN    (16)
#define MAX_SIMLOCK_KEY_COUNT   (5)



const int PKT_BUFFER_MAXLEN = 0x10000; // 64K
const int RCV_BUFFLEN = 0x10000; // 64K
const int MAX_TA_UUIDLEN = 16;
const int HI_SECURE_COMMANDID = 0x10000000;
const BYTE SECURE_COMMANDFLAG = 0x80;

const BYTE SECURE_PACKAGEVERSION = 0x00;

const BYTE SECURE_RESPONSEFLAG = 0x00;


//这里设置字节对齐
#pragma pack(push,1)
typedef struct _SECURE_CMD_HEADERVT
{
	unsigned int nLen;
	unsigned short ID;
	BYTE Flag;
	BYTE TAUUID[MAX_TA_UUIDLEN];
	unsigned int CommandID;
	_SECURE_CMD_HEADERVT()
	{
		ZeroMemory(this, sizeof(_SECURE_CMD_HEADERVT));
		Flag = SECURE_COMMANDFLAG;
	}

}SECURE_CMD_HEADERV;

typedef struct _SECURE_CMD_TAILVT
{
	BYTE  Version;
	BYTE XOR;
	_SECURE_CMD_TAILVT()
	{
		ZeroMemory(this, sizeof(_SECURE_CMD_TAILVT));
	}

}SECURE_CMD_TAILV;

typedef struct  _SECURE_REP_HEADERVT
{
	unsigned int nLen;
	unsigned short ID;
	BYTE Flag;
	unsigned int nReturnCode;
	 _SECURE_REP_HEADERVT()
	{
		ZeroMemory(this, sizeof( _SECURE_REP_HEADERVT));
		Flag = SECURE_RESPONSEFLAG;
	}

}SECURE_REP_HEADERV;

#pragma pack(pop)
typedef enum _SIMLOCK_STATUS
{
    SIMLOCK_STATUS_NCK,
    SIMLOCK_STATUS_NSCK,
    SIMLOCK_STATUS_SPCK,
    SIMLOCK_STATUS_CCK,
    SIMLOCK_STATUS_PCK
}SIMLOCK_STATUS;

typedef struct _SimlockStatus
{
    int NCK = 0;
    int NSCK = 0;
    int SPCK = 0;
    int CCK = 0;
    int PCK = 0;
    _SimlockStatus(int nStatus)
    {
        if (((nStatus >> SIMLOCK_STATUS_NCK) & 0x1) == 1 )
        {
            NCK = 1;
        }
        if (((nStatus >> SIMLOCK_STATUS_NSCK) & 0x1) == 1)
        {
            NSCK = 1;
        }
        if (((nStatus >> SIMLOCK_STATUS_SPCK) & 0x1) == 1)
        {
            SPCK = 1;
        }
        if (((nStatus >> SIMLOCK_STATUS_CCK) & 0x1) == 1)
        {
            CCK = 1;
        }
        if (((nStatus >> SIMLOCK_STATUS_PCK) & 0x1) == 1)
        {
            PCK = 1;
        }

    }
private:
	_SimlockStatus() {};
}SimlockStatus;



typedef struct _SimlockKeyParam
{
	uint8 arrKeys[MAX_SIMLOCK_KEY_COUNT][MAX_SIMLOCK_KEY_LEN];
	/*_SimlockKeyParam()
	{
		memset(this, 0, sizeof(_SimlockKeyParam));
	}*/
}SimlockKeyParam;

typedef struct _SimlockKeyInfo {
	SimlockKeyParam pin_keys;
	SimlockKeyParam puk_keys;
} SimlockKeyInfo;

typedef struct _X_RSA_KEY
{
	uint8 szPassWord[8];
	WCHAR szProductName[50];
    uint8 szRsa_n[256];
	uint8 szRsa_e[4];
	uint8 szRsa_d[256];
	uint8 szRsa_p[128];
	uint8 szRsa_q[128];
	uint8 szRsa_dmp1[128];
	uint8 szRsa_dmq1[128];
	uint8 szRsa_iqmp[128];
}X_RSA_KEY;


enum RsaKeyType
{
    RsaKey_Public,
    RsaKey_Private
};

enum RsaType
{
    RSA1024 = 1,
    RSA2048
};

enum RsaPadding
{
    RsaPadding_Pkcs,
    RsaPadding_OAEP,
    RsaSign_Pss
};

enum Hash_Type
{
    Hash_Type_RAW_1,
    Hash_Type_MD2_1,
    Hash_Type_MD4_1,
    Hash_Type_MD5_1,
    Hash_Type_SHA1_1,
    Hash_Type_SHA256_1
};

typedef struct _RSA_PK
{
	std::string strPK;
	std::string strSignPK;

	_RSA_PK()
	{
		Clear();
	}

	void Clear()
	{
		strPK.clear();
		strSignPK.clear();
	}
}RSA_PK, *PRSA_PK_PTR_T;

typedef struct _IMEI_CODES_T
{
	unsigned int nFlag; // if only write IMEI1: nFlag = FLAG_IMEI1;
						// if write IMEI1 and IMEI2: nFlag = FLAG_IMEI1|FLAG_IMEI2;
						// if write IMEI1-IMEI3: nFlag = FLAG_IMEI1|FLAG_IMEI2|FLAG_IMEI3;
						// if write IMEI1-IMEI4: nFlag = FLAG_IMEI1|FLAG_IMEI2|FLAG_IMEI3|FLAG_IMEI4;
	char szIMEI1[IMEI_LENGTH + 1];
	char szIMEI2[IMEI_LENGTH + 1];
	char szIMEI3[IMEI_LENGTH + 1];
	char szIMEI4[IMEI_LENGTH + 1];

}IMEI_CODES_T;

typedef enum {
	E_BLOB_ITEM_IMEI1 = 0,		//IMEI1
	E_BLOB_ITEM_IMEI2 = 1,		//IMEI2
	E_BLOB_ITEM_SALT = 2,		//Salt
	E_BLOB_ITEM_ITERATION_COUNT = 3,		//Iteration Count
	E_BLOB_ITEM_HCK = 4,		//HCK
	E_BLOB_ITEM_PUBLIC_KEY_N = 5,		//Public Key Modulus
	E_BLOB_ITEM_PUBLIC_KEY_E = 6,		//Public Key Exponent
	E_BLOB_ITEM_COUNT = 7
} SUBSIDY_LOCK_BLOB_INFO;

typedef struct _SIMLOCKKEY_PARAM_T
{
	BYTE agKeys[5][16];
	_SIMLOCKKEY_PARAM_T()
	{
		memset(this, 0, sizeof(_SIMLOCKKEY_PARAM_T));
	}
}SIMLOCKKEY_PARAM_T;


typedef struct _Sqlite3Data
{
	uint32	nItemId;			/// id
	uint32	nDataLen;
	int8* lpDataBuf;

}Sqlite_Data_3;