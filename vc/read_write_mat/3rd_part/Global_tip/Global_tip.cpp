// Global_tip.cpp: implementation of the CGlobal_tip class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Global_tip.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGlobal_tip::CGlobal_tip()
{

}

CGlobal_tip::~CGlobal_tip()
{

}

void CGlobal_tip::release()
{
	::SendMessage(m_hwndTT, TTM_TRACKACTIVATE, false, (LPARAM)(LPTOOLINFO) &m_ti);
}

void CGlobal_tip::init( CRect rc, CString text )
{
	m_hwndTT = CreateWindowEx(WS_EX_TOPMOST,
								TOOLTIPS_CLASS,
								NULL,
								TTS_NOPREFIX | TTS_ALWAYSTIP,		
								CW_USEDEFAULT,
								CW_USEDEFAULT,
								CW_USEDEFAULT,
								CW_USEDEFAULT,
								NULL,
								NULL,
								NULL,
								NULL
								);
	
	// INITIALIZE MEMBERS OF THE TOOLINFO STRUCTURE
	m_ti.cbSize = sizeof(TOOLINFO);
	m_ti.uFlags = TTF_TRACK;
	m_ti.hwnd = NULL;
	m_ti.hinst = NULL;
	m_ti.uId = 0;
	m_ti.lpszText = (LPSTR)(LPCSTR)text;		
	// ToolTip control will cover the whole window
	m_ti.rect.left = 0;
	m_ti.rect.top = 0;
	m_ti.rect.right = 0;
	m_ti.rect.bottom = 0;
	
	// SEND AN ADDTOOL MESSAGE TO THE TOOLTIP CONTROL WINDOW
	::SendMessage(m_hwndTT, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &m_ti);
	::SendMessage(m_hwndTT, TTM_TRACKPOSITION, 0, (LPARAM)(DWORD) MAKELONG(rc.left, rc.top));
	::SendMessage(m_hwndTT, TTM_TRACKACTIVATE, true, (LPARAM)(LPTOOLINFO) &m_ti);	
}
