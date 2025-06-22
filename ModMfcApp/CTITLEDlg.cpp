#include "CTITLEDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "ModMfcAppDlg.h"

IMPLEMENT_DYNAMIC(CTITLEDlg, CDialogEx)

CTITLEDlg::CTITLEDlg(CWnd* pParent) : CDialogEx(IDD_TITLE_DLG, pParent)
{

}

CTITLEDlg::~CTITLEDlg()
{

}

void CTITLEDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_LOGO, m_staticLogo);
	DDX_Control(pDX, IDC_BUTTON_START_TITLE, m_btnStart);
}

BEGIN_MESSAGE_MAP(CTITLEDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_START_TITLE, &CTITLEDlg::OnBnClickedButtonStartTitle)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL CTITLEDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 위치(x,y) 크기 (w,h)
	MoveWindow(100, 100, 1200, 600);

    HBITMAP hBmpLogo = (HBITMAP)::LoadImage(
        AfxGetInstanceHandle(),
        MAKEINTRESOURCE(IDB_LOGO),
        IMAGE_BITMAP,
        0, 0,
        LR_CREATEDIBSECTION
    );

    if (hBmpLogo == nullptr)
    {
        AfxMessageBox(_T("로고 이미지를 로드할 수 없습니다."));
    }
    else
    {
        // Picture Control 스타일을 Bitmap으로 변경 후 이미지 설정
        m_staticLogo.ModifyStyle(0, SS_BITMAP);
        m_staticLogo.SetBitmap(hBmpLogo);
    }

    // 4. 로고 이미지 가운데 정렬 (동적으로 위치 계산)
    CRect rcDlg, rcLogo;
    GetClientRect(&rcDlg);
    m_staticLogo.GetWindowRect(&rcLogo);
    ScreenToClient(&rcLogo);
    int x = (rcDlg.Width() - rcLogo.Width()) / 2;
    int y = (rcDlg.Height() - rcLogo.Height()) / 2;
    m_staticLogo.SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE 반환
}

BOOL CTITLEDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rc;
	GetClientRect(&rc);
	// 배경색을 연한 회색(RGB 240,240,240)으로 채움
	pDC->FillSolidRect(rc, RGB(255, 255, 255));
	return TRUE;  // TRUE 반환하여 기본 배경 그리기를 하지 않음
}

void CTITLEDlg::OnBnClickedButtonStartTitle()
{
    AfxMessageBox(_T("시작버튼 클릭됨"));

    CWnd* pParent = GetOwner();

    if (pParent)
    {
        pParent->SendMessage(WM_USER_TITLE_CLOSED, 0, 0);
    }
    else
    {
        AfxMessageBox(_T("부모 창이 없습니다"));
    }
}