// opencvMFC.h : main header file for the OPENCVMFC application
//

#if !defined(AFX_OPENCVMFC_H__D03A1894_BBC5_4317_BC99_B59D2BC77909__INCLUDED_)
#define AFX_OPENCVMFC_H__D03A1894_BBC5_4317_BC99_B59D2BC77909__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// COpencvMFCApp:
// See opencvMFC.cpp for the implementation of this class
//

class COpencvMFCApp : public CWinApp
{
public:
	COpencvMFCApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COpencvMFCApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(COpencvMFCApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPENCVMFC_H__D03A1894_BBC5_4317_BC99_B59D2BC77909__INCLUDED_)
