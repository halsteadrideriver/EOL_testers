#include "stdafx.h"
#include "Resource.h"
#include "ButtonEx.h"
#include "CoolStatusBar.h"

BEGIN_MESSAGE_MAP( CButtonEx, CButton )
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSELEAVE()
    ON_WM_MOUSEHOVER()
END_MESSAGE_MAP()


void CButtonEx::OnMouseMove( UINT nFlags, CPoint point )
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    TRACKMOUSEEVENT tme;
    tme.cbSize = sizeof( TRACKMOUSEEVENT );
    tme.dwFlags = TME_LEAVE | TME_HOVER;
    tme.hwndTrack = m_hWnd;
    tme.dwHoverTime = 1;
    _TrackMouseEvent( &tme );
    
    CButton::OnMouseMove( nFlags, point );
}


void CButtonEx::OnMouseLeave()
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    CCoolStatusBar* pStatusBar = ( CCoolStatusBar* )GetParent();
    if ( NULL != pStatusBar )
    {
        pStatusBar->SetPaneText( 0, _T( "Ready" ), TRUE );
    }
    CButton::OnMouseLeave();
}


void CButtonEx::OnMouseHover( UINT nFlags, CPoint point )
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    CCoolStatusBar* pStatusBar = ( CCoolStatusBar* )GetParent();
    if ( NULL != pStatusBar )
    {
        int nID = ::GetDlgCtrlID( this->m_hWnd );
        switch ( nID )
        {
        case ID_OPEN_LOG: pStatusBar->SetPaneText( 0, _T( "Open Log Folder" ), TRUE ); break;
        case ID_RELOAD_SETTING: pStatusBar->SetPaneText( 0, _T( "Reload Setting" ), TRUE ); break;
        }
    }
    CButton::OnMouseHover( nFlags, point );
}
