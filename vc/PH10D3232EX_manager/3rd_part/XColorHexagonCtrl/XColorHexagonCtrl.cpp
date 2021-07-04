// XColorHexagonCtrl.cpp  Version 1.1 - see article at CodeProject.com
//
// Author:  Hans Dietrich
//          hdietrich@gmail.com
//
// Description:
//     XColorHexagonCtrl implements CXColorHexagonCtrl, a control that mimics the
//     functionality of the "Standard Colors" color picker in MS Office
//
// History
//     Version 1.1 - 2008 April 4
//     - Bug fixes
//
//     Version 1.0 - 2008 March 15
//     - Initial public release
//
// License:
//     This software is released under the Code Project Open License (CPOL),
//     which may be found here:  http://www.codeproject.com/info/eula.aspx
//     You are free to use this software in any way you like, except that you 
//     may not sell this source code.
//
//     This software is provided "as is" with no expressed or implied warranty.
//     I accept no liability for any damage or loss of business that this 
//     software may cause.
//
///////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "windows.h"
#include "windowsx.h"
#include <stdio.h>
#include <tchar.h>
#include <crtdbg.h>
#include "XColorHexagonCtrl.h"
#include "XColorHexagonCtrlConstants.h"
#include "rgbhsl.h"

#pragma warning(disable : 4127)	// for _ASSERTE: conditional expression is constant
#pragma warning(disable : 4996)	// disable bogus deprecation warning
#define UNUSED(x) x

#ifdef _DEBUG
// extracted from mfc\src\afxmem.cpp
static void * operator new(size_t nSize, LPCSTR lpszFileName, int nLine)
{
	return ::operator new(nSize, _NORMAL_BLOCK, lpszFileName, nLine);
}
static void operator delete(void *pData, LPCSTR /*lpszFileName*/, int /*nLine*/)
{
	::operator delete(pData);
}
#define DEBUG_NEW new(THIS_FILE, __LINE__)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef __noop
#if _MSC_VER < 1300
#define __noop ((void)0)
#endif
#endif

#undef TRACE
#define TRACE __noop
#undef TRACERECT
#define TRACERECT __noop

//=============================================================================
// if you want to see the TRACE output, uncomment this line:
//#include "XTrace.h"


//=============================================================================
// Messages sent to parent window - the wParam of these messages is 
// the RGB color value
UINT WM_XCOLORPICKER_SELCHANGE2 = ::RegisterWindowMessage(_T("WM_XCOLORPICKER_SELCHANGE"));
UINT WM_XCOLORPICKER_SELENDOK2  = ::RegisterWindowMessage(_T("WM_XCOLORPICKER_SELENDOK"));


//=============================================================================
//
// CXColorHexagonCtrl()
//
// Purpose:     Construct CXColorHexagonCtrl object.
//
// Parameters:  None
//
// Returns:     None
//
// Notes:       Construction is a two-step process.  First, construct the
//              CXColorHexagonCtrl object.  Second, call 
//              CXColorHexagonCtrl::Create() to create the CXColorHexagonCtrl 
//              window.
//
CXColorHexagonCtrl::CXColorHexagonCtrl()
 :  m_hWnd(0),
	m_hParent(0),
	m_pCurrentCell(0),
	m_pSmallWhiteCell(0),
	m_pLargeWhiteCell(0),
	m_pLargeBlackCell(0),
	m_crBackground(NO_COLOR),
	m_crInitial(NO_COLOR),
	m_crLastSent(NO_COLOR),
	m_bmpHexagon(0),
	m_bmpValid(0),
	m_OldBitmap1(0),
	m_OldBitmap2(0),
	m_eToolTipFormat(XCOLOR_TOOLTIP_NONE),
	m_pToolTip(0)
{
	TRACE(_T("in CXColorHexagonCtrl::CXColorHexagonCtrl\n"));
	memset(m_paColorCells, 0, sizeof(m_paColorCells));
}

//=============================================================================
//
// ~CXColorHexagonCtrl()
//
// Purpose:     Destroy CXColorHexagonCtrl object.
//
// Parameters:  None
//
// Returns:     None
//
CXColorHexagonCtrl::~CXColorHexagonCtrl()
{
	DeleteAll();

	if (m_pToolTip)
		delete m_pToolTip;
	m_pToolTip = 0;
}

//=============================================================================
//
// DefWindowProcX()
//
// Purpose:     Initial window proc to dispatch messages to 
//              CXColorHexagonCtrl::WindowProc().  This allows us to set up
//              the 'this' pointer to CXColorHexagonCtrl instance.
//
// Parameters:  Standard windows message parameters.
//
// Returns:     LRESULT - The return value is the result of the message 
//                        processing and depends on the message.
//
static 
LRESULT __stdcall DefWindowProcX(HWND hWnd,		// handle to window
								 UINT message,	// message identifier
								 WPARAM wParam,	// first message parameter
								 LPARAM lParam)	// second message parameter
{
	switch (message)
	{
		case WM_CREATE:
		{
			// save 'this' pointer in windows extra memory - the lParam
			// is set when ::CreateWindowEx() is called
			CREATESTRUCT* pcs = (CREATESTRUCT *) lParam;
			if (!pcs)
			{
				TRACE(_T("ERROR - CREATESTRUCT lParam is zero\n"));
				_ASSERTE(pcs);
				return -1;		// abort creation
			}
			::SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pcs->lpCreateParams);
			return 0;
		}
		break;

		default:
		{
			// dispatch via saved 'this' pointer
			LONG_PTR lData = ::GetWindowLongPtr(hWnd, GWLP_USERDATA);
			if (lData)
			{
				CXColorHexagonCtrl *pCtrl = (CXColorHexagonCtrl *) lData;
				return pCtrl->WindowProc(message, wParam, lParam);
			}
			else
			{
				// probably some WM_NCxxxx message
				TRACE(_T("GWLP_USERDATA = 0 for message = 0x%04X\n"), message);
			}
		}
		break;
	}

	return ::DefWindowProc(hWnd, message, wParam, lParam);
}

