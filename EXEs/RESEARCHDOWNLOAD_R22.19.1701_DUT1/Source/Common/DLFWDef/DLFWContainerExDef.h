#pragma once

#pragma warning( push,3 )
#include <map>
#include <vector>
#include <string>
#include <list>
#pragma warning( pop )
using namespace std;

#include "DLFWStructExDef.h"

typedef CMap<CString, LPCTSTR, CString, LPCTSTR>                        MAP_MCP, MAP_DLFILE, MAP_PRODUCT_CONFIG_FILE;
typedef CMap<CString, LPCTSTR, BOOL, BOOL>                              MAP_DLSTATE; //id->check
typedef CMap<CString, LPCTSTR, __int64, __int64>                        MAP_DLSIZE;  //id->Size
typedef CMap<CString, LPCTSTR, DATA_INFO_T, DATA_INFO_T>                MAP_DATAINFO;  //FileName->DATAINFO
typedef CMap<CString, LPCTSTR, FILETIME, FILETIME>                      MAP_FILELASTTIME;      //FileName->LastWriteTime
typedef CMap<CString, LPCTSTR, DWORD, DWORD>                            MAP_STRINT;
typedef map<pair<CString, CString>, EXT_IMG_INFO_PTR>                   MAP_FILEBUF_PB; //page block

typedef CMap<DWORD, DWORD, EXT_IMG_INFO_PTR, EXT_IMG_INFO_PTR>          MAP_EXTIMG;
typedef map<pair<DWORD, CString>, EXT_IMG_INFO_PTR>                     MAP_FILEBUF;
typedef CMap<CString, LPCTSTR, BACKUP_INFO_PTR, BACKUP_INFO_PTR>        MAP_NVFILE;
typedef map<pair<DWORD, CString>, BACKUP_INFO_PTR>                      MAP_MULTI_NVFILE;

typedef vector<WORD>                                                    VEC_WORD;
typedef CMap<int, int, VEC_WORD, VEC_WORD >								MAP_CALIFLAG;
typedef CMap<CString, LPCTSTR, PNV_BACKUP_INFO_T, PNV_BACKUP_INFO_T>    MAP_NV_BAK;
typedef CMap<CString, LPCTSTR, COMPARE_FILE_INFO, COMPARE_FILE_INFO>    MAP_CMP_INFO;
typedef vector<CString>                                                 VEC_CU;
typedef vector<FILE_INFO_T>                                             VEC_FILE_INFO;
typedef vector<PRODUCT_INFO_T*>                                         VEC_PRODUCT_INFO;
typedef CMap<DWORD, DWORD, DWORD, DWORD>                                MAP_PORT;
typedef list< TCHAR* >                                                  LIST_CHAR;
typedef CMap< CString, LPCTSTR, __int64, __int64 >                      MAP_PARTITION;