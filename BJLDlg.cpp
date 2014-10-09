// BJLDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BJL.h"
#include "BJLDlg.h"
#include "AnalyzePic.h"
#include "MsgDlg.h"
#include <math.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CBJLDlg dialog




CBJLDlg::CBJLDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBJLDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_paintPicIndex = 0;
	m_bCopyFile = true;
	m_bTip = true;
}

void CBJLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBJLDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_ANALYZE, &CBJLDlg::OnBnClickedAnalyze)
	ON_BN_CLICKED(IDC_SEARCH, &CBJLDlg::OnBnClickedSearch)
	ON_BN_CLICKED(IDC_BUTTON1, &CBJLDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CBJLDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CBJLDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CBJLDlg message handlers

BOOL CBJLDlg::OnInitDialog()
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
	Load();
	ShowCollectPicNum();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBJLDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CBJLDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		CPaintDC dc(this);
		CFileFind ff;
		if (ff.FindFile(m_searchPic))
		{
			CRect rc;
			GetDlgItem(IDC_CMPPIC)->GetWindowRect(rc);
			ScreenToClient(rc);
			CImage img;
			HRESULT hr = img.Load(m_searchPic);
			if (SUCCEEDED(hr))
			{
				img.Draw(dc.GetSafeHdc(),rc);
			}
		}
		
		for (size_t i=m_paintPicIndex; i<m_matchIDToPicVec.size() && i<m_paintPicIndex+2; ++i)
		{
			CString strmatchPic = GetDataDir()+m_matchIDToPicVec[i].second;
			CFileFind ff;
			if (ff.FindFile(strmatchPic))
			{
				DWORD debugID = (i%2==0)?IDC_DEBUG_SHOWID1:IDC_DEBUG_SHOWID2;
				GetDlgItem(debugID)->SetWindowText(GetChineseID(m_matchIDToPicVec[i].first));

				DWORD ctrlID = (i%2==0)?IDC_PIC1:IDC_PIC2;
				CRect rc;
				GetDlgItem(ctrlID)->GetWindowRect(rc);
				ScreenToClient(rc);

				CImage img;
				img.Load(strmatchPic);
				img.Draw(dc.GetSafeHdc(),rc);
			}
		}
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBJLDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CBJLDlg::EnableBtn(BOOL bEnable)
{
	GetDlgItem(IDC_ANALYZE)->EnableWindow(bEnable);
	GetDlgItem(IDC_SEARCH)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON1)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(bEnable);
}

void CBJLDlg::ShowMsg(const CString& strMsg)
{
	GetDlgItem(IDC_SHOWMSG)->SetWindowText(strMsg);
}

void CBJLDlg::ShowCollectPicNum()
{
	CString strNum;
	strNum.Format(_T("共有%d张样例图"),m_IDToFileMap.GetCount());
	GetDlgItem(IDC_SHOWMSG)->SetWindowText(strNum);
}

void CBJLDlg::MsgBox(const CString& strMsg)
{
	//MessageBox(strMsg);
	if (m_bTip)
	{
		CMsgDlg dlg(strMsg);
		dlg.DoModal();
		m_bTip = dlg.m_bTip;
	}
}

CString CBJLDlg::GetDBFile()
{
	return GetDataDir()+_T("db.dat");
}

CString	CBJLDlg::GetDataDir()
{
	return GetModulePath()+_T("data\\");
}

CString CBJLDlg::GetModulePath()
{
	//得到系统路径
	TCHAR f[MAX_PATH];
	GetModuleFileName(NULL,f,MAX_PATH);
	CString appDir(f);
	int pos = appDir.ReverseFind('\\');
	appDir = appDir.Left(pos + 1);
	return appDir;
}

void CBJLDlg::Save()
{
	CFile f;
	if (!f.Open(GetDBFile(),CFile::modeCreate|CFile::modeWrite))
	{
		MsgBox(_T("保存失败"));
		return;
	}
	
	CArchive ar(&f,CArchive::store);
	m_IDToFileMap.Serialize(ar);
}

void CBJLDlg::Load()
{
	CFile f;
	if (!f.Open(GetDBFile(),CFile::modeRead))
	{
		return;
	}

	CArchive ar(&f,CArchive::load);
	m_IDToFileMap.Serialize(ar);
}

UINT Analyze(LPVOID pParam)
{
	CBJLDlg* pDlg = (CBJLDlg*)(pParam);
	pDlg->EnableBtn(FALSE);
	
	CAnalyzePic analyzePic;
	size_t cnt = pDlg->m_strPathVec.size();
	for (size_t i=0; i<cnt; ++i)
	{
		CString strFilePath = pDlg->m_strPathVec[i];
		CString strFileName = strFilePath.Right(strFilePath.GetLength()-strFilePath.ReverseFind('\\')-1);
		CString strMsg,strFindFile;
		strMsg.Format(_T("正在处理文件:%s    %d/%d"),strFileName,i+1,cnt);
		pDlg->ShowMsg(strMsg);
		CString strID = analyzePic.AnalyzePic(strFilePath);
		if (strID.IsEmpty())
		{
			pDlg->MsgBox(_T("分析图片失败！"));
		}
		else if (pDlg->m_IDToFileMap.Lookup(strID,strFindFile))
		{
			pDlg->MsgBox(_T("该图片的图案已经存在！"));
		}
		else
		{
			if (pDlg->m_bCopyFile)
			{
				if (CopyFile(strFilePath,pDlg->GetDataDir()+strFileName,TRUE))
				{
					pDlg->m_IDToFileMap.SetAt(strID,strFileName);
				}
				else
				{
					pDlg->MsgBox(_T("存在相同名称的图片！"));
				}
			}
			else
			{
				pDlg->m_IDToFileMap.SetAt(strID,strFileName);
			}
			
		}
	}
	pDlg->Save();
	pDlg->ShowMsg(_T("完成！"));
	pDlg->EnableBtn(TRUE);
	Sleep(2000);
	pDlg->ShowCollectPicNum();
	return 0;
}

void CBJLDlg::OnBnClickedAnalyze()
{
	// TODO: Add your control notification handler code here
	m_strPathVec.clear();
	CFileDialog loadDlg(true,_T("*.png"),_T(""),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT,_T("图片文件 (*.jpg;*.png;*bmp)|*.jpg;*.png;*bmp|所有文件 (*.*)|*.*||"));
	//最多可以打开500个文件  
	loadDlg.m_ofn.nMaxFile = 500 * MAX_PATH;  
     
	TCHAR* ch = new TCHAR[loadDlg.m_ofn.nMaxFile];  
	loadDlg.m_ofn.lpstrFile = ch;  
 
	//对内存块清零  
	ZeroMemory(loadDlg.m_ofn.lpstrFile,sizeof(TCHAR) * loadDlg.m_ofn.nMaxFile);  


	if(loadDlg.DoModal()==IDOK)
	{
		POSITION pos = loadDlg.GetStartPosition();
		while (pos)
		{
			m_strPathVec.push_back(loadDlg.GetNextPathName(pos));
		}
		m_bCopyFile = true;
		AfxBeginThread(Analyze,this);
	}
}

void CBJLDlg::OnBnClickedSearch()
{
	// TODO: Add your control notification handler code here
	CFileDialog loadDlg(true,_T("*.png"),_T(""),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("图片文件 (*.jpg;*.png;*bmp)|*.jpg;*.png;*bmp|所有文件 (*.*)|*.*||"));
	if(loadDlg.DoModal()==IDOK)
	{
		m_paintPicIndex = 0;
		m_matchIDToPicVec.clear();
		m_searchPic = loadDlg.GetPathName();
		CAnalyzePic analyzePic;
		CString strID = analyzePic.AnalyzePic(m_searchPic);
		if (strID.IsEmpty())
		{
			MessageBox(_T("输入的图片分析错误！"));
			return;
		}
		
		
		GetDlgItem(IDC_DEBUG_SHOWID)->SetWindowText(GetChineseID(strID));

		POSITION pos = NULL;
		for(pos=m_IDToFileMap.GetStartPosition(); pos!=NULL;)
		{
			CString strKey,strPic;
			m_IDToFileMap.GetNextAssoc(pos, strKey, strPic);
			if (strKey.Find(strID)!=-1)
			{
				m_matchIDToPicVec.push_back(std::make_pair(strKey,strPic));
			}
		}
		ShowPage();
		Invalidate();
		if (m_matchIDToPicVec.empty())
		{
			MessageBox(_T("未找到匹配的图案！"));
		}
	}
}

CString CBJLDlg::GetChineseID(const CString& strID)
{
	CString strTmp(strID);
	strTmp.Replace(_T("r"),_T("红"));
	strTmp.Replace(_T("0"),_T("红绿"));
	strTmp.Replace(_T("b"),_T("蓝"));
	strTmp.Replace(_T("1"),_T("蓝绿"));
	return strTmp;
}



void CBJLDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	if (m_paintPicIndex-2<0)
	{
		return;
	}
	m_paintPicIndex-=2;
	ShowPage();
}

void CBJLDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	if (m_paintPicIndex+2>=m_matchIDToPicVec.size())
	{
		return;
	}
	m_paintPicIndex+=2;
	ShowPage();
}

void CBJLDlg::ShowPage()
{
	int total = ceil(m_matchIDToPicVec.size()/2.0);
	CString strShowPage;
	strShowPage.Format(_T("%d/%d页"),m_paintPicIndex/2+1,total);
	GetDlgItem(IDC_SHOWPAGE)->SetWindowText(strShowPage);
	Invalidate();
}

void CBJLDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	m_strPathVec.clear();
	CString strDataDir = GetDataDir();
	CFileFind ff;
	BOOL bFind = ff.FindFile(strDataDir+_T("*.jpg"));
	while(bFind)
	{
		bFind = ff.FindNextFile();
		m_strPathVec.push_back(ff.GetFilePath());
	}

	bFind = ff.FindFile(strDataDir+_T("*.png"));
	while(bFind)
	{
		bFind = ff.FindNextFile();
		m_strPathVec.push_back(ff.GetFilePath());
	}

	bFind = ff.FindFile(strDataDir+_T("*.bmp"));
	while(bFind)
	{
		bFind = ff.FindNextFile();
		m_strPathVec.push_back(ff.GetFilePath());
	}
	m_bCopyFile = false;
	m_IDToFileMap.RemoveAll();
	AfxBeginThread(Analyze,this);

}