//=============================================================================
//
// Create()
//
// Purpose:     This virtual function creates the color hexagon window.
//
// Parameters:  hInstance      - handle to the instance that contains 
//                               the window procedure 
//              dwStyle        - specifies the window style attributes
//              rect           - the size and position of the window
//              hParent        - the parent window HWND
//              nID            - the ID of the child window
//              crInitialColor - initial color selection
//              eToolTipFormat - tooltip format
//
// Returns:     BOOL - TRUE = window created successfully
//
BOOL CXColorHexagonCtrl::Create(HINSTANCE hInstance, 
								DWORD dwStyle, 
								const RECT& rect, 
								HWND hParent,		
								UINT nID, 
								COLORREF crInitialColor /*= NO_COLOR*/,		
								TOOLTIP_FORMAT eToolTipFormat /*= XCOLOR_TOOLTIP_NONE*/)
{
	TRACE(_T("in CXColorHexagonCtrl::Create\n"));

	m_hParent = hParent;
	_ASSERTE(m_hParent);
	if (!m_hParent)
		return FALSE;

	m_rectCtrl = rect;
	TRACERECT(m_rectCtrl);

	m_crInitial = crInitialColor;

	TCHAR * pszClassName = _T("XColorHexagonCtrl");

	WNDCLASS wc =
	{
		CS_DBLCLKS,									// class style - want WM_LBUTTONDBLCLK messages
		DefWindowProcX,								// window proc
		0,											// class extra bytes
		0,											// window extra bytes
		hInstance,									// instance handle
		0,											// icon
		::LoadCursor(0, IDC_ARROW),					// cursor
		0,											// background brush
		0,											// menu name
		pszClassName								// class name
	};

	if (!::RegisterClass(&wc))
	{
		DWORD dwLastError = GetLastError();
		if (dwLastError != ERROR_CLASS_ALREADY_EXISTS)
		{
			TRACE(_T("ERROR - RegisterClass failed, GetLastError() returned %u\n"), dwLastError);
			_ASSERTE(FALSE);
			return FALSE;
		}
	}

	// we pass 'this' pointer as lpParam, so DefWindowProcX will see it 
	// in WM_CREATE message
	m_hWnd = ::CreateWindowEx(0, pszClassName, _T(""), dwStyle, 
		m_rectCtrl.left, m_rectCtrl.top, m_rectCtrl.Width(), m_rectCtrl.Height(),
		hParent, (HMENU)nID, hInstance, this);

	if (m_hWnd == 0)
	{
#ifdef _DEBUG
		DWORD dwLastError = GetLastError();
		UNUSED(dwLastError);
		TRACE(_T("ERROR - CreateWindowEx failed, GetLastError() returned %u\n"), dwLastError);
		_ASSERTE(m_hWnd);
#endif
		return FALSE;
	}

	m_crLastSent = crInitialColor;

	m_eToolTipFormat = eToolTipFormat;

	CreateTooltip();

	return m_hWnd != 0;
}

//=============================================================================
//
// CreateTooltip()
//
// Purpose:     This function creates the CXColorHexagonCtrl tooltip.
//
// Parameters:  None
//
// Returns:     None
//
void CXColorHexagonCtrl::CreateTooltip()
{
	TRACE(_T("in CXColorHexagonCtrl::CreateTooltip\n"));

	if (m_hWnd && (m_eToolTipFormat != XCOLOR_TOOLTIP_NONE))
	{
		_ASSERTE(m_pToolTip == NULL);

		m_pToolTip = new CXToolTipCtrl;
		_ASSERTE(m_pToolTip);
		if (m_pToolTip)
		{
			if (!m_pToolTip->Create(m_hWnd, TTS_ALWAYSTIP))
			{
				TRACE(_T("ERROR - failed to create tooltip control\n"));
				_ASSERTE(FALSE);
				return;
			}
			m_pToolTip->SetMaxTipWidth(40);

			// include entire rect
			CXRect rectTool(0, 0, m_rectCtrl.right-m_rectCtrl.left, 
				m_rectCtrl.bottom-m_rectCtrl.top);

			if (!m_pToolTip->AddTool(m_hWnd, LPSTR_TEXTCALLBACK, &rectTool, 10000))
			{
				TRACE(_T("ERROR - failed to add tool\n"));
				_ASSERTE(FALSE);
			}
		}
	}
}

//=============================================================================
//
// GoLeft()
//
// Purpose:     This function processes left arrow keystrokes.
//
// Parameters:  None
//
// Returns:     None
//
void CXColorHexagonCtrl::GoLeft()
{
	COLOR_CELL *pCell = NULL;
	if (m_pCurrentCell == NULL)
	{
		pCell = (COLOR_CELL *) m_paColorCells[0];
	}
	else
	{
		int index = m_pCurrentCell->index;
		if (index > 0)
		{
			pCell = (COLOR_CELL *) m_paColorCells[index-1];
		}
	}
	if (pCell)
	{
		DrawSelector(pCell, TRUE);
		m_pCurrentCell = pCell;
	}
}

//=============================================================================
//
// GoRight()
//
// Purpose:     This function processes right arrow keystrokes.
//
// Parameters:  None
//
// Returns:     None
//
void CXColorHexagonCtrl::GoRight()
{
	COLOR_CELL *pCell = NULL;
	if (m_pCurrentCell == NULL)
	{
		pCell = (COLOR_CELL *) m_paColorCells[0];
	}
	else
	{
		int index = m_pCurrentCell->index;
		if (index < (NUMBER_COLOR_CELLS-1))
		{
			pCell = (COLOR_CELL *) m_paColorCells[index+1];
		}
	}
	if (pCell)
	{
		DrawSelector(pCell, TRUE);
		m_pCurrentCell = pCell;
	}
}

//=============================================================================
//
// GoUp()
//
// Purpose:     This function processes up arrow keystrokes.
//
// Parameters:  None
//
// Returns:     None
//
void CXColorHexagonCtrl::GoUp()
{
	// Up is always up and to the left;  if this is the first
	// row, up just goes left.  There is no wrapping.

	if (IsLargeCell(m_pCurrentCell))
	{
		GoLeft();
	}
	else
	{
		COLOR_CELL *pCell = NULL;
		if (m_pCurrentCell == NULL)
		{
			pCell = (COLOR_CELL *) m_paColorCells[0];
		}
		else
		{
			int x = m_pCurrentCell->startx;		// this is mid-point of cell 
												// in previous row
			int y = m_pCurrentCell->starty - 2;

			// get color of new cell
			COLORREF cr = m_dcHexagon.GetPixel(x, y);

			// special case for white, because there are two whites
			if (cr == RGB(255,255,255))
			{
				if (y >= m_pLargeWhiteCell->starty)
					pCell = m_pLargeWhiteCell;
				else
					pCell = m_pSmallWhiteCell;
			}
			else	// every other color is unique in the ptr array
			{
				pCell = FindCellByColor(cr);
			}
		}
		if (pCell)
		{
			DrawSelector(pCell, TRUE);
			m_pCurrentCell = pCell;
		}
		else
		{
			GoLeft();
		}
	}
}

//=============================================================================
//
// GoDown()
//
// Purpose:     This function processes down arrow keystrokes.
//
// Parameters:  None
//
// Returns:     None
//
void CXColorHexagonCtrl::GoDown()
{
	// Down is always down and to the right;  if this is the last
	// row, down just goes right.  There is no wrapping.

	if (IsLargeCell(m_pCurrentCell))
	{
		GoRight();
	}
	else
	{
		COLOR_CELL *pCell = NULL;
		if (m_pCurrentCell == NULL)
		{
			pCell = (COLOR_CELL *) m_paColorCells[0];
		}
		else
		{
			int x = m_pCurrentCell->startx;
			int y = m_pCurrentCell->starty;
			x += SMALL_CELL_WIDTH;
			y += SMALL_CELL_HEIGHT + 1;

			// get color of new cell
			COLORREF cr = m_dcHexagon.GetPixel(x, y);

			// special case for white, because there are two whites
			if (cr == RGB(255,255,255))
			{
				if (y >= m_pLargeWhiteCell->starty)
					pCell = m_pLargeWhiteCell;
				else
					pCell = m_pSmallWhiteCell;
			}
			else	// every other color is unique in the ptr array
			{
				pCell = FindCellByColor(cr);
			}
		}
		if (pCell)
		{
			DrawSelector(pCell, TRUE);
			m_pCurrentCell = pCell;
		}
		else
		{
			GoRight();
		}
	}
}

//=============================================================================
//
// GoHome()
//
// Purpose:     This function processes Home keystrokes.
//
// Parameters:  None
//
// Returns:     None
//
void CXColorHexagonCtrl::GoHome()
{
	COLOR_CELL *pCell = (COLOR_CELL *) m_paColorCells[0];

	if (pCell)
	{
		DrawSelector(pCell, TRUE);
		m_pCurrentCell = pCell;
	}
}

//=============================================================================
//
// GoEnd()
//
// Purpose:     This function processes End keystrokes.
//
// Parameters:  None
//
// Returns:     None
//
void CXColorHexagonCtrl::GoEnd()
{
	int index = NUMBER_COLOR_CELLS-1;
	COLOR_CELL *pCell = (COLOR_CELL *) m_paColorCells[index];

	if (pCell)
	{
		DrawSelector(pCell, TRUE);
		m_pCurrentCell = pCell;
	}
}

//=============================================================================
//
// IsLeftButtonDown()
//
// Purpose:     This function returns state of left mouse button.
//
// Parameters:  None
//
// Returns:     BOOL - TRUE = left mouse button is down
//
BOOL CXColorHexagonCtrl::IsLeftButtonDown()
{
	BOOL rc = FALSE;

	SHORT state = 0;
	if (GetSystemMetrics(SM_SWAPBUTTON))		// check if buttons have been swapped
		state = GetAsyncKeyState(VK_RBUTTON);	// buttons swapped, get right button state
	else
		state = GetAsyncKeyState(VK_LBUTTON);

	// if the most significant bit is set, the button is down
	if (state < 0)
		rc = TRUE;

	return rc;
}

//=============================================================================
//
// IsLargeCell()
//
// Purpose:     This function returns cell size.
//
// Parameters:  pCell - pointer to COLOR_CELL struct
//
// Returns:     BOOL - TRUE = cell is large
//
BOOL CXColorHexagonCtrl::IsLargeCell(COLOR_CELL *pCell)
{
	BOOL rc = FALSE;

	if ((pCell == m_pLargeWhiteCell) ||
		(pCell == m_pLargeBlackCell))
	{
		rc = TRUE;
	}

	return rc;
}

//=============================================================================
//
// DrawSmallCell()
//
// Purpose:     This function draws a small color cell.
//
// Parameters:  pDC    - pointer to device context to use for drawing
//              startx - x coord of left edge
//              starty - y coord of top edge 
//              crFill - fill color
//
// Returns:     None
//
// Notes:
//
//  1        ###
//  2      #######
//  3    ###########
//  4  ##############
//  5  ##############
//  6  ##############
//  7  ##############
//  8  ##############
//  9  ##############
// 10  ##############
// 11  ##############
// 12  ##############
// 13    ###########
// 14      #######
// 15        ###
//              11111
//     12345678901234
//
//     1.  Cell is not symmetrical left-to-right - right side is shorter
//         by one pixel.
//     2.  Each row of cells overlaps previous row by 3 pixels, so cells
//         will form interlocking pattern.

void CXColorHexagonCtrl::DrawSmallCell(CXDC *pDC, 
									   int startx, 
									   int starty, 
									   COLORREF crFill)
{
	// For each byte in this array:
	//    0 = skip
	//    1 = set pixel to fill color
	static BYTE pixels[SMALL_CELL_HEIGHT][SMALL_CELL_WIDTH] = 
	{
		0,0,0,0,0,0,1,1,1,0,0,0,0,0,	// 1
		0,0,0,0,1,1,1,1,1,1,1,0,0,0,	// 2
		0,0,1,1,1,1,1,1,1,1,1,1,1,0,	// 3
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,	// 4
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,	// 5
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,	// 6
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,	// 7
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,	// 8
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,	// 9
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,	// 10
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,	// 11
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,	// 12
		0,0,1,1,1,1,1,1,1,1,1,1,1,0,	// 13
		0,0,0,0,1,1,1,1,1,1,1,0,0,0,	// 14
		0,0,0,0,0,0,1,1,1,0,0,0,0,0,	// 15
	};

	_ASSERTE(pDC);
	if (!pDC)
		return;

	for (int row = 0; row < SMALL_CELL_HEIGHT; row++)
	{
		for (int col = 0; col < SMALL_CELL_WIDTH; col++)
		{
			if (pixels[row][col])
				pDC->SetPixelV(startx+col, starty+row, crFill);
		}
	}
}

//=============================================================================
//
// DrawLargeCell()
//
// Purpose:     This function draws a large color cell.
//
// Parameters:  pDC    - pointer to device context to use for drawing
//              startx - x coord of left edge
//              starty - y coord of top edge 
//              crFill - fill color
//
// Returns:     None
//
void CXColorHexagonCtrl::DrawLargeCell(CXDC *pDC, 
									   int startx, 
									   int starty, 
									   COLORREF crFill)
{
	// For each byte in this array:
	//    0 = skip
	//    1 = set pixel to fill color
	static BYTE pixels[LARGE_CELL_HEIGHT][LARGE_CELL_WIDTH] = 
	{
		0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,	// 1
		0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,	// 2
		0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,	// 3
		0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,	// 4
		0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,	// 5
		0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,	// 6
		0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,	// 7
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,	// 8
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,	// 9
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,	// 10
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,	// 11
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,	// 12
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,	// 13
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,	// 14
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,	// 15
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,	// 16
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,	// 17
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,	// 18
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,	// 19
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,	// 20
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,	// 21
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,	// 22
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,	// 23
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,	// 24
		0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,	// 25
		0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,	// 26
		0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,	// 27
		0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,	// 28
		0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,	// 29
		0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,	// 30
		0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0		// 31
	};

	_ASSERTE(pDC);
	if (!pDC)
		return;

	for (int row = 0; row < LARGE_CELL_HEIGHT; row++)
	{
		for (int col = 0; col < LARGE_CELL_WIDTH; col++)
		{
			if (pixels[row][col])
				pDC->SetPixelV(startx+col, starty+row, crFill);
		}
		//pDC->SetPixelV(startx+col-1, starty+row, RGB(255,0,0));
	}
}

//=============================================================================
//
// DrawSmallSelector()
//
// Purpose:     This function draws a small selector.
//
// Parameters:  pDC       - pointer to device context to use for drawing
//              pCell     - pointer to COLOR_CELL struct
//              bHasFocus - TRUE = cell has focus; this determines how the 
//                          selector is drawn
//
// Returns:     None
//
void CXColorHexagonCtrl::DrawSmallSelector(CXDC *pDC, 
										   COLOR_CELL *pCell, 
										   BOOL bHasFocus)
{
	_ASSERTE(pCell);
	if (pCell)
		DrawSmallSelector(pDC, pCell->startx, pCell->starty, bHasFocus);
}

//=============================================================================
//
// DrawSmallSelector()
//
// Purpose:     This function draws a small selector.
//
// Parameters:  pDC       - pointer to device context to use for drawing
//              startx    - x coord of left edge
//              starty    - y coord of top edge 
//              bHasFocus - TRUE = cell has focus; this determines how the 
//                          selector is drawn
//
// Returns:     None
//
void CXColorHexagonCtrl::DrawSmallSelector(CXDC *pDC, 
										   int startx, 
										   int starty, 
										   BOOL bHasFocus)
{
	BOOL bRelease = FALSE;

	if (pDC == NULL)
	{
		pDC = new CXDC(m_hWnd);
		bRelease = TRUE;
	}

	// For each byte in this array:
	// 0 = skip
	// 1 = COLOR_WINDOWTEXT (COLOR_BTNSHADOW if bHasFocus == FALSE)
	// 2 = COLOR_WINDOW
	// 3 = COLOR_BTNSHADOW
	static BYTE pixels[SMALL_SELECTOR_HEIGHT][SMALL_SELECTOR_WIDTH] = 
	{
		0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,	// 1
		0,0,0,0,0,1,1,2,2,2,2,2,1,1,0,0,0,0,0,	// 2
		0,0,0,1,1,2,2,2,2,2,2,2,2,2,1,1,0,0,0,	// 3
		0,1,1,2,2,2,2,2,3,3,3,2,2,2,2,2,1,1,0,	// 4
		1,2,2,2,2,2,3,3,0,0,0,3,3,2,2,2,2,2,1,	// 5
		1,2,2,2,3,3,0,0,0,0,0,0,0,3,3,2,2,2,1,	// 6
		1,2,2,3,0,0,0,0,0,0,0,0,0,0,0,3,2,2,1,	// 7
		1,2,2,3,0,0,0,0,0,0,0,0,0,0,0,3,2,2,1,	// 8
		1,2,2,3,0,0,0,0,0,0,0,0,0,0,0,3,2,2,1,	// 9
		1,2,2,3,0,0,0,0,0,0,0,0,0,0,0,3,2,2,1,	// 10
		1,2,2,3,0,0,0,0,0,0,0,0,0,0,0,3,2,2,1,	// 11
		1,2,2,3,0,0,0,0,0,0,0,0,0,0,0,3,2,2,1,	// 12
		1,2,2,3,0,0,0,0,0,0,0,0,0,0,0,3,2,2,1,	// 13
		1,2,2,2,3,3,0,0,0,0,0,0,0,3,3,2,2,2,1,	// 14
		1,2,2,2,2,2,3,3,0,0,0,3,3,2,2,2,2,2,1,	// 15
		0,1,1,2,2,2,2,2,3,3,3,2,2,2,2,2,1,1,0,	// 16
		0,0,0,1,1,2,2,2,2,2,2,2,2,2,1,1,0,0,0,	// 17
		0,0,0,0,0,1,1,2,2,2,2,2,1,1,0,0,0,0,0,	// 18
		0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0	// 19
	};

	COLORREF crWindow = GetSysColor(COLOR_WINDOW);
	COLORREF crWindowText = GetSysColor(COLOR_WINDOWTEXT);
	COLORREF crShadow = GetSysColor(COLOR_BTNSHADOW);
	if (!bHasFocus)
		crWindowText = crShadow;

	for (int row = 0; row < SMALL_SELECTOR_HEIGHT; row++)
	{
		for (int col = 0; col < SMALL_SELECTOR_WIDTH; col++)
		{
			COLORREF crFill = crWindowText;
			if (pixels[row][col] == 1)
				crFill = crWindowText;
			else if  (pixels[row][col] == 2)
				crFill = crWindow;
			else if  (pixels[row][col] == 3)
				crFill = crShadow;
			else
				continue;
			pDC->SetPixelV(startx+col-2, starty+row-2, crFill);
		}
	}
	if (bRelease)
		delete pDC;
}

//=============================================================================
//
// DrawLargeSelector()
//
// Purpose:     This function draws a large selector.
//
// Parameters:  pDC       - pointer to device context to use for drawing
//              pCell     - pointer to COLOR_CELL struct
//              bHasFocus - TRUE = cell has focus; this determines how the 
//                          selector is drawn
//
// Returns:     None
//
void CXColorHexagonCtrl::DrawLargeSelector(CXDC *pDC, 
										   COLOR_CELL *pCell, 
										   BOOL bHasFocus)
{
	_ASSERTE(pCell);
	if (pCell)
		DrawLargeSelector(pDC, pCell->startx, pCell->starty, bHasFocus);
}

//=============================================================================
//
// DrawLargeSelector()
//
// Purpose:     This function draws a large selector.
//
// Parameters:  pDC       - pointer to device context to use for drawing
//              startx    - x coord of left edge
//              starty    - y coord of top edge 
//              bHasFocus - TRUE = cell has focus; this determines how the 
//                          selector is drawn
//
// Returns:     None
//
void CXColorHexagonCtrl::DrawLargeSelector(CXDC *pDC, 
										   int startx, 
										   int starty, 
										   BOOL bHasFocus)
{
	BOOL bRelease = FALSE;

	if (pDC == NULL)
	{
		pDC = new CXDC(m_hWnd);
		bRelease = TRUE;
	}

	// For each byte in this array:
	// 0 = skip
	// 1 = COLOR_WINDOWTEXT (COLOR_BTNSHADOW if bHasFocus == FALSE)
	// 2 = COLOR_WINDOW
	// 3 = COLOR_BTNSHADOW
	static BYTE pixels[LARGE_SELECTOR_HEIGHT][LARGE_SELECTOR_WIDTH] = 
	{
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	// 1
		0,0,0,0,0,0,0,0,0,0,0,0,1,1,2,2,2,2,2,1,1,0,0,0,0,0,0,0,0,0,0,0,0,	// 2
		0,0,0,0,0,0,0,0,0,0,1,1,2,2,2,2,2,2,2,2,2,1,1,0,0,0,0,0,0,0,0,0,0,	// 3
		0,0,0,0,0,0,0,0,1,1,2,2,2,2,2,3,3,3,2,2,2,2,2,1,1,0,0,0,0,0,0,0,0,	// 4
		0,0,0,0,0,0,1,1,2,2,2,2,2,3,3,0,0,0,3,3,2,2,2,2,2,1,1,0,0,0,0,0,0,	// 5
		0,0,0,0,1,1,2,2,2,2,2,3,3,0,0,0,0,0,0,0,3,3,2,2,2,2,2,1,1,0,0,0,0,	// 6
		0,0,1,1,2,2,2,2,2,3,3,0,0,0,0,0,0,0,0,0,0,0,3,3,2,2,2,2,2,1,1,0,0,	// 7
		0,1,2,2,2,2,2,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,2,2,2,2,2,1,0,	// 8
		1,2,2,2,2,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,2,2,2,2,1,	// 9
		1,2,2,2,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,2,2,2,1,	// 10
		1,2,2,2,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,2,2,2,1,	// 11
		1,2,2,2,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,2,2,2,1,	// 12
		1,2,2,2,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,2,2,2,1,	// 13
		1,2,2,2,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,2,2,2,1,	// 14
		1,2,2,2,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,2,2,2,1,	// 15
		1,2,2,2,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,2,2,2,1,	// 16
		1,2,2,2,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,2,2,2,1,	// 17
		1,2,2,2,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,2,2,2,1,	// 18
		1,2,2,2,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,2,2,2,1,	// 19
		1,2,2,2,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,2,2,2,1,	// 20
		1,2,2,2,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,2,2,2,1,	// 21
		1,2,2,2,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,2,2,2,1,	// 22
		1,2,2,2,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,2,2,2,1,	// 23
		1,2,2,2,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,2,2,2,1,	// 24
		1,2,2,2,2,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,2,2,2,2,2,	// 25
		0,1,2,2,2,2,2,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,2,2,2,2,2,1,0,	// 26
		0,0,1,1,2,2,2,2,2,3,3,0,0,0,0,0,0,0,0,0,0,0,3,3,2,2,2,2,2,1,1,0,0,	// 27
		0,0,0,0,1,1,2,2,2,2,2,3,3,0,0,0,0,0,0,0,3,3,2,2,2,2,2,1,1,0,0,0,0,	// 28
		0,0,0,0,0,0,1,1,2,2,2,2,2,3,3,0,0,0,3,3,2,2,2,2,2,1,1,0,0,0,0,0,0,	// 29
		0,0,0,0,0,0,0,0,1,1,2,2,2,2,2,3,3,3,2,2,2,2,2,1,1,0,0,0,0,0,0,0,0,	// 30
		0,0,0,0,0,0,0,0,0,0,1,1,2,2,2,2,2,2,2,2,2,1,1,0,0,0,0,0,0,0,0,0,0,	// 31
		0,0,0,0,0,0,0,0,0,0,0,0,1,1,2,2,2,2,2,1,1,0,0,0,0,0,0,0,0,0,0,0,0,	// 32
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0	// 33
	};

	COLORREF crWindow = GetSysColor(COLOR_WINDOW);
	COLORREF crWindowText = GetSysColor(COLOR_WINDOWTEXT);
	COLORREF crShadow = GetSysColor(COLOR_BTNSHADOW);
	if (!bHasFocus)
		crWindowText = crShadow;

	for (int row = 0; row < LARGE_SELECTOR_HEIGHT; row++)
	{
		for (int col = 0; col < LARGE_SELECTOR_WIDTH; col++)
		{
			COLORREF crFill = crWindowText;
			if (pixels[row][col] == 1)
				crFill = crWindowText;
			else if  (pixels[row][col] == 2)
				crFill = crWindow;
			else if  (pixels[row][col] == 3)
				crFill = crShadow;
			else
				continue;
			pDC->SetPixelV(startx+col-2, starty+row-2, crFill);
		}
	}

	if (bRelease)
		delete pDC;
}

//=============================================================================
//
// GetClientCursorPos()
//
// Purpose:     This function retrieves the cursor location relative to the
//              control's rect.
//
// Parameters:  point - reference to the variable that receives the location
//
// Returns:     BOOL - TRUE = location is valid
//
BOOL CXColorHexagonCtrl::GetClientCursorPos(POINT& point)
{
	BOOL rc = FALSE;

	if (::GetCursorPos(&point))		// check if screensaver kicked in
	{
		::ScreenToClient(m_hWnd, &point);
		rc = TRUE;
	}

	return rc;
}

//=============================================================================
//
// GetCellUnderCursor()
//
// Purpose:     This function retrieves the cell under the cursor.
//
// Parameters:  None
//
// Returns:     COLOR_CELL * - pointer to COLOR_CELL struct, or NULL
//
CXColorHexagonCtrl::COLOR_CELL * CXColorHexagonCtrl::GetCellUnderCursor()
{
	COLOR_CELL *pCell = NULL;

	POINT ptClient;
	if (GetClientCursorPos(ptClient))
	{
		pCell = GetCellAtPoint(ptClient);
	}

	return pCell;
}

//=============================================================================
//
// IsPointInCell()
//
// Purpose:     This function checks if point is in valid cell.
//
// Parameters:  point - position to check
//
// Returns:     BOOL - TRUE = point is in valid cell
//
BOOL CXColorHexagonCtrl::IsPointInCell(POINT point)
{
	COLORREF cr = RGB(0,0,0);

	if (m_OldBitmap2)
	{
		// check if point is in a valid cell
		cr = m_dcValid.GetPixel(point.x, point.y);

		// color in m_dcValid is black if not valid cell
	}

	return cr != RGB(0,0,0);
}

//=============================================================================
//
// GetCellAtPoint()
//
// Purpose:     This function retrieves the cell at a point.
//
// Parameters:  point - position
//
// Returns:     COLOR_CELL * - pointer to COLOR_CELL struct, or NULL
//
CXColorHexagonCtrl::COLOR_CELL * CXColorHexagonCtrl::GetCellAtPoint(POINT point)
{
	COLOR_CELL *pCell = NULL;

	if (m_OldBitmap1 && m_OldBitmap2)
	{
		// check if point is in a valid cell
		if (IsPointInCell(point))
		{
			// get color of pixel under cursor
			COLORREF cr = m_dcHexagon.GetPixel(point.x, point.y);

			// special case for white, because there are two whites
			if (cr == RGB(255,255,255))
			{
				if (point.y >= m_pLargeWhiteCell->starty)
					pCell = m_pLargeWhiteCell;
				else
					pCell = m_pSmallWhiteCell;
			}
			else	// every other color is unique in the ptr array
			{
				pCell = FindCellByColor(cr);
			}
		}
	}

	return pCell;
}

//=============================================================================
//
// GetColorUnderCursor()
//
// Purpose:     This function retrieves the color under the cursor.
//
// Parameters:  None
//
// Returns:     COLORREF - RGB color value, or NO_COLOR
//
COLORREF CXColorHexagonCtrl::GetColorUnderCursor()
{
	COLORREF cr = NO_COLOR;

	POINT ptClient;
	if (GetClientCursorPos(ptClient))
	{
		if (m_OldBitmap1 && m_OldBitmap2)
		{
			// check if point is in a valid cell
			if (IsPointInCell(ptClient))
			{
				// get color of pixel under cursor
				cr = m_dcHexagon.GetPixel(ptClient.x, ptClient.y);
			}
		}
	}

	return cr;
}

//=============================================================================
//
// DrawSelector()
//
// Purpose:     This function draws a selector around the cell pointed to
//              by pCell.
//
// Parameters:  pCell      - pointer ro COLOR_CELL struct
//              bSendColor - TRUE = send WM_XCOLORPICKER_SELCHANGE to parent
//
// Returns:     None
//
void CXColorHexagonCtrl::DrawSelector(COLOR_CELL *pCell, BOOL bSendColor)
{
	HWND hWndFocus = GetFocus();
	BOOL bHasFocus = hWndFocus == m_hWnd;

	CXDC dc(m_hWnd);

	// restore bitmap - get rid of previous selection
	if (m_OldBitmap1)
		dc.BitBlt(0, 0, m_rectCtrl.Width(), m_rectCtrl.Height(),
			&m_dcHexagon, 0, 0, SRCCOPY);

	m_pCurrentCell = pCell;

	if (pCell)
	{
		if (pCell == m_pLargeBlackCell)
		{
			DrawLargeSelector(&dc, m_pLargeBlackCell, bHasFocus);
		}
		else if (pCell == m_pLargeWhiteCell)
		{
			// user clicked on large white cell, only select it
			DrawLargeSelector(&dc, m_pLargeWhiteCell, bHasFocus);
		}
		else if (pCell == m_pSmallWhiteCell)
		{
			// user clicked on small white cell, select both large and small
			DrawLargeSelector(&dc, m_pLargeWhiteCell, bHasFocus);
			DrawSmallSelector(&dc, m_pSmallWhiteCell, bHasFocus);
		}
		else
		{
			DrawSmallSelector(&dc, pCell->startx, pCell->starty, bHasFocus);
		}

		// let parent know there is a new selected cell
		if (bSendColor && m_pCurrentCell && 
			IsWindow(m_hParent) &&
			(m_pCurrentCell->cr != m_crLastSent))
		{
			::SendMessage(m_hParent, WM_XCOLORPICKER_SELCHANGE2, 
				m_pCurrentCell->cr, ::GetDlgCtrlID(m_hWnd));
			m_crLastSent = m_pCurrentCell->cr;
		}
	}
}

//=============================================================================
//
// DrawSelector()
//
// Purpose:     This function drawa a selector around the cell under the cursor.
//
// Parameters:  bSendColor - TRUE = send WM_XCOLORPICKER_SELCHANGE to parent
//
// Returns:     None
//
void CXColorHexagonCtrl::DrawSelector(BOOL bSendColor)
{
	COLOR_CELL *pCell = NULL;

	POINT ptClient;
	if (GetClientCursorPos(ptClient))
	{
		if (IsPointInCell(ptClient))
		{
			// get color of pixel under mouse
			COLORREF cr = GetColorUnderCursor();

			if (cr == RGB(0,0,0))
			{
				pCell = m_pLargeBlackCell;
			}
			else if (cr == RGB(255,255,255))
			{
				if (ptClient.y >= m_pLargeWhiteCell->starty)
				{
					pCell = m_pLargeWhiteCell;
				}
				else
				{
					pCell = m_pSmallWhiteCell;
				}
			}
			else
			{
				pCell = FindCellByColor(cr);
			}
		}
	}

	DrawSelector(pCell, bSendColor);
}

//=============================================================================
//
// FindCellByColor()
//
// Purpose:     This function returns a pointer to the COLOR_CELL struct that
//              has the specified color.
//
// Parameters:  cr - RGB color value
//
// Returns:     COLOR_CELL * - pointer to cell that has cr color, or NULL
//
// Notes:       If cr is white, this function always returns pointer to small
//              white cell, since it is before large white cell in array.
//
CXColorHexagonCtrl::COLOR_CELL * CXColorHexagonCtrl::FindCellByColor(COLORREF cr)
{
	COLOR_CELL * pCellFound = 0;

	int n = NUMBER_COLOR_CELLS;
	for (int i = 0; i < n; i++)
	{
		COLOR_CELL *pCell = (COLOR_CELL *) m_paColorCells[i];
		if (pCell && (pCell->cr == cr))
		{
			pCellFound = pCell;
			break;
		}
	}

	return pCellFound;
}

//=============================================================================
//
// IsHexagonColor()
//
// Purpose:     This function determines if cr is a valid hexagon color.
//
// Parameters:  cr - RGB color value
//
// Returns:     BOOL - TRUE if cr color is one of the hexagon colors
//
BOOL CXColorHexagonCtrl::IsHexagonColor(COLORREF cr)
{
	return (FindCellByColor(cr) != 0);
}

//=============================================================================
//
// DeleteAll()
//
// Purpose:     This function deletes all the hexagon color cells, and the hexagon
//              bitmap and dc.
//
// Parameters:  None
//
// Returns:     None
//
void CXColorHexagonCtrl::DeleteAll()
{
	if (m_OldBitmap1)
		m_dcHexagon.SelectObject(m_OldBitmap1);
	m_OldBitmap1 = 0;

	if (m_bmpHexagon)
		::DeleteObject(m_bmpHexagon);

	if (m_OldBitmap2)
		m_dcValid.SelectObject(m_OldBitmap2);
	m_OldBitmap2 = 0;

	if (m_bmpValid)
		::DeleteObject(m_bmpValid);

	for (int i = 0; i < NUMBER_COLOR_CELLS; i++)
	{
		COLOR_CELL *pCell = (COLOR_CELL *) m_paColorCells[i];
		if (pCell)
			delete pCell;
		m_paColorCells[i] = 0;
	}
}

//=============================================================================
//
// DrawHexagon()
//
// Purpose:     This function draws all the color cells, sets up the color cell 
//              pointer array, and initializes the hexagon bitmap and memory dc.
//
// Parameters:  pDC - pointer to device context used for drawing
//
// Returns:     None
//
void CXColorHexagonCtrl::DrawHexagon(CXDC *pDC)
{
	_ASSERTE(pDC);
	if (!pDC)
		return;

	TRACERECT(m_rectCtrl);

	if (m_OldBitmap1 == 0)
	{
		// hexagon bitmap hasn't been drawn yet

		if (!m_dcHexagon.CreateCompatibleDC(pDC))
		{
			TRACE(_T("ERROR - CreateCompatibleDC failed\n"));
			_ASSERTE(FALSE);
			return;
		}
		m_bmpHexagon = pDC->CreateCompatibleBitmap(m_rectCtrl.Width(), m_rectCtrl.Height());
		m_OldBitmap1 = (HBITMAP) m_dcHexagon.SelectObject(m_bmpHexagon);
		if (m_crBackground == NO_COLOR)
		{
			// no background color specified, just copy current background
			m_dcHexagon.BitBlt(0, 0, m_rectCtrl.Width(), m_rectCtrl.Height(),
				pDC, 0, 0, SRCCOPY);
		}
		else
		{
			m_dcHexagon.FillSolidRect(0, 0, m_rectCtrl.Width(), m_rectCtrl.Height(),
				m_crBackground);
		}

		// create validating dc - this is used to determine if cursor is over 
		// a valid cell.  Pixels in this dc:  black = not a valid cell, 
		// white = valid
		if (!m_dcValid.CreateCompatibleDC(pDC))
		{
			TRACE(_T("ERROR - CreateCompatibleDC failed\n"));
			_ASSERTE(FALSE);
			return;
		}
		m_bmpValid = pDC->CreateCompatibleBitmap(m_rectCtrl.Width(), m_rectCtrl.Height());
		m_OldBitmap2 = (HBITMAP) m_dcValid.SelectObject(m_bmpValid);
		// initially fill with invalid color
		m_dcValid.FillSolidRect(0, 0, m_rectCtrl.Width(), m_rectCtrl.Height(),
			RGB(0,0,0));

		int index = 0;
		int array_index = 0;
		int x = 0;
		int y = XY_OFFSET;
		int row = 0;
		int col = 0;

		// loop to draw rows
		for (row = 0; nRowSize[row] != 0; row++)
		{
			x = XY_OFFSET + nRowOffset[row];

			// loop to draw columns
			for (col = 0; col < nRowSize[row]; col++)
			{
				COLORREF crCell = g_crCellColors[index++];

				DrawSmallCell(&m_dcHexagon, x, y, crCell);
				DrawSmallCell(&m_dcValid, x, y, RGB(255,255,255));

				COLOR_CELL * pCell = new COLOR_CELL(x, y, crCell);
				m_paColorCells[array_index] = pCell;
				pCell->index = array_index++;

				_ASSERTE(array_index < NUMBER_COLOR_CELLS);

				// save small white cell pointer
				if (crCell == RGB(255,255,255))
					m_pSmallWhiteCell = pCell;

				x += SMALL_CELL_WIDTH;
			}
			y += SMALL_CELL_HEIGHT - 3;		// interlock with previous row
		}

		index = 0;
		y += SMALL_CELL_HEIGHT + 1;			// ystart of gray scale
		int large_cell_y = y - 4;			// save for later
		int black_cell_x = x + SMALL_CELL_WIDTH;

		// insert large white cell into array here, so left and right arrow keys
		// will work correctly
		DrawLargeCell(&m_dcHexagon, XY_OFFSET, large_cell_y, RGB(255,255,255));
		DrawLargeCell(&m_dcValid, XY_OFFSET, large_cell_y, RGB(255,255,255));
		m_pLargeWhiteCell = new COLOR_CELL(XY_OFFSET, large_cell_y, RGB(255,255,255));
		m_paColorCells[array_index] = m_pLargeWhiteCell;
		m_pLargeWhiteCell->index = array_index++;

		// loop to draw gray scale rows
		for (row = 0; nGrayScaleRowSize[row] != 0; row++)
		{
			x = 1 + nGrayScaleRowOffset[row];

			// loop to draw columns
			for (col = 0; col < nGrayScaleRowSize[row]; col++)
			{
				COLORREF crCell = g_crGrayScale[index++];

				DrawSmallCell(&m_dcHexagon, x, y, crCell);
				DrawSmallCell(&m_dcValid, x, y, RGB(255,255,255));

				COLOR_CELL * pCell = new COLOR_CELL(x, y, crCell);
				m_paColorCells[array_index] = pCell;
				pCell->index = array_index++;

				_ASSERTE(array_index < NUMBER_COLOR_CELLS);

				x += SMALL_CELL_WIDTH;
			}
			y += SMALL_CELL_HEIGHT - 3;		// interlock with previous row
		}

		// draw large black cell
		DrawLargeCell(&m_dcHexagon, black_cell_x, large_cell_y, RGB(0,0,0));
		DrawLargeCell(&m_dcValid, black_cell_x, large_cell_y, RGB(255,255,255));
		m_pLargeBlackCell = new COLOR_CELL(black_cell_x, large_cell_y, RGB(0,0,0));
		m_paColorCells[array_index] = m_pLargeBlackCell;
		m_pLargeBlackCell->index = array_index++;

		_ASSERTE(array_index == NUMBER_COLOR_CELLS);

		// now that array is set up, check if there's an initial color
		if (m_crInitial != NO_COLOR)
		{
			COLOR_CELL * pCell = FindCellByColor(m_crInitial);
			if (pCell)
				m_pCurrentCell = pCell;
		}
	}

	// write hexagon bitmap to control's dc
	pDC->BitBlt(0, 0, m_rectCtrl.Width(), m_rectCtrl.Height(),
		&m_dcHexagon, 0, 0, SRCCOPY);			
}

//=============================================================================
// Helper functions extracted from mfc\src\strcore.cpp
#ifndef _UNICODE

static int _mbstowcsz(wchar_t* wcstr, const char* mbstr, size_t count)
{
	if (count == 0 && wcstr != NULL)
		return 0;

	int result = ::MultiByteToWideChar(CP_ACP, 0, mbstr, -1,
		wcstr, count);
	_ASSERTE(wcstr == NULL || result <= (int)count);
	if (result > 0)
		wcstr[result-1] = 0;
	return result;
}

#else

static int _wcstombsz(char* mbstr, const wchar_t* wcstr, size_t count)
{
	if (count == 0 && mbstr != NULL)
		return 0;

	int result = ::WideCharToMultiByte(CP_ACP, 0, wcstr, -1,
		mbstr, count, NULL, NULL);
	_ASSERTE(mbstr == NULL || result <= (int)count);
	if (result > 0)
		mbstr[result-1] = 0;
	return result;
}

#endif //_UNICODE

//=============================================================================
//
// OnToolTipText()
//
// Purpose:     This virtual function is called when a TTN_NEEDTEXT is received.
//              The tooltip control send this WM_NOTIFY message when a tooltip 
//              is about to be displayed, and LPSTR_TEXTCALLBACK has been 
//              specified as the tooltip text.
//
// Parameters:  id      - Identifier of the control that sent the notification
//                        (not used).
//              pNMHDR  - A pointer to a  NMHDR structure, which is either a
//                        TOOLTIPTEXTA or a TOOLTIPTEXTW struct.
//              pResult - A pointer to the result code.
//
// Returns:     BOOL - TRUE = message handled, do not display automatic tooltip;
//                     FALSE = message not handled, allow tooltip to be 
//                     displayed, using updated TOOLTIPTEXT struct.
//
// Notes:       The tooltip is the color of the cell that the cursor 
//              is over, displayed in the format that was selected when the 
//              control was created, or set by SetTooltipFormat().
//
BOOL CXColorHexagonCtrl::OnToolTipText(UINT /*id*/, NMHDR * pNMHDR, LRESULT * pResult)
{
	UINT nID = (UINT)pNMHDR->idFrom;
	
	// check if this is the automatic tooltip of the control
	if (nID == 0) 
		return TRUE;	// do not allow display of automatic tooltip,
						// or our tooltip will disappear
	
	// handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	
	*pResult = 0;
	
	// the max tooltip string is hardcoded as 80 in commctrl.h
	// (for both ansi and unicode)
	const int nMaxTooltipString = sizeof(pTTTA->szText);

	TCHAR szToolTip[nMaxTooltipString+2];
	szToolTip[0] = _T('\0');

	COLORREF cr = GetColorUnderCursor();

	// ensure that cursor is over a color cell
	COLOR_CELL * pCell = FindCellByColor(cr);	// pCell == NULL if not over cell

	if (pCell && (cr != NO_COLOR))
	{
		BYTE r = GetRValue(cr);
		BYTE g = GetGValue(cr);
		BYTE b = GetBValue(cr);
		double H, S, L;

		switch (m_eToolTipFormat)
		{
			default:
			case XCOLOR_TOOLTIP_RGB:
				_sntprintf(szToolTip, nMaxTooltipString, _T("RGB(%d,%d,%d)"), r, g, b);
				break;

			case XCOLOR_TOOLTIP_HTML:
				_sntprintf(szToolTip, nMaxTooltipString, _T("#%02X%02X%02X"), r, g, b);
				break;

			case XCOLOR_TOOLTIP_VB:
				_sntprintf(szToolTip, nMaxTooltipString, _T("%02X%02X%02X"), b, g, r);
				break;

			case XCOLOR_TOOLTIP_HSL:
				RGBtoHSL(cr, &H, &S, &L);
				H += 0.5;
				S += 0.5;
				L += 0.5;
				_sntprintf(szToolTip, nMaxTooltipString, _T("HSL(%d,%d,%d)"), (BYTE)H, (BYTE)S, (BYTE)L);
				break;
		}
	}

	szToolTip[nMaxTooltipString-1] = _T('\0');

	if (szToolTip[0] == _T('\0'))
		return TRUE;

#ifndef _UNICODE
	if (pNMHDR->code == TTN_NEEDTEXTA)
		lstrcpyn(pTTTA->szText, szToolTip, sizeof(pTTTA->szText));
	else
		_mbstowcsz(pTTTW->szText, szToolTip, sizeof(pTTTW->szText)/sizeof(TCHAR));
#else
	if (pNMHDR->code == TTN_NEEDTEXTA)
		_wcstombsz(pTTTA->szText, szToolTip, sizeof(pTTTA->szText));
	else
		lstrcpyn(pTTTW->szText, szToolTip, sizeof(pTTTW->szText)/sizeof(TCHAR));
#endif // _UNICODE

	return FALSE;	// we didn't handle the message, let the 
					// framework continue propagating the message
}

//=============================================================================
//
// GetRGB()
//
// Purpose:     This function retrieves the current RGB value.
//
// Parameters:  None
//
// Returns:     COLORREF - the RGB value
//
COLORREF CXColorHexagonCtrl::GetRGB()
{
	return m_crLastSent;
}

//=============================================================================
//
// SetRGB()
//
// Purpose:     This function sets the RGB value.
//
// Parameters:  cr - RGB value to set
//
// Returns:     CXColorHexagonCtrl& - reference to 'this'
//
CXColorHexagonCtrl& CXColorHexagonCtrl::SetRGB(COLORREF cr)
{
	m_crLastSent = cr;

	COLOR_CELL * pCell = FindCellByColor(cr);

	DrawSelector(pCell, FALSE);

	return *this;
}

//=============================================================================
//
// GetHSL()
//
// Purpose:     This function retrieves the current HSL values.
//
// Parameters:  h - BYTE pointer to retrieved h value
//              s - BYTE pointer to retrieved s value
//              l - BYTE pointer to retrieved l value
//
// Returns:     None
//
void CXColorHexagonCtrl::GetHSL(BYTE* h, BYTE* s, BYTE* l)
{
	RGBtoHSL(m_crLastSent, h, s, l);
}

//=============================================================================
//
// SetHSL()
//
// Purpose:     This function sets the HSL value.
//
// Parameters:  h - BYTE h value
//              s - BYTE s value
//              l - BYTE l value
//
// Returns:     CXColorHexagonCtrl& - reference to 'this'
//
CXColorHexagonCtrl& CXColorHexagonCtrl::SetHSL(BYTE h, BYTE s, BYTE l)
{
	if (h > 239)
		h = 239;
	if (s > 240)
		s = 240;
	if (l > 240)
		l = 240;
	SetRGB(HSLtoRGB(h, s, l));
	return *this;
}

//=============================================================================
//
// SetTooltipFormat()
//
// Purpose:     This function sets the tooltip format.
//
// Parameters:  eFormat - tooltip format
//
// Returns:     CXColorHexagonCtrl& - reference to 'this'
//
CXColorHexagonCtrl& CXColorHexagonCtrl::SetTooltipFormat(TOOLTIP_FORMAT eFormat)
{
	m_eToolTipFormat = eFormat;

	if (m_eToolTipFormat == XCOLOR_TOOLTIP_NONE)
	{
		if (m_pToolTip)
		{
			delete m_pToolTip;
		}
		m_pToolTip = 0;
	}
	else
	{
		if (!m_pToolTip)
			CreateTooltip();
	}
	return *this;
}

//=============================================================================
//
// KeyDown()
//
// Purpose:     This function processes WM_KEYDOWN messages.
//
// Parameters:  wParam - virtual-key code
//              lParam - not used
//
// Returns:     LRESULT - zero = message was processed
//
LRESULT CXColorHexagonCtrl::KeyDown(WPARAM wParam, LPARAM /*lParam*/)
{
	if (wParam == VK_DOWN)
	{
		GoDown();
	}
	else if (wParam == VK_UP)
	{
		GoUp();
	}
	else if (wParam == VK_RIGHT)
	{
		GoRight();
	}
	else if (wParam == VK_LEFT)
	{
		GoLeft();
	}
	else if (wParam == VK_HOME)
	{
		GoHome();
	}
	else if (wParam == VK_END)
	{
		GoEnd();
	}

	return 1;		// let default processing continue
}

//=============================================================================
//
// WindowProc()
//
// Purpose:     This function is the window proc for CXColorHexagonCtrl object.  
//              Messages are forwarded to this function from DefWindowProcX().
//
// Parameters:  message - message identifier
//              wParam  - first message parameter
//              lParam  - second message parameter
//
// Returns:     LRESULT - The return value is the result of the message 
//                        processing and depends on the message.
//
LRESULT CXColorHexagonCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message)
	{
		case WM_CREATE:
			// will never see this message - it's handled by DefWindowProcX
			break;

		case WM_ERASEBKGND:
			// nothing to erase, since we draw the entire client area
			return TRUE;

		case WM_PAINT:
		{
			PAINTSTRUCT ps; 
			HDC hdc = (wParam != NULL) ? (HDC) wParam : ::BeginPaint(m_hWnd, &ps);
			if (hdc == 0)
				return 0;
			CXDC dc(hdc);
			DrawHexagon(&dc);						// draw color hexagon
			DrawSelector(m_pCurrentCell, FALSE);	// draw selection if any
			if (wParam == NULL)
				::EndPaint(m_hWnd, &ps);
			return 0;
		}

		case WM_KEYDOWN:
			if (!KeyDown(wParam, lParam))
				return 0;
			break;

		case WM_GETDLGCODE:
			return DLGC_WANTARROWS;

		case WM_LBUTTONDOWN:
		{
			POINT point;
			point.x = GET_X_LPARAM(lParam);
			point.y = GET_Y_LPARAM(lParam);
			TRACE(_T("in WM_LBUTTONDOWN:  point.x=%d  point.y=%d\n"), point.x, point.y);

			COLOR_CELL *pCell = GetCellAtPoint(point);
			if (pCell)
				m_pCurrentCell = pCell;

			DrawSelector(TRUE);	// draw selection now, to send message

			HWND hWndFocus = ::GetFocus();
			if (hWndFocus != m_hWnd)
			{
				::SetFocus(m_hWnd);
			}

			break;
		}

		case WM_LBUTTONDBLCLK:
		{
			// let parent know there is a new selected cell
			if (m_pCurrentCell && IsWindow(m_hParent))
			{
				::SendMessage(m_hParent, WM_XCOLORPICKER_SELENDOK2, 
					m_pCurrentCell->cr, ::GetDlgCtrlID(m_hWnd));
				m_crLastSent = m_pCurrentCell->cr;
			}
			break;
		}

		case WM_SETFOCUS:
		{
			LRESULT lResult = ::DefWindowProc(m_hWnd, message, wParam, lParam);

			if (!m_pCurrentCell)
			{
				// no cell selected, select the first one (top left)
				m_pCurrentCell = (COLOR_CELL *) m_paColorCells[0];
			}

			DrawSelector(m_pCurrentCell, FALSE);	// draw selection if any

			return lResult;
		}

		case WM_KILLFOCUS:
		{
			LRESULT lResult = ::DefWindowProc(m_hWnd, message, wParam, lParam);

			DrawSelector(m_pCurrentCell, FALSE);	// draw selection if any

			return lResult;
		}

		case WM_MOUSEMOVE:
		{
			if (IsLeftButtonDown())
			{
				COLOR_CELL * pCell = GetCellUnderCursor();

				if (pCell && (pCell != m_pCurrentCell))
				{
					m_pCurrentCell = pCell;
					DrawSelector(pCell, TRUE);
				}
			}

			if (m_pToolTip)
				m_pToolTip->Update();
			
			break;
		}

		case WM_NOTIFY:
		{
			// for tooltips
			NMHDR *pNMHDR = (NMHDR *) lParam;
			if (pNMHDR->code == TTN_NEEDTEXTA || pNMHDR->code == TTN_NEEDTEXTW)
			{
				LRESULT lResult = 0;
				return OnToolTipText(0, pNMHDR, &lResult);
			}
			break;
		}

		default:
			break;
	}
	
	return ::DefWindowProc(m_hWnd, message, wParam, lParam);
}
