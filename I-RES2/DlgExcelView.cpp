// DlgExcelView.cpp: 구현 파일
//

#include "stdafx.h"
#include "I-RES2.h"
#include "DlgExcelView.h"
#include "afxdialogex.h"
#include "OptImportExportBase.h"

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
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CDlgExcelView::OnBnClickedButtonSave)
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
	int row_count = 0;
	int col_count = 0;
	if (m_iType == 0)
	{
		CString job_file;
		job_file = m_strProjectPath + "\\JOB\\" + m_strJobName + "\\ice_result.OUT";
		if (PathFileExists(job_file))
		{
			m_wndExcelView.SetNumberCols(7);
			col_count = 7;
			FILE* fp;
			fopen_s(&fp, job_file, "rt");
			if (fp)
			{
				COptImportExportBase ifp;
				ifp.m_fp_input = fp;
				ifp.m_array_strSplit.push_back(' ');
				if (ifp.ReadOneLineFromFile() > 6)
				{
					m_wndExcelView.QuickSetText(0, -1, ifp.m_array_strOutput[0]);
					m_wndExcelView.QuickSetText(1, -1, ifp.m_array_strOutput[1]);
					m_wndExcelView.QuickSetText(2, -1, ifp.m_array_strOutput[2]);
					m_wndExcelView.QuickSetText(3, -1, ifp.m_array_strOutput[3]);
					m_wndExcelView.QuickSetText(4, -1, ifp.m_array_strOutput[4]);
					m_wndExcelView.QuickSetText(5, -1, ifp.m_array_strOutput[5]);
					m_wndExcelView.QuickSetText(6, -1, ifp.m_array_strOutput[6]);
				}
				while (ifp.ReadOneLineFromFile() > 6)
				{
					row_count++;
					m_wndExcelView.SetNumberRows(row_count);
					m_wndExcelView.QuickSetNumber(-1, row_count - 1, row_count);
					m_wndExcelView.QuickSetText(0, row_count-1, ifp.m_array_strOutput[0]);
					m_wndExcelView.QuickSetText(1, row_count-1, ifp.m_array_strOutput[1]);
					m_wndExcelView.QuickSetText(2, row_count-1, ifp.m_array_strOutput[2]);
					m_wndExcelView.QuickSetText(3, row_count-1, ifp.m_array_strOutput[3]);
					m_wndExcelView.QuickSetText(4, row_count-1, ifp.m_array_strOutput[4]);
					m_wndExcelView.QuickSetText(5, row_count-1, ifp.m_array_strOutput[5]);
					m_wndExcelView.QuickSetText(6, row_count-1, ifp.m_array_strOutput[6]);
				}
			}
		}
	}	
	else if (m_iType == 1)
	{
		CString job_file;
		job_file = m_strProjectPath + "\\JOB\\" + m_strJobName + "\\Attainable_speed.out";
		if (PathFileExists(job_file))
		{
			m_wndExcelView.SetNumberCols(5);
			col_count = 5;
			FILE* fp;
			fopen_s(&fp, job_file, "rt");
			if (fp)
			{
				COptImportExportBase ifp;
				ifp.m_fp_input = fp;
				ifp.m_array_strSplit.push_back(' ');
				ifp.m_array_strSplit.push_back(',');
				if (ifp.ReadOneLineFromFile() > 4)
				{
					m_wndExcelView.QuickSetText(0, -1, ifp.m_array_strOutput[0]);
					m_wndExcelView.QuickSetText(1, -1, ifp.m_array_strOutput[1]);
					m_wndExcelView.QuickSetText(2, -1, ifp.m_array_strOutput[2]);
					m_wndExcelView.QuickSetText(3, -1, ifp.m_array_strOutput[3]);
					m_wndExcelView.QuickSetText(4, -1, ifp.m_array_strOutput[4]);
				}
				while (ifp.ReadOneLineFromFile() > 4)
				{
					row_count++;
					m_wndExcelView.SetNumberRows(row_count);
					m_wndExcelView.QuickSetNumber(-1, row_count - 1, row_count);
					m_wndExcelView.QuickSetText(0, row_count-1, ifp.m_array_strOutput[0]);
					m_wndExcelView.QuickSetText(1, row_count-1, ifp.m_array_strOutput[1]);
					m_wndExcelView.QuickSetText(2, row_count-1, ifp.m_array_strOutput[2]);
					m_wndExcelView.QuickSetText(3, row_count-1, ifp.m_array_strOutput[3]);
					m_wndExcelView.QuickSetText(4, row_count-1, ifp.m_array_strOutput[4]);
				}
			}
		}
	}
	
	m_wndExcelView.BestFit(-1, col_count - 1, 0, UG_BESTFIT_TOPHEADINGS);
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

		GetDlgItem(IDC_BUTTON_SAVE)->MoveWindow(0, 0, btn_width, btn_height);
		m_wndExcelView.MoveWindow(0, btn_height, cx, cy - btn_height);
		//m_wndExcelView.MoveWindow(0, 0, cx, cy - (btn_height + btn_offset));
		//GetDlgItem(IDOK)->MoveWindow(cx - (btn_width * 2 + btn_offset), cy - btn_height, btn_width, btn_height);
		//GetDlgItem(IDCANCEL)->MoveWindow(cx - btn_width, cy - btn_height, btn_width, btn_height);
	}
}


void CDlgExcelView::OnBnClickedButtonSave()
{
	CString job_file;
	if (m_iType == 0)
	{
		job_file = m_strProjectPath + "\\JOB\\" + m_strJobName + "\\ice_result.OUT";
	}
	else if (m_iType == 1)
	{
		job_file = m_strProjectPath + "\\JOB\\" + m_strJobName + "\\Attainable_speed.out";
	}
	if (PathFileExists(job_file))
	{
		CFileDialog pDlg(FALSE);
		if (pDlg.DoModal() == IDOK)
		{
			CopyFile(job_file, pDlg.GetPathName(), FALSE);
		}
	}
}
