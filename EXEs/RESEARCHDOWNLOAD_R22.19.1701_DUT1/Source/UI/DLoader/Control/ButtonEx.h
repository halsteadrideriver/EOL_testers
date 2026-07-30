#pragma once
#include <afxwin.h>

class CButtonEx :
    public CButton
{
public:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnMouseMove( UINT nFlags, CPoint point );
    afx_msg void OnMouseLeave();
    afx_msg void OnMouseHover(UINT nFlags, CPoint point);
};

