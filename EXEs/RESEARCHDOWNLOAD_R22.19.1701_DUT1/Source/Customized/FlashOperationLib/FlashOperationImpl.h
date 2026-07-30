#pragma once
#include "Tr.h"
#include "IFlashOperation.h"

struct WRITE_FILE_VALUE_OP
{
public:
    LPFLASH_WRITE_FILE_VALUE Find( LPCWCH pFileId )
    {
        for ( size_t i = 0; i < m_arrWrite.size(); i++ )
        {
            DWORD dwFileSize = ( *m_arrWrite[i].pFileSize );
            DWORD dwFileOffset = ( *m_arrWrite[i].pFileOffset );
            //根据需要去修改判断条件，比如去比对FileSize和FileOffset;
            if ( _wcsicmp( m_arrWrite[i].pFileId, pFileId ) == 0 )
            {
                return &m_arrWrite[i];
            }
        }
        return NULL;
    }
    
    void PushBack( LPFLASH_WRITE_FILE_VALUE lpValue )
    {
        m_arrWrite.push_back( *lpValue );
    }
    
    void Clear()
    {
        m_arrWrite.clear();
    }
    
private:
    std::vector<FLASH_WRITE_FILE_VALUE> m_arrWrite;
};

class CFlashOperationImpl : public IFlashOperation, public CTr
{
public:
    CFlashOperationImpl( LPVOID pLogUtil );
    virtual ~CFlashOperationImpl();
    
public:
    virtual void Release( void );
    virtual void GetErrorMsg( LPTSTR lpErrMsg, DWORD dwCount );
    virtual SPRESULT SetProperty( INT nProperty, INT nFlags, LPCVOID lpValue );
    virtual SPRESULT GetProperty( INT nProperty, INT nFlags, LPVOID lpValue );
    virtual SPRESULT EnableOperation( DL_TYPE_ENUM eType, LPFLASH_OPERATION_SWITCH lpOperation );
    virtual SPRESULT CheckReadBuffer( LPCSTR lpReadBuffer, LPFLASH_READ_FILE lpReadFile );
    virtual SPRESULT PrepareReadFlashAfterFDL2( LPFLASH_READ_FILE lpReadFile, INT& nCount );
    virtual SPRESULT PrepareWriteFlashAfterFDL2( LPFLASH_WRITE_FILE lpWriteFile, INT& nCount );
    virtual SPRESULT PrepareWriteFlashAfterDlEnd( LPFLASH_WRITE_FILE lpWriteFile, INT& nCount );
    virtual SPRESULT PrepareEraseFlashAfterFDL2( LPFLASH_ERASE_FILE lpEraseFile, INT& nCount );
    
private:
    BOOL SaveFileToLocal( LPCSTR pszFileName, LPBYTE pBuf, DWORD dwSize );
    
private:
    INT m_nCount = 0;
    INT m_nCheckOffset1 = 12288 + 50;
    INT m_nCheckOffset2 = 12288 * 3 + 50;
    TCHAR m_szErrorMsg[256] = {0};
    WRITE_FILE_VALUE_OP m_arrWriteFdl2;
    WRITE_FILE_VALUE_OP m_arrWriteDlEnd;
};