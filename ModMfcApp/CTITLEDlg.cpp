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

	// ��ġ(x,y) ũ�� (w,h)
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
        AfxMessageBox(_T("�ΰ� �̹����� �ε��� �� �����ϴ�."));
    }
    else
    {
        // Picture Control ��Ÿ���� Bitmap���� ���� �� �̹��� ����
        m_staticLogo.ModifyStyle(0, SS_BITMAP);
        m_staticLogo.SetBitmap(hBmpLogo);
    }

    // 4. �ΰ� �̹��� ��� ���� (�������� ��ġ ���)
    CRect rcDlg, rcLogo;
    GetClientRect(&rcDlg);
    m_staticLogo.GetWindowRect(&rcLogo);
    ScreenToClient(&rcLogo);
    int x = (rcDlg.Width() - rcLogo.Width()) / 2;
    int y = (rcDlg.Height() - rcLogo.Height()) / 2;
    m_staticLogo.SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE ��ȯ
}

BOOL CTITLEDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rc;
	GetClientRect(&rc);
	// ������ ���� ȸ��(RGB 240,240,240)���� ä��
	pDC->FillSolidRect(rc, RGB(255, 255, 255));
	return TRUE;  // TRUE ��ȯ�Ͽ� �⺻ ��� �׸��⸦ ���� ����
}

void CTITLEDlg::OnBnClickedButtonStartTitle()
{
    AfxMessageBox(_T("���۹�ư Ŭ����"));

    CWnd* pParent = GetOwner();

    if (pParent)
    {
        pParent->SendMessage(WM_USER_TITLE_CLOSED, 0, 0);
    }
    else
    {
        AfxMessageBox(_T("�θ� â�� �����ϴ�"));
    }
}