// jlink_exampleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "jlink_example.h"
#include "jlink_exampleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Cjlink_exampleDlg dialog

Cjlink_exampleDlg::Cjlink_exampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(Cjlink_exampleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(Cjlink_exampleDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cjlink_exampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Cjlink_exampleDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(Cjlink_exampleDlg, CDialog)
	//{{AFX_MSG_MAP(Cjlink_exampleDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Cjlink_exampleDlg message handlers

BOOL Cjlink_exampleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	//
	//
	//
	//

	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void Cjlink_exampleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void Cjlink_exampleDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR Cjlink_exampleDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void Cjlink_exampleDlg::OnButton1() 
{
	const char* sErr;
	U32 Core;
	char acBuffer[50];
	U8 acBuffer2[256];
	int speed;

	sErr = JLINKARM_Open();		// Connect to J-Link
	if (sErr)
	{
		MessageBox(sErr, "J-Link", MB_OK);
	}
	JLINKARM_EMU_GetProductName((LPSTR)acBuffer2, sizeof(acBuffer2));
	MessageBox((LPSTR)acBuffer2, "J-Link", MB_OK);

	JLINKARM_TIF_Select(JLINKARM_TIF_SWD);
	JLINKARM_SetSpeed(25000);
	speed = JLINKARM_GetSpeed();
	mPutsEx("%frblue");
	mPuts("speed: %d\n", speed);
	mPutsEx("%endfr");
	JLINKARM_SetMaxSpeed();
	speed = JLINKARM_GetSpeed();
	mPutsEx("%frblue");
	mPuts("speed: %d\n", speed);
	mPutsEx("%endfr");

	JLINKARM_Connect();                // Connect to target
	Core = JLINKARM_CORE_GetFound();
	JLINKARM_Core2CoreName(Core, acBuffer, sizeof(acBuffer));

	JLINKARM_Close();
}

int Cjlink_exampleDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	debugInit();
	
	return 0;
}

void Cjlink_exampleDlg::OnButton2() 
{
	//
	//
	//
}

void Cjlink_exampleDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	debugFree();
	
	CDialog::PostNcDestroy();
}
