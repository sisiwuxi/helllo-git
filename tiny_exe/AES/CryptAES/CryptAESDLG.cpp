// CryptAESDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CryptAES.h"
#include "CryptAESDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCryptAESDlg dialog

CCryptAESDlg::CCryptAESDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCryptAESDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCryptAESDlg)
	m_KeyDecrypt = _T("");
	m_KeyEncrypt = _T("");
	m_FileDecrypt = _T("");
	m_FileEncrypt = _T("");
	m_FileDecryptOut = _T("");
	m_FileEncryptOut = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCryptAESDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCryptAESDlg)
	DDX_Text(pDX, IDC_EDIT_DECRYPT, m_KeyDecrypt);
	DDX_Text(pDX, IDC_EDIT_ENCRYPT, m_KeyEncrypt);
	DDX_Text(pDX, IDC_EDIT_DECRYPTFILE, m_FileDecrypt);
	DDX_Text(pDX, IDC_EDIT_ENCRYPTFILE, m_FileEncrypt);
	DDX_Text(pDX, IDC_EDIT_DECRYPTOUT, m_FileDecryptOut);
	DDX_Text(pDX, IDC_EDIT_ENCRYPTFILEOUT, m_FileEncryptOut);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCryptAESDlg, CDialog)
	//{{AFX_MSG_MAP(CCryptAESDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_ENCRYPTFILE, OnButtonEncryptfile)
	ON_BN_CLICKED(IDC_BUTTON_DECRYPTFILE, OnButtonDecryptfile)
	ON_BN_CLICKED(IDC_BUTTON_ENCRYPT, OnButtonEncrypt)
	ON_BN_CLICKED(IDC_BUTTON_DECRYPT, OnButtonDecrypt)
	ON_BN_CLICKED(IDC_BUTTON_DECRYPTFOLDER, OnButtonDecryptfolder)
	ON_BN_CLICKED(IDC_BUTTON_ENCRYPTFOLDER, OnButtonEncryptfolder)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCryptAESDlg message handlers

BOOL CCryptAESDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_FileNameEncrypt = _T("");
	m_FileNameDecrypt = _T("");
	m_FolderEncrypt = _T("");
	m_FolderDecrypt = _T("");
	GetDlgItem( IDC_BUTTON_ENCRYPT )->EnableWindow(false);
	GetDlgItem( IDC_BUTTON_DECRYPT )->EnableWindow(false);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCryptAESDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCryptAESDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCryptAESDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCryptAESDlg::OnButtonEncryptfile() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	char szFilter[] = "all Files(*.*)|*.*||";
	CFileDialog FileDlg( TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter );

	if( FileDlg.DoModal() == IDOK )
	{
		m_FileEncrypt = FileDlg.GetPathName();
		m_FileNameEncrypt = FileDlg.GetFileName();
		if(m_FolderEncrypt=="")
			m_FileEncryptOut = m_FileEncrypt+".aes";
		else
			m_FileEncryptOut = m_FolderEncrypt+"\\"+m_FileNameEncrypt+".aes";
		GetDlgItem( IDC_BUTTON_ENCRYPT )->EnableWindow(true);
	}	
	UpdateData(false);
}

void CCryptAESDlg::OnButtonEncryptfolder() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	BROWSEINFO	bi; 
	ITEMIDLIST	*pidl; 
    char		Dir[256];

	memset(&bi, 0, sizeof(bi));
	bi.pszDisplayName = Dir; 
	bi.lpszTitle = "请选择目录"; 
	bi.ulFlags = BIF_RETURNONLYFSDIRS; 

	pidl = SHBrowseForFolder(&bi);
	SHGetPathFromIDList(pidl,Dir);
    GlobalFree(pidl);
	m_FolderEncrypt = Dir;	
	if(strlen(Dir)==3)
		m_FileEncryptOut = m_FolderEncrypt+m_FileNameEncrypt+".aes";
	else
		m_FileEncryptOut = m_FolderEncrypt+"\\"+m_FileNameEncrypt+".aes";
	UpdateData(false);
}

void CCryptAESDlg::OnButtonEncrypt() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	HFILE	fh_out,fh_in;//输入输出文件句柄
	fh_in=_lopen(m_FileEncrypt,OF_READ);
	fh_out=_lcreat(m_FileEncryptOut,0);
	long     len;	
	// 读取明文到缓冲区
	while( (len=_lread(fh_in,databuf,BUFSIZE)) >0 )
	{  
		// 设置密钥
		if(m_KeyEncrypt.GetLength()>=32)
			m_KeyEncrypt=m_KeyEncrypt.Left(32);
        // 将缓冲区长度变为8的倍数
		len = ((len+7)>>3)<<3;
		// 在缓冲区中加密
		unsigned char* key=(unsigned char*)m_KeyEncrypt.GetBuffer(8);
		m_KeyEncrypt.ReleaseBuffer();
		m_aes1.init(Rijndael::ECB, Rijndael::Encrypt, (const RD_UINT8 *)key,Rijndael::Key32Bytes, 0);
		m_aes1.blockEncrypt((const RD_UINT8 *)databuf, len*8, (RD_UINT8 *)databuf);

		// 将密文写入输出文件
		_lwrite(fh_out,databuf,len);
	}
	_lclose(fh_in);
	_lclose(fh_out);
	AfxMessageBox("文件加密完毕!",MB_OK);

}

void CCryptAESDlg::OnButtonDecryptfile() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	char szFilter[] = "all Files(*.*)|*.*||";
	CFileDialog FileDlg( TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter );

	if( FileDlg.DoModal() == IDOK )
	{
		m_FileDecrypt = FileDlg.GetPathName();
		m_FileNameDecrypt = FileDlg.GetFileName();
		if(m_FolderDecrypt=="")
			m_FileDecryptOut = m_FileDecrypt+".aes";
		else
			m_FileDecryptOut = m_FolderDecrypt+"\\"+m_FileNameDecrypt+".aes";
		GetDlgItem( IDC_BUTTON_DECRYPT )->EnableWindow(true);
		
	}	
	UpdateData(false);	
}


void CCryptAESDlg::OnButtonDecrypt() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	HFILE	fh_out,fh_in;//输入输出文件句柄
	fh_in=_lopen(m_FileDecrypt,OF_READ);
	fh_out=_lcreat(m_FileDecryptOut,0);
	long     len;	
	// 读取明文到缓冲区
	while( (len=_lread(fh_in,databuf,BUFSIZE)) >0 )
	{  
		// 设置子密钥
		if(m_KeyDecrypt.GetLength()>=32)
			m_KeyDecrypt=m_KeyEncrypt.Left(32);
        // 将缓冲区长度变为8的倍数
		len = ((len+7)>>3)<<3;
		// 在缓冲区中加密
		unsigned char* key=(unsigned char*)m_KeyDecrypt.GetBuffer(8);
		m_KeyDecrypt.ReleaseBuffer();
		m_aes2.init(Rijndael::ECB, Rijndael::Decrypt, (const RD_UINT8 *)key,Rijndael::Key32Bytes, 0);
		m_aes2.blockDecrypt((const RD_UINT8 *)databuf, len*8, (RD_UINT8 *)databuf);
		// 将密文写入输出文件
		_lwrite(fh_out,databuf,len);
	}
	_lclose(fh_in);
	_lclose(fh_out);
	AfxMessageBox("文件解密完毕!",MB_OK);
	
}

void CCryptAESDlg::OnButtonDecryptfolder() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	BROWSEINFO	bi; 
	ITEMIDLIST	*pidl; 
    char		Dir[256];

	memset(&bi, 0, sizeof(bi));
	bi.pszDisplayName = Dir; 
	bi.lpszTitle = "请选择目录"; 
	bi.ulFlags = BIF_RETURNONLYFSDIRS; 

	pidl = SHBrowseForFolder(&bi);
	SHGetPathFromIDList(pidl,Dir);
    GlobalFree(pidl);
	m_FolderDecrypt = Dir;	
	if(strlen(Dir)==3)
		m_FileDecryptOut = m_FolderDecrypt+m_FileNameDecrypt+".aes";
	else
		m_FileDecryptOut = m_FolderDecrypt+"\\"+m_FileNameDecrypt+".aes";
	UpdateData(false);	
}


