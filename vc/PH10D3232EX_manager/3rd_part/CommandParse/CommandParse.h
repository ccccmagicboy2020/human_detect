#pragma once
#include "afxwin.h"

class CCommandParse : public CCommandLineInfo
{
public:
	CCommandParse(void);
	virtual ~CCommandParse(void);

	//Sample 03: Get functions for params and switches
	void GetParams(CStringArray& params);
	void GetSwitches(CStringArray& switches);

private:
	//Sample 01: Private Members
	CStringArray m_params;
	CStringArray m_switches;

	//Sample 02: Override for Base class virtual
	void ParseParam(const TCHAR *pszParam, BOOL bFlag, BOOL bLast);
};
