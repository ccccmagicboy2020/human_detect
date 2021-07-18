// PH10D3232EX_logger.h : main header file for the PH10D3232EX_logger application
//

#if !defined(AFX_PH10D3232EX_logger_H__61121FF6_25BD_4873_A48A_67CD0F9FF2C1__INCLUDED_)
#define AFX_PH10D3232EX_logger_H__61121FF6_25BD_4873_A48A_67CD0F9FF2C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPH10D3232EX_loggerApp:
// See PH10D3232EX_logger.cpp for the implementation of this class
//

class CPH10D3232EX_loggerApp : public CWinApp
{
public:
	CPH10D3232EX_loggerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPH10D3232EX_loggerApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPH10D3232EX_loggerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		void init_jlink();
	void init_console();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PH10D3232EX_logger_H__61121FF6_25BD_4873_A48A_67CD0F9FF2C1__INCLUDED_)
