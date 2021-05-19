// XColorHexagonCtrl.h  Version 1.1 - article available at www.codeproject.com
//
// Author:  Hans Dietrich
//          hdietrich@gmail.com
//
// License:
//     This software is released into the public domain.  You are free to use
//     it in any way you like, except that you may not sell this source code.
//
//     This software is provided "as is" with no expressed or implied warranty.
//     I accept no liability for any damage or loss of business that this 
//     software may cause.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef XColorHexagonCtrl_H
#define XColorHexagonCtrl_H

#include "CXDC.h"
#include "CXRect.h"
#include "CXToolTipCtrl.h"

#ifndef NO_COLOR
#define NO_COLOR ((COLORREF)-1)
#endif

extern UINT WM_XCOLORPICKER_SELCHANGE2;
extern UINT WM_XCOLORPICKER_SELENDOK2;


//=============================================================================
class CXColorHexagonCtrl
//=============================================================================
{
// Construction
public:
	CXColorHexagonCtrl();
	virtual ~CXColorHexagonCtrl();


// Attributes
public:
	HWND m_hWnd;

	enum TOOLTIP_FORMAT
	{
		XCOLOR_TOOLTIP_NONE = 0,
		XCOLOR_TOOLTIP_RGB,
		XCOLOR_TOOLTIP_HTML,
		XCOLOR_TOOLTIP_VB,
		XCOLOR_TOOLTIP_HSL
	};

	COLORREF GetBackground() { return m_crBackground; }
	void GetHSL(BYTE* h, BYTE* s, BYTE* l);
	COLORREF GetRGB();
	TOOLTIP_FORMAT GetTooltipFormat() { return m_eToolTipFormat; }

	CXColorHexagonCtrl& SetBackground(COLORREF cr) 
	{ 
		m_crBackground = cr; 
		return *this;
	}
	CXColorHexagonCtrl& SetHSL(BYTE h, BYTE s, BYTE l);
	CXColorHexagonCtrl& SetRGB(COLORREF cr);
	CXColorHexagonCtrl& SetTooltipFormat(TOOLTIP_FORMAT eFormat);


// Operations
public:

// Overrides
public:
	virtual BOOL Create(HINSTANCE hInstance, DWORD dwStyle, const RECT& rect, 
		HWND hParent, UINT nID, COLORREF crInitialColor = NO_COLOR, 
		TOOLTIP_FORMAT eToolTipFormat = XCOLOR_TOOLTIP_NONE);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnToolTipText(UINT id, NMHDR * pNMHDR, LRESULT * pResult);


// Implementation
protected:
	// struct defined for each cell
	struct COLOR_CELL
	{
		COLOR_CELL(int x, int y, COLORREF crFill)
		{
			index = 0;
			startx = x;
			starty = y;
			cr = crFill;
		}

		int index;		// index inro m_paColorCells, used for left/right arrows
		int startx;		// starting left coord
		int starty;		// starting top coord
		COLORREF cr;	// RGB color value
	};

	enum { NUMBER_COLOR_CELLS = 144 };	// includes both whites

	COLOR_CELL *	m_paColorCells[NUMBER_COLOR_CELLS];	// array of pointers to 
										// COLOR_CELL structs
	HWND			m_hParent;			// parent window
	COLORREF		m_crInitial;		// initial color selection
	COLORREF		m_crBackground;		// background color; defaults to transparent
	COLORREF		m_crLastSent;		// last color sent to parent
	CXDC			m_dcHexagon;		// device context for hexagon, used for 
										// color determination and to restore bitmap 
										// after selector has been drawn
	CXDC			m_dcValid;			// device context for client area, used to
										// determine valid cell regions
	HBITMAP			m_bmpHexagon;		// bitmap for m_dcHexagon
	HBITMAP			m_bmpValid;			// bitmap for m_dcValid
	HBITMAP			m_OldBitmap1;		// old bitmap for m_dcHexagon
	HBITMAP			m_OldBitmap2;		// old bitmap for m_dcValid
	CXRect			m_rectCtrl;			// rect of color hexagon control, passed 
										// in Create()
	COLOR_CELL *	m_pSmallWhiteCell;	// pointer to small white cell
	COLOR_CELL *	m_pLargeWhiteCell;	// pointer to lerge white cell
	COLOR_CELL *	m_pLargeBlackCell;	// pointer to large black cell
	COLOR_CELL *	m_pCurrentCell;		// pointer to current cell, or NULL
	CXToolTipCtrl *	m_pToolTip;			// pointer to tooltip control
	TOOLTIP_FORMAT	m_eToolTipFormat;	// tooltip format

	void CreateTooltip();
	void DeleteAll();
	void DrawHexagon(CXDC *pDC);
	void DrawLargeCell(CXDC *pDC, int startx, int starty, COLORREF crFill);
	void DrawLargeSelector(CXDC *pDC, COLOR_CELL *pCell, BOOL bHasFocus);
	void DrawLargeSelector(CXDC *pDC, int startx, int starty, BOOL bHasFocus);
	void DrawSelector(BOOL bSendColor);
	void DrawSelector(COLOR_CELL *pCell, BOOL bSendColor);
	void DrawSmallCell(CXDC *pDC, int startx, int starty, COLORREF crFill);
	void DrawSmallSelector(CXDC *pDC, COLOR_CELL *pCell, BOOL bHasFocus);
	void DrawSmallSelector(CXDC *pDC, int startx, int starty, BOOL bHasFocus);
	COLOR_CELL * FindCellByColor(COLORREF cr);
	COLOR_CELL * GetCellAtPoint(POINT point);
	COLOR_CELL * GetCellUnderCursor();
	BOOL GetClientCursorPos(POINT& point);
	COLORREF GetColorUnderCursor();

	void GoDown();
	void GoLeft();
	void GoRight();
	void GoUp();
	void GoHome();
	void GoEnd();

	LRESULT KeyDown(WPARAM, LPARAM);

	BOOL IsHexagonColor(COLORREF cr);
	BOOL IsLargeCell(COLOR_CELL *pCell);
	BOOL IsLeftButtonDown();
	BOOL IsPointInCell(POINT point);
};

#endif //XColorHexagonCtrl_H
