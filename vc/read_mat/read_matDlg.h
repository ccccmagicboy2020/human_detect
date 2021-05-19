// read_matDlg.h : header file
//

#if !defined(AFX_read_matDLG_H__2FA31913_73F8_4C8C_B6BC_1C2497ACD5CD__INCLUDED_)
#define AFX_read_matDLG_H__2FA31913_73F8_4C8C_B6BC_1C2497ACD5CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// Cread_matDlg dialog

class Cread_matDlg : public CDialog
{
// Construction
public:
	Cread_matDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(Cread_matDlg)
	enum { IDD = IDD_read_mat_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Cread_matDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(Cread_matDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


	//
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_read_matDLG_H__2FA31913_73F8_4C8C_B6BC_1C2497ACD5CD__INCLUDED_)
