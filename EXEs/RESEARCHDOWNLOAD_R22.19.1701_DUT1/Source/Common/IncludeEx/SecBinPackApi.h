#ifndef _SEC_BINPACK_API_H_
#define _SEC_BINPACK_API_H_

/// µ¼³öºê
#ifdef SECBINPACK_EXPORTS
#define SECBINPACK_API extern "C" __declspec(dllexport)
#else
#define SECBINPACK_API extern "C" __declspec(dllimport)
#endif

#include "packet_def.h"

struct _X_NV_BACKUP_FLAG_T
{
    _TCHAR szFlagName[MAX_PATH];
    DWORD  dwCheck;
};

struct _X_NV_BACKUP_ITEM_T
{
    _X_NV_BACKUP_ITEM_T()
    {
        memset( this, 0, sizeof( _X_NV_BACKUP_ITEM_T ) );
    }
    _TCHAR szItemName[MAX_PATH];
    WORD   wIsBackup;
    WORD   wIsUseFlag;
    DWORD  dwID;
    DWORD  dwFlagCount;
    _X_NV_BACKUP_FLAG_T nbftArray[_MAX_NV_BACKUP_FALG_NUM];
};

/************************************************************************/
/* Class CBinPack, it packets the bin files and unpackets the packet    */
/************************************************************************/


class IBinPack
{
public:
    virtual ~IBinPack() = 0;
    virtual void SetSafeKey( DWORD dwKey = 0 ) = 0;
    virtual void SetReceiver( DWORD dwHandle, BOOL bThread = FALSE ) = 0;
    
    /** Open bin packet file for read or write
    *
    * @param lpszFileName: packet file name
    * @param bOpenFlag: if true, open for read, otherwise open for write
    * @return: true,if open successfully;false,otherwise
    */
    virtual BOOL Open( LPCTSTR lpszFileName, BOOL bOpenFlag = FALSE ) = 0;
    
    /** Packet bin files into one file with certain struct
    *
    * @param pbph: the packet header
    * @param lpszCfgFile: the name of configure file (*.xml)
    * @param pFileArray: point to an array of FILE_T struct
    * @param bSafeMode:  if set SafeMode, pac is encrypted
    * @return: 0,if packet successfully;otherwise,packet fail
    */
    virtual unsigned int Packet( LPCBIN_PACKET_HEADER pbph, LPCTSTR lpszCfgFile, LPFILE_INFO pFileArray, LPCTSTR lpszOrgPacFile = NULL, BOOL bSafeMode = FALSE ) = 0;
    
    
    /** Unpacket the packet to bin files into one specified directory
    *
    * @param lpszReleaseDirPath: the directory to release bin files
    *                            If it equals NULL, program will create the directory to system temp directory.
    *                            Suggest to set it null.
    * @param bph: store the packet header
    * @param ppFileArray: store the bin files information into FILE_T struct
    *                     it must be release by the "ReleaseMem" function
    * @param bFactoryMode: whether it is factory download mode
    * @return: 0,if unpacket successfully; otherwise,unpacket fail
    */
    virtual unsigned int Unpacket( LPCTSTR lpszReleaseDirPath, BIN_PACKET_HEADER& bph, LPFILE_INFO* ppFileArray, BOOL bFactoryMode = FALSE ) = 0;
    
    /** Close the opened file
    * You can use it or not, for it will be invoked in the destruct function
    *
    */
    virtual void Close() = 0;
    
    /** Release the memory newed by Unpacket function
    *
    * @param paFile: point to FILE_T buffer
    */
    virtual void ReleaseMem( LPFILE_INFO paFile ) = 0;
    
    /** Get released directory
    *
    * @return: the path of released path
    */
    virtual void GetReleaseDir( TCHAR* szReleaseDir, int nSize ) = 0;
    
    /** Get xml configure file path
    *
    * @return: the path of xml configure
    */
    virtual void GetConfigFilePath( TCHAR* szXMLFile, int nSize ) = 0;
    
    /** Get Software version saved in Vpac
    *
    * @return: the pointer of Vpac byte array
    */
    virtual void GetVPacVersion( LPTSTR& pVpac, int& nVpacSize ) = 0;
    
    virtual BOOL IsVPacFilled() = 0;
    
    virtual void Release() = 0;
    virtual BOOL IsExistEmptyFile() = 0;
    virtual BOOL IsMapFromPac( LPCTSTR lpDLFile ) = 0;
    virtual void GetErrString( DWORD dwErrorCode, TCHAR* lpszOutString, int nSize ) = 0;
    virtual void SetPacSTVersion( LPCTSTR lpVersion ) = 0; //_tsclen(lpVersion) <= 21
    
};

SECBINPACK_API BOOL CreateSecPacParse( IBinPack** pSecPacParse, LPCVOID lpParam );

#endif // _SEC_BINPACK_API_H_
