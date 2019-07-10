// DlgOutputToolbar.cpp: 구현 파일
//

#include "stdafx.h"
#include "I-RES2.h"
#include "DlgOutputToolbar.h"
#include "afxdialogex.h"


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


void CDlgOutputToolbar::OnBnClickedMfcbuttonIceExcel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CDlgOutputToolbar::OnBnClickedMfcbuttonIcdGraph()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CDlgOutputToolbar::OnBnClickedMfcbuttonSpeedExcel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CDlgOutputToolbar::OnBnClickedMfcbuttonSpeedGraph()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
