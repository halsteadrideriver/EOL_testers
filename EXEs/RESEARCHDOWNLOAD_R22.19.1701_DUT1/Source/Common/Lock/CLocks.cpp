#include "StdAfx.h"
#include "CLocks.h"


/////////////////////////////////////////////////////////////////////////////


CLocks::CLocks(CRITICAL_SECTION &stCriticalSection)
    : m_stCriticalSection(stCriticalSection)
{
    ::EnterCriticalSection(&m_stCriticalSection);
}


CLocks::~CLocks(VOID)
{
    ::LeaveCriticalSection(&m_stCriticalSection);
}