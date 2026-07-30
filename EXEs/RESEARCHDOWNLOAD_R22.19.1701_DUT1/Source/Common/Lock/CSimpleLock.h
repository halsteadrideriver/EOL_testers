#pragma once


/////////////////////////////////////////////////////////////////////////////


#include <Windows.h>


/////////////////////////////////////////////////////////////////////////////


/// 简单锁类：线程安全
class CSimpleLock
{
public:

    /// 构造函数
    CSimpleLock(VOID);

    /// 析构函数
    virtual ~CSimpleLock(VOID);

    /// 类型转换操作符
    /// @return 临界区
    operator CRITICAL_SECTION &(VOID);

private:

    /// 临界区
    CRITICAL_SECTION m_stCriticalSection;


    /// 拷贝构造函数：屏蔽外部拷贝操作
    /// @param other - 另一个简单锁对象
    CSimpleLock(CONST CSimpleLock &other);

    /// 拷贝构造函数：屏蔽外部拷贝操作
    /// @param other - 另一个简单锁对象
    /// @return 本简单锁对象
    CSimpleLock &operator =(CONST CSimpleLock &other);
};


inline CSimpleLock::operator CRITICAL_SECTION &(VOID)
{
    return m_stCriticalSection;
}