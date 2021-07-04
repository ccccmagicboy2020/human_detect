// LogEdit.cpp : implementation file
//

#include "stdafx.h"
#include "LogEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/*************************************************************************
 FILE :		  LogEdit.cpp Version 1.0

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
// CLogEdit

CLogEdit::CLogEdit()
{
	flag=1;
	PromptStr="Error::";
	p_Font	=	NULL;
}

CLogEdit::~CLogEdit()
{
	if (NULL != p_Font)
	{
		delete p_Font;
		p_Font = NULL;
	}
}

BEGIN_MESSAGE_MAP(CLogEdit, CEdit)
	//{{AFX_MSG_MAP(CLogEdit)
	ON_WM_SETFOCUS()
	ON_WM_CHAR()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogEdit message handlers

void CLogEdit::InsertLines(CString Line,BOOL st)
{
	CString wndtext;
	GetWindowText(wndtext);
	int text_length = wndtext.GetLength() ;
	if( text_length <=1) 
	{
		if(!st)
			wndtext = wndtext + Line ;
		else
			wndtext = wndtext + PromptStr + Line ;
	}
	else
	{
		if(!st)
			wndtext = wndtext +"\r\n"+  Line ;
		else
			wndtext = wndtext +"\r\n"+ PromptStr + Line ;
	}

	SetWindowText(wndtext);	
	LineScroll(GetLineCount(), 0x0);
}

void CLogEdit::UpdateCaretPos()
{
	for (int i = 0;i < GetLineCount();i++)
		SendMessage(WM_KEYDOWN, VK_DOWN , 0x0);
	for (i = 0 ;i < LineLength(GetLineCount() - 1);i++)
		SendMessage(WM_KEYDOWN, VK_RIGHT, 0x0);
}

void CLogEdit::OnSetFocus(CWnd* pOldWnd) 
{
	CEdit::OnSetFocus(pOldWnd);
	
	if(flag==1)
	{
		flag=0;
		//InsertLines("ZK_Interface",FALSE);
		//InsertLines("By KT Control China 2013",FALSE);
		//InsertLines("By cccc",FALSE);
		//InsertLines("------------------------------------------ ",FALSE);
	}
}

void CLogEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CEdit::OnChar(nChar, nRepCnt, nFlags);
	return;
}

void CLogEdit::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CEdit::OnLButtonDblClk(nFlags, point);
	return;
}

void CLogEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CEdit::OnLButtonDown(nFlags, point);
	return;
}

void CLogEdit::PreSubclassWindow() 
{
	//���ñ༭�ؼ��ɽ��յ�����ֽ���
    SetLimitText(-1);
	m_FontSize = 110;
    m_FontName = _T("Courier New");
	p_Font = new CFont;
	//�����������
	p_Font->CreatePointFont(m_FontSize, m_FontName);
	//��������
	SetFont(p_Font);
	//���ÿؼ�Ĭ������
#ifdef _DEBUG
	m_ForeColor	=	RGB(230, 219, 116);	//ɧ��
#else
	m_ForeColor	=	RGB(166, 226, 42);	//ɧ��
#endif
	//m_BackColor	=	RGB(0x00, 0x00, 0x00);	//��ɫ
	m_BackColor	=	RGB(39, 40, 34);	//ɧ��
	m_BkBrush.CreateSolidBrush(m_BackColor);
	CEdit::PreSubclassWindow();
}

HBRUSH CLogEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	//�������Ϣ
    pDC->SetTextColor(m_ForeColor);
    pDC->SetBkColor(m_BackColor);
	pDC->SetBkMode(TRANSPARENT);
    return (HBRUSH)m_BkBrush;
}
