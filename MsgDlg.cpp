// MsgDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BJL.h"
#include "MsgDlg.h"
#include "afxdialogex.h"


// CMsgDlg 对话框

IMPLEMENT_DYNAMIC(CMsgDlg, CDialogEx)

CMsgDlg::CMsgDlg(const CString& strTip,CWnd* pParent /*=NULL*/)
	: CDialogEx(CMsgDlg::IDD, pParent)
	, m_bTip(FALSE),m_strTip(strTip)
{

}

CMsgDlg::~CMsgDlg()
{
}

void CMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_bTip);
}


BEGIN_MESSAGE_MAP(CMsgDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CMsgDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CMsgDlg 消息处理程序


void CMsgDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	CDialogEx::OnOK();
}


BOOL CMsgDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	GetDlgItem(IDC_MSG)->SetWindowText(m_strTip);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
