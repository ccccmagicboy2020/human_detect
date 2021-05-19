// read_write_matDlg.cpp : implementation file
//

#include "stdafx.h"
#include "read_write_mat.h"
#include "read_write_matDlg.h"

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
// Cread_write_matDlg dialog

Cread_write_matDlg::Cread_write_matDlg(CWnd* pParent /*=NULL*/)
	: CDialog(Cread_write_matDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(Cread_write_matDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cread_write_matDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Cread_write_matDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(Cread_write_matDlg, CDialog)
	//{{AFX_MSG_MAP(Cread_write_matDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Cread_write_matDlg message handlers

BOOL Cread_write_matDlg::OnInitDialog()
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

void Cread_write_matDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void Cread_write_matDlg::OnPaint() 
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
HCURSOR Cread_write_matDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void Cread_write_matDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	MATFile *pMF;	// mat�ļ�
	mxArray *pA;	// ����ָ��
	double *A;		// ����ָ��

	int i, j;
	// ��test.mat�ļ���ֻ����������Ϊ��ʾ�����������飩
	pMF = matOpen("test.mat", "r");
	// ��þ���
	pA = matGetVariable(pMF, "x");
	// ��þ������ݵ�ַ
	A = mxGetPr(pA);
	mPutsEx("%frgreen");
	mPuts("read var: x\n");
	mPutsEx("%endfr");
	// ��ӡ����
	for (i = 0; i < 12; i++) 
	{
		for (j = 0; j < 4; j++)
		{
			mPutsEx("%frgreen");
			mPuts("(%d, %d): %.3lf\n", i, j, A[j * 12 + i]);
			mPutsEx("%endfr");
		}
	}
	// �ͷž���ռ�
	mxDestroyArray(pA);
	// �ر��ļ�
    matClose(pMF);	
}

int Cread_write_matDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	debugInit();
	
	return 0;
}

void Cread_write_matDlg::OnButton2() 
{
	MATFile *pMF; // mat�ļ�
	mxArray *pA; // ����ָ��
	double *A; // ����ָ��

	int i, j;
	// ��һ��test.mat�ļ���ֻд����������ھ��½�������ɾ���ؽ�
	pMF = matOpen("test.mat", "w");
	// �½�һ��12*4�ľ���ֻ��ʵ������MATLAB������Ǹ�����
	pA = mxCreateDoubleMatrix(12, 4, mxREAL);
	// ��ָ��Arrayָ��������������
	A = mxGetPr(pA);
	// ��VC�������ǰ����ŷŵģ���MATLAB������ǰ����ŷŵ�
	mPutsEx("%frmag");
	mPuts("write var: x\n");
	mPutsEx("%endfr");
	for (i = 0; i < 12; i++)
	{
		for (j = 0; j < 4; j++)
		{
			A[j * 12 + i] = i + 0.001 * j;
			mPutsEx("%frmag");
			mPuts("(%d, %d): %.3lf\n", i, j, A[j * 12 + i]);
			mPutsEx("%endfr");
		}
	}
	// ������д���ļ�test��
	matPutVariable(pMF, "x", pA);
	// �ͷž���ռ�
	mxDestroyArray(pA);
	// �ر��ļ�
	matClose(pMF);
}

void Cread_write_matDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	debugFree();
	
	CDialog::PostNcDestroy();
}
