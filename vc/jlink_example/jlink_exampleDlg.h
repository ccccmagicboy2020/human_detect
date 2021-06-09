// jlink_exampleDlg.h : header file
//

#if !defined(AFX_jlink_exampleDLG_H__2FA31913_73F8_4C8C_B6BC_1C2497ACD5CD__INCLUDED_)
#define AFX_jlink_exampleDLG_H__2FA31913_73F8_4C8C_B6BC_1C2497ACD5CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// Cjlink_exampleDlg dialog

class Cjlink_exampleDlg : public CDialog
{
// Construction
public:
	Cjlink_exampleDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(Cjlink_exampleDlg)
	enum { IDD = IDD_jlink_example_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Cjlink_exampleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(Cjlink_exampleDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	afx_msg void OnButton7();
	afx_msg void OnButton8();
	afx_msg void OnButton9();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
public:
	void update_rtt_ch0();
	void update_light_sensor();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_jlink_exampleDLG_H__2FA31913_73F8_4C8C_B6BC_1C2497ACD5CD__INCLUDED_)
