#pragma once
#include <afxdialogex.h>
class CTITLEDlg :public CDialogEx
{
	DECLARE_DYNAMIC(CTITLEDlg)

public:
	CTITLEDlg(CWnd* pParent = nullptr);
	virtual ~CTITLEDlg();

	//���̾�α� ������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TITLE_DLG };
#endif // AFX_DESIGN_TIME

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()

	// �ΰ��̹��� Ÿ��Ʋ ǥ��
	CStatic m_staticLogo;

	// ���۹�ư
	CButton m_btnStart;

public:
	afx_msg void OnBnClickedButtonStartTitle();

};

