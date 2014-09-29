// BJLDlg.h : header file
//

#pragma once
#include <vector>

// CBJLDlg dialog
class CBJLDlg : public CDialog
{
// Construction
public:
	CBJLDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_BJL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAnalyze();
	afx_msg void OnBnClickedSearch();
public:
	void	Save();
	void	Load();
	void	EnableBtn(BOOL bEnable);
	void	ShowMsg(const CString& strMsg);
	void	MsgBox(const CString& strMsg);

	CString GetDBFile();
	CString	GetDataDir();
	CString GetModulePath();
	void	ShowPage();
	void	ShowCollectPicNum();
	CString GetChineseID(const CString& strID);
public:
	CMapStringToString m_IDToFileMap;
	std::vector<CString> m_strPathVec;
	std::vector<std::pair<CString,CString>> m_matchIDToPicVec;
	CString				 m_searchPic;
	int					 m_paintPicIndex;
	bool				 m_bCopyFile;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};
