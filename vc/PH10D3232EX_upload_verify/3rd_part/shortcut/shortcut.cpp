#include "stdafx.h"
#include "shortcut.h"

BOOL CreateLink(LPSTR   szPath,
				LPSTR   szLink, LPSTR szParameter)
{
	HRESULT   hres;   
	IShellLink* psl;   
	IPersistFile*   ppf;   
	WCHAR wsz[MAX_PATH];  
	  
	CoInitialize(NULL);  
	
	hres = CoCreateInstance(CLSID_ShellLink,NULL, CLSCTX_INPROC_SERVER,   IID_IShellLink, (void   **)&psl);   
	if(FAILED(hres))   
	{   
		CoUninitialize();   
		return FALSE;   
	}  
	 
	psl->SetPath(szPath);  
	
	CString csWorkDir(szPath);
	int idx = csWorkDir.ReverseFind('\\');
	csWorkDir = csWorkDir.Left(idx);
	psl->SetWorkingDirectory(csWorkDir);
	//cv
	psl->SetArguments(szParameter);
	
	hres = psl->QueryInterface(IID_IPersistFile, (void**)&ppf);
	if(FAILED(hres))
	{   
		CoUninitialize();   
		return FALSE;   
	}  
	
	MultiByteToWideChar(CP_ACP, 0, szLink, -1, wsz, MAX_PATH);  
	
	hres = ppf ->Save(wsz, STGM_READWRITE);
	ppf   ->   Release(   )   ;   
	psl   ->   Release(   )   ;  
	
	CoUninitialize();
	return   TRUE;   
}

void CreateDestopShortcut(LPSTR szPath, LPSTR szLinkName, LPSTR szParameter)   
{   
	char szLink[MAX_PATH];  
	
	SHGetSpecialFolderPath(NULL, szLink, CSIDL_COMMON_DESKTOPDIRECTORY, FALSE);
	
	if(szLink[lstrlen(szLink)-1] == '\\')
	{
		lstrcat(szLink, szLinkName); 
	}
	else
	{
		lstrcat(szLink, "\\"); 
		lstrcat(szLink, szLinkName); 
	}
	if(!CreateLink(szPath, szLink, szParameter))   
		return;
}


