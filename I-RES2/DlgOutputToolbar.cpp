// DlgOutputToolbar.cpp: 구현 파일
//

#include "stdafx.h"
#include "I-RES2.h"
#include "DlgOutputToolbar.h"
#include "afxdialogex.h"
#include "DlgExcelView.h"
#include "DlgChartView.h"

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
	DDX_Control(pDX, IDC_MFCBUTTON_EXCEL1, m_btnExcel1);
	DDX_Control(pDX, IDC_MFCBUTTON_GRAPH1, m_btnGraph1);
	DDX_Control(pDX, IDC_MFCBUTTON_EXCEL2, m_btnExcel2);
	DDX_Control(pDX, IDC_MFCBUTTON_GRAPH2, m_btnGraph2);
}


BEGIN_MESSAGE_MAP(CDlgOutputToolbar, CDialog)
	//ON_BN_CLICKED(IDC_MFCBUTTON_ICE_EXCEL, &CDlgOutputToolbar::OnBnClickedMfcbuttonIceExcel)
	//ON_BN_CLICKED(IDC_MFCBUTTON_ICD_GRAPH, &CDlgOutputToolbar::OnBnClickedMfcbuttonIcdGraph)
	//ON_BN_CLICKED(IDC_MFCBUTTON_SPEED_EXCEL, &CDlgOutputToolbar::OnBnClickedMfcbuttonSpeedExcel)
	//ON_BN_CLICKED(IDC_MFCBUTTON_SPEED_GRAPH, &CDlgOutputToolbar::OnBnClickedMfcbuttonSpeedGraph)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_MFCBUTTON_EXCEL1, &CDlgOutputToolbar::OnBnClickedMfcbuttonExcel1)
	ON_BN_CLICKED(IDC_MFCBUTTON_GRAPH1, &CDlgOutputToolbar::OnBnClickedMfcbuttonGraph1)
	ON_BN_CLICKED(IDC_MFCBUTTON_EXCEL2, &CDlgOutputToolbar::OnBnClickedMfcbuttonExcel2)
	ON_BN_CLICKED(IDC_MFCBUTTON_GRAPH2, &CDlgOutputToolbar::OnBnClickedMfcbuttonGraph2)
END_MESSAGE_MAP()


// CDlgOutputToolbar 메시지 처리기

#include "DlgExcelView.h"


void CDlgOutputToolbar::ResizeControl(int cx, int cy)
{
	int width = 28;
	int top = 0;

	if (m_btnExcel1)
	{
		m_btnExcel1.MoveWindow(0, top, width, width);
		top += width;
		m_btnGraph1.MoveWindow(0, top, width, width);
		top += width;
		top += 10;
		m_btnExcel2.MoveWindow(0, top, width, width);
		top += width;
		m_btnGraph2.MoveWindow(0, top, width, width);
	}
}

BOOL CDlgOutputToolbar::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetButtons(&m_btnExcel1, IDB_BITMAP_EXCEL, "OutPut");
	SetButtons(&m_btnGraph1, IDB_BITMAP_GRAPH, "OutPut");
	SetButtons(&m_btnExcel2, IDB_BITMAP_EXCEL, "Attainable_Speed");
	SetButtons(&m_btnGraph2, IDB_BITMAP_GRAPH, "Attainable_Speed");

	int width = 28;
	int top = 0;

	m_btnExcel1.MoveWindow(0, top, width, width);
	top += width;
	m_btnGraph1.MoveWindow(0, top, width, width);
	top += width;
	top += 10;
	m_btnExcel2.MoveWindow(0, top, width, width);
	top += width;
	m_btnGraph2.MoveWindow(0, top, width, width);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgOutputToolbar::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	ResizeControl(cx, cy);
}

const int NSIZE = 101;

void CDlgOutputToolbar::OnBnClickedMfcbuttonExcel1()
{
	CDlgExcelView pDlg;
	pDlg.m_strJobName = this->m_strJobName;
	pDlg.m_iType = 0;
	//pDlg.m_strInputFile = m_aAnalysisOutput[index1];
	pDlg.DoModal();
}


void CDlgOutputToolbar::OnBnClickedMfcbuttonGraph1()
{
	CDlgChartView pDlg;
	pDlg.m_strJobName = this->m_strJobName;
	pDlg.m_iType = 0;
	pDlg.DoModal();
}


void CDlgOutputToolbar::OnBnClickedMfcbuttonExcel2()
{
	CDlgExcelView pDlg;
	pDlg.m_strJobName = this->m_strJobName;
	pDlg.m_iType = 1;
	//pDlg.m_strInputFile = m_aAnalysisOutput[index1];
	pDlg.DoModal();
}


void CDlgOutputToolbar::OnBnClickedMfcbuttonGraph2()
{
	CString job_file;

	job_file = m_strProjectPath + "\\JOB\\" + m_strJobName + "\\satellite_to_map.out";
	if (PathFileExists(job_file))
	{
		CFileDialog dlg(FALSE,
			NULL,
			NULL,
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			"Image Files (*.bmp)|*.bmp|All Files (*.*)|*.*||",
			NULL);
		if (dlg.DoModal() == IDOK)
		{
			m_pView->SaveMap(dlg.GetPathName());
		}
	}
	else
	{
		CDlgChartView pDlg;
		pDlg.m_strJobName = this->m_strJobName;
		pDlg.m_iType = 1;
		pDlg.DoModal();
	}
}
