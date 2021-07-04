#include "StdAfx.h"
#include "CommandParse.h"

CCommandParse::CCommandParse(void)
{
}

CCommandParse::~CCommandParse(void)
{
}

//Sample 04: Implement the Parse Param
void CCommandParse::ParseParam(const TCHAR *pszParam, BOOL bFlag, BOOL bLast) 
{
	//Sample 04_1: Collect the parameters and switches in a separate Array
	CString param_or_switch(pszParam);
	if (bFlag)
		m_switches.Add(param_or_switch);
	else
		m_params.Add(param_or_switch);
}

//Sample 05: Get Functions. 
void CCommandParse::GetParams(CStringArray& params) 
{
	int size = m_params.GetSize();
	for (int i = 0; i<size; i++)
		params.Add( m_params.GetAt(i)); 
}

void CCommandParse::GetSwitches(CStringArray& switches)
{
	int size = m_switches.GetSize();
	for (int i = 0; i<size; i++)
		switches.Add( m_switches.GetAt(i)); 
}