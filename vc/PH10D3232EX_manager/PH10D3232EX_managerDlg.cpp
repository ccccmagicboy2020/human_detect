// PH10D3232EX_managerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PH10D3232EX_manager.h"
#include "PH10D3232EX_managerDlg.h"

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
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CStatic	m_ver;
	CHyperLink	m_link;
public:
	void init_static();
	void init_window();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_STATIC_VER, m_ver);
	DDX_Control(pDX, IDC_STATIC_LINK, m_link);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CAboutDlg::init_static()
{
#define IDS_MAILADDR	_T("http://www.phosense-tech.com/")
	
	CVersionInfo	ver;
	CString version;
	
	ver.GetVersionInfo(AfxGetInstanceHandle());
	
#ifdef _DEBUG
	version	=	_T("debug: v") + ver.m_strFixedProductVersion;
#else
	version	=	_T("release: v") + ver.m_strFixedProductVersion;
#endif
	
	version.Replace(',', '.');	
	m_ver.SetWindowText(version);
	
	m_link.SetWindowText(_T("www.phosense-tech.com"));
	m_link.SetURL(IDS_MAILADDR);
	m_link.SetUnderline(CHyperLink::ulAlways);
	
	Invalidate();
	UpdateWindow();	
}

void CAboutDlg::init_window()
{
	ModifyStyle(NULL, WS_CLIPCHILDREN);
}

/////////////////////////////////////////////////////////////////////////////
// CPH10D3232EX_managerDlg dialog

CPH10D3232EX_managerDlg::CPH10D3232EX_managerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPH10D3232EX_managerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPH10D3232EX_managerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPH10D3232EX_managerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPH10D3232EX_managerDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPH10D3232EX_managerDlg, CDialog)
	//{{AFX_MSG_MAP(CPH10D3232EX_managerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON10, OnButton10)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPH10D3232EX_managerDlg message handlers

BOOL CPH10D3232EX_managerDlg::OnInitDialog()
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

void CPH10D3232EX_managerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPH10D3232EX_managerDlg::OnPaint() 
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
HCURSOR CPH10D3232EX_managerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

int CPH10D3232EX_managerDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	
	return 0;
}

void CPH10D3232EX_managerDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
}

void CPH10D3232EX_managerDlg::OnTimer(UINT nIDEvent) 
{
	switch (nIDEvent)
	{
	case 0:
		update_rtt_ch0();
		break;
	case 1:
		update_light_sensor();
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	default:
		break;
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CPH10D3232EX_managerDlg::update_rtt_ch0()
{
	U32 read_num;
	char buf[1024];

	read_num = JLINK_RTTERMINAL_Read(0, buf, 1024);

	if (read_num)
	{
		buf[read_num] = 0;
		mPutsEx("%fryellow");
		mPuts("%s", buf);
		mPutsEx("%endfr");		
	}
}

void CPH10D3232EX_managerDlg::update_light_sensor()
{
	U32 aBuffer[2*20];
	int NumBytesRem;
	int i = 0;

	NumBytesRem = JLINK_HSS_Read(&aBuffer[0], sizeof(aBuffer));

	do 
	{
		mPutsEx("%frgreen");
		mPuts("Timestamp: %d\n", aBuffer[i + 0]);
		mPuts("0x1fff80d8: 0x%.8X(%d)\n", aBuffer[i + 1], aBuffer[i + 1]);
		mPutsEx("%endfr");

		i += 2;
		NumBytesRem -= 8;
	} while (NumBytesRem);

}

void CPH10D3232EX_managerDlg::OnButton10() 
{
	BOOL isOpen = TRUE;		//是否打开(否则为保存)
	CString defaultDir = L"";	//默认打开的文件路径
	CString fileName = L"";			//默认打开的文件名
	//CString filter = L"文件 (*.bin; *.hex; *.axf)|*.bin;*.hex;*.axf||";	//文件过虑的类型
	CString filter = L"文件 (*.bin; *.hex)|*.bin;*.hex||";	//文件过虑的类型
	CString filePath;
	INT_PTR result;
	
	CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY|OFN_READONLY, filter, NULL);
	
	result = openFileDlg.DoModal();
	if(result == IDOK)
	{
		filePath = openFileDlg.GetPathName();
		mPutsEx("%frgreen");
		mPuts("file name: %s\n", filePath);
		mPutsEx("%endfr");
		
		if (1)
		{			
			TRACE("Downloading sample application...");
			JLINKARM_Reset();
			JLINKARM_Halt();

			JLINK_DownloadFile((LPSTR)(LPCTSTR)filePath, 0);
			
			TRACE("Starting target application...");
			JLINKARM_Reset();
			JLINKARM_Go();
		}
	}
}

void CPH10D3232EX_managerDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	JLINKARM_Reset();
	JLINKARM_Halt();
	JLINK_EraseChip();
	JLINKARM_Reset();
	JLINKARM_Go();
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	init_window();
	init_static();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
