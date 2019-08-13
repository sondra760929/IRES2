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
	//DDX_Control(pDX, IDC_MFCBUTTON_ICE_EXCEL, m_btnIceExcel);
	//DDX_Control(pDX, IDC_MFCBUTTON_ICD_GRAPH, m_btnIceGraph);
	//DDX_Control(pDX, IDC_MFCBUTTON_SPEED_EXCEL, m_btnSpeedExcel);
	//DDX_Control(pDX, IDC_MFCBUTTON_SPEED_GRAPH, m_btnSpeedGraph);
}


BEGIN_MESSAGE_MAP(CDlgOutputToolbar, CDialog)
	//ON_BN_CLICKED(IDC_MFCBUTTON_ICE_EXCEL, &CDlgOutputToolbar::OnBnClickedMfcbuttonIceExcel)
	//ON_BN_CLICKED(IDC_MFCBUTTON_ICD_GRAPH, &CDlgOutputToolbar::OnBnClickedMfcbuttonIcdGraph)
	//ON_BN_CLICKED(IDC_MFCBUTTON_SPEED_EXCEL, &CDlgOutputToolbar::OnBnClickedMfcbuttonSpeedExcel)
	//ON_BN_CLICKED(IDC_MFCBUTTON_SPEED_GRAPH, &CDlgOutputToolbar::OnBnClickedMfcbuttonSpeedGraph)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgOutputToolbar 메시지 처리기

#include "DlgExcelView.h"
#include "DlgGraph.h"

//void CDlgOutputToolbar::OnBnClickedMfcbuttonIceExcel()
//{
//	CDlgExcelView pDlg;
//	pDlg.m_strJobName = this->m_strJobName;
//	pDlg.m_iType = 0;
//	pDlg.DoModal();
//}
//
//
//void CDlgOutputToolbar::OnBnClickedMfcbuttonIcdGraph()
//{
//	CDlgGraph pDlg;
//	pDlg.m_strJobName = this->m_strJobName;
//	pDlg.m_iType = 0;
//	pDlg.DoModal();
//}
//
//
//void CDlgOutputToolbar::OnBnClickedMfcbuttonSpeedExcel()
//{
//	CDlgExcelView pDlg;
//	pDlg.m_strJobName = this->m_strJobName;
//	pDlg.m_iType = 1;
//	pDlg.DoModal();
//}
//
//
//void CDlgOutputToolbar::OnBnClickedMfcbuttonSpeedGraph()
//{
//	CDlgGraph pDlg;
//	pDlg.m_strJobName = this->m_strJobName;
//	pDlg.m_iType = 1;
//	pDlg.DoModal();
//}

void CDlgOutputToolbar::ResizeControl(int cx, int cy)
{
	for (int i = 0; i < m_aExcelButton.size(); i++)
	{
		m_aExcelButton[i]->MoveWindow(0, (i * 2)*cx, cx, cx);
		m_aGraphButton[i]->MoveWindow(0, (i * 2 + 1)*cx, cx, cx);
	}
}

BOOL CDlgOutputToolbar::OnInitDialog()
{
	CDialog::OnInitDialog();

	for (int i = 0; i < m_aAnalysisCommand.size(); i++)
	{
		CMFCButton* btn_excel = new CMFCButton();
		CMFCButton* btn_graph = new CMFCButton();

		btn_excel->Create(m_aAnalysisCommand[i], WS_CHILD | WS_VISIBLE, CRect(10, 10, 10, 10), this, i * 2 + 100);
		btn_graph->Create(m_aAnalysisCommand[i], WS_CHILD | WS_VISIBLE, CRect(10, 10, 10, 10), this, i * 2 + 101);
		btn_excel->ShowWindow(SW_SHOW);
		btn_graph->ShowWindow(SW_SHOW);

		SetButtons(btn_excel, IDB_BITMAP_EXCEL, m_aAnalysisCommand[i]);
		SetButtons(btn_graph, IDB_BITMAP_GRAPH, m_aAnalysisCommand[i]);

		m_aExcelButton.push_back(btn_excel);
		m_aGraphButton.push_back(btn_graph);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgOutputToolbar::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	ResizeControl(cx, cy);
}


BOOL CDlgOutputToolbar::OnCommand(WPARAM wParam, LPARAM lParam)
{
	int index = (int)wParam - 100;
	int index1 = index / 2;
	int index2 = (index - (index1 * 2));
	if (index1 > -1 && index1 < m_aExcelButton.size())
	{
		if (index2 == 1)
		{
			//	graph
			CDlgGraph pDlg;
			pDlg.m_strJobName = this->m_strJobName;
			pDlg.m_strInputFile = m_aAnalysisOutput[index1];
			pDlg.DoModal();
		}
		else
		{
			//	excel
			CDlgExcelView pDlg;
			pDlg.m_strJobName = this->m_strJobName;
			pDlg.m_strInputFile = m_aAnalysisOutput[index1];
			pDlg.DoModal();
		}
	}

	return CDialog::OnCommand(wParam, lParam);
}
