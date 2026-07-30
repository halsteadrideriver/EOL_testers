/************************************************************************
*                                                                       *
*   dl_def.h -  download definitions                                  *
*                                                                       *
*   COPYRIGHT (C) 2019 UNISOC TECHNOLOGIES INC.                         *
*                                                                       *
************************************************************************/
#if !defined(AFX_DL_DEF_H__3C71714E_9156_4801_B01D_E4079A65C17A__INCLUDED_)
#define AFX_DL_DEF_H__3C71714E_9156_4801_B01D_E4079A65C17A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DLFWContainerExDef.h"
#include "DLFWEnumExDef.h"

#define OPR_SUCCESS                 (0)
#define OPR_FAIL                    (1)

#define PRODUCTION_INFO_SIZE        (MAX_PRODUCTIONINFO_SIZE)         //8K

#define MAX_PORT_NUMBER             (100)
#define MAX_FILE_COUNT              (40)
#define MAX_BUF_SIZE                (MAX_FILE_COUNT * (256 + 10))

#define IMG_BACKUP_FOLDER           _T("imgstore")

#define NV_LENGTH                   0x10000
#define UBOOT_LENGTH                0x100000

#define UDSIK_IMG_NAME_LEN          (13)

#define DI_SET_KEY_ITEM(d)          ((d) | (0x10000000))
#define DI_IS_KEY_ITEM(d)           ((d) & (0x10000000))
#define DI_IS_NEED_FILE(d)          ((d) & (0x00000001))

#endif /* AFX_DL_DEF_H__3C71714E_9156_4801_B01D_E4079A65C17A__INCLUDED_ */