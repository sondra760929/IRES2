// DlgChartView.cpp: 구현 파일
//

#include "stdafx.h"
#include "I-RES2.h"
#include "DlgChartView.h"
#include "afxdialogex.h"
#include "OptImportExportBase.h"


// CDlgChartView 대화 상자

IMPLEMENT_DYNAMIC(CDlgChartView, CDialogEx)

CDlgChartView::CDlgChartView(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CHART, pParent)
{

}

CDlgChartView::~CDlgChartView()
{
}

void CDlgChartView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STCHARTCONTAINER, m_chartContainer);
}


BEGIN_MESSAGE_MAP(CDlgChartView, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgChartView 메시지 처리기


BOOL CDlgChartView::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_chartContainer.SetContainerName("Chart");
	UpdateData();
	bool show_pnts = true;

	if (m_iType == 0)
	{
		CString job_file;
		job_file = m_strProjectPath + "\\JOB\\" + m_strJobName + "\\ice_result.OUT";
		if (PathFileExists(job_file))
		{
			FILE* fp;
			fopen_s(&fp, job_file, "rt");
			if (fp)
			{
				V_CHARTDATAD vData1;
				V_CHARTDATAD vData2;
				V_CHARTDATAD vData3;
				V_CHARTDATAD vData4;
				COptImportExportBase ifp;
				ifp.m_fp_input = fp;
				ifp.m_array_strSplit.push_back(' ');
				float prev_x = 0;
				int row_count = 0;
				CString str1, str2, str3, str4;
				if (ifp.ReadOneLineFromFile() > 6)
				{
					str1 = ifp.m_array_strOutput[3];
					str2 = ifp.m_array_strOutput[4];
					str3 = ifp.m_array_strOutput[5];
					str4 = ifp.m_array_strOutput[6];
				}
				while (ifp.ReadOneLineFromFile() > 6)
				{
					float x = atof(ifp.m_array_strOutput[1]);
					if (x < prev_x)
						break;
					vData1.push_back(PointD(x, atof(ifp.m_array_strOutput[3])));
					vData2.push_back(PointD(x, atof(ifp.m_array_strOutput[4])));
					vData3.push_back(PointD(x, atof(ifp.m_array_strOutput[5])));
					vData4.push_back(PointD(x, atof(ifp.m_array_strOutput[6])));
					prev_x = x;
				}

				int chartIdx = m_chartContainer.AddChart(true, show_pnts, string(str1), "Y", 3, DashStyleSolid, 2, float(0), Color(255, 255, 0, 0), vData1, true);
				chartIdx = m_chartContainer.AddChart(true, show_pnts, string(str2), "Y", 3, DashStyleSolid, 2, float(0), Color(255, 0, 255, 0), vData2, true);
				chartIdx = m_chartContainer.AddChart(true, show_pnts, string(str3), "Y", 3, DashStyleSolid, 2, float(0), Color(255, 0, 0, 255), vData3, true);
				chartIdx = m_chartContainer.AddChart(true, show_pnts, string(str4), "Y", 3, DashStyleSolid, 2, float(0), Color(255, 255, 2550, 0), vData4, true);
			}
		}
	}	
	else if (m_iType == 1)
	{
		//CString job_file;
		//job_file = m_strProjectPath + "\\JOB\\" + m_strJobName + "\\Attainable_speed.out";
		//if (PathFileExists(job_file))
		//{
		//	m_wndExcelView.SetNumberCols(5);
		//	FILE* fp;
		//	fopen_s(&fp, job_file, "rt");
		//	if (fp)
		//	{
		//		COptImportExportBase ifp;
		//		ifp.m_fp_input = fp;
		//		ifp.m_array_strSplit.push_back(' ');
		//		int row_count = 0;
		//		if (ifp.ReadOneLineFromFile() > 4)
		//		{
		//			m_wndExcelView.QuickSetText(0, -1, ifp.m_array_strOutput[0]);
		//			m_wndExcelView.QuickSetText(1, -1, ifp.m_array_strOutput[1]);
		//			m_wndExcelView.QuickSetText(2, -1, ifp.m_array_strOutput[2]);
		//			m_wndExcelView.QuickSetText(3, -1, ifp.m_array_strOutput[3]);
		//			m_wndExcelView.QuickSetText(4, -1, ifp.m_array_strOutput[4]);
		//		}
		//		while (ifp.ReadOneLineFromFile() > 4)
		//		{
		//			row_count++;
		//			m_wndExcelView.SetNumberRows(row_count);
		//			m_wndExcelView.QuickSetNumber(-1, row_count - 1, row_count);
		//			m_wndExcelView.QuickSetText(0, row_count-1, ifp.m_array_strOutput[0]);
		//			m_wndExcelView.QuickSetText(1, row_count-1, ifp.m_array_strOutput[1]);
		//			m_wndExcelView.QuickSetText(2, row_count-1, ifp.m_array_strOutput[2]);
		//			m_wndExcelView.QuickSetText(3, row_count-1, ifp.m_array_strOutput[3]);
		//			m_wndExcelView.QuickSetText(4, row_count-1, ifp.m_array_strOutput[4]);
		//		}
		//	}
		//}
	}
	m_chartContainer.ShowAxisXBoundaries(true, true);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgChartView::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (m_chartContainer)
	{
		m_chartContainer.MoveWindow(0, 0, cx, cy);
		m_chartContainer.RefreshWnd();
	}
}
