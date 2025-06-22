#pragma once
#include <afxdialogex.h>
class CTITLEDlg :public CDialogEx
{
	DECLARE_DYNAMIC(CTITLEDlg)

public:
	CTITLEDlg(CWnd* pParent = nullptr);
	virtual ~CTITLEDlg();

	//다이얼로그 데이터
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TITLE_DLG };
#endif // AFX_DESIGN_TIME

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()

	// 로고이미지 타이틀 표시
	CStatic m_staticLogo;

	// 시작버튼
	CButton m_btnStart;

public:
	afx_msg void OnBnClickedButtonStartTitle();

};

