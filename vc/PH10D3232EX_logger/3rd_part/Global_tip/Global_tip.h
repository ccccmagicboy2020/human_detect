// Global_tip.h: interface for the CGlobal_tip class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLOBAL_TIP_H__68EB7CEE_9407_4D40_A020_CC4CD56C4FA9__INCLUDED_)
#define AFX_GLOBAL_TIP_H__68EB7CEE_9407_4D40_A020_CC4CD56C4FA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGlobal_tip  
{
public:
	CGlobal_tip();
	virtual ~CGlobal_tip();
	
	void release();
	void init(CRect rc, CString text);
private:
	HWND m_hwndTT;      // handle to the ToolTip control	
    TOOLINFO m_ti;		 // struct specifying info about tool in ToolTip control
};

#endif // !defined(AFX_GLOBAL_TIP_H__68EB7CEE_9407_4D40_A020_CC4CD56C4FA9__INCLUDED_)
