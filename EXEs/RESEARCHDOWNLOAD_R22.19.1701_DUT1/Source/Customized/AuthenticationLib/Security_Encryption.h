#pragma once
#include "global_err.h"
#include "SecurityDefine.h"



#ifdef  SP_API
#undef  SP_API
#endif 
#define SP_API  //__stdcall

#ifndef SP_EXPORT
#ifdef __cplusplus
#ifdef SECURITY_ENCRYPTION_EXPORTS
#define SP_EXPORT extern "C" __declspec (dllexport)
#else
#define SP_EXPORT extern "C" __declspec (dllimport)
#endif
#else
#ifdef SECURITY_ENCRYPTION_EXPORTS
#define SP_EXPORT __declspec (dllexport)
#else
#define SP_EXPORT __declspec (dllimport)
#endif
#endif
#endif // !SP_EXPORT

SP_EXPORT SPRESULT SP_API RSA_Encryption(X_RSA_KEY Key,
	RsaKeyType KeyType,
	uint8* pInputData,
	int nInputLength,
	uint8* pOutputData,
	int* pOutputLength,
	RsaType Type,
	RsaPadding Padding
);

SP_EXPORT SPRESULT SP_API  RSA_Decryption(X_RSA_KEY Key,
	RsaKeyType KeyType,
	uint8* pInputData,
	int nInputLength,
	uint8* pOutputData,
	int* pOutputLength,
	RsaType Type,
	RsaPadding Padding
);


SP_EXPORT SPRESULT SP_API  RSA_Sign(X_RSA_KEY Key,
	RsaKeyType KeyType,
	uint8* pInputData,
	int nInputLength,
	uint8* pOutputData,
	int* pOutputLength,
	RsaType Type,
	RsaPadding Padding,
    Hash_Type HashId
);

SP_EXPORT SPRESULT SP_API  RSA_Verfiy(X_RSA_KEY Key,
	RsaKeyType KeyType,
	uint8* pInputData,
	int nInputLength,
	uint8* nHashSign,
	RsaType Type,
	RsaPadding Padding,
    Hash_Type HashId
);


SP_EXPORT SPRESULT SP_API  AES_Encryption(uint8* pInputData,
	int nInputLength,
	uint8* output,
	uint8* ikey,
	int keylen
);


SP_EXPORT SPRESULT SP_API  AES_Decryption(uint8* pInputData,
	int nInputLength,
	uint8* output,
	uint8* ikey,
	int keylen
);

SP_EXPORT SPRESULT SP_API HASH_1_Encrypt(uint8* pInputData, uint32 nInputLength, uint8* output);

SP_EXPORT SPRESULT SP_API HASH_256_Encrypt(const uint8* pInputData, uint32 nInputLength, uint8* output);

SP_EXPORT SPRESULT SP_API  PEM_To_RSA_Key(uint8* PukKey, uint8* PriKey, X_RSA_KEY Key, RsaType Type);

SP_EXPORT SPRESULT SP_API CRC16(const uint16 input, const uint8* buffer, uint32 length, uint16* output);


