//
//	Class:		CCylinderProgressCtrlST
//
//	Compiler:	Visual C++
//	Tested on:	Visual C++ 6.0
//
//	Version:	See GetVersionC() or GetVersionI()
//
//	Created:	27/June/2003
//	Updated:	27/June/2003
//
//	Author:		Davide Calabro'		davide_calabro@yahoo.com
//									http://www.softechsoftware.it
//
//	Disclaimer
//	----------
//	THIS SOFTWARE AND THE ACCOMPANYING FILES ARE DISTRIBUTED "AS IS" AND WITHOUT
//	ANY WARRANTIES WHETHER EXPRESSED OR IMPLIED. NO REPONSIBILITIES FOR POSSIBLE
//	DAMAGES OR EVEN FUNCTIONALITY CAN BE TAKEN. THE USER MUST ASSUME THE ENTIRE
//	RISK OF USING THIS SOFTWARE.
//
//	Terms of use
//	------------
//	THIS SOFTWARE IS FREE FOR PERSONAL USE OR FREEWARE APPLICATIONS.
//	IF YOU USE THIS SOFTWARE IN COMMERCIAL OR SHAREWARE APPLICATIONS YOU
//	ARE GENTLY ASKED TO DONATE 5$ (FIVE U.S. DOLLARS) TO THE AUTHOR:
//
//		Davide Calabro'
//		P.O. Box 65
//		21019 Somma Lombardo (VA)
//		Italy
//
#ifndef _CYLINDERPROGRESSCTRLST_H
#define _CYLINDERPROGRESSCTRLST_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Return values
#ifndef	CYLINDERPROGRESSCTRLST_OK
#define	CYLINDERPROGRESSCTRLST_OK					0
#endif
#ifndef	CYLINDERPROGRESSCTRLST_BADPARAM
#define	CYLINDERPROGRESSCTRLST_BADPARAM				1
#endif
#ifndef	CYLINDERPROGRESSCTRLST_INVALIDRESOURCE
#define	CYLINDERPROGRESSCTRLST_INVALIDRESOURCE		2
#endif

class CCylinderProgressCtrlST : public CProgressCtrl
{
public:
	CCylinderProgressCtrlST();
	virtual ~CCylinderProgressCtrlST();

	void SetRange(short shLower, short shUpper);
	void SetRange32(int nLower, int nUpper);
	int SetStep(int nStep);
	int SetPos(int nPos);
	int StepIt();

	void SetColors(Color cFullColor, Color cEmptyColor, Color cOutlineColor, Color cTextColor, Color cBkColor, BOOL bRepaint = TRUE);
	void SetFullColor(Color cColor, BOOL bRepaint = TRUE);
	void SetEmptyColor(Color cColor, BOOL bRepaint = TRUE);
	void SetOutlineColor(Color cColor, BOOL bRepaint = TRUE);
	void SetTextColor(Color cColor, BOOL bRepaint = TRUE);
	void SetBkColor(Color cColor, BOOL bRepaint = TRUE);

	void SetShowText(BOOL bShowText, BOOL bRepaint = TRUE);
	void SetShowOutline(BOOL bShowOutline, BOOL bRepaint = TRUE);

	void SetAlpha(BYTE byAlpha, BOOL bRepaint = TRUE);
	BYTE GetAlpha();

	void SetTooltipText(int nId, BOOL bActivate = TRUE);
	void SetTooltipText(LPCTSTR lpszText, BOOL bActivate = TRUE);
	void ActivateTooltip(BOOL bActivate = TRUE);

	DWORD DrawTransparent(BOOL bRepaint = FALSE);
	DWORD SetBk(CDC* pDC);

	static short GetVersionI()		{return 10;}
	static LPCTSTR GetVersionC()	{return (LPCTSTR)_T("1.0");}

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCylinderProgressCtrlST)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CCylinderProgressCtrlST)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnNcPaint();
	//}}AFX_MSG

	virtual DWORD OnDrawBackground(Graphics* pGfx, Rect* rpCtrl);
	virtual DWORD OnDrawText(Graphics* pGfx, int nPercentage, Rect* rpCtrl, Rect* rpEmpty, Rect* rpFull, BOOL bHorizontal);

	CToolTipCtrl	m_ToolTip;

	BOOL			m_bDrawTransparent;	// Draw transparent?
	BOOL			m_bShowText;		// Show text?
	BOOL			m_bOutline;			// Outline control?
	Color			m_cFullColor;
	Color			m_cEmptyColor;
	Color			m_cOutlineColor;
	Color			m_cBkColor;
	Color			m_cTextColor;

	int				m_nLower;
	int				m_nUpper;
	int				m_nRange;

	int				m_nStep;
	int				m_nPos;

	enum WallSide
	{
		Left,
		Right,
		Top,
		Bottom,
		Back,
		Front
	};

private:
	void DrawItem(CDC* pDC, Graphics* pGfx);
	void PaintBk(Graphics* pGfx);
	Color GetSideColor(Color color, WallSide side);
	void FillCylinderShadow(Graphics* pGfx, Rect bound, GraphicsPath* gp, BOOL horizontal);
	void FillCylinder(Graphics* pGfx, BOOL bHorizontal, Rect rCtrl, Color cFillColor, Color cOutlineColor, BOOL bDrawBottom, BOOL bOutline, BOOL bOutlineTop, BOOL bOutlineBottom);
	void CalcRange();
	void InitToolTip();

	CDC				m_dcBk;
	CBitmap			m_bmpBk;
	CBitmap*		m_pbmpOldBk;

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif
