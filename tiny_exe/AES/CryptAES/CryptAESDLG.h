// CryptAESDlg.h : header file
//

#if !defined(AFX_CryptAESDLG_H__C2CBC27F_45AE_4C7F_8310_3FC9E09F43AF__INCLUDED_)
#define AFX_CryptAESDLG_H__C2CBC27F_45AE_4C7F_8310_3FC9E09F43AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCryptAESDlg dialog
#define BUFSIZE (1024*50)
#include "rijndael.h"
class CCryptAESDlg : public CDialog
{
// Construction
public:
	CCryptAESDlg(CWnd* pParent = NULL);	// standard constructor
	CString m_FolderEncrypt,m_FolderDecrypt;
	CString m_FileNameEncrypt,m_FileNameDecrypt;
	Rijndael m_aes1,m_aes2;
	char databuf[BUFSIZE];//Êý¾Ý»º³åÇø
// Dialog Data
	//{{AFX_DATA(CCryptAESDlg)
	enum { IDD = IDD_CryptAES_DIALOG };
	CString	m_KeyDecrypt;
	CString	m_KeyEncrypt;
	CString	m_FileDecrypt;
	CString	m_FileEncrypt;
	CString	m_FileDecryptOut;
	CString	m_FileEncryptOut;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCryptAESDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCryptAESDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonEncryptfile();
	afx_msg void OnButtonDecryptfile();
	afx_msg void OnButtonEncrypt();
	afx_msg void OnButtonDecrypt();
	afx_msg void OnButtonDecryptfolder();
	afx_msg void OnButtonEncryptfolder();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CryptAESDLG_H__C2CBC27F_45AE_4C7F_8310_3FC9E09F43AF__INCLUDED_)
