// read_mat.h : main header file for the read_mat application
//

#if !defined(AFX_read_mat_H__61121FF6_25BD_4873_A48A_67CD0F9FF2C1__INCLUDED_)
#define AFX_read_mat_H__61121FF6_25BD_4873_A48A_67CD0F9FF2C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// Cread_matApp:
// See read_mat.cpp for the implementation of this class
//

class Cread_matApp : public CWinApp
{
public:
	Cread_matApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Cread_matApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(Cread_matApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_read_mat_H__61121FF6_25BD_4873_A48A_67CD0F9FF2C1__INCLUDED_)
