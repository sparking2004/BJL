#pragma once


// CMsgDlg �Ի���

class CMsgDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMsgDlg)

public:
	CMsgDlg(const CString& strTip, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMsgDlg();

// �Ի�������
	enum { IDD = IDD_MSGDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bTip;
	CString m_strTip;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
