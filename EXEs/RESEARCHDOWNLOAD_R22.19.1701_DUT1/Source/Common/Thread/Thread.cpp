#include "stdafx.h"
#include <process.h>
#include <assert.h>
#include "Thread.h"
//////////////////////////////////////////////////////////////////////////
CThread::~CThread(void)
{
    if (NULL != m_hThread)
    {
        ::CloseHandle(m_hThread);
        m_hThread = NULL;
    }

    if (NULL != m_lpRunner)
    {
        delete m_lpRunner;
        m_lpRunner = NULL;
    }
}

BOOL CThread::start(void)
{
    if (NULL != m_hThread)
    {
        return TRUE;
    }
    
    ThreadArgs* lpArgs = NULL;
    try
    {
        lpArgs = new ThreadArgs(this); 
    }
    catch (const std::bad_alloc& /*e*/)
    {
        lpArgs = NULL;
    }

    if (NULL == lpArgs)
    {
        assert(0);
        return FALSE;
    }

    m_hThread = reinterpret_cast<HANDLE>(_beginthreadex(0, 0, CThread::getThreadProc, (void* )lpArgs, 0, &m_nID));
    if (NULL == m_hThread)
    {
        delete lpArgs;
        return FALSE;
    }

    ::WaitForSingleObject(lpArgs->hEvent, INFINITE);
    delete lpArgs;

    return TRUE;
}

void CThread::join(DWORD dwTimeOut/* = INFINITE */)
{
    if (NULL == m_hThread)
    {
        return ;
    }

    for ( ; ; )
    {
        switch(::MsgWaitForMultipleObjects(1, &m_hThread, FALSE, dwTimeOut, QS_ALLINPUT))
        {
        case WAIT_OBJECT_0:
            {
                ::CloseHandle(m_hThread);
                m_hThread = NULL;
            }
            break;
        case WAIT_OBJECT_0 + 1:
            {
                MSG msg;
                if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
                {
                    ::TranslateMessage(&msg);
                    ::DispatchMessage(&msg);
                }
            }
            continue;
        default:
            break;
        }

        break;
    }

    m_nID = 0;
}

unsigned int __stdcall CThread::getThreadProc(void* pParam)
{
    ThreadArgs* lpArgs = static_cast<ThreadArgs* >(pParam);
    assert(lpArgs);
    CThread*    lpThis = static_cast<CThread* >(lpArgs->pThis);
    assert(lpThis);

    /// Notify thread is running
    ::SetEvent(lpArgs->hEvent);

    lpThis->_threadProc();

    return 0;
}

void CThread::_threadProc(void)
{
    if (NULL != m_lpRunner)
    {
        m_lpRunner->run();
    }
    else
    {
        run();
    }

    /// Thread is finished, free handle
    ::CloseHandle(m_hThread);
    m_hThread = NULL;
}
