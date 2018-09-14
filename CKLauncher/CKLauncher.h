// CKLauncher.h : main header file for the CKLAUNCHER application
//

#if !defined(AFX_CKLAUNCHER_H__59A366AF_FF0F_4B76_8143_217F8D3D9412__INCLUDED_)
#define AFX_CKLAUNCHER_H__59A366AF_FF0F_4B76_8143_217F8D3D9412__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCKLauncherApp:
// See CKLauncher.cpp for the implementation of this class
//

class CCKLauncherApp : public CWinApp
{
public:
	CCKLauncherApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCKLauncherApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCKLauncherApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CKLAUNCHER_H__59A366AF_FF0F_4B76_8143_217F8D3D9412__INCLUDED_)
