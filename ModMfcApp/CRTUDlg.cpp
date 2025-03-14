#include "CRTUDlg.h"
#include "afxdialogex.h"
#include "ModMfcApp.h"
#include <string>

IMPLEMENT_DYNAMIC(CRTUDlg, CDialogEx)

CRTUDlg::CRTUDlg(CWnd* pParent)
	: CDialogEx(IDD_RTU_DLG, pParent)
	, m_strPortName(_T(""))
	, m_strBaud(_T(""))
	, m_strParity(_T(""))
	, m_strDataBits(_T(""))
	, m_strStopBits(_T(""))
	, m_strUnitId(_T(""))
	, m_strFuncCode(_T(""))
	, m_strStartAddr(_T(""))
	, m_strQuantity(_T(""))
	, m_nTxCount(0)
	, m_bTimerStarted(FALSE)
{

}

CRTUDlg::~CRTUDlg()
{

}

void CRTUDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO_PORT, m_comboPort);
	DDX_Control(pDX, IDC_COMBO_BAUD, m_comboBaud);
	DDX_Control(pDX, IDC_COMBO_PARITY, m_comboParity);
	DDX_Control(pDX, IDC_COMBO_DATABITS, m_comboDataBits);
	DDX_Control(pDX, IDC_COMBO_STOPBITS, m_comboStopBits);

	DDX_Text(pDX, IDC_EDIT_UNITID, m_strUnitId);
	DDX_Text(pDX, IDC_EDIT_FUNC, m_strFuncCode);
	DDX_Text(pDX, IDC_EDIT_ADDR2, m_strStartAddr);
	DDX_Text(pDX, IDC_EDIT_QUANTITY2, m_strQuantity);
	
	//리스트 컨트롤
	DDX_Control(pDX, IDC_LIST_RESULT, m_listResult);

	DDX_Radio(pDX, IDC_RADIO_HEX, m_nDisplayMode);
}

BEGIN_MESSAGE_MAP(CRTUDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_START, &CRTUDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CRTUDlg::OnBnClickedButtonStop)
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CRTUDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//포트
	m_comboPort.AddString(_T("COM1"));
	m_comboPort.AddString(_T("COM2"));
	m_comboPort.AddString(_T("COM3"));
	m_comboPort.AddString(_T("COM4"));
	m_comboPort.SetCurSel(0); // 기본 com1

	//전송속도
	m_comboBaud.AddString(_T("9600"));
	m_comboBaud.AddString(_T("19200"));
	m_comboBaud.AddString(_T("38400"));
	m_comboBaud.AddString(_T("115200"));
	m_comboBaud.SetCurSel(0); // 기본 9600

	//패리티
	m_comboParity.AddString(_T("None"));
	m_comboParity.AddString(_T("Even"));
	m_comboParity.AddString(_T("Odd"));
	m_comboParity.SetCurSel(0); // 기본 None

	//데이터비트
	m_comboDataBits.AddString(_T("7"));
	m_comboDataBits.AddString(_T("8"));
	m_comboDataBits.SetCurSel(1); // 기본 8

	//스탑비트
	m_comboStopBits.AddString(_T("1"));
	m_comboStopBits.AddString(_T("2"));
	m_comboStopBits.SetCurSel(0); // 기본 1

	// 리스트 컨트롤 초기화 설정 (레포트 모드, 컬럼 추가 등)
	m_listResult.ModifyStyle(0, LVS_REPORT);
	m_listResult.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// 기타 초기화
	m_nTxCount = 0;
	m_bTimerStarted = FALSE;

	return TRUE;
}

void CRTUDlg::OnBnClickedButtonStart()
{
	// 콤보박스 선택 문자열 가져오기
	int index;

	index = m_comboPort.GetCurSel();
	m_comboPort.GetLBText(index, m_strPortName);

	index = m_comboBaud.GetCurSel();
	m_comboBaud.GetLBText(index, m_strBaud);

	index = m_comboParity.GetCurSel();
	m_comboParity.GetLBText(index, m_strParity);

	index = m_comboDataBits.GetCurSel();
	m_comboDataBits.GetLBText(index, m_strDataBits);

	index = m_comboStopBits.GetCurSel();
	m_comboStopBits.GetLBText(index, m_strStopBits);

	UpdateData(TRUE);

	if (m_strPortName.IsEmpty() || m_strBaud.IsEmpty() || m_strParity.IsEmpty() || m_strDataBits.IsEmpty() || m_strStopBits.IsEmpty())
	{
		MessageBox(_T("모든 시리얼 설정을 선택해주세요"), _T("설정 오류"), MB_ICONERROR);
		return;
	}

	if (m_strFuncCode.IsEmpty() || m_strUnitId.IsEmpty() || m_strStartAddr.IsEmpty() || m_strQuantity.IsEmpty())
	{
		MessageBox(_T("모두 입력해주세요"), _T("입력 오류"), MB_ICONERROR);
		return;
	}

	m_nTxCount++;
	UpdateTxInfo();

	if (!m_bTimerStarted)
	{
		SetTimer(1, 1000, NULL);
		m_bTimerStarted = TRUE;
	}
	
}

void CRTUDlg::DoModbusRead()
{
	//RTU
	int baud = _ttoi(m_strBaud);
	int dataBits = _ttoi(m_strDataBits);
	int stopBits = _ttoi(m_strStopBits);

	//기본설정
	int unitId = _ttoi(m_strUnitId);
	int funcCode = _ttoi(m_strFuncCode);
	int startAddr = _ttoi(m_strStartAddr);
	int quantity = _ttoi(m_strQuantity);

	//패리티 문자 변환
	char parity;

	if (m_strParity.CompareNoCase(_T("None")) == 0)
	{
		parity = 'N';
	}
	else if (m_strParity.CompareNoCase(_T("Even")) == 0)
	{
		parity = 'E';
	}
	else if (m_strParity.CompareNoCase(_T("Odd")) == 0)
	{
		parity = 'O';
	}
	else
	{
		parity = 'N';
	}

	CT2A asciiPort(m_strPortName);
	std::string device = asciiPort;

	modbus_t* ctx = modbus_new_rtu(device.c_str(), baud, parity, dataBits, stopBits);

	if (!ctx)
	{
		MessageBox(_T("모드버스 RTU 컨텍스트 초기화 실패"), _T("오류"), MB_ICONERROR);
		return;
	}

	// slave id 설정
	if (modbus_set_slave(ctx, unitId) == -1)
	{
		MessageBox(_T("슬레이브 설정 실패"), _T("오류"), MB_ICONERROR);
		modbus_free(ctx);
		return;
	}

	// RTU 연결
	if (modbus_connect(ctx) == -1)
	{
		MessageBox(_T("RTU 연결실패"), _T("오류"), MB_ICONERROR);
		modbus_free(ctx);
		return;
	}

	// 통신 후 데이터 읽기
	uint16_t response[256] = { 0 };
	int rc = -1;

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
		MessageBox(_T("지원되지 않는 펑션 코드"), _T("오류"), MB_ICONERROR);
		modbus_close(ctx);
		modbus_free(ctx);
		return;
	}

	modbus_close(ctx);
	modbus_free(ctx);

	if (rc <= 0)
	{
		m_listResult.DeleteAllItems();
		return;
	}

	quantity = rc;

	// 리스트 컨트롤 초기화
	m_listResult.DeleteAllItems();

	// 기존 열 삭제
	int colCount = m_listResult.GetHeaderCtrl()->GetItemCount();

	for (int c = colCount - 1; c >= 0; c--)
	{
		m_listResult.DeleteColumn(c);
	}

	int sets = (quantity + 9) / 10;

	// 열생성
	for (int s = 0; s < sets; s++)
	{
		CString colAlias;
		colAlias.Format(_T("Alias[%d~%d"), s * 10, s * 10 + 9);

		CString colAddr;
		colAddr.Format(_T("Address[%d~%d"), s * 10, s * 10 + 9);

		CString colVal;
		colVal.Format(_T("Value[%d~%d"), s * 10, s * 10 + 9);

		int baseCol = s * 3;
		m_listResult.InsertColumn(baseCol, colAlias, LVCFMT_LEFT, 80);
		m_listResult.InsertColumn(baseCol + 1, colAddr, LVCFMT_LEFT, 80);
		m_listResult.InsertColumn(baseCol + 2, colVal, LVCFMT_LEFT, 80);
	}

	// 행 최대 10개
	int rows = (quantity < 10) ? quantity : 10;

	for (int i = 0; i < rows; i++)
	{
		int nItem = m_listResult.InsertItem(i, _T(""));

		for (int s = 0; s < sets; s++)
		{
			int regIndex = s * 10 + i;
			if (regIndex >= quantity)
			{
				continue;
			}

			// Alias
			CString strAlias;
			strAlias.Format(_T("%d"), regIndex);
			int colAlias = s * 3;
			m_listResult.SetItemText(nItem, colAlias, strAlias);

			// Address
			CString strAddr;
			strAddr.Format(_T("%05d"), startAddr + regIndex);
			int colAddr = s * 3 + 1;
			m_listResult.SetItemText(nItem, colAddr, strAddr);

			// Value
			CString strVal;
			switch (m_nDisplayMode)
			{
			case 0:
				strVal.Format(_T("0x%04X"), response[regIndex]);
				break;

			case 1:
			{
				int16_t val16 = static_cast<int16_t>(response[regIndex]);
				strVal.Format(_T("%d"), val16);
			}
			break;

			case 2:
			{
				if (regIndex + 1 < quantity)
				{
					uint32_t high = response[regIndex];
					uint32_t low = response[regIndex + 1];
					int32_t val32 = static_cast<int32_t>((high << 16) | (low & 0xFFFF));
					strVal.Format(_T("%d"), val32);
				}
				else
				{
					strVal = _T("-");
				}
			}

			break;
			}

			int colVal = s * 3 + 2;
			m_listResult.SetItemText(nItem, colVal, strVal);
		}
	}
}

void CRTUDlg::OnBnClickedButtonStop()
{
	// 정지
	if (m_bTimerStarted)
	{
		KillTimer(1);
		m_bTimerStarted = FALSE;
	}

	// TX 카운트 및 표시값 초기화
	m_nTxCount = 0;
	UpdateTxInfo();

	// 리스트초기화
	m_listResult.DeleteAllItems();
	int colCount = m_listResult.GetHeaderCtrl()->GetItemCount();
	for (int i = colCount - 1; i >= 0; i--)
	{
		m_listResult.DeleteColumn(i);
	}
}

void CRTUDlg::OnTimer(UINT_PTR nIDEvent)
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

void CRTUDlg::UpdateTxInfo()
{
	CString strInfo;

	int unitId = _ttoi(m_strUnitId);
	int funcCode = _ttoi(m_strFuncCode);
	strInfo.Format(_T("TX = %d / ID = %d / FC = %d"), m_nTxCount, unitId, funcCode);
	SetDlgItemText(IDC_STATIC_TXINFO, strInfo);
}