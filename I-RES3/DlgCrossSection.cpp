// DlgCrossSection.cpp: 구현 파일
//

#include "stdafx.h"
#include "I-RES3.h"
#include "DlgCrossSection.h"
#include "afxdialogex.h"


// CDlgCrossSection 대화 상자

IMPLEMENT_DYNAMIC(CDlgCrossSection, CDialog)

CDlgCrossSection::CDlgCrossSection(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_CROSSSECTION, pParent)
{

}

CDlgCrossSection::~CDlgCrossSection()
{
}

void CDlgCrossSection::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCBUTTON_CALCULATE, m_btnCalc);
}


BEGIN_MESSAGE_MAP(CDlgCrossSection, CDialog)
END_MESSAGE_MAP()


// CDlgCrossSection 메시지 처리기


BOOL CDlgCrossSection::OnInitDialog()
{
	CDialog::OnInitDialog();

	CImage btn_image1;
	btn_image1.LoadFromResource(AfxGetInstanceHandle(), IDB_BITMAP_CALC);
	HBITMAP hBit1;
	hBit1 = btn_image1.Detach();
	m_btnCalc.SetImage(hBit1, TRUE, hBit1);
	m_btnCalc.SetTooltip(_T("Calculate Section Points"));
	m_btnCalc.EnableFullTextTooltip(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
