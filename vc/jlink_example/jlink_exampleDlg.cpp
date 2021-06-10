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
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	ON_BN_CLICKED(IDC_BUTTON9, OnButton9)
	ON_BN_CLICKED(IDC_BUTTON10, OnButton10)
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
	int result;
	I32 num_up = 0;
	I32 num_down = 0;
	U32 dir;
	CString num_of_buffer;
	rtt_start start;
	rtt_buf_desc desc;
	CString temp_str;

	start.ConfigBlockAddress = 0x200158b0;	//.bss                segger_rtt.o

	result = JLINK_RTTERMINAL_Control(JLINKARM_RTTERMINAL_CMD_START, &start);

	if (result >= 0)
	{
		dir = 0;
		num_up = JLINK_RTTERMINAL_Control(JLINKARM_RTTERMINAL_CMD_GETNUMBUF, &dir);
		if (num_up == -2)
		{
			TRACE("not find cb");
		}
		else
		{
			num_of_buffer.Format("up: %d", num_up);
			TRACE(num_of_buffer);

			mPutsEx("%frblue");
			mPuts("%s\n", num_of_buffer);
			mPutsEx("%endfr");

			for (int i=0;i<num_up;i++)
			{
				desc.BufferIndex = i;
				desc.Direction = dir;
				JLINK_RTTERMINAL_Control(JLINKARM_RTTERMINAL_CMD_GETDESC, &desc);
				temp_str.Format("%s: size=%d, flag=%d", desc.acName, desc.SizeOfBuffer, desc.Flags);
				TRACE(temp_str);
				mPutsEx("%frmag");
				mPuts("%s\n", temp_str);
				mPutsEx("%endfr");
			}
		}
		dir = 1;
		num_down = JLINK_RTTERMINAL_Control(JLINKARM_RTTERMINAL_CMD_GETNUMBUF, &dir);
		if (num_down == -2)
		{
			TRACE("not find cb");
		}
		else
		{
			num_of_buffer.Format("down: %d", num_down);
			TRACE(num_of_buffer);

			mPutsEx("%frblue");
			mPuts("%s\n", num_of_buffer);
			mPutsEx("%endfr");
			
			for (int i=0;i<num_up;i++)
			{
				desc.BufferIndex = i;
				desc.Direction = dir;
				JLINK_RTTERMINAL_Control(JLINKARM_RTTERMINAL_CMD_GETDESC, &desc);
				temp_str.Format("%s: size=%d, flag=%d", desc.acName, desc.SizeOfBuffer, desc.Flags);
				TRACE(temp_str);
				mPutsEx("%frmag");
				mPuts("%s\n", temp_str);
				mPutsEx("%endfr");
			}
		}
	}
	else
	{
		//
	}
}

int Cjlink_exampleDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	
	return 0;
}

void Cjlink_exampleDlg::OnButton2() 
{
	int result;

	result = JLINK_RTTERMINAL_Control(JLINKARM_RTTERMINAL_CMD_STOP, NULL);
	
	if (result >= 0)
	{
		//
	}
	else
	{
		//
	}
}

void Cjlink_exampleDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
}

void Cjlink_exampleDlg::OnButton3() 
{
	static bn = 0;

	if (bn == 0)
	{
		SetTimer(0, 100, NULL);	//100ms一次
		bn = 1;
	}
	else
	{
		KillTimer(0);
		bn = 0;
	}
}

void Cjlink_exampleDlg::OnTimer(UINT nIDEvent) 
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

void Cjlink_exampleDlg::update_rtt_ch0()
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

void Cjlink_exampleDlg::OnButton4() 
{
	// TODO: Add your control notification handler code here
	JLINKARM_ResetNoHalt();
}

void Cjlink_exampleDlg::OnButton5() 
{
	// TODO: Add your control notification handler code here
	JLINKARM_SetLogFile("log.log");
}

