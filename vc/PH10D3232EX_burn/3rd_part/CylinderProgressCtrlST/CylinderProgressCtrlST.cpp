#include "stdafx.h"
#include "CylinderProgressCtrlST.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCylinderProgressCtrlST::CCylinderProgressCtrlST()
{
	// Default colors
	m_cFullColor.SetValue(Color.Gold);
	m_cEmptyColor.SetValue(Color.LightGray);
	m_cOutlineColor.SetValue(Color.Gray);
	m_cBkColor.SetFromCOLORREF(::GetSysColor(COLOR_3DFACE));
	m_cTextColor.SetFromCOLORREF(::GetSysColor(COLOR_BTNTEXT));

	// Show text
	m_bShowText = TRUE;

	// Outline control
	m_bOutline = TRUE;

	// Do not draw as a transparent button
	m_bDrawTransparent = FALSE;
	m_pbmpOldBk = NULL;

	// Default range of the control
	m_nLower = 0;
	m_nUpper = 100;
	CalcRange();

	// Default position
	m_nPos = 0;

	// Default step
	m_nStep = 10;
}

CCylinderProgressCtrlST::~CCylinderProgressCtrlST()
{
	// Restore old bitmap (if any)
	if (m_dcBk.m_hDC && m_pbmpOldBk)
	{
		m_dcBk.SelectObject(m_pbmpOldBk);
	} // if
}

BEGIN_MESSAGE_MAP(CCylinderProgressCtrlST, CProgressCtrl)
	//{{AFX_MSG_MAP(CCylinderProgressCtrlST)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_NCPAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CCylinderProgressCtrlST::PreTranslateMessage(MSG* pMsg) 
{
	InitToolTip();
	m_ToolTip.RelayEvent(pMsg);
	
	return CProgressCtrl::PreTranslateMessage(pMsg);
} // End of PreTranslateMessage

void CCylinderProgressCtrlST::OnNcPaint() 
{
} // End of OnNcPaint

void CCylinderProgressCtrlST::OnPaint() 
{
	CPaintDC dc(this);		// device context for painting
	Graphics g(dc.m_hDC);	// GDI+ object for painting
	
	DrawItem(&dc, &g);
} // End of OnPaint

BOOL CCylinderProgressCtrlST::OnEraseBkgnd(CDC* pDC) 
{
	return FALSE;
} // End of OnEraseBkgnd

HBRUSH CCylinderProgressCtrlST::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	return (HBRUSH)::GetStockObject(NULL_BRUSH); 
} // End of CtlColor

void CCylinderProgressCtrlST::DrawItem(CDC* pDC, Graphics* pGfx)
{
	CRect		rCtrl;
	Rect		rectCtrl;
	DWORD		dwStyle		= 0;
	BOOL		bHorizontal	= FALSE;
	int			nPercentage = 0;
	float		f = 0;

	GetClientRect(rCtrl);

	dwStyle = GetStyle();
	bHorizontal = (dwStyle & PBS_VERTICAL) != PBS_VERTICAL;

	Bitmap OffScreenImage(rCtrl.Width(), rCtrl.Height());
	Graphics gOffScreen(&OffScreenImage);
	gOffScreen.SetSmoothingMode(SmoothingModeAntiAlias);

	Rect bound(rCtrl.left, rCtrl.top, rCtrl.Width() - 1, rCtrl.Height() - 1);
	rectCtrl = bound;

	if (bHorizontal)	// Horizontal
	{
		bound.Width -= 10;
	} // if
	else	// Vertical
	{
		bound.Y += 10;
		bound.Height -= 10;
	} // else

	// Prepare draw... paint control background

	// Draw transparent?
	if (m_bDrawTransparent)
		PaintBk(&gOffScreen);
	else
		OnDrawBackground(&gOffScreen, &rectCtrl);

	// Calculate control's percentage to draw
	nPercentage = (int)((100.0/m_nRange)*(abs(m_nLower)+m_nPos));

	Rect	rEmpty = bound;
	Rect	rFull = bound;
	Rect*	rpEmptyTopPlane = NULL;

	if (bHorizontal)	// Horizontal
	{
		f = ((float)(bound.Width)/100)*nPercentage;

		// Calculate empty rectangle
		rEmpty.X += (int)f;
		rEmpty.Width -= (int)f;

		rpEmptyTopPlane = ::new Rect(rEmpty.GetRight() /*- 10*/, rEmpty.Y, 10, rEmpty.Height);
		//RectF	rEmptyBottomPlane(rEmpty.X, rEmpty.Y, 10, rEmpty.Height);

		// Calculate full rectangle
		rFull.Width = (int)f;
	} // if
	else	// Vertical
	{
		f = ((float)(bound.Height)/100)*nPercentage;

		// Calculate empty rectangle
		rEmpty.Height -= (int)f;

		rpEmptyTopPlane = ::new Rect(rEmpty.X, rEmpty.Y - 10, rEmpty.Width, 10);
		//RectF	rEmptyBottomPlane(rEmpty.X, rEmpty.GetBottom() - 10, rEmpty.Width, 10);

		// Calculate full rectangle
		rFull = bound;
		rFull.Y = rEmpty.GetBottom();
		rFull.Height -= rEmpty.Height;
	} // else

	// Draw control
	FillCylinder(&gOffScreen, bHorizontal, rEmpty, m_cEmptyColor, m_cOutlineColor, (nPercentage == 0), m_bOutline, FALSE, m_bOutline);
	if (nPercentage) 
	{
		FillCylinder(&gOffScreen, bHorizontal, rFull, m_cFullColor, m_cOutlineColor, FALSE, m_bOutline, m_bOutline, FALSE);
	} // if

	// Outline top (empty) plane
	if (m_bOutline)
	{
		Pen penOutline(m_cOutlineColor);
		gOffScreen.DrawEllipse(&penOutline, *rpEmptyTopPlane);
	} // if

	delete rpEmptyTopPlane;

	OnDrawText(&gOffScreen, nPercentage, &rectCtrl, &rEmpty, &rFull, bHorizontal);

	// Drawimage on screen                    
	pGfx->DrawImage(&OffScreenImage, 0, 0);
} // End of DrawItem

void CCylinderProgressCtrlST::PaintBk(Graphics* pGfx)
{
	Bitmap* pbmpBk = Bitmap::FromHBITMAP((HBITMAP)m_bmpBk, NULL);
	pGfx->DrawImage(pbmpBk, 0, 0);

	delete pbmpBk;
} // End of PaintBk

Color CCylinderProgressCtrlST::GetSideColor(Color color, WallSide side)
{
	float bright = 1.0f;

	byte a = color.GetA();
	byte r = color.GetR();
	byte g = color.GetG();
	byte b = color.GetB();

	switch (side)
	{
		case Back:
		{
			bright = 1.0f;
			break;
		}

		case Front:
		{
			bright = 1.0f;
			break;
		}
		
		case Left:
		{
			bright = 0.7f;
			break;
		}

		case Right:
		{
			bright = 0.5f;
			break;
		}

		case Top:
		{
			bright = 0.8f;
			break;
		}

		case Bottom:
		{
			bright = 0.4f;
			break;
		}

	} // switch

	r = (byte)(r/2 + r*bright/2);
	g = (byte)(g/2 + g*bright/2);
	b = (byte)(b/2 + b*bright/2);

	Color c1(a, r, g, b);
	return c1;
} // End of GetSideColor

void CCylinderProgressCtrlST::FillCylinderShadow(Graphics* pGfx, Rect bound, GraphicsPath* gp, BOOL horizontal)
{
	float angle = -180.0f;

	if (horizontal) angle = -90.0f;

	LinearGradientBrush linGrBrush(	bound,
									Color::MakeARGB(100, 128, 128, 128),	// Opaque black 
									Color::MakeARGB(100, 255, 255, 255),	// Opaque red
									angle);

	linGrBrush.SetBlendBellShape(0.70f);

	pGfx->FillPath(&linGrBrush, gp);
} // End of FillCylinderShadow

void CCylinderProgressCtrlST::FillCylinder(Graphics* pGfx, BOOL bHorizontal, Rect rCtrl, Color cFillColor, Color cOutlineColor, BOOL bDrawBottom, BOOL bOutline, BOOL bOutlineTop, BOOL bOutlineBottom)
{
	// Create graphics object
	GraphicsPath gfxPath;

	Rect*	rpTopPlane = NULL;
	Rect*	rpBottomPlane = NULL;

	// Brush 
	SolidBrush brFill(cFillColor);

	// Outline pen
	Pen penOutline(cOutlineColor);

	if (bHorizontal)
	{
		rpTopPlane = ::new Rect(rCtrl.GetRight() /*- 10*/, rCtrl.Y, 10, rCtrl.Height);
		rpBottomPlane = ::new Rect(rCtrl.X, rCtrl.Y, 10, rCtrl.Height);
		//RectF	rTopPlane(rCtrl.GetRight() /*- 10*/, rCtrl.Y, 10, rCtrl.Height);
		//RectF	rBottomPlane(rCtrl.X, rCtrl.Y, 10, rCtrl.Height);

		//
		// Draw body
		//
		//gfxPath.Reset(); 
		gfxPath.AddArc(*rpTopPlane, 90, 180); 
		//gfxPath.AddLine(gfxPath.GetLastPoint(), new PointF((front.X+aback.X)/2, (front.Bottom+aback.Bottom)/2.0f)); 
		gfxPath.AddArc(*rpBottomPlane, 180+90, -180); 
		gfxPath.CloseFigure();
	} // if
	else
	{
		rpTopPlane = ::new Rect(rCtrl.X, rCtrl.Y - 10, rCtrl.Width, 10);
		rpBottomPlane = ::new Rect(rCtrl.X, rCtrl.GetBottom() - 10, rCtrl.Width, 10);
		//RectF	rTopPlane(rCtrl.X, rCtrl.Y - 10, rCtrl.Width, 10);
		//RectF	rBottomPlane(rCtrl.X, rCtrl.GetBottom() - 10, rCtrl.Width, 10);

		//
		// Draw body
		//
		//gfxPath.Reset(); 
		gfxPath.AddArc(*rpTopPlane, 0, 180); 
		//gfxPath.AddLine(gfxPath.GetLastPoint(), new PointF((front.X+aback.X)/2, (front.Bottom+aback.Bottom)/2.0f)); 
		gfxPath.AddArc(*rpBottomPlane, 180, -180); 
		gfxPath.CloseFigure();
	} // else
	
	// Color for body is real fill color
	//brFill.SetColor(cFillColor);

	// Fill body
	pGfx->FillPath(&brFill, &gfxPath);

	// Shadow of the body
	FillCylinderShadow(pGfx, rCtrl, &gfxPath, bHorizontal);

	// Outline body ?
	if (bOutline) pGfx->DrawPath(&penOutline, &gfxPath);

	//
	// Draw bottom plane
	//
	if (bDrawBottom)
	{
		gfxPath.Reset();
		gfxPath.AddEllipse(*rpBottomPlane);

		// Get bottom color 
		brFill.SetColor(GetSideColor(cFillColor, Bottom));

		// Fill bottom plane 
		pGfx->FillPath(&brFill, &gfxPath);

		// Outline bottom plane ?
		if (bOutlineBottom) pGfx->DrawPath(&penOutline, &gfxPath);
	} // if

	//
	// Draw top plane
	//
	gfxPath.Reset(); 
	gfxPath.AddEllipse(*rpTopPlane); 

	// Get top color 
	brFill.SetColor(GetSideColor(cFillColor, Top)); 

	// Fill top plane 
	pGfx->FillPath(&brFill, &gfxPath); 

	// Outline top plane ?
	if (bOutlineTop) pGfx->DrawPath(&penOutline, &gfxPath);

	delete rpTopPlane;
	delete rpBottomPlane;
} // End of FillCylinder

void CCylinderProgressCtrlST::CalcRange()
{
	m_nRange = abs(m_nUpper - m_nLower);
	// Avoid divide by zero
	if (m_nRange == 0) m_nRange = 1;
} // End of CalcRange

void CCylinderProgressCtrlST::InitToolTip()
{
	if (m_ToolTip.m_hWnd == NULL)
	{
		// Create ToolTip control
		m_ToolTip.Create(this);
		// Create inactive
		m_ToolTip.Activate(FALSE);
		// Enable multiline
		m_ToolTip.SendMessage(TTM_SETMAXTIPWIDTH, 0, 400);
	} // if
} // End of InitToolTip

// This function sets the text to show in the control's tooltip.
//
// Parameters:
//		[IN]	nId
//				ID number of the string resource containing the text to show.
//		[IN]	bActivate
//				If TRUE the tooltip will be created active.
//
void CCylinderProgressCtrlST::SetTooltipText(int nId, BOOL bActivate)
{
	CString sText;

	// load string resource
	sText.LoadString(nId);
	// If string resource is not empty
	if (sText.IsEmpty() == FALSE) SetTooltipText((LPCTSTR)sText, bActivate);
} // End of SetTooltipText

// This function sets the text to show in the control's tooltip.
//
// Parameters:
//		[IN]	lpszText
//				Pointer to a null-terminated string containing the text to show.
//		[IN]	bActivate
//				If TRUE the tooltip will be created active.
//
void CCylinderProgressCtrlST::SetTooltipText(LPCTSTR lpszText, BOOL bActivate)
{
	// We cannot accept NULL pointer
	if (lpszText == NULL) return;

	// Initialize ToolTip
	InitToolTip();

	// If there is no tooltip defined then add it
	if (m_ToolTip.GetToolCount() == 0)
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		m_ToolTip.AddTool(this, lpszText, rectBtn, 1);
	} // if

	// Set text for tooltip
	m_ToolTip.UpdateTipText(lpszText, this, 1);
	m_ToolTip.Activate(bActivate);
} // End of SetTooltipText

// This function enables or disables the control's tooltip.
//
// Parameters:
//		[IN]	bActivate
//				If TRUE the tooltip will be activated.
//
void CCylinderProgressCtrlST::ActivateTooltip(BOOL bActivate)
{
	// If there is no tooltip then do nothing
	if (m_ToolTip.GetToolCount() == 0) return;

	// Activate tooltip
	m_ToolTip.Activate(bActivate);
} // End of ActivateTooltip

// This function is called every time the control's background needs to be painted.
// If the control is in transparent mode this function will NOT be called.
// This is a virtual function that can be rewritten in CCylinderProgressCtrlST-derived classes.
//
// Parameters:
//		[IN]	pGfx
//				Pointer to a GDI+ Graphics object that indicates the graphic context.
//		[IN]	rpCtrl
//				Pointer to a GDI+ Rect object that indicates the bounds of the
//				area to be painted.
//
// Return value:
//		CYLINDERPROGRESSCTRLST_OK
//			Function executed successfully.
//
DWORD CCylinderProgressCtrlST::OnDrawBackground(Graphics* pGfx, Rect* rpCtrl)
{
	// Draw control background
	pGfx->Clear(m_cBkColor);
	return CYLINDERPROGRESSCTRLST_OK;
} // End of OnDrawBackground

// This function is called each time the progress bar is redrawn.
// It is a virtual function to let derived classes do custom drawing.
// The default implementation shows the current bar percentage.
//
// Parameters:
//		[IN]	pGfx
//				Pointer to a GDI+ Graphics object that indicates the graphic context.
//		[IN]	nPercentage
//				Current percentage of the progress bar.
//		[IN]	rpCtrl
//				Pointer to a GDI+ Rect object that indicates the dimensions of the entire control.
//		[IN]	rpEmpty
//				Pointer to a GDI+ Rect object that indicates the dimensions of the control's
//				portion currently left empty.
//		[IN]	rpFull
//				Pointer to a GDI+ Rect object that indicates the dimensions of the currently displayed bar.
//		[IN]	bHorizontal
//				TRUE if the progress is horizontal, otherwise FALSE.
//
// Return value:
//		CYLINDERPROGRESSCTRLST_OK
//			Function executed successfully.
//
DWORD CCylinderProgressCtrlST::OnDrawText(Graphics* pGfx, int nPercentage, Rect* rpCtrl, Rect* rpEmpty, Rect* rpFull, BOOL bHorizontal)
{
	if (m_bShowText)
	{
		CFont*	pFont = NULL;

		pFont = GetParent()->GetFont();
		if (pFont)
		{
			TCHAR			szText[256];
			LOGFONT			csLogFont;
			StringFormat	sf;
			REAL			emSize = 0;
			SolidBrush		brText(m_cTextColor);
			RectF			rCtrl((REAL)rpCtrl->X, (REAL)rpCtrl->Y, (REAL)rpCtrl->Width, (REAL)rpCtrl->Height);
			
			pFont->GetLogFont(&csLogFont);
			
			emSize = (REAL)-MulDiv(csLogFont.lfHeight, 72, (int)pGfx->GetDpiY());
			
#ifdef _UNICODE
			Font f(csLogFont.lfFaceName, emSize, FontStyleRegular, UnitPoint, NULL);
#else
#if _ATL_VER >= 0x0700
			CA2W lpszwFont((LPCSTR)csLogFont.lfFaceName);
			Font f(lpszwFont, emSize, FontStyleRegular, UnitPoint, NULL);
#else
			USES_CONVERSION;
			Font f(A2W((LPCSTR)csLogFont.lfFaceName),
				emSize, FontStyleRegular, UnitPoint, NULL);
#endif
#endif
			sf.SetAlignment(StringAlignmentCenter);
			sf.SetLineAlignment(StringAlignmentCenter);
			
			_stprintf(szText, _T("%d%%"), nPercentage);
			pGfx->SetTextRenderingHint(TextRenderingHintAntiAlias);
			
#ifdef _UNICODE
			pGfx->DrawString(szText, -1, &f, rCtrl, &sf, &brText);
#else
#if _ATL_VER >= 0x0700
			CA2W lpszwText((LPCSTR)szText);
			pGfx->DrawString(lpszwText, -1, &f, rCtrl, &sf, &brText);
#else
// 			Font f(A2W((LPCSTR)szText),
// 				emSize, FontStyleRegular, UnitPoint, NULL);
#endif		
#endif

		} // if
	} // if

	return CYLINDERPROGRESSCTRLST_OK;
} // End of OnDrawText

// This function sets the control's colors.
//
// Parameters:
//		[IN]	cFullColor
//				A GDI+ Color object indicating the color of the currently displayed bar.
//		[IN]	cEmptyColor
//				A GDI+ Color object indicating the color of the control's portion currently left empty.
//		[IN]	cOutlineColor
//				A GDI+ Color object indicating the color used to outline the control.
//		[IN]	cTextColor
//				A GDI+ Color object indicating the color of the displayed text.
//		[IN]	cBkColor
//				A GDI+ Color object indicating the background color of the control.
//		[IN]	bRepaint
//				If TRUE the control will be repainted.
//
void CCylinderProgressCtrlST::SetColors(Color cFullColor, Color cEmptyColor, Color cOutlineColor, Color cTextColor, Color cBkColor, BOOL bRepaint)
{
	SetFullColor(cFullColor, FALSE);
	SetEmptyColor(cEmptyColor, FALSE);
	SetOutlineColor(cOutlineColor, FALSE);
	SetTextColor(cTextColor, FALSE);
	SetBkColor(cBkColor, bRepaint);
} // End of SetColors

// This function sets the color of the currently displayed bar.
//
// Parameters:
//		[IN]	cColor
//				A GDI+ Color object indicating the color of the currently displayed bar.
//		[IN]	bRepaint
//				If TRUE the control will be repainted.
//
void CCylinderProgressCtrlST::SetFullColor(Color cColor, BOOL bRepaint)
{
	m_cFullColor = cColor;

	// Repaint control
	if (bRepaint)	Invalidate();
} // End of SetFullColor

// This function sets the color of the control's portion currently left empty.
//
// Parameters:
//		[IN]	cColor
//				A GDI+ Color object indicating the color of the control's portion currently left empty.
//		[IN]	bRepaint
//				If TRUE the control will be repainted.
//
void CCylinderProgressCtrlST::SetEmptyColor(Color cColor, BOOL bRepaint)
{
	m_cEmptyColor = cColor;

	// Repaint control
	if (bRepaint)	Invalidate();
} // End of SetEmptyColor

// This function sets the color used to outline the control.
//
// Parameters:
//		[IN]	cColor
//				A GDI+ Color object indicating the color used to outline the control.
//		[IN]	bRepaint
//				If TRUE the control will be repainted.
//
void CCylinderProgressCtrlST::SetOutlineColor(Color cColor, BOOL bRepaint)
{
	m_cOutlineColor = cColor;

	// Repaint control
	if (bRepaint)	Invalidate();
} // End of SetOutlineColor

// This function sets the color of the displayed text.
//
// Parameters:
//		[IN]	cColor
//				A GDI+ Color object indicating the color of the displayed text.
//		[IN]	bRepaint
//				If TRUE the control will be repainted.
//
void CCylinderProgressCtrlST::SetTextColor(Color cColor, BOOL bRepaint)
{
	m_cTextColor = cColor;

	// Repaint control
	if (bRepaint)	Invalidate();
} // End of SetTextColor

// This function sets the background color of the control.
//
// Parameters:
//		[IN]	cColor
//				A GDI+ Color object indicating the background color of the control.
//		[IN]	bRepaint
//				If TRUE the control will be repainted.
//
void CCylinderProgressCtrlST::SetBkColor(Color cColor, BOOL bRepaint)
{
	m_cBkColor = cColor;

	// Repaint control
	if (bRepaint)	Invalidate();
} // End of SetBkColor

// This function sets if text must be displayed on the control.
//
// Parameters:
//		[IN]	bShowText
//				TRUE if text must be displayed on the control.
//		[IN]	bRepaint
//				If TRUE the control will be repainted.
//
void CCylinderProgressCtrlST::SetShowText(BOOL bShowText, BOOL bRepaint)
{
	m_bShowText = bShowText;

	// Repaint control
	if (bRepaint)	Invalidate();
} // End of SetShowText

// This function sets if the control must be outlined.
//
// Parameters:
//		[IN]	bShowOutline
//				TRUE if the control must be outlined.
//		[IN]	bRepaint
//				If TRUE the control will be repainted.
//
void CCylinderProgressCtrlST::SetShowOutline(BOOL bShowOutline, BOOL bRepaint)
{
	m_bOutline = bShowOutline;

	// Repaint control
	if (bRepaint)	Invalidate();
} // End of SetShowOutline

// This function sets the alpha component of the colors used to draw the control.
//
// Parameters:
//		[IN]	byAlpha
//				A BYTE value indicating the alpha component.
//		[IN]	bRepaint
//				If TRUE the control will be repainted.
//
void CCylinderProgressCtrlST::SetAlpha(BYTE byAlpha, BOOL bRepaint)
{
	ARGB argb = 0;

	// Create an ARGB value from the four component values.
	argb = Color::MakeARGB(byAlpha, m_cFullColor.GetR(), m_cFullColor.GetG(), m_cFullColor.GetB());
	m_cFullColor.SetValue(argb);

	// Create an ARGB value from the four component values.
	argb = Color::MakeARGB(byAlpha, m_cEmptyColor.GetR(), m_cEmptyColor.GetG(), m_cEmptyColor.GetB());
	m_cEmptyColor.SetValue(argb);

	// Create an ARGB value from the four component values.
	argb = Color::MakeARGB(byAlpha, m_cOutlineColor.GetR(), m_cOutlineColor.GetG(), m_cOutlineColor.GetB());
	m_cOutlineColor.SetValue(argb);

	// Create an ARGB value from the four component values.
	argb = Color::MakeARGB(byAlpha, m_cTextColor.GetR(), m_cTextColor.GetG(), m_cTextColor.GetB());
	m_cTextColor.SetValue(argb);

	// Repaint control
	if (bRepaint)	Invalidate();
} // End of SetAlpha

// This function returns the alpha component of the colors used to draw the control.
//
// Return value:
//		A BYTE indicating the alpha component.
//
BYTE CCylinderProgressCtrlST::GetAlpha()
{
	return m_cFullColor.GetAlpha();
} // End of GetAlpha

// This function enables the transparent mode.
// Note: this operation is not reversible.
// DrawTransparent should be called just after the control is created.
// Do not use trasparent controls until you really need it (you have a bitmapped
// background) since each transparent control makes a copy in memory of its background.
// This may bring unnecessary memory use and execution overload.
//
// Parameters:
//		[IN]	bRepaint
//				If TRUE the control will be repainted.
//
// Return value:
//		CYLINDERPROGRESSCTRLST_OK
//			Function executed successfully.
//
DWORD CCylinderProgressCtrlST::DrawTransparent(BOOL bRepaint)
{
	m_bDrawTransparent = TRUE;

	// Restore old bitmap (if any)
	if (m_dcBk.m_hDC != NULL && m_pbmpOldBk != NULL)
	{
		m_dcBk.SelectObject(m_pbmpOldBk);
	} // if

	m_bmpBk.DeleteObject();
	m_dcBk.DeleteDC();

	// Repaint the button
	if (bRepaint) Invalidate();

	return CYLINDERPROGRESSCTRLST_OK;
} // End of DrawTransparent

DWORD CCylinderProgressCtrlST::SetBk(CDC* pDC)
{
	if (m_bDrawTransparent && pDC)
	{
		// Restore old bitmap (if any)
		if (m_dcBk.m_hDC != NULL && m_pbmpOldBk != NULL)
		{
			m_dcBk.SelectObject(m_pbmpOldBk);
		} // if

		m_bmpBk.DeleteObject();
		m_dcBk.DeleteDC();

		CRect rect;
		CRect rect1;

		GetClientRect(rect);

		GetWindowRect(rect1);
		GetParent()->ScreenToClient(rect1);

		m_dcBk.CreateCompatibleDC(pDC);
		m_bmpBk.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
		m_pbmpOldBk = m_dcBk.SelectObject(&m_bmpBk);
		m_dcBk.BitBlt(0, 0, rect.Width(), rect.Height(), pDC, rect1.left, rect1.top, SRCCOPY);

		return CYLINDERPROGRESSCTRLST_OK;
	} // if

	return CYLINDERPROGRESSCTRLST_BADPARAM;
} // End of SetBk

// This function sets the upper and lower limits of the progress bar control's range
// and redraws the bar to reflect the new ranges.
//
// Parameters:
//		[IN]	shLower
//				Specifies the lower limit of the range (default is zero).
//		[IN]	shUpper
//				Specifies the upper limit of the range (default is 100).
//
void CCylinderProgressCtrlST::SetRange(short shLower, short shUpper)
{
	m_nLower = (int)shLower;
	m_nUpper = (int)shUpper;
	CalcRange();

	CProgressCtrl::SetRange(shLower, shUpper);
} // End of SetRange

// This function sets the upper and lower limits of the progress bar control's range
// and redraws the bar to reflect the new ranges.
//
// Parameters:
//		[IN]	nLower
//				Specifies the lower limit of the range (default is zero).
//		[IN]	nUpper
//				Specifies the upper limit of the range (default is 100).
//
void CCylinderProgressCtrlST::SetRange32(int nLower, int nUpper)
{
	m_nLower = nLower;
	m_nUpper = nUpper;
	CalcRange();

	CProgressCtrl::SetRange32(nLower, nUpper);
} // End of SetRange32

// This function specifies the step increment for a progress bar control.
// The step increment is the amount by which a call to StepIt increases
// the progress bar's current position.
//
// Parameters:
//		[IN]	nStep
//				New step increment.
//
// Return value:
//		The previous step increment.
//
int CCylinderProgressCtrlST::SetStep(int nStep)
{
	m_nStep = nStep;
	return CProgressCtrl::SetStep(nStep);
} // End Of SetStep

// This function sets the progress bar control's current position as specified by nPos
// and redraws the bar to reflect the new position.
// The position of the progress bar control is not the physical location on the screen,
// but rather is between the upper and lower range indicated in SetRange.
//
// Parameters:
//		[IN]	nPos
//				New position of the progress bar control.
//
// Return value:
//		The previous position of the progress bar control.
//
int CCylinderProgressCtrlST::SetPos(int nPos)
{
	// Bound checking
	if (nPos < m_nLower) nPos = m_nLower;
	if (nPos > m_nUpper) nPos = m_nUpper;

	m_nPos = nPos;
	return CProgressCtrl::SetPos(nPos);
} // End of SetPos

// This function advances the current position for a progress bar control by the step increment
// and redraws the bar to reflect the new position.
// The step increment is set by the SetStep method.
//
// Return value:
//		The previous position of the progress bar control.
//
int CCylinderProgressCtrlST::StepIt()
{
	m_nPos += m_nStep;

	// Bound checking
	if (m_nPos > m_nUpper) m_nPos = m_nLower + (m_nPos - m_nUpper);

	return CProgressCtrl::StepIt();
} // End of StepIt
