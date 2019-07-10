// DlgExcelView.cpp: 구현 파일
//

#include "stdafx.h"
#include "I-RES2.h"
#include "DlgExcelView.h"
#include "afxdialogex.h"


// CDlgExcelView 대화 상자

IMPLEMENT_DYNAMIC(CDlgExcelView, CDialog)

CDlgExcelView::CDlgExcelView(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_EXCEL, pParent)
{

}

CDlgExcelView::~CDlgExcelView()
{
}

void CDlgExcelView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgExcelView, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgExcelView 메시지 처리기


BOOL CDlgExcelView::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_wndExcelView.AttachGrid(this, IDC_GRID1);
	m_wndExcelView.m_pParentDlg = this;
	m_wndExcelView.SetVScrollMode(UG_SCROLLTRACKING);
	m_wndExcelView.SetHScrollMode(UG_SCROLLTRACKING);

	CRect rect;
	GetClientRect(&rect);
	SetSize(rect.Width(), rect.Height());

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgExcelView::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	SetSize(cx, cy);
}

void CDlgExcelView::SetSize(int cx, int cy)
{
	if (m_wndExcelView)
	{
		int btn_height = 25;
		int btn_width = 70;
		int btn_offset = 5;

		m_wndExcelView.MoveWindow(0, 0, cx, cy);
		//m_wndExcelView.MoveWindow(0, 0, cx, cy - (btn_height + btn_offset));
		//GetDlgItem(IDOK)->MoveWindow(cx - (btn_width * 2 + btn_offset), cy - btn_height, btn_width, btn_height);
		//GetDlgItem(IDCANCEL)->MoveWindow(cx - btn_width, cy - btn_height, btn_width, btn_height);
	}
}
