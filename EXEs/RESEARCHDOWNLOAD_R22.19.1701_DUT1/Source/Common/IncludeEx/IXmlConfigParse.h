#ifndef _IXMLCONFIGPARSE_H__
#define _IXMLCONFIGPARSE_H__

#include "BMAFExport.h"
#include "BMAGlobal.h"

class IXmlConfigParse
{
public:
    /** initialize the configure file
      *
      * @param lpszFileName: the basic configure file name
      * @param nFlag: if "1", the lpszFileName is spectial one,
      *               program will not find other configure file in the same directory
      *               if "0", program will find all the (*.xml) configue file except "BMAConfigSchema.xml",
      *               and load them by modified time from new to old
      * @return: true, if initialize successfule; false, otherwise
      */
    virtual BOOL    LoadProductInfo( LPCTSTR lpszFileName, BOOL bAll = TRUE ) = 0;
    virtual DWORD   GetProductCount() = 0;
    virtual void    GetProductNameList( LPTSTR pProductNameList, DWORD dwSize, DWORD& dwRealSize ) = 0;
    virtual PPRODUCT_INFO_T  GetProdInfo( LPCTSTR lpszProductName ) = 0;
    virtual LPTSTR  GetConfigFile( LPCTSTR lpszProductName ) = 0;
    virtual void    Release() = 0;
    virtual LPCOMPARE_SOFTWARE GetCompareSoftware( INT& nCount ) = 0; // 共板共模防呆
    virtual LPCSTR GetSignatureEmmc( INT& nCount ) = 0; //分区表保护
    virtual LPCSTR GetSignatureUfs( INT& nCount ) = 0; //分区表保护
    virtual LPCSTR GetBuildTime( INT& nCount ) = 0; //写软件包编译时的时间戳
    virtual BOOL GetPreloadConfigInfo( LPCTSTR lpszPath, LPPRELOAD_CONFIG lpConfig, INT nSize, INT& nRealSize ) = 0;
    virtual BOOL GetMutiSoftConfigInfo( LPCTSTR lpszPath, LPPROJECT_CONFIG lpConfig, INT nSize, INT& nRealSize ) = 0;
};

BMAF_EXPORT_API BOOL CreateXmlConfigParse( IXmlConfigParse** pXmlConfigParse );

#endif //_IXMLCONFIGPARSE_H__