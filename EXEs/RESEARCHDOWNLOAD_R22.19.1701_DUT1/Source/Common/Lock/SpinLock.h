#pragma once
#include <Windows.h>

//////////////////////////////////////////////////////////////////////////
#if (_MSC_VER <= 1200)
class CSpinLock
#else
class CSpinLock sealed
#endif 
{
public:
#if (_MSC_VER <= 1200)
    #define VOLATILES 
#else
    #define VOLATILES        volatile
#endif 

    CSpinLock(void);

    operator VOLATILES LONG &(void);
    BOOL IsLocked(void)const;
    void Lock(void);
    void Unlock(void);

private:
    VOLATILES LONG m_nLockState;
};

inline CSpinLock::operator VOLATILES LONG &(void)
{
    return m_nLockState;
}
