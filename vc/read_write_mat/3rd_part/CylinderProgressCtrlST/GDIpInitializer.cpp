#include "StdAfx.h"
#include "gdipinitializer.h"

long CGDIpInitializer::m_initcount=0;  

CGDIpInitializer::CGDIpInitializer(bool bInitCtorDtor) 
: m_bInitCtorDtor(bInitCtorDtor), 
			m_bInited(false), m_hMap(NULL), m_gdiplusToken(NULL), 
			m_gdiplusStartupInput(NULL)
{
	if (m_bInitCtorDtor) 
		Initialize();
}

CGDIpInitializer::~CGDIpInitializer()  
{
	if (m_bInitCtorDtor) 
		Deinitialize();
}

void CGDIpInitializer::Initialize()
{
		if (!m_bInited) {
			TCHAR buffer[1024];
			_stprintf(buffer, _T("GDIPlusInitID=%x"), GetCurrentProcessId());
			m_hMap = CreateFileMapping((HANDLE) INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE | SEC_COMMIT, 0, sizeof(long), buffer);
			if (m_hMap != NULL) {
				// We might have a winner
				if (GetLastError() == ERROR_ALREADY_EXISTS) { 
					CloseHandle(m_hMap); 
				} else {
					// Yes, we have a winner
					m_bInited = true;
					Gdiplus::GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, NULL);
					TRACE(_T("Inited GDIPlus\n"));
				}
			}
		}
		m_initcount++;
}

void CGDIpInitializer::Deinitialize()
{
	using namespace Gdiplus;

	m_initcount--;
	if (m_bInited && m_initcount == 0) {
		TRACE(_T("GDIPlus shutdown\n"));
		Gdiplus::GdiplusShutdown(m_gdiplusToken);
		CloseHandle(m_hMap);
		m_bInited = false;
	}
}
