#pragma once

/*! \brief A class wrapper for GDI+ initialization

 Author: Ryan Johnston, see http://www.codeproject.com/vcpp/gdiplus/GDIPlusControl.asp
 \ingroup GDIpGroup
*/
class CGDIpInitializer 
{
public:
	//! Constructor offers the ability to initialize on construction, or delay until needed.
	CGDIpInitializer(bool bInitCtorDtor = false);

	//! If GDI+ has not explicitly been Deinitialized, do it in the destructor
	virtual ~CGDIpInitializer();

	/*! \brief Initialize function, makes sure only one GDI+ is called for each process.
	
	  This function creates a file mapping based on the current process id.
	If the mapping already exists, it knows that another instance of this class
	elsewhere in the process has already taken care of starting GDI+.
	*/
	void Initialize();

	/*! \brief Deinitialize or decrease initialize count.
	
	  No tricks to this function.  If this was the class that originally started GDI+,
	 and its initialization count has reached zero, it shuts down GDI+.
	 */
	void Deinitialize();

private:
	HANDLE							m_hMap;
	bool							m_bInited, m_bInitCtorDtor;
	ULONG_PTR						m_gdiplusToken;
	Gdiplus::GdiplusStartupInput	m_gdiplusStartupInput;
	static long							m_initcount;
};
