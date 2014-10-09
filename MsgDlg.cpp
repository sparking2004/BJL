// MsgDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BJL.h"
#include "MsgDlg.h"
#include "afxdialogex.h"


// CMsgDlg �Ի���

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


// CMsgDlg ��Ϣ�������


void CMsgDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	CDialogEx::OnOK();
}


BOOL CMsgDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	GetDlgItem(IDC_MSG)->SetWindowText(m_strTip);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