void Cjlink_exampleDlg::OnButton6() 
{
	// TODO: Add your control notification handler code here
	JLINK_HSS_CAPS Caps;
	JLINK_HSS_MEM_BLOCK_DESC aBlock[2];
	int result = 0;
	//check the map file to find the var address
	aBlock[0].Addr = 0x1fff80d8;  //address copy from jscope
	aBlock[0].NumBytes = 4;
	aBlock[0].Flags = 0;
	aBlock[0].Dummy = 0;

	JLINK_HSS_GetCaps(&Caps);
	mPutsEx("%frgreen");
	mPuts("Max. num blocks: %d\nMax sampling frequency: %d kHz\n", Caps.MaxBlocks, Caps.MaxFreq / 1000);
	mPutsEx("%endfr");

	//100000us == 10Hz, with ms timestamp
	result = JLINK_HSS_Start(&aBlock[0], 1, 100000, 0);

	mPutsEx("%frgreen");
	mPuts("hss start: %d\n", result);
	mPutsEx("%endfr");
}

void Cjlink_exampleDlg::OnButton7() 
{
	// TODO: Add your control notification handler code here
	JLINK_HSS_Stop();
}

void Cjlink_exampleDlg::OnButton8() 
{
	// TODO: Add your control notification handler code here
	static bn = 0;
	
	if (bn == 0)
	{
		SetTimer(1, 100, NULL);	//100ms一次
		bn = 1;
	}
	else
	{
		KillTimer(1);
		bn = 0;
	}
}

void Cjlink_exampleDlg::update_light_sensor()
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

void Cjlink_exampleDlg::OnButton9() 
{
	BOOL isOpen = TRUE;		//是否打开(否则为保存)
	CString defaultDir = L"";	//默认打开的文件路径
	CString fileName = L"";			//默认打开的文件名
	//CString filter = L"文件 (*.bin; *.hex; *.axf)|*.bin;*.hex;*.axf||";	//文件过虑的类型
	CString filter = L"文件 (*.bin)|*.bin||";
	CString filePath;
	INT_PTR result;
	int n = 0;
	CString cmd;
	int r = 0;
	FILE	*	pFile;
	int	FileSize;
	U8	*	pBuffer;

	CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY|OFN_READONLY, filter, NULL);

	result = openFileDlg.DoModal();
	if(result == IDOK)
	{
		filePath = openFileDlg.GetPathName();
		mPutsEx("%frgreen");
		mPuts("file name: %s\n", filePath);
		mPutsEx("%endfr");

		n = filePath.Find(".bin");
		if (n != -1)
		{
			mPutsEx("%frgreen");
			mPuts("%d - bin file name: %s\n", n, filePath);
			mPutsEx("%endfr");
			
			TRACE("Downloading sample application...");
			JLINKARM_Reset();
			JLINKARM_Halt();

			pFile = fopen((LPSTR)(LPCTSTR)filePath, "rb");
			if (pFile == NULL) 
			{
				TRACE("Could not open file.\n");
				return;
			}
			fseek(pFile, 0, SEEK_END);
			FileSize = ftell(pFile);
			TRACE("bin file size: %d bytes\n", FileSize);
			fseek(pFile, 0, SEEK_SET);

			pBuffer  = (U8*) malloc(FileSize);
			if (pBuffer == NULL) 
			{
				TRACE("Could not allocate file buffer.\n");
				return;
			}
			fread(pBuffer, 1, FileSize, pFile);
			TRACE("bin file read O.K.\n");
			fclose(pFile);
			
			JLINKARM_BeginDownload(0);
			JLINKARM_WriteMem(0, FileSize, pBuffer);
			JLINKARM_EndDownload();
			free(pBuffer);

			TRACE("Starting target application...");
			JLINKARM_Reset();
			JLINKARM_Go();
		}

		n = filePath.Find(".hex");
		if (n != -1)
		{
			//
		}
	}
}

void Cjlink_exampleDlg::OnButton10() 
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
