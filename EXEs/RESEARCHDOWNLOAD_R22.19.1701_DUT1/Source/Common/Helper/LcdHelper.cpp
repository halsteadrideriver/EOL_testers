#include "stdafx.h"
#include "LcdHelper.h"

BOOL CLcdHelper::FindLCDItem( LPCTSTR lpszFilePath, VEC_LCD_CFIG& vLcdCfig, FILETIME* pLastWriteTime )
{
    USES_CONVERSION;
    if ( lpszFilePath == NULL )
    {
        return FALSE;
    }
    
    CFileFind finder;
    if ( !finder.FindFile( lpszFilePath ) )
    {
        return FALSE;
    }
    finder.Close();
    
    vLcdCfig.clear();
    
    BOOL bRlt = FALSE;
    
    HANDLE hFile = INVALID_HANDLE_VALUE;
    hFile = ::CreateFile( lpszFilePath,
                          GENERIC_READ,
                          FILE_SHARE_READ,
                          NULL,
                          OPEN_EXISTING,
                          FILE_ATTRIBUTE_NORMAL,
                          NULL );
                          
    if ( hFile == INVALID_HANDLE_VALUE )
    {
        return FALSE;
    }
    
    if ( pLastWriteTime != NULL )
    {
        ::GetFileTime( hFile, NULL, NULL, pLastWriteTime );
    }
    
    DWORD dwSize = GetFileSize( hFile, NULL );
    
    if ( dwSize == 0 || dwSize == 0xFFFFFFFF )
    {
        CloseHandle( hFile );
        return FALSE;
    }
    
    BYTE* pBuf = new BYTE[dwSize];
    if ( NULL == pBuf )
    {
        CloseHandle( hFile );
        return FALSE;
    }
    
    DWORD dwRealRead = 0;
    ReadFile( hFile, pBuf, dwSize, &dwRealRead, NULL );
    
    const char szBeginFlag[] = "LCDTABLEBEGIN#*##";
    const char szEndFlag[] = "LCDTABLEEND#*##";
    const char szNameFlag[] = "LCDNAME_";
    
    BYTE* lpPos = NULL;
    BYTE* lpBeginPos = NULL;
    BYTE* lpEndPos = NULL;
    BYTE* lpTmpPos = NULL;
    lpPos = std::search( pBuf, pBuf + dwSize, szBeginFlag, szBeginFlag + strlen( szBeginFlag ) - 1 );
    
    LCD_ITEM li = { 0 };
    char szName[128] = { 0 };
    
    
    if ( lpPos < ( pBuf + dwSize ) )
    {
        lpBeginPos = lpPos;
        
        lpPos = std::search( lpBeginPos, lpBeginPos + ( dwSize - ( lpBeginPos - pBuf ) ), szEndFlag, szEndFlag + strlen( szEndFlag ) - 1 );
        if ( lpPos < ( lpBeginPos + ( dwSize - ( lpBeginPos - pBuf ) ) ) )
        {
            lpEndPos = lpPos;
            
            char szTmp[4] = { 0 };
            memcpy( szTmp, lpBeginPos + strlen( szBeginFlag ), 2 );
            int nNameLen = 0;
            sscanf_s( szTmp, "%d", &nNameLen );
            if ( nNameLen > 0 )
            {
                lpTmpPos = lpBeginPos + strlen( szBeginFlag ) + 2;
                while ( lpTmpPos < lpEndPos )
                {
                    lpPos = std::search( lpTmpPos, lpEndPos, szNameFlag, szNameFlag + strlen( szNameFlag ) - 1 );
                    lpTmpPos = lpPos + nNameLen;
                    
                    if ( lpPos < lpEndPos )
                    {
                        bRlt = TRUE;
                        li.dwFlagOffset = lpTmpPos - pBuf;
                        li.dwNameLen = nNameLen;
                        li.dwFlag = ( BYTE )( *lpTmpPos );
                        memset( szName, 0, sizeof( szName ) );
                        memcpy( szName, lpPos, nNameLen );
                        _tcsncpy_s( li.szName, 128, A2W( szName ), _tcslen( A2W( szName ) ) );
                        vLcdCfig.push_back( li );
                    }
                }
            }
        }
    }
    
    CloseHandle( hFile );
    
    delete[] pBuf;
    
    return bRlt;
}

BOOL CLcdHelper::CheckLCDConfig( VEC_LCD_CFIG& vLcdCfig1, VEC_LCD_CFIG& vLcdCfig2 )
{
    if ( vLcdCfig1.size() != vLcdCfig2.size() )
    {
        return FALSE;
    }
    
    int nSize = vLcdCfig1.size();
    for ( int i = 0; i < nSize; i++ )
    {
        if ( vLcdCfig1[i].dwFlagOffset != vLcdCfig2[i].dwFlagOffset )
        {
            return FALSE;
        }
    }
    return TRUE;
}