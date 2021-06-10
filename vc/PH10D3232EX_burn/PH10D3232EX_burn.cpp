// PH10D3232EX_burn.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "PH10D3232EX_burn.h"
#include "PH10D3232EX_burnDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPH10D3232EX_burnApp

BEGIN_MESSAGE_MAP(CPH10D3232EX_burnApp, CWinApp)
	//{{AFX_MSG_MAP(CPH10D3232EX_burnApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPH10D3232EX_burnApp construction

CPH10D3232EX_burnApp::CPH10D3232EX_burnApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CPH10D3232EX_burnApp object

CPH10D3232EX_burnApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CPH10D3232EX_burnApp initialization

BOOL CPH10D3232EX_burnApp::InitInstance()
{
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	init_console();
	init_jlink();

	CPH10D3232EX_burnDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

void CPH10D3232EX_burnApp::init_jlink()
{
	const char* sErr;
	U32 Core;
	char acBuffer[50];
	U8 acBuffer2[256];
	int speed;

	TCHAR tchBuffer[MAX_PATH];
	LPTSTR lpszCurDir;	
	lpszCurDir = tchBuffer;	
	CString	currentDir;
	CString cmd;

	CString temp_str;
	char result;
	int SN;
	int CPU_clock;

	U32 Ver;
	char ac[256];
	
	::GetModuleFileName(NULL, lpszCurDir, MAX_PATH);
	currentDir	=	lpszCurDir;
	
	currentDir.MakeUpper();
	currentDir.Replace(".EXE", ".xml");
	TRACE(currentDir);

	Ver = JLINKARM_GetDLLVersion();
	mPutsEx("%frblue");
	mPuts("DLL: v%.2lf\n", Ver/10000.0f);
	mPutsEx("%endfr");

	JLINKARM_GetFeatureString(ac);
	mPutsEx("%frblue");
	mPuts("Embedded features: %s\n",ac);
	mPutsEx("%endfr");

	sErr = JLINKARM_Open();		// Connect to J-Link
	TRACE(sErr);

	JLINKARM_EMU_GetProductName((LPSTR)acBuffer2, sizeof(acBuffer2));

	temp_str.Format("%s", acBuffer2);
	TRACE(temp_str);

	cmd.Format("JLinkDevicesXMLPath %s", currentDir);
	TRACE(cmd);
	JLINKARM_ExecCommand(cmd, NULL, 0);
	
	cmd.Format("device HC32F46X");
	TRACE(cmd);
	JLINKARM_ExecCommand(cmd, NULL, 0);
	
	JLINKARM_TIF_Select(JLINKARM_TIF_SWD);
	JLINKARM_SetSpeed(25000);
	speed = JLINKARM_GetSpeed();
	mPutsEx("%frblue");
	mPuts("speed: %dKBytes/s\n", speed);
	mPutsEx("%endfr");
// 	JLINKARM_SetMaxSpeed();
// 	speed = JLINKARM_GetSpeed();
// 	mPutsEx("%frblue");
// 	mPuts("speed: %d\n", speed);
// 	mPutsEx("%endfr");


	
	JLINKARM_Connect();                // Connect to target
	Core = JLINKARM_CORE_GetFound();
	JLINKARM_Core2CoreName(Core, acBuffer, sizeof(acBuffer));

	TRACE(acBuffer);
	mPutsEx("%frblue");
	mPuts("core: %s\n", acBuffer);
	mPutsEx("%endfr");

	result = JLINKARM_IsConnected();

	if (result)
	{
		TRACE("mcu connected!");
		SN = JLINKARM_GetSN();
		mPutsEx("%frblue");
		mPuts("SN: %d\n", SN);
		mPutsEx("%endfr");

		CPU_clock = JLINKARM_MeasureCPUSpeed(0x1FFF8000, 128);//for f460
		mPutsEx("%frblue");
		mPuts("CPU clock: %dHz\n", CPU_clock);
		mPutsEx("%endfr");
	}
	else
	{
		TRACE("muc not connected!");
	}
}

int CPH10D3232EX_burnApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	JLINKARM_Close();
	debugFree();
	
	return CWinApp::ExitInstance();
}

void CPH10D3232EX_burnApp::init_console()
{
	debugInit();
}
