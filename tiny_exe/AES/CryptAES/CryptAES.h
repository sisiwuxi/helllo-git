// CryptAES.h : main header file for the CryptAES application
//

#if !defined(AFX_CryptAES_H__72EBD634_122F_4D42_83CB_8370006C392D__INCLUDED_)
#define AFX_CryptAES_H__72EBD634_122F_4D42_83CB_8370006C392D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCryptAESApp:
// See CryptAES.cpp for the implementation of this class
//

class CCryptAESApp : public CWinApp
{
public:
	CCryptAESApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCryptAESApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCryptAESApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CryptAES_H__72EBD634_122F_4D42_83CB_8370006C392D__INCLUDED_)
