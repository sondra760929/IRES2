// DlgOutputToolbar.cpp: 구현 파일
//

#include "stdafx.h"
#include "I-RES2.h"
#include "DlgOutputToolbar.h"
#include "afxdialogex.h"
#include "DlgExcelView.h"
#include "DlgGraph.h"

// CDlgOutputToolbar 대화 상자

IMPLEMENT_DYNAMIC(CDlgOutputToolbar, CDialog)

CDlgOutputToolbar::CDlgOutputToolbar(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_OUTPUT_TOOLBAR, pParent)
{

}

CDlgOutputToolbar::~CDlgOutputToolbar()
{
}

void CDlgOutputToolbar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCBUTTON_ICE_EXCEL, m_btnIceExcel);
	DDX_Control(pDX, IDC_MFCBUTTON_ICD_GRAPH, m_btnIceGraph);
	DDX_Control(pDX, IDC_MFCBUTTON_SPEED_EXCEL, m_btnSpeedExcel);
	DDX_Control(pDX, IDC_MFCBUTTON_SPEED_GRAPH, m_btnSpeedGraph);
}


BEGIN_MESSAGE_MAP(CDlgOutputToolbar, CDialog)
	ON_BN_CLICKED(IDC_MFCBUTTON_ICE_EXCEL, &CDlgOutputToolbar::OnBnClickedMfcbuttonIceExcel)
	ON_BN_CLICKED(IDC_MFCBUTTON_ICD_GRAPH, &CDlgOutputToolbar::OnBnClickedMfcbuttonIcdGraph)
	ON_BN_CLICKED(IDC_MFCBUTTON_SPEED_EXCEL, &CDlgOutputToolbar::OnBnClickedMfcbuttonSpeedExcel)
	ON_BN_CLICKED(IDC_MFCBUTTON_SPEED_GRAPH, &CDlgOutputToolbar::OnBnClickedMfcbuttonSpeedGraph)
END_MESSAGE_MAP()


// CDlgOutputToolbar 메시지 처리기

#include "DlgExcelView.h"
#include "DlgGraph.h"

void CDlgOutputToolbar::OnBnClickedMfcbuttonIceExcel()
{
	CDlgExcelView pDlg;
	pDlg.m_strJobName = this->m_strJobName;
	pDlg.m_iType = 0;
	pDlg.DoModal();
}


void CDlgOutputToolbar::OnBnClickedMfcbuttonIcdGraph()
{
	CDlgGraph pDlg;
	pDlg.m_strJobName = this->m_strJobName;
	pDlg.m_iType = 0;
	pDlg.DoModal();
}


void CDlgOutputToolbar::OnBnClickedMfcbuttonSpeedExcel()
{
	CDlgExcelView pDlg;
	pDlg.m_strJobName = this->m_strJobName;
	pDlg.m_iType = 1;
	pDlg.DoModal();
}


void CDlgOutputToolbar::OnBnClickedMfcbuttonSpeedGraph()
{
	CDlgGraph pDlg;
	pDlg.m_strJobName = this->m_strJobName;
	pDlg.m_iType = 1;
	pDlg.DoModal();
}


BOOL CDlgOutputToolbar::OnInitDialog()
{
	CDialog::OnInitDialog();

	CImage btn_image1;
	CImage btn_image2;
	CImage btn_image3;
	CImage btn_image4;
	btn_image1.LoadFromResource(AfxGetInstanceHandle(), IDB_BITMAP_EXCEL);
	btn_image2.LoadFromResource(AfxGetInstanceHandle(), IDB_BITMAP_GRAPH);
	btn_image3.LoadFromResource(AfxGetInstanceHandle(), IDB_BITMAP_EXCEL);
	btn_image4.LoadFromResource(AfxGetInstanceHandle(), IDB_BITMAP_GRAPH);
	HBITMAP hBit1;
	HBITMAP hBit2;
	HBITMAP hBit3;
	HBITMAP hBit4;
	hBit1 = btn_image1.Detach();
	hBit2 = btn_image2.Detach();
	hBit3 = btn_image3.Detach();
	hBit4 = btn_image4.Detach();

	m_btnIceExcel.SetImage(hBit1, TRUE, hBit1);
	m_btnIceGraph.SetImage(hBit2, TRUE, hBit2);
	m_btnSpeedExcel.SetImage(hBit3, TRUE, hBit3);
	m_btnSpeedGraph.SetImage(hBit4, TRUE, hBit4);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
