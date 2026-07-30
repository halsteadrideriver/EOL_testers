#pragma once

#include "def\global_err.h"
#include "def\global_def.h"
#include "def\callback_def.h"
#include "Global.h"

#define MAX_AUTH_M1_SIZE       (512)
#define MAX_AUTH_M2_SIZE       (512)

enum AUTH_MODE
{
    E_AUTH_DISABLE = 0,
    E_AUTH_RANDOM_DATA,
    E_AUTH_41M1
};

//M1£ºData returned from DUT to server
typedef struct _tagAUTH_DUT
{
    uint8 u8M1[256];
    uint8 u8ProjectName[32];
    uint16 u16Status; // 0£ºNeed Auth ;1 - Not Auth
    STRUCT_INITIALIZE( _tagAUTH_DUT );
} AUTH_DUT, *LPAUTH_DUT;

//M2£ºData returned from the server to the DUT
typedef struct _tagAUTH_SERVER
{
    uint8 u8M2[256];
    uint8 u8Nonce[32]; // Random Number
    
    STRUCT_INITIALIZE( _tagAUTH_SERVER );
} AUTH_SERVER, * LPAUTH_SERVER;

//Keys£ºRsaKey & AesKey
typedef struct _tagSECURE_KEY
{
    uint8 u8PriviteKey[1160];
    uint8 u8AesKey[32];
    
    STRUCT_INITIALIZE( _tagSECURE_KEY );
} SECURE_KEY, * LPSECURE_KEY;

class IAuthentication
{
public:
    virtual void Release( void ) = 0;
    virtual SPRESULT  EnableAuth( LPCRANDOM_DATA lpRandomData, AUTH_MODE& eMode ) = 0;
    virtual SPRESULT  ServerAuth( LPAUTH_DUT lpDut, LPAUTH_SERVER lpServer, DWORD& dwLength ) = 0;
    virtual SPRESULT  GetSecureKey( LPSECURE_KEY lpKey ) = 0;
    
protected:
    virtual ~IAuthentication( void ) { };
};

SP_EXPORT IAuthentication* SP_API CreateAuthenticationObject( LPVOID pLogUtil );

