#pragma once
#include <afxdialogex.h>
#include "afxwin.h"
#include "afxcmn.h"

extern "C"
{
#include <modbus.h>
}

class CRTUDlg :public CDialogEx
{
	DECLARE_DYNAMIC(CRTUDlg)

public:
	CRTUDlg(CWnd* pParent = nullptr);
	virtual ~CRTUDlg();

#ifdef AFX_DESIGN_TIME
	enum {IDD = IDD_RTU_DLG};
#endif // AFX_DESIGN_TIME

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	//시리얼 설정값
	CString m_strPortName;
	CString m_strBaud;
	CString m_strParity;
	CString m_strDataBits;
	CString m_strStopBits;

	CString m_strUnitId;
	CString m_strFuncCode;
	CString m_strStartAddr;
	CString m_strQuantity;

	CComboBox m_comboPort;
	CComboBox m_comboBaud;
	CComboBox m_comboParity;
	CComboBox m_comboDataBits;
	CComboBox m_comboStopBits;

	CListCtrl m_listResult;
	int m_nTxCount;
	BOOL m_bTimerStarted;

	void DoModbusRead();
	void UpdateTxInfo();

	int m_nDisplayMode;

public:
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

