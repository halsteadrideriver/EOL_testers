#pragma once


/////////////////////////////////////////////////////////////////////////////


#include <Windows.h>


/////////////////////////////////////////////////////////////////////////////


/// 锁类：线程安全
class CLocks
{
public:

    /// 构造函数
    /// @param stCriticalSection - 临界区
    CLocks(CRITICAL_SECTION &stCriticalSection);

    /// 析构函数
    virtual ~CLocks(VOID);

private:

    /// 临界区
    CRITICAL_SECTION &m_stCriticalSection;


    /// 拷贝构造函数：屏蔽外部拷贝操作
    /// @param other - 另一个锁对象
    CLocks(CONST CLocks &other);

    /// 拷贝构造函数：屏蔽外部拷贝操作
    /// @param other - 另一个锁对象
    /// @return 本锁对象
    CLocks &operator =(CONST CLocks &other);
};