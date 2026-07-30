#pragma once
#include <windows.h>
#include <exception>

//////////////////////////////////////////////////////////////////////////
/// bind  
#if (_MSC_VER <= 1500 ) /* VS2008 */

template <class _Result>
struct trivial_function 
{
    typedef _Result result_type;
};

template <class _Operation>
class binder : public trivial_function<typename _Operation::result_type> 
{
public:
    binder(const _Operation &x, const typename _Operation::argument_type &y)
        : op(x)
        , value(y) 
    {
    }

    typename _Operation::result_type operator()(void) const 
    {
        return op(value);
    }
protected:
    _Operation op;
    typename _Operation::argument_type value;
};

template <class _Operation, class _Tp>
inline binder<_Operation> bind(const _Operation& __fn, const _Tp& __x) 
{
    typedef typename _Operation::argument_type _Arg_type;
    return binder<_Operation>(__fn, _Arg_type(__x));
}

#else
    #include <functional>
    using namespace std;
#endif


//////////////////////////////////////////////////////////////////////////
/// thread body
class IRunner
{
public:
    virtual ~IRunner(void) { };
    virtual void run(void) = 0;
};

//////////////////////////////////////////////////////////////////////////
template <class T>
class CRunnerFunctor : public IRunner 
{
public:
    CRunnerFunctor(const T &f) 
        : m_func(f) 
    {

    }

    virtual void run(void) 
    { 
        m_func(); 
    }

private:
    T m_func;
};

/// base template for no argument functor
template <class T>
struct FuncImpl 
{
    static IRunner* transfer(const T &t) 
    {
        try
        {
            return new CRunnerFunctor<T>(t);
        }
        catch (const std::bad_alloc& /*e*/)
        {
        	return NULL;
        }
    }
};

/// partial specialization for T*
template <class T>
struct FuncImpl<T *> 
{
    static IRunner* transfer(T *t) 
    {
        return t;
    }
};

/// partial specialization for no argument function
template <class T>
struct FuncImpl<T (*)()> 
{
    static IRunner* transfer(T (*t)()) 
    {
        try
        {
            return new CRunnerFunctor<T (*)()>(t);
        }
        catch (const std::bad_alloc& /*e*/)
        {
            return NULL;
        }
    }
};

template <class T>
inline IRunner* transfer(const T &t) 
{
    return FuncImpl<T>::transfer(t);
};

//////////////////////////////////////////////////////////////////////////
struct ThreadArgs
{
    HANDLE hEvent;
    void*  pThis;

    ThreadArgs(void* object)
        : pThis(object)
    {
        hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
    }
    ~ThreadArgs(void)
    {
        if (NULL != hEvent)
        {
            ::CloseHandle(hEvent);
        }
    }
};

//////////////////////////////////////////////////////////////////////////
class CThread : public IRunner
{
public:
    CThread(void) 
        : m_lpRunner(NULL)
        , m_hThread(NULL)
        , m_nID(0) 
    {
    }

    template <class T>
    explicit CThread(const T &op)
        : m_lpRunner(transfer(op))
        , m_hThread(NULL) 
        , m_nID(0)
    {
    }

    virtual ~CThread(void);

    /*
        Dynamical binding runner, Example:
        Class CMyRunner
        {
        public:
             CMyRunner(void) { }
            ~CMyRunner(void) { }
            void MyRun(void) 
            {
                // TODO: Thread body
            }
        }
        
        void main(void)
        {
            CMyRunner runner;
            CThread thread;
            thread.bind_runner(bind(&CMyRunner::MyRun, &runner));
            thread.start();
            thread.join();
        }
    */
    template <class T>
    void bind_runner(const T &op)
    {
        if (NULL != m_lpRunner)
        {
            delete m_lpRunner;
            m_lpRunner = NULL;
        }

        m_lpRunner = transfer(op);
    }

    /// thread body
    virtual void run(void) { }

    /// start 
    BOOL start(void);

    /// stop 
    void join(DWORD dwTimeOut=INFINITE);

    /// 
    BOOL isRunning(void)const { return NULL != m_hThread; };
    unsigned int getID(void)const { return m_nID; };
    HANDLE getHandle(void)const { return m_hThread; };

private:
    CThread(const CThread &other);
    CThread &operator =(const CThread &other);

    static unsigned int __stdcall getThreadProc(void *pParam);
    void _threadProc(void);

private:
    IRunner* m_lpRunner;
    HANDLE   m_hThread;
    unsigned int m_nID;
};


