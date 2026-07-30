#ifndef _CRASHDUMP_EXPORT_H_339A7F7A_9092_4674_9893_27B39968A6EC_
#define _CRASHDUMP_EXPORT_H_339A7F7A_9092_4674_9893_27B39968A6EC_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Windows.h>
//////////////////////////////////////////////////////////////////////////
/// Export APIs
#ifdef  CRASHDUMP_EXPORT
#undef  CRASHDUMP_EXPORT
#endif
#ifdef  CRASHDUMP_EXPORTS
    #define CRASHDUMP_EXPORT EXTERN_C __declspec (dllexport)
#else
    #define CRASHDUMP_EXPORT EXTERN_C __declspec (dllimport)
#endif

CRASHDUMP_EXPORT void CrashDump(void);


#endif /*_CRASHDUMP_EXPORT_H_339A7F7A_9092_4674_9893_27B39968A6EC_*/