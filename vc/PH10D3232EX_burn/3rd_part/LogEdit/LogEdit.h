#if !defined(AFX_LOGEDIT_H__D1EA82CD_C372_4935_9467_E6FD80C35892__INCLUDED_)
#define AFX_LOGEDIT_H__D1EA82CD_C372_4935_9467_E6FD80C35892__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogEdit.h : header file
//
/*************************************************************************
 FILE :		  LogEdit.h Version 1.0

 Author :	  Ranojay Sen  (sen_ranojay@email.com)

 Description: CLogEdit is a CEdit derived class and runs on MS-Windows

			  CLogEdit class can be used very easily for the 
			  purpose of logging data by any application or any 
			  Hardware. The automatic scrolling and updating the
			  Cursor position to the end of the line are the main 
			  features of this class. this code can be modified by 
			  profesional programmers to serve their specific needs
			  

Copyright(c) 2008
by  Ranojay Sen  (sen_ranojay@email.com)

This code may be used in compiled form in any way you desire. This
file may be redistributed unmodified by any means PROVIDING it is 
not sold for profit without the authors written consent, and 
providing that this notice and the authors name is included. If 
the source code in this file is used in any commercial application 
then a simple email to the author would be nice.

This file is provided "as is" with no expressed or implied warranty.
The author accepts no liability if it causes any damage.

*************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// CLogEdit window

class CLogEdit : public CEdit
{
// Construction
public:
	CLogEdit();

// Attributes
public:
int flag;
CFont F;
CString PromptStr;
// Operations
public:
void InsertLines(CString Line, BOOL st);
void UpdateCaretPos();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogEdit)
	//}}AFX_VIRTUAL

// Implementation
private:
	COLORREF    m_ForeColor;
	//文本颜色	
	COLORREF	m_BackColor;
	//背景色		
	CBrush	m_BkBrush;
	//背景刷	
	CFont*	p_Font;
	//字体指针	
	int	m_FontSize;
	//字体大小(1/10点)
	CString	m_FontName;
	//字体名
public:
	virtual ~CLogEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLogEdit)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void PreSubclassWindow();
	afx_msg HBRUSH CLogEdit::CtlColor(CDC* pDC, UINT nCtlColor) ;
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGEDIT_H__D1EA82CD_C372_4935_9467_E6FD80C35892__INCLUDED_)
