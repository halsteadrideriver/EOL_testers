#include "StdAfx.h"
#include "SpinLock.h"

//////////////////////////////////////////////////////////////////////////
#define SPIN_LOCK_STATE_LOCK        (1)
#define SPIN_LOCK_STATE_UNLOCK      (0)

//////////////////////////////////////////////////////////////////////////
CSpinLock::CSpinLock(void)
: m_nLockState(SPIN_LOCK_STATE_UNLOCK)
{
}

BOOL CSpinLock::IsLocked(void)const
{
    return (SPIN_LOCK_STATE_LOCK == m_nLockState);
}

void CSpinLock::Lock(void)
{
#if (_MSC_VER <= 1200)
    while ((LONG)::InterlockedCompareExchange((PVOID* )&m_nLockState, (PVOID)SPIN_LOCK_STATE_LOCK, 
        (PVOID)SPIN_LOCK_STATE_UNLOCK) != SPIN_LOCK_STATE_UNLOCK)
#else
    while (::InterlockedCompareExchange(&m_nLockState, SPIN_LOCK_STATE_LOCK, 
        SPIN_LOCK_STATE_UNLOCK) != SPIN_LOCK_STATE_UNLOCK)
#endif
    {
    #ifdef WIN64
        _mm_pause();
    #else
        __asm {pause}
    #endif
    }
}

void CSpinLock::Unlock(void)
{
    ::InterlockedExchange(&m_nLockState, SPIN_LOCK_STATE_UNLOCK);
}