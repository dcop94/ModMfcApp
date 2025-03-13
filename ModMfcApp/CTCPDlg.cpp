#include "CTCPDlg.h"
#include "ModMfcApp.h"
#include <string>

IMPLEMENT_DYNAMIC(CTCPDlg, CDialogEx)

CTCPDlg::CTCPDlg(CWnd* pParent)
	:CDialogEx(IDD_TCP_DLG, pParent)
	, m_strIP(_T(""))
	, m_strPort(_T(""))
	, m_strUnitId(_T(""))
	, m_strFuncCode(_T(""))
	, m_strQuantity(_T(""))
	, m_nTxCount(0)
	, m_bTimerStarted(FALSE)

{

}

CTCPDlg::~CTCPDlg()
{

}

void CTCPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_IP, m_strIP);
	DDX_Text(pDX, IDC_EDIT_PORT, m_strPort);
	DDX_Text(pDX, IDC_EDIT_UNITID, m_strUnitId);
	DDX_Text(pDX, IDC_EDIT_FUNC, m_strFuncCode);
	DDX_Text(pDX, IDC_EDIT_ADDR, m_strStartAddr);
	DDX_Text(pDX, IDC_EDIT_QUANTITY, m_strQuantity);
	
	DDX_Control(pDX, IDC_LIST_RESULT, m_listResult);
}

BEGIN_MESSAGE_MAP(CTCPDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_OK, &CTCPDlg::OnBnClickedButtonOk)
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CTCPDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ����Ʈ ��Ʈ�� �ʱ�ȭ ���� (����Ʈ ���, �÷� �߰� ��)
	m_listResult.ModifyStyle(0, LVS_REPORT);
	m_listResult.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listResult.DeleteAllItems();

	// �÷��߰�
	m_listResult.InsertColumn(0, _T("Alias"), LVCFMT_LEFT, 60);
	m_listResult.InsertColumn(1, _T("Address"), LVCFMT_LEFT, 80);
	m_listResult.InsertColumn(2, _T("Value"), LVCFMT_LEFT, 80);


	return TRUE;
}

void CTCPDlg::OnBnClickedButtonOk()
{
	// �ѹ��� �������� �б�
	UpdateData(TRUE);

	if (m_strIP.IsEmpty() || m_strPort.IsEmpty() || m_strFuncCode.IsEmpty() || m_strUnitId.IsEmpty() || m_strStartAddr.IsEmpty() || m_strQuantity.IsEmpty())
	{
		MessageBox(_T("��� �Է����ּ���"), _T("�Է� ����"), MB_ICONERROR);
		return;
	}

	DoModbusRead();

	m_nTxCount++;
	UpdateTxInfo();

	if (!m_bTimerStarted)
	{
		SetTimer(1, 1000, NULL);
		m_bTimerStarted = TRUE;
	}

}

void CTCPDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		UpdateData(TRUE);
		DoModbusRead();
		m_nTxCount++;
		UpdateTxInfo();
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CTCPDlg::DoModbusRead()
{
	int port = _ttoi(m_strPort);
	int unitId = _ttoi(m_strUnitId);
	int funcCode = _ttoi(m_strFuncCode);
	int startAddr = _ttoi(m_strStartAddr);
	int quantity = _ttoi(m_strQuantity);

	// IP ���ڿ� ASC
	CT2A asciiIP(m_strIP);
	std::string ip = asciiIP;

	// ������ TCP �������
	modbus_t* ctx = modbus_new_tcp(ip.c_str(), port);
	if (!ctx)
	{
		MessageBox(_T("������ �ʱ�ȭ ����"), _T("����"), MB_ICONERROR);
		return;
	}

	if (modbus_connect(ctx) == -1)
	{
		modbus_free(ctx);
		return;
	}

	// ���� ID����
	modbus_set_slave(ctx, unitId);

	// ��ûó��
	uint16_t response[256] = { 0 };
	int rc = -1;

	//Function Code ���� ��ûó��
	if (funcCode == 3)
	{
		rc = modbus_read_registers(ctx, startAddr, quantity, response);
	}
	else if (funcCode == 4)
	{
		rc = modbus_read_input_registers(ctx, startAddr, quantity, response);
	}
	else
	{
		MessageBox(_T("�������� �ʴ� ��� �ڵ��Դϴ�"), _T("����"), MB_ICONERROR);
		modbus_close(ctx);
		modbus_free(ctx);
		return;
	}
	
	if (rc > 0)
	{
		// ���� ����Ʈ ���� �����
		m_listResult.DeleteAllItems();

		// rc �� ��ŭ ���� �����͸� ����Ʈ�� �߰� (2����Ʈ ����)
		for (int i = 0; i < rc; i++)
		{
			CString strAlias;
			strAlias.Format(_T("%d"), i);
			int nItem = m_listResult.InsertItem(i, strAlias);

			CString strAddress;
			strAddress.Format(_T("%05d"), startAddr + i);
			m_listResult.SetItemText(nItem, 1, strAddress);

			CString strValue;
			strValue.Format(_T("0x%04X"), response[i]);
			m_listResult.SetItemText(nItem, 2, strValue);
		}
	}

	

	modbus_close(ctx);
	modbus_free(ctx);
}

void CTCPDlg::UpdateTxInfo()
{
	CString strInfo;

	int unitId = _ttoi(m_strUnitId);
	int funcCode = _ttoi(m_strFuncCode);
	strInfo.Format(_T("TX = %d / ID = %d / FC = %d"), m_nTxCount, unitId, funcCode);
	SetDlgItemText(IDC_STATIC_TXINFO, strInfo);
}
