#include "StdAfx.h"
#include "CSimpleLock.h"


/////////////////////////////////////////////////////////////////////////////


CSimpleLock::CSimpleLock(VOID)
{
    ::InitializeCriticalSection(&m_stCriticalSection);
}


CSimpleLock::~CSimpleLock(VOID)
{
    ::DeleteCriticalSection(&m_stCriticalSection);
}