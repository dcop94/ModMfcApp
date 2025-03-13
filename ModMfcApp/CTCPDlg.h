#pragma once
#include <afxdialogex.h>
#include "afxwin.h"
#include "afxcmn.h"

extern "C"
{
	#include <modbus.h>
}

// CTCPDlg 대화상자
class CTCPDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTCPDlg)

public:
	CTCPDlg(CWnd* pParent = nullptr);
	virtual ~CTCPDlg();

// 대화상자 데이터
#ifdef AFX_DESIGN_TIME
	enum {IDD = IDD_TCP_DLG};
#endif

protected :
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV 지원
	virtual BOOL OnInitDialog(); // 대화상자 초기화
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()

private:
	// 멤버변수 
	CString m_strIP;
	CString m_strPort;
	CString m_strUnitId;
	CString m_strFuncCode;
	CString m_strStartAddr;
	CString m_strQuantity;

	// 결과 표시 List Control
	CListCtrl m_listResult;

	// TX COUNT
	int m_nTxCount;
	BOOL m_bTimerStarted;

	// 모드버스 통신 함수
	void DoModbusRead();

	void UpdateTxInfo();

public:
	
	afx_msg void OnBnClickedButtonOk();
};

