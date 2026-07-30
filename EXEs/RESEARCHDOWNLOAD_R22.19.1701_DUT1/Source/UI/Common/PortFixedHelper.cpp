#include "stdafx.h"
#include "PortFixedHelper.h"
#include "Utility.h"
#include "DLoader.h"
#include "DLoaderView.h"

CPortFixedHelper::CPortFixedHelper( CDLoaderView* pView )
    : m_pView( pView )
{
    ASSERT( pView );
}

CPortFixedHelper::~CPortFixedHelper()
{

}

void CPortFixedHelper::LoadSettings()
{
    m_bEnable = g_theApp.m_pCfgObject->GetInt( CFGTYPE_DL, _T( "PortSetting" ), _T( "EnableFixedPort" ), 0 );
    LPCWSTR lpszString = g_theApp.m_pCfgObject->GetString( CFGTYPE_DL, _T( "PortSetting" ), _T( "FixedPortList" ), _T( "" ) );
    _tcscpy_s( m_szPortList, lpszString );
}

void CPortFixedHelper::SaveSettings()
{
    if ( m_bEnable )
    {
        g_theApp.m_pCfgObject->SetString( CFGTYPE_DL, _T( "PortSetting" ), _T( "FixedPortList" ), m_szPortList );
    }
}

void CPortFixedHelper::AddFixedProg( int nPort )
{
    if ( m_bEnable )
    {
        std::set<INT>::iterator it = m_setPort.find( nPort );
        if ( it == m_setPort.end() )
        {
            m_setPort.insert( nPort );
            TCHAR szPort[30] = { 0 };
            if ( _tcsclen( m_szPortList ) > 0 )
            {
                _stprintf_s( szPort, _T( ",%d" ), nPort );
            }
            else
            {
                _stprintf_s( szPort, _T( "%d" ), nPort );
            }
            _tcscat_s( m_szPortList, szPort );
        }
    }
}

void CPortFixedHelper::InitPortList( )
{
    if ( m_bEnable )
    {
        m_setPort.clear();
        CUtility utility;
        int nCount = 0;
        LPTSTR* lppString = utility.GetTokenString( m_szPortList, _T( "," ), nCount );
        for ( int i = 0; i < nCount; i++ )
        {
            int nPort = _tstoi( lppString[i] );
            if ( nPort > 0 )
            {
                std::set<INT>::iterator it = m_setPort.find( nPort );
                if ( it == m_setPort.end() )
                {
                    m_pView->AddProg( nPort, TRUE );
                    m_setPort.insert( nPort );
                }
            }
        }
    }
}