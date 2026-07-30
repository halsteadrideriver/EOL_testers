#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/// Include files
#include "global_err.h"

//////////////////////////////////////////////////////////////////////////
/// Export APIs
#ifdef CONFIG_EXPORT
#undef CONFIG_EXPORT
#endif

#ifdef __cplusplus
#ifdef CONFIG_EXPORTS
#define CONFIG_EXPORT extern "C" __declspec (dllexport)
#else
#define CONFIG_EXPORT extern "C" __declspec (dllimport)
#endif
#else
#ifdef CONFIG_EXPORTS
#define CONFIG_EXPORT __declspec (dllexport)
#else
#define CONFIG_EXPORT __declspec (dllimport)
#endif
#endif


enum DLFW_CFGTYPE_ENUM
{
    /// FactoryDownload.ini¡¢ResearchDownload.ini¡¢UpgradeDownload.ini
    /// Differentiated by DL TYPE ENUM
    CFGTYPE_DL = 0,
    /// BMFileType.ini 
    CFGTYPE_BMFT,
    /// BMTimeOut.ini
    CFGTYPE_BMTO,
    /// BMError.ini
    CFGTYPE_BMER,
    /// BinPack.ini
    CFGTYPE_BINP,
    /// MCPType.ini
    CFGTYPE_MCPT,
    /// CmdDloader.ini
    CFGTYPE_CMD,
    /// DLFramework.ini
    CFGTYPE_DLFW,
    
    MAX_CFG_TYPE
};

#ifdef __cplusplus
//////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
class IConfig
{
public:
    virtual void Release( void ) = 0;
    virtual LPCWSTR GetIniPath( DLFW_CFGTYPE_ENUM eType ) = 0;
    
    virtual SPRESULT LoadConfig( DLFW_CFGTYPE_ENUM eType ) = 0;
    virtual SPRESULT LoadConfigBySection( DLFW_CFGTYPE_ENUM eType, LPCWSTR lpszSection ) = 0;
    
    virtual void ClearAllConfig() = 0;
    virtual void ClearConfig( DLFW_CFGTYPE_ENUM eType ) = 0;
    
    virtual SPRESULT SetString( DLFW_CFGTYPE_ENUM eType, LPCWSTR lpszSection, LPCWSTR lpszKey, LPCWSTR lpszValue ) = 0;
    virtual SPRESULT SetInt( DLFW_CFGTYPE_ENUM eType, LPCWSTR lpszSection, LPCWSTR lpszKey, int nValue ) = 0;
    
    virtual LPCWSTR GetString( DLFW_CFGTYPE_ENUM eType, LPCWSTR lpszSection, LPCWSTR lpszKey, LPCWSTR lpszDefault ) = 0;
    virtual int GetInt( DLFW_CFGTYPE_ENUM eType, LPCWSTR lpszSection, LPCWSTR lpszKey, int nDefault ) = 0;
    
    virtual void GetSection( DLFW_CFGTYPE_ENUM eType, LPCWSTR lpszSection, LPWSTR lpszValue, DWORD dwSize ) = 0;
    virtual void SetSection( DLFW_CFGTYPE_ENUM eType, LPCWSTR lpszSection, LPCWSTR lpszValue ) = 0;
    
protected:
    virtual ~IConfig( void ) { };
};

//////////////////////////////////////////////////////////////////////////
/// Export API
CONFIG_EXPORT IConfig* SP_API CreateConfigObject( DL_TYPE_ENUM eType, LPVOID pLogUtil );

#endif
#endif

CONFIG_EXPORT SP_HANDLE SP_API CFG_CreateObject( DL_TYPE_ENUM eType, LPVOID pLogUtil );
CONFIG_EXPORT void      SP_API CFG_FreeObject( SP_HANDLE hHandle );

CONFIG_EXPORT LPCWSTR   SP_API CFG_GetIniPath( SP_HANDLE hHandle, DLFW_CFGTYPE_ENUM eType );

CONFIG_EXPORT SPRESULT  SP_API CFG_LoadConfig( SP_HANDLE hHandle, DLFW_CFGTYPE_ENUM eType );
CONFIG_EXPORT SPRESULT  SP_API CFG_LoadConfigBySection( SP_HANDLE hHandle, DLFW_CFGTYPE_ENUM eType, LPCWSTR lpszSection );

CONFIG_EXPORT void      SP_API CFG_ClearAllConfig( SP_HANDLE hHandle );
CONFIG_EXPORT void      SP_API CFG_ClearConfig( SP_HANDLE hHandle, DLFW_CFGTYPE_ENUM eType );

CONFIG_EXPORT SPRESULT  SP_API CFG_SetString( SP_HANDLE hHandle, DLFW_CFGTYPE_ENUM eType, LPCWSTR lpszSection, LPCWSTR lpszKey, LPCWSTR lpszValue );
CONFIG_EXPORT SPRESULT  SP_API CFG_SetInt( SP_HANDLE hHandle, DLFW_CFGTYPE_ENUM eType, LPCWSTR lpszSection, LPCWSTR lpszKey, int nValue );

CONFIG_EXPORT LPCWSTR   SP_API CFG_GetString( SP_HANDLE hHandle, DLFW_CFGTYPE_ENUM eType, LPCWSTR lpszSection, LPCWSTR lpszKey, LPCWSTR lpszDefault );
CONFIG_EXPORT int       SP_API CFG_GetInt( SP_HANDLE hHandle, DLFW_CFGTYPE_ENUM eType, LPCWSTR lpszSection, LPCWSTR lpszKey, int nDefault );

CONFIG_EXPORT void      SP_API CFG_GetSection( SP_HANDLE hHandle, DLFW_CFGTYPE_ENUM eType, LPCWSTR lpszSection, LPTSTR lpszValue, DWORD dwSize );
CONFIG_EXPORT void      SP_API CFG_SetSection( SP_HANDLE hHandle, DLFW_CFGTYPE_ENUM eType, LPCWSTR lpszSection, LPCWSTR lpszValue );