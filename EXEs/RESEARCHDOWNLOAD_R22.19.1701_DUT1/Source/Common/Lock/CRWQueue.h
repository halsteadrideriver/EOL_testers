#ifndef _CRWQUEUE_H_
#define _CRWQUEUE_H_


/////////////////////////////////////////////////////////////////////////////


#include <new>
#include <Windows.h>
#include <Limits.h>


/////////////////////////////////////////////////////////////////////////////


/// 最小读、写队列块容量
#define MIN_RW_QUEUE_BLOCK_CAPACITY                         (1024 * 1)

/// 默认读、写队列块容量
#define DEFAULT_RW_QUEUE_BLOCK_CAPACITY                     (1024 * 64)

#ifdef WIN64                    /// 64Bit

/// 最大读、写队列块容量
#define MAX_RW_QUEUE_BLOCK_CAPACITY                         (((ULONGLONG)-1) / 4)

#else                           /// 32Bit

/// 最大读、写队列块容量
#define MAX_RW_QUEUE_BLOCK_CAPACITY                         (((UINT)-1) / 4)

#endif


/////////////////////////////////////////////////////////////////////////////


/// 读、写队列：线程安全（仅限一线程读队列、另一线程写队列，即单消费者、生产者问题）
/// @param TItem - 队列元素类型
template<typename TItem>
class CRWQueue
{
public:

    /// 构造函数：线程安全时调用
    /// @param uBlockCapacity - 队列块容量，MIN_RW_QUEUE_BLOCK_CAPACITY ～ MAX_RW_QUEUE_BLOCK_CAPACITY，默认值=DEFAULT_RW_QUEUE_BLOCK_CAPACITY
    /// @param bSetAllBlockItemDataZero - TRUE=全置队列块元素数据为 0；FALSE=不全置队列块元素数据为 0（默认值）
    /// @param bMemoryCopyItem - TRUE=内存拷贝元素（默认值）；FALSE=非内存拷贝元素
    CRWQueue(size_t uBlockCapacity = DEFAULT_RW_QUEUE_BLOCK_CAPACITY,
        BOOL bSetAllBlockItemDataZero = FALSE, BOOL bMemoryCopyItem = TRUE);

    /// 析构函数：线程安全时调用
    virtual ~CRWQueue(VOID);

    /// 取得队列块容量
    /// @return 队列块容量
    size_t BlockCapacity(VOID) CONST;

    /// 取得队列大小
    /// @return 队列大小
    size_t Size(VOID) CONST;

    /// 是否是空队列
    /// @return TRUE=空队列；FALSE=非空队列
    BOOL IsEmpty(VOID) CONST;

    /// 取得队列开始元素
    /// @return 队列开始元素
    TItem *Begin(VOID);

    /// 取得队列结束元素
    /// @return 队列结束元素
    TItem *End(VOID);

    /// 队尾入队
    /// @param pItems - 入队元素集，非 NULL
    /// @param uSize - 入队大小，不超过队列块容量，默认值=1
    /// @return 实际入队大小
    size_t Push(CONST TItem *pItems, size_t uSize = 1);

    /// 队尾入队
    /// @param tItem - 入队元素（单元素）
    /// @return 实际入队大小
    size_t Push(CONST TItem &tItem);

    /// 队首出队
    /// @param pItems - NULL=无效；非 NULL=出队元素集
    /// @param uSize - 出队大小，不超过 pItems 大小，不超过队列块容量，默认值=SIZE_MAX
    /// @return 实际出队大小
    size_t Pop(TItem *pItems, size_t uSize = SIZE_MAX);

    /// 队首出队
    /// @param tItem - 出队元素（单元素）
    /// @return TRUE=成功；FALSE=失败
    BOOL Pop(TItem &tItem);

    /// 清空：线程安全时调用
    VOID Clear(VOID);

private:

    /// 块队列类型
    typedef struct TBlockQueue
    {
    public:

        /// 队列
        TItem *pQueue;

        /// 下一块队列
        TBlockQueue *pNext;


        /// 构造函数
        /// @param uBlockCapacity - 队列块容量，MIN_RW_QUEUE_BLOCK_CAPACITY ～ MAX_RW_QUEUE_BLOCK_CAPACITY，默认值=DEFAULT_RW_QUEUE_BLOCK_CAPACITY
        /// @param bSetAllBlockItemDataZero - TRUE=全置队列块元素数据为 0；FALSE=不全置队列块元素数据为 0（默认值）
        TBlockQueue(size_t uBlockCapacity, BOOL bSetAllBlockItemDataZero)
            : pNext(NULL)
        {
#ifdef _DEBUG

            pQueue = new TItem[uBlockCapacity];

#else

            pQueue = new(std::nothrow) TItem[uBlockCapacity];

#endif

            if (bSetAllBlockItemDataZero && pQueue)
            {
                ::ZeroMemory(pQueue, sizeof(TItem) * uBlockCapacity);
            }
        }


        /// 析构函数
        ~TBlockQueue(VOID)
        {
            pNext = NULL;

            if (pQueue)
            {
                delete []pQueue;
                pQueue = NULL;
            }
        }
    }*TBlockQueuePtr;


    /// 块队列链表头
    volatile TBlockQueuePtr m_pHeadBlockQueue;

    /// 块队列链表尾
    volatile TBlockQueuePtr m_pTailBlockQueue;

    /// 队列块容量
    CONST size_t m_uBlockCapacity;

    /// 是否全置队列块元素数据为 0：TRUE=全置队列块元素数据为 0；FALSE=不全置队列块元素数据为 0（默认值）
    CONST BOOL m_bSetAllBlockItemDataZero;

    /// 是否是内存拷贝元素：TRUE=内存拷贝元素（默认值）；FALSE=非内存拷贝元素
    CONST BOOL m_bMemoryCopyItem;

    /// 队列读指针
    volatile UINT64 m_uReadPointer;

    /// 队列写指针
    volatile UINT64 m_uWritePointer;


    /// 拷贝构造函数：屏蔽外部拷贝操作
    /// @param other - 另一个读、写队列对象
    CRWQueue(CONST CRWQueue &other);

    /// 赋值操作符：屏蔽外部赋值操作
    /// @param other - 另一个读、写队列对象
    /// @return 本读、写队列对象
    CRWQueue &operator=(CONST CRWQueue &other);


    /// 检查块队列链表头
    VOID CheckHeadBlockQueue(VOID);

    /// 拷贝元素
    /// @param pSourceItems - 源元素集，非 NULL
    /// @param pTargetItems - 目标元素集，非 NULL
    /// @param uSize - 大小
    VOID CopyItem(CONST TItem *pSourceItems, TItem *pTargetItems, size_t uSize);
};


template<typename TItem>
CRWQueue<TItem>::CRWQueue(size_t uBlockCapacity, BOOL bSetAllBlockItemDataZero, BOOL bMemoryCopyItem)
    : m_uBlockCapacity(max(min(uBlockCapacity, MAX_RW_QUEUE_BLOCK_CAPACITY), MIN_RW_QUEUE_BLOCK_CAPACITY))
    , m_bSetAllBlockItemDataZero(bSetAllBlockItemDataZero)
    , m_bMemoryCopyItem(bMemoryCopyItem)
    , m_uReadPointer(0)
    , m_uWritePointer(0)
{
#ifdef _DEBUG

    m_pHeadBlockQueue = new TBlockQueue(m_uBlockCapacity, m_bSetAllBlockItemDataZero);

#else

    m_pHeadBlockQueue = new(std::nothrow) TBlockQueue(m_uBlockCapacity, m_bSetAllBlockItemDataZero);

#endif

    m_pTailBlockQueue = m_pHeadBlockQueue;
}


template<typename TItem>
CRWQueue<TItem>::~CRWQueue(VOID)
{
    Clear();
    m_pTailBlockQueue = NULL;

    if (m_pHeadBlockQueue)
    {
        delete m_pHeadBlockQueue;
        m_pHeadBlockQueue = NULL;
    }
}


template<typename TItem>
inline size_t CRWQueue<TItem>::BlockCapacity(VOID) CONST
{
    return m_uBlockCapacity;
}


template<typename TItem>
inline size_t CRWQueue<TItem>::Size(VOID) CONST
{
    return (size_t)(m_uWritePointer - m_uReadPointer);
}


template<typename TItem>
inline BOOL CRWQueue<TItem>::IsEmpty(VOID) CONST
{
    return (m_uWritePointer <= m_uReadPointer);
}


template<typename TItem>
inline TItem *CRWQueue<TItem>::Begin(VOID)
{
    if (!m_pHeadBlockQueue || !m_pHeadBlockQueue->pQueue || IsEmpty())
    {
        return NULL;
    }

    /// 检查块队列链表头：临界、越界
    CheckHeadBlockQueue();

    return (m_pHeadBlockQueue->pQueue + m_uReadPointer % m_uBlockCapacity);
}


template<typename TItem>
inline TItem *CRWQueue<TItem>::End(VOID)
{
    if (!m_pTailBlockQueue || !m_pTailBlockQueue->pQueue || IsEmpty())
    {
        return NULL;
    }

    return (m_pTailBlockQueue->pQueue + (m_uWritePointer - 1) % m_uBlockCapacity);
}


template<typename TItem>
VOID CRWQueue<TItem>::CheckHeadBlockQueue(VOID)
{
    /// 索引号
    size_t uIndex = m_uReadPointer % m_uBlockCapacity;

    /// 释放块队列链表头资源
    if (m_pHeadBlockQueue
        && (m_uReadPointer > 0)
        && (uIndex == 0))
    {
        TBlockQueuePtr pTemp = m_pHeadBlockQueue;
        m_pHeadBlockQueue = m_pHeadBlockQueue->pNext;
        delete pTemp;
        pTemp = NULL;
    }
}


template<typename TItem>
VOID CRWQueue<TItem>::CopyItem(CONST TItem *pSourceItems, TItem *pTargetItems, size_t uSize)
{
    if (pSourceItems && pTargetItems && (pSourceItems != pTargetItems))
    {
        if (m_bMemoryCopyItem)
        {
            ::memcpy(pTargetItems, pSourceItems, sizeof(TItem) * uSize);
        }
        else
        {
            for (size_t i = 0; i < uSize; i++)
            {
                *(pTargetItems + i) = *(pSourceItems + i);
            }
        }
    }
}


template<typename TItem>
size_t CRWQueue<TItem>::Push(CONST TItem *pItems, size_t uSize)
{
    if (!m_pTailBlockQueue || !m_pTailBlockQueue->pQueue || !pItems || (uSize == 0))
    {
        return 0;
    }

    /// 索引号
    size_t uReturned = min(uSize, m_uBlockCapacity);
    size_t uIndex = m_uWritePointer % m_uBlockCapacity;
    uSize = uIndex + uReturned;

    /// 未越界
    if (((uIndex > 0) || (m_uWritePointer < 1)) && (uSize <= m_uBlockCapacity))
    {
        CopyItem(pItems, m_pTailBlockQueue->pQueue + uIndex, uReturned);
    }

    /// 越界、补充
    else
    {
        size_t uNewIndex = uSize % m_uBlockCapacity;
        uSize = uReturned - uNewIndex;
        CopyItem(pItems, m_pTailBlockQueue->pQueue + uIndex, uSize);
        m_pTailBlockQueue->pNext = new TBlockQueue(m_uBlockCapacity, m_bSetAllBlockItemDataZero);
        m_pTailBlockQueue = m_pTailBlockQueue->pNext;
        CopyItem(pItems + uSize, m_pTailBlockQueue->pQueue, uNewIndex);
    }

    m_uWritePointer += uReturned;

    return uReturned;
}


template<typename TItem>
inline size_t CRWQueue<TItem>::Push(CONST TItem &tItem)
{
    return Push(&tItem, 1);
}


template<typename TItem>
size_t CRWQueue<TItem>::Pop(TItem *pItems, size_t uSize)
{
    if (!m_pHeadBlockQueue || !m_pHeadBlockQueue->pQueue || IsEmpty() || !pItems || (uSize == 0))
    {
        return 0;
    }

    /// 检查块队列链表头：临界、越界
    CheckHeadBlockQueue();

    /// 大小
    size_t uReturned = Size();
    uSize = min(uSize, m_uBlockCapacity);
    uReturned = min(uSize, uReturned);

    /// 索引号
    size_t uIndex = m_uReadPointer % m_uBlockCapacity;

    /// 越界、截断
    if ((uReturned + uIndex) > m_uBlockCapacity)
    {
        uReturned = m_uBlockCapacity - uIndex;
    }

    /// 拷贝数据
    CopyItem(m_pHeadBlockQueue->pQueue + uIndex, pItems, uReturned);

    /// 增加读指针
    m_uReadPointer += uReturned;

    return uReturned;
}


template<typename TItem>
inline BOOL CRWQueue<TItem>::Pop(TItem &tItem)
{
    return (Pop(&tItem, 1) > 0);
}


template<typename TItem>
VOID CRWQueue<TItem>::Clear(VOID)
{
    m_uWritePointer = 0;
    m_uReadPointer = 0;

    if (m_pHeadBlockQueue)
    {
        /// 截断块队列链表
        TBlockQueuePtr pHeadBlockQueue = m_pHeadBlockQueue->pNext;
        m_pTailBlockQueue = m_pHeadBlockQueue;
        m_pHeadBlockQueue->pNext = NULL;

        /// 释放块队列链表资源
        while (pHeadBlockQueue)
        {
            TBlockQueuePtr pTemp = pHeadBlockQueue;
            pHeadBlockQueue = pHeadBlockQueue->pNext;
            delete pTemp;
            pTemp = NULL;
        }
    }
}


/////////////////////////////////////////////////////////////////////////////


/// ASCII 字符读、写队列
typedef CRWQueue<CHAR>                                      CACharRWQueue;


/// UNICODE 字符读、写队列
typedef CRWQueue<WCHAR>                                     CWCharRWQueue;


#ifdef UNICODE

/// 字符读、写队列
typedef CWCharRWQueue                                       CCharRWQueue;

#else

/// 字符读、写队列
typedef CACharRWQueue                                       CCharRWQueue;

#endif


/////////////////////////////////////////////////////////////////////////////


#endif