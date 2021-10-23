// DlgCalc.cpp: 구현 파일
//
#include "stdafx.h"
#include "I-RES2.h"
#include "DlgCalc.h"
#include "afxdialogex.h"
#include "OptImportExportBase.h"
#include <stdio.h>
#include <stdlib.h>
// CDlgCalc 대화 상자

IMPLEMENT_DYNAMIC(CDlgCalc, CDialog)

CDlgCalc::CDlgCalc(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_CALC, pParent)
	, m_fAvg(0)
	, m_fMean(0)
	, m_iRows(0)
	, m_strResult(_T(""))
{

}

CDlgCalc::~CDlgCalc()
{
}

void CDlgCalc::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_1, m_fAvg);
	DDX_Text(pDX, IDC_EDIT_2, m_fMean);
	DDX_Text(pDX, IDC_EDIT_3, m_iRows);
	//DDX_Control(pDX, IDC_STCHARTCONTAINER, m_chartContainer);
	DDX_Text(pDX, IDC_EDIT_RESULT, m_strResult);
}


BEGIN_MESSAGE_MAP(CDlgCalc, CDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_LOAD_TXT, &CDlgCalc::OnBnClickedButtonLoadTxt)
	ON_BN_CLICKED(IDC_BUTTON_SET_ROWS, &CDlgCalc::OnBnClickedButtonSetRows)
	ON_BN_CLICKED(IDC_BUTTON_DO_CALC, &CDlgCalc::OnBnClickedButtonDoCalc)
END_MESSAGE_MAP()


// CDlgCalc 메시지 처리기


BOOL CDlgCalc::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pCurveCtrl = new CCurveCtrl;
	m_pCurveCtrl->Create(CRect(70, 50, 50, 50), this, ID_CURVE_CONTROL);
	m_pCurveCtrl->SetGridLineStyle(PS_DOT);
	m_pCurveCtrl->SetMargin(CRect(70, 50, 50, 50));
	m_pCurveCtrl->ShowCross(TRUE);

	m_wndExcelView.AttachGrid(this, IDC_GRID);
	m_wndExcelView.m_pParentDlg = this;
	m_wndExcelView.SetVScrollMode(UG_SCROLLTRACKING);
	m_wndExcelView.SetHScrollMode(UG_SCROLLTRACKING);

	m_wndReportView.AttachGrid(this, IDC_GRID2);
	m_wndReportView.m_pParentDlg = this;
	m_wndReportView.SetVScrollMode(UG_SCROLLTRACKING);
	m_wndReportView.SetHScrollMode(UG_SCROLLTRACKING);
	
	chartIdx = -1;

	CRect rect;
	GetClientRect(&rect);
	SetSize(rect.Width(), rect.Height());

	m_wndExcelView.SetNumberCols(2);
	m_wndExcelView.QuickSetText(0, -1, "Time(s)");
	m_wndExcelView.QuickSetText(1, -1, "R(N)");
	SetNumRows(100);

	m_wndReportView.SetNumberCols(3);
	m_wndReportView.QuickSetText(0, -1, "X");
	m_wndReportView.QuickSetText(1, -1, "Range");
	m_wndReportView.QuickSetText(2, -1, "Count");

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgCalc::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	SetSize(cx, cy);
}

void CDlgCalc::SetSize(int cx, int cy)
{
	if (m_wndExcelView)
	{
		int btn_height = 25;
		int btn_width = 70;
		int btn_offset = 5;
		int chart_height;

		GetDlgItem(IDC_BUTTON_LOAD_TXT)->MoveWindow(btn_offset, btn_offset, btn_width, btn_height);
		GetDlgItem(IDC_EDIT_3)->MoveWindow(btn_width * 2 + btn_offset, btn_offset, btn_width, btn_height);
		GetDlgItem(IDC_BUTTON_SET_ROWS)->MoveWindow(btn_width * 3 + btn_offset, btn_offset, btn_width, btn_height);

		GetDlgItem(IDC_BUTTON_DO_CALC)->MoveWindow(cx - btn_width * 2 - btn_offset, btn_offset, btn_width * 2, btn_height);

		GetDlgItem(IDC_STATIC_1)->MoveWindow(cx - btn_width * 2 - btn_offset, btn_height + btn_offset + btn_offset, btn_width, btn_height);
		GetDlgItem(IDC_EDIT_1)->MoveWindow(cx - btn_width - btn_offset, btn_height + btn_offset + btn_offset, btn_width, btn_height);
		GetDlgItem(IDC_STATIC_2)->MoveWindow(cx - btn_width * 2 - btn_offset, (btn_height + btn_offset) * 2 + btn_offset, btn_width, btn_height);
		GetDlgItem(IDC_EDIT_2)->MoveWindow(cx - btn_width - btn_offset, (btn_height + btn_offset) * 2 + btn_offset, btn_width, btn_height);

		GetDlgItem(IDC_EDIT_RESULT)->MoveWindow((cx - (btn_offset * 3)) / 2 + btn_offset * 2, btn_height + btn_offset + btn_offset, (cx - (btn_offset * 3)) / 2, btn_height*2);
		
		m_wndExcelView.MoveWindow(btn_offset, btn_height + btn_offset*2, (cx - (btn_offset*3)) / 2, cy - (btn_height + btn_offset + btn_offset) * 2);

		chart_height = (cy - (btn_height + btn_offset) * 4 - btn_offset * 2) / 2;
		if (m_pCurveCtrl)
		{
			m_pCurveCtrl->MoveWindow((cx - (btn_offset * 3)) / 2 + btn_offset * 2, (btn_height + btn_offset) * 3 + btn_offset, (cx - (btn_offset * 3)) / 2, chart_height);
			m_pCurveCtrl->Invalidate();
		}

		m_wndReportView.MoveWindow((cx - (btn_offset * 3)) / 2 + btn_offset * 2, (btn_height + btn_offset) * 3 + btn_offset + chart_height, (cx - (btn_offset * 3)) / 2, chart_height);

		GetDlgItem(IDOK)->MoveWindow(cx - btn_width * 2 - btn_offset, cy - btn_height - btn_offset, btn_width * 2, btn_height);
		//m_wndExcelView.MoveWindow(0, 0, cx, cy - (btn_height + btn_offset));
		//GetDlgItem(IDOK)->MoveWindow(cx - (btn_width * 2 + btn_offset), cy - btn_height, btn_width, btn_height);
		//GetDlgItem(IDCANCEL)->MoveWindow(cx - btn_width, cy - btn_height, btn_width, btn_height);

		UpdateWindow();
	}
}


void CDlgCalc::OnBnClickedButtonLoadTxt()
{
	CFileDialog pDlg(TRUE);
	if (pDlg.DoModal() == IDOK)
	{
		CString txt_filepath = pDlg.GetPathName();

		if (PathFileExists(txt_filepath))
		{
			FILE* fp;
			fopen_s(&fp, txt_filepath, "rt");
			if (fp)
			{
				vector< float > a_time;
				vector< float > a_r;
				COptImportExportBase ifp;
				ifp.m_fp_input = fp;
				ifp.m_array_strSplit.push_back(' ');
				ifp.m_array_strSplit.push_back(',');
				ifp.m_array_strSplit.push_back(';');
				ifp.m_array_strSplit.push_back(':');
				while (ifp.ReadOneLineFromFile() > 1)
				{
					a_time.push_back(atof(ifp.m_array_strOutput[0]));
					a_r.push_back(atof(ifp.m_array_strOutput[1]));
				}
				fclose(fp);

				SetNumRows(a_time.size());
				for (int i = 0; i < a_time.size(); i++)
				{
					m_wndExcelView.QuickSetNumber(0, i, a_time[i]);
					m_wndExcelView.QuickSetNumber(1, i, a_r[i]);
				}

				m_wndExcelView.BestFit(-1, 1, 0, UG_BESTFIT_TOPHEADINGS);

				OnBnClickedButtonDoCalc();
			}
		}
	}
}

void CDlgCalc::SetNumRows(int irow)
{
	m_wndExcelView.SetNumberRows(irow);
	CString temp_string;
	for (int i = 0; i < irow; i++)
	{
		temp_string.Format("%d", i + 1);
		m_wndExcelView.QuickSetText(-1, i, temp_string);
	}
}

void CDlgCalc::OnBnClickedButtonSetRows()
{
	UpdateData();

	SetNumRows(m_iRows);
}


void CDlgCalc::OnBnClickedButtonDoCalc()
{
	vector< float > values;
	double sum = 0;
	double min, max;
	double cal_min, cal_max;
	long row_count = m_wndExcelView.GetNumberRows();
	for (long i = 0; i < row_count; i++)
	{
		float value = atof(m_wndExcelView.QuickGetText(1, i));
		values.push_back(value);
		sum += value;
		if (i == 0)
		{
			min = value;
			max = value;
		}
		else
		{
			if (min > value)
			{
				min = value;
			}
			if (max < value)
			{
				max = value;
			}
		}
	}

	m_fAvg = sum / (float)row_count;

	cal_min = 0;
	cal_max = 0;
	while (cal_min > min)
	{
		cal_min -= 10;
	}
	while (cal_max < max)
	{
		cal_max += 10;
	}
	vector< int > count;
	count.resize((cal_max - cal_min) / 10 + 1, 0);

	sum = 0;
	float temp_min, temp_max;
	for (long i = 0; i < row_count; i++)
	{
		sum += pow(values[i] - m_fAvg, 2);
		for (int j = 0; j < count.size(); j++)
		{
			temp_min = cal_min - 5 + (j * 10);
			temp_max = cal_min + 5 + (j * 10);
			if (values[i] > temp_min && values[i] <= temp_max)
			{
				count[j] = count[j] + 1;
				break;
			}
		}
	}
	double variance = sum / (float)row_count;
	m_fMean = sqrt(variance);

	m_wndReportView.SetNumberRows(count.size());
	CString x_string, range_string;

	m_pCurveCtrl->RemoveAll();
	int chart_index = m_pCurveCtrl->AddCurve("STD", RGB(255, 0, 0));
	m_pCurveCtrl->GetCurve(chart_index)->ShowCurve();
	m_pCurveCtrl->GetCurve(chart_index)->Select();
	m_pCurveCtrl->GetCurve(chart_index)->SetCurveWidth(2);

	//V_CHARTDATAD vData1;
	for (int i = 0; i < count.size(); i++)
	{
		x_string.Format("%.1lf", cal_min + (i * 10));
		range_string.Format("%.1lf < X <= %.1lf", cal_min - 5 + (i * 10), cal_min + 5 + (i * 10));
		m_wndReportView.QuickSetText(0, i, x_string);
		m_wndReportView.QuickSetText(1, i, range_string);
		m_wndReportView.QuickSetNumber(2, i, count[i]);
		//vData1.push_back(PointD(cal_min + (i * 10), count[i]));
		m_pCurveCtrl->AddData("STD", cal_min + (i * 10), count[i]);
	}

	//if(chartIdx > -1)
	//	m_chartContainer.RemoveChart(chartIdx, true, true);

	//chartIdx = m_chartContainer.AddChart(true, true, string("STD"), "Y", 3, DashStyleSolid, 2, float(0), Color(255, 255, 0, 0), vData1, true);
	//m_chartContainer.RefreshWnd();
	//m_chartContainer.ShowAxisXBoundaries(true, true);
	m_pCurveCtrl->Restore();
	m_pCurveCtrl->Invalidate();
	m_wndReportView.BestFit(-1, 2, 0, UG_BESTFIT_TOPHEADINGS);

	m_strResult.Format("\tAvg : %lf\r\n\tSTD : %lf", m_fAvg, m_fMean);
	//	normality test
	if (values.size() > 0)
	{
		FILE* fp_test;
		fopen_s(&fp_test, m_strProjectPath + "\\normal.txt", "wt");
		if (fp_test)
		{
			fprintf_s(fp_test, "x <- c(%lf", values[0]);
			for (int i = 1; i < values.size(); i++)
			{
				fprintf_s(fp_test, ",%lf", values[i]);
			}
			fprintf_s(fp_test, ")\nlibrary(nortest)\nad.test(x)");
			fclose(fp_test);

			const char  *pszCommand = m_strProjectPath + "\\R-4.0.2\\bin\\RScript " + m_strProjectPath + "\\normal.txt";
			FILE        *fp = NULL;
			// 명령어 실행
			fp = _popen(pszCommand, "r");
			if (fp)
			{
				COptImportExportBase ifp;
				ifp.m_fp_input = fp;
				ifp.m_array_strSplit.push_back(' ');
				ifp.m_array_strSplit.push_back(',');
				ifp.m_array_strSplit.push_back('=');
				ifp.m_array_strSplit.push_back(':');
				ifp.ReadOneLineFromFile();
				ifp.ReadOneLineFromFile();
				CString title = ifp.m_strOneLine;
				ifp.ReadOneLineFromFile();
				ifp.ReadOneLineFromFile();
				ifp.ReadOneLineFromFile();
				if (ifp.m_array_strOutput.size() > 3)
				{
					m_strResult.Format("\tAvg : %lf\r\n\tSTD : %lf\r\n%s : %.2lf %%", m_fAvg, m_fMean, title, atof(ifp.m_array_strOutput[3])*100.0f);
				}

			}
		}
	}

	UpdateData(FALSE);
}
