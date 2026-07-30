
#if !defined(AFX_PACKET_DEF_H__3C71714E_9156_4801_B01D_E4079A65C17A__INCLUDED_)
#define AFX_PACKET_DEF_H__3C71714E_9156_4801_B01D_E4079A65C17A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/************************************************************************/
/* FILE_T struct storing file information                               */
/************************************************************************/

typedef struct _tagFILE_INFO
{
    _tagFILE_INFO()
    {
        memset( this, 0, sizeof( _tagFILE_INFO ) );
        dwSize = sizeof( _tagFILE_INFO );
    }
    
    DWORD  dwSize;              // size of this struct itself
    WCHAR  szFileID[256];        // file ID,such as FDL,Fdl2,NV and etc.
    WCHAR  szFileName[256];     // file name,in the packet bin file,it only stores file name
    // but after unpacketing, it stores the full path of bin file
    WCHAR  szFileVersion[252];  // Reserved now; V1->V2 : 256*2 --> 252*2
    DWORD  dwHiFileSize;        // hight file size
    DWORD  dwHiDataOffset;      // hight file size
    DWORD  dwLoFileSize;           // file size
    int    nFileFlag;           // if "0", means that it need not a file, and
    // it is only an operation or a list of operations, such as file ID is "FLASH"
    // if "1", means that it need a file
    DWORD  nCheckFlag;          // if "1", this file must be downloaded;
    // if "0", this file can not be downloaded;
    DWORD  dwLoDataOffset;        // the offset from the packet file header to this file data
    DWORD  dwCanOmitFlag;       // if "1", this file can not be downloaded and not check it as "All files"
    //   in download and spupgrade tool.
    DWORD  dwAddrNum;
    DWORD  dwAddr[5];
    DWORD  dwReserved[249];     // Reserved for future,not used now
} FILE_INFO, *LPFILE_INFO;

/************************************************************************/
/* BIN_PACKET_HEADER_T struct storing packet header information         */
/************************************************************************/
#define PAC_MAGIC       (0xFFFAFFFA)

typedef struct _tagBIN_PACKET_HEADER
{
    _tagBIN_PACKET_HEADER()
    {
        memset( this, 0, sizeof( _tagBIN_PACKET_HEADER ) );
        dwMagic = PAC_MAGIC;
    }
    WCHAR  szVersion[22];       // packet struct version; V1->V2 : 24*2 -> 22*2
    DWORD  dwHiSize;            // the whole packet hight size;
    DWORD  dwLoSize;            // the whole packet low size;
    WCHAR  szPrdName[256];      // product name
    WCHAR  szPrdVersion[256];   // product version
    int    nFileCount;          // the number of files that will be downloaded, the file may be an operation
    DWORD  dwFileOffset;        // the offset from the packet file header to the array of FILE_T struct buffer
    DWORD  dwMode;
    DWORD  dwFlashType;
    DWORD  dwNandStrategy;
    DWORD  dwIsNvBackup;
    DWORD  dwNandPageType;
    WCHAR  szPrdAlias[100];    // product alias
    DWORD  dwOmaDmProductFlag;
    DWORD  dwIsOmaDM;
    DWORD  dwIsPreload;
    //Added start
    WORD   wEncrypted;
    WORD   wFTCRC;
    DWORD  dwFTOrgSize;
    DWORD  dwFTEncryptedSize;
    //Added end
    DWORD  dwReserved[197];
    DWORD  dwMagic;
    WORD   wCRC1;
    WORD   wCRC2;
    
} BIN_PACKET_HEADER, *LPBIN_PACKET_HEADER;
typedef const BIN_PACKET_HEADER* LPCBIN_PACKET_HEADER;


#define _MAX_BLOCK_NUM              5
#define _MAX_NV_BACKUP_FALG_NUM     5
#define MAX_FLAG_LEN                5
#define FLAG_FILE_SUFFIX            _T(".flag")

#endif /* AFX_PACKET_DEF_H__3C71714E_9156_4801_B01D_E4079A65C17A__INCLUDED_ */