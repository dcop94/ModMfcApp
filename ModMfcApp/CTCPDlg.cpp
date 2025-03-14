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
	, m_strStartAddr(_T(""))
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

	DDX_Radio(pDX, IDC_RADIO_HEX, m_nDisplayMode);
}

BEGIN_MESSAGE_MAP(CTCPDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_START, &CTCPDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CTCPDlg::OnBnClickedButtonStop)
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CTCPDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 리스트 컨트롤 초기화 설정 (레포트 모드, 컬럼 추가 등)
	m_listResult.ModifyStyle(0, LVS_REPORT);
	m_listResult.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	//m_listResult.DeleteAllItems();

	//// 컬럼추가
	//m_listResult.InsertColumn(0, _T("Alias"), LVCFMT_LEFT, 60);
	//m_listResult.InsertColumn(1, _T("Address"), LVCFMT_LEFT, 80);
	//m_listResult.InsertColumn(2, _T("Value"), LVCFMT_LEFT, 80);


	return TRUE;
}

void CTCPDlg::OnBnClickedButtonStart()
{
	// 한번만 수동으로 읽기
	UpdateData(TRUE);

	if (m_strIP.IsEmpty() || m_strPort.IsEmpty() || m_strFuncCode.IsEmpty() || m_strUnitId.IsEmpty() || m_strStartAddr.IsEmpty() || m_strQuantity.IsEmpty())
	{
		MessageBox(_T("모두 입력해주세요"), _T("입력 오류"), MB_ICONERROR);
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


void CTCPDlg::OnBnClickedButtonStop()
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
	UpdateData(TRUE);

	int port = _ttoi(m_strPort);
	int unitId = _ttoi(m_strUnitId);
	int funcCode = _ttoi(m_strFuncCode);
	int startAddr = _ttoi(m_strStartAddr);
	int quantity = _ttoi(m_strQuantity);

	// IP 문자열 ASC
	CT2A asciiIP(m_strIP);
	std::string ip = asciiIP;

	// 모드버스 TCP 연결생성
	modbus_t* ctx = modbus_new_tcp(ip.c_str(), port);
	if (!ctx)
	{
		MessageBox(_T("모드버스 초기화 실패"), _T("오류"), MB_ICONERROR);
		return;
	}

	if (modbus_connect(ctx) == -1)
	{
		modbus_free(ctx);
		return;
	}

	// 유닛 ID설정
	modbus_set_slave(ctx, unitId);

	// 요청처리
	uint16_t response[256] = { 0 };
	int rc = -1;

	//Function Code 따른 요청처리
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
		MessageBox(_T("지원되지 않는 펑션 코드입니다"), _T("오류"), MB_ICONERROR);
		modbus_close(ctx);
		modbus_free(ctx);
		return;
	}

	modbus_close(ctx);
	modbus_free(ctx);
	
	if (rc <= 0)
	{
		// 기존 리스트 내용 지우기
		m_listResult.DeleteAllItems();
		return;


		//// rc 값 만큼 응답 데이터를 리스트에 추가 (2바이트 단위)
		//for (int i = 0; i < rc; i++)
		//{
		//	CString strAlias;
		//	strAlias.Format(_T("%d"), i);
		//	int nItem = m_listResult.InsertItem(i, strAlias);

		//	CString strAddress;
		//	strAddress.Format(_T("%05d"), startAddr + i);
		//	m_listResult.SetItemText(nItem, 1, strAddress);

		//	CString strValue;
		//	strValue.Format(_T("0x%04X"), response[i]);
		//	m_listResult.SetItemText(nItem, 2, strValue);
		//}
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
		m_listResult.InsertColumn(baseCol+1, colAddr, LVCFMT_LEFT, 80);
		m_listResult.InsertColumn(baseCol+2, colVal, LVCFMT_LEFT, 80);
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

void CTCPDlg::UpdateTxInfo()
{
	CString strInfo;

	int unitId = _ttoi(m_strUnitId);
	int funcCode = _ttoi(m_strFuncCode);
	strInfo.Format(_T("TX = %d / ID = %d / FC = %d"), m_nTxCount, unitId, funcCode);
	SetDlgItemText(IDC_STATIC_TXINFO, strInfo);
}





