#pragma once
#include <afxdialogex.h>
#include "afxwin.h"
#include "afxcmn.h"

extern "C"
{
	#include <modbus.h>
}

// CTCPDlg ��ȭ����
class CTCPDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTCPDlg)

public:
	CTCPDlg(CWnd* pParent = nullptr);
	virtual ~CTCPDlg();

// ��ȭ���� ������
#ifdef AFX_DESIGN_TIME
	enum {IDD = IDD_TCP_DLG};
#endif

protected :
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV ����
	virtual BOOL OnInitDialog(); // ��ȭ���� �ʱ�ȭ
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()

private:
	// ������� 
	CString m_strIP;
	CString m_strPort;
	CString m_strUnitId;
	CString m_strFuncCode;
	CString m_strStartAddr;
	CString m_strQuantity;

	// ��� ǥ�� List Control
	CListCtrl m_listResult;

	// TX COUNT
	int m_nTxCount;
	BOOL m_bTimerStarted;

	// ������ ��� �Լ�
	void DoModbusRead();

	void UpdateTxInfo();

public:
	
	afx_msg void OnBnClickedButtonOk();
};

