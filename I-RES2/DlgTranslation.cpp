// DlgTranslation.cpp: 구현 파일
//

#include "stdafx.h"
#include "I-RES2.h"
#include "DlgTranslation.h"
#include "afxdialogex.h"


// CDlgTranslation 대화 상자

IMPLEMENT_DYNAMIC(CDlgTranslation, CDialog)

CDlgTranslation::CDlgTranslation(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_TRANSLATE, pParent)
	, m_strCaption(_T(""))
	, m_strVector(_T(""))
{

}

CDlgTranslation::~CDlgTranslation()
{
}

void CDlgTranslation::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_CAPTION, m_strCaption);
	DDX_Text(pDX, IDC_EDIT1, m_strVector);
}


BEGIN_MESSAGE_MAP(CDlgTranslation, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgTranslation 메시지 처리기


BOOL CDlgTranslation::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_strCaption = "   << Select a start point for the translation vector or enter X, Y, Z ";
	UpdateData(FALSE);

	CRect rect;
	GetClientRect(&rect);
	ResizeControl(rect.Width(), rect.Height());
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgTranslation::ResizeControl(int cx, int cy)
{
	CWnd* p_cpation = GetDlgItem(IDC_STATIC_CAPTION);
	if (p_cpation)
	{
		int width = 400;
		int button_width = 40;
		p_cpation->MoveWindow(0, 0, width, cy);
		GetDlgItem(IDC_EDIT1)->MoveWindow(width, 0, cx-width - button_width, cy);
		GetDlgItem(IDC_BUTTON_OK)->MoveWindow(cx - button_width, 0, button_width, cy);
	}
}

void CDlgTranslation::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	ResizeControl(cx, cy);
}
