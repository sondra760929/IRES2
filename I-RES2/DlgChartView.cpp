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
	m_pCurveCtrl = NULL;
}

CDlgChartView::~CDlgChartView()
{
}

void CDlgChartView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_STCHARTCONTAINER, m_chartContainer);
}


BEGIN_MESSAGE_MAP(CDlgChartView, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgChartView 메시지 처리기


BOOL CDlgChartView::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_pCurveCtrl = new CCurveCtrl;
	m_pCurveCtrl->Create(CRect(70, 50, 50, 50), this, ID_CURVE_CONTROL);
	m_pCurveCtrl->SetGridLineStyle(PS_DOT);
	m_pCurveCtrl->SetMargin(CRect(70, 50, 50, 50));
	m_pCurveCtrl->ShowCross(TRUE);
	//m_chartContainer.SetContainerName("Chart");
	UpdateData();
	bool show_pnts = true;

	if (m_iType == 0)
	{
		CString job_file;
		job_file = m_strProjectPath + "\\JOB\\" + m_strJobName + "\\ice_result.OUT";
		//	satellite 파일이 있는지 확인
		CFileFind finder;
		BOOL bworking = finder.FindFile(m_strProjectPath + "\\JOB\\" + m_strJobName + "\\*.*");
		while (bworking)
		{
			bworking = finder.FindNextFile();
			CString file_name = finder.GetFileName();
			CString extention = file_name.Left(9).MakeLower();
			if (extention == "satellite")
			{
				satellite_files.push_back(finder.GetFilePath());
				file_name.Delete(0, 9);
				file_name.Delete(file_name.GetLength() - 4, 4);
				int index = file_name.Find("_");
				if (index > 0)
				{
					satellite_files_option[0].push_back(file_name.Left(index));

					file_name.Delete(0, index + 1);

					index = file_name.Find("_");
					if (index > 0)
					{
						satellite_files_option[1].push_back(file_name.Left(index));

						file_name.Delete(0, index + 1);
						satellite_files_option[2].push_back(file_name);
					}
				}
			}
		}
		if (satellite_files.size() > 0)
		{
			for (int i = 0; i < satellite_files.size(); i++)
			{
				CButton* new_button = new CButton();
				new_button->Create(NULL, BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD,
					CRect(0, 0, 10, 10), this, WM_USER + i);
				new_button->SetWindowText(satellite_files_option[0][i]);
				m_aButtons.push_back(new_button);
			}
			CRect rect;
			GetClientRect(&rect);
			SetSize(rect.Width(), rect.Height());

			LoadFile(0);
			//int col_index = 0;
			//int current_index = 0;
			//for (int i = 0; i < satellite_files.size(); i++)
			//{
			//	FILE* fp;
			//	fopen_s(&fp, satellite_files[i], "rt");
			//	if (fp)
			//	{
			//		V_CHARTDATAD vData1;
			//		COptImportExportBase ifp;
			//		vector< float > resiatance;
			//		ifp.m_fp_input = fp;
			//		ifp.m_array_strSplit.push_back(' ');
			//		ifp.ReadOneLineFromFile();	//	첫줄 타이틀
			//		int count = ifp.ReadOneLineFromFile();
			//		current_index = 0;
			//		while (count > 0)
			//		{
			//			vData1.push_back(PointD(atof(ifp.m_array_strOutput[0]), atof(ifp.m_array_strOutput[count-1])));
			//			resiatance.push_back(atof(ifp.m_array_strOutput[count - 1]));
			//			count = ifp.ReadOneLineFromFile();
			//		}
			//		int chartIdx = m_chartContainer.AddChart(true, show_pnts, string(satellite_files_option[0][i]), "Y", 3, DashStyleSolid, 2, float(0), Color(255, 0, 0, 0), vData1, true);
			//		if (resiatance.size() > 15)
			//		{
			//			if (m_pCurrentView)
			//			{
			//				if (m_pCurrentView->m_fTargetResistance > 0.0f)
			//				{
			//					if (m_pCurrentView->m_fTargetResistance < resiatance[0])
			//					{
			//						m_pCurrentView->m_fEstimationSpeed = 0.0f;
			//					}
			//					else if (m_pCurrentView->m_fTargetResistance > resiatance[resiatance.size() - 1])
			//					{
			//						float offset = resiatance[resiatance.size() - 1] - resiatance[resiatance.size() - 2];
			//						float target_offset = m_pCurrentView->m_fTargetResistance - resiatance[resiatance.size() - 1];
			//						float ratio = target_offset / offset;
			//						int up_speed = round(ratio);
			//						m_pCurrentView->m_fEstimationSpeed = 16 + up_speed;
			//					}
			//					else
			//					{
			//						for (int i = 0; i < 15; i++)
			//						{
			//							if (m_pCurrentView->m_fTargetResistance >= resiatance[i] && m_pCurrentView->m_fTargetResistance <= resiatance[i + 1])
			//							{
			//								float offset = resiatance[i + 1] - resiatance[i];
			//								float target_offset = m_pCurrentView->m_fTargetResistance - resiatance[i];
			//								float ratio = target_offset / offset;
			//								m_pCurrentView->m_fEstimationSpeed = (float)(i + 1) + ratio;
			//								break;
			//							}
			//						}
			//					}

			//					//	추정 속도 찾음
			//					V_CHARTDATAD vData2;
			//					vData2.push_back(PointD(0.0, m_pCurrentView->m_fTargetResistance));
			//					vData2.push_back(PointD(17.0, m_pCurrentView->m_fTargetResistance));
			//					chartIdx = m_chartContainer.AddChart(true, show_pnts, string(satellite_files_option[0][i] + "_Target"), "Y", 3, DashStyleSolid, 2, float(0), Color(255, 255, 0, 0), vData2, true);
			//				}
			//			}
			//		}
			//	}
			//}
		}
		else if (PathFileExists(job_file))
		{
			FILE* fp;
			fopen_s(&fp, job_file, "rt");
			if (fp)
			{
				//V_CHARTDATAD vData1;
				//V_CHARTDATAD vData2;
				//V_CHARTDATAD vData3;
				//V_CHARTDATAD vData4;
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

					int index = m_pCurveCtrl->AddCurve(str1, RGB(255, 0, 0));
					m_pCurveCtrl->GetCurve(index)->ShowCurve();
					index = m_pCurveCtrl->AddCurve(str2, RGB(0, 255, 0));
					m_pCurveCtrl->GetCurve(index)->ShowCurve();
					index = m_pCurveCtrl->AddCurve(str3, RGB(0, 0, 255));
					m_pCurveCtrl->GetCurve(index)->ShowCurve();
					index = m_pCurveCtrl->AddCurve(str4, RGB(255, 255, 0));
					m_pCurveCtrl->GetCurve(index)->ShowCurve();
				}
				while (ifp.ReadOneLineFromFile() > 6)
				{
					float x = atof(ifp.m_array_strOutput[1]);
					if (x < prev_x)
						break;
					m_pCurveCtrl->AddData(str1, x, atof(ifp.m_array_strOutput[3]));
					m_pCurveCtrl->AddData(str2, x, atof(ifp.m_array_strOutput[4]));
					m_pCurveCtrl->AddData(str3, x, atof(ifp.m_array_strOutput[5]));
					m_pCurveCtrl->AddData(str4, x, atof(ifp.m_array_strOutput[6]));

					//vData1.push_back(PointD(x, atof(ifp.m_array_strOutput[3])));
					//vData2.push_back(PointD(x, atof(ifp.m_array_strOutput[4])));
					//vData3.push_back(PointD(x, atof(ifp.m_array_strOutput[5])));
					//vData4.push_back(PointD(x, atof(ifp.m_array_strOutput[6])));
					prev_x = x;
				}
				m_pCurveCtrl->Invalidate();

				//int chartIdx = m_chartContainer.AddChart(true, show_pnts, string(str1), "Y", 3, DashStyleSolid, 2, float(0), Color(255, 255, 0, 0), vData1, true);
				//chartIdx = m_chartContainer.AddChart(true, show_pnts, string(str2), "Y", 3, DashStyleSolid, 2, float(0), Color(255, 0, 255, 0), vData2, true);
				//chartIdx = m_chartContainer.AddChart(true, show_pnts, string(str3), "Y", 3, DashStyleSolid, 2, float(0), Color(255, 0, 0, 255), vData3, true);
				//chartIdx = m_chartContainer.AddChart(true, show_pnts, string(str4), "Y", 3, DashStyleSolid, 2, float(0), Color(255, 255, 255, 0), vData4, true);
			}
		}
	}	
	else if (m_iType == 1)
	{
		CString job_file;
		job_file = m_strProjectPath + "\\JOB\\" + m_strJobName + "\\Attainable_speed.out";
		if (PathFileExists(job_file))
		{
			FILE* fp;
			fopen_s(&fp, job_file, "rt");
			if (fp)
			{
				//V_CHARTDATAD vData1;
				//V_CHARTDATAD vData2;
				//V_CHARTDATAD vData3;
				int index = m_pCurveCtrl->AddCurve("60", RGB(255, 0, 0));
				m_pCurveCtrl->GetCurve(index)->ShowCurve();
				index = m_pCurveCtrl->AddCurve("80", RGB(0, 255, 0));
				m_pCurveCtrl->GetCurve(index)->ShowCurve();
				index = m_pCurveCtrl->AddCurve("90", RGB(0, 0, 255));
				m_pCurveCtrl->GetCurve(index)->ShowCurve();
				float prev_strength = 0;
				float thickness_offset = 0;
				float current_thickness = 0;
				bool init = false;
				COptImportExportBase ifp;
				ifp.m_fp_input = fp;
				ifp.m_array_strSplit.push_back(' ');
				ifp.m_array_strSplit.push_back(',');
				int row_count = 0;
				if (ifp.ReadOneLineFromFile() > 4)
				{
				}
				while (ifp.ReadOneLineFromFile() > 4)
				{
					float strength = atof(ifp.m_array_strOutput[0]);
					float thickness = atof(ifp.m_array_strOutput[1]);
					float speed_60 = atof(ifp.m_array_strOutput[2]);
					float speed_80 = atof(ifp.m_array_strOutput[3]);
					float speed_90 = atof(ifp.m_array_strOutput[4]);
					if (prev_strength < strength)
					{
						if (init)
						{
							thickness_offset = current_thickness;
						}
						prev_strength = strength;
					}
					current_thickness = thickness_offset + thickness;
					m_pCurveCtrl->AddData("60", current_thickness, speed_60);
					m_pCurveCtrl->AddData("80", current_thickness, speed_80);
					m_pCurveCtrl->AddData("90", current_thickness, speed_90);
					init = true;
					//vData1.push_back(PointD(current_thickness, speed_60));
					//vData2.push_back(PointD(current_thickness, speed_80));
					//vData3.push_back(PointD(current_thickness, speed_90));
				}
				m_pCurveCtrl->Invalidate();
				//if (vData1.size() > 0)
				//{
				//	int chartIdx = m_chartContainer.AddChart(true, show_pnts, to_string(prev_strength), "60", 3, DashStyleSolid, 2, float(0), Color(255, 255, 0, 0), vData1, true);
				//	chartIdx = m_chartContainer.AddChart(true, show_pnts, to_string(prev_strength), "80", 3, DashStyleSolid, 2, float(0), Color(255, 0, 255, 0), vData2, true);
				//	chartIdx = m_chartContainer.AddChart(true, show_pnts, to_string(prev_strength), "90", 3, DashStyleSolid, 2, float(0), Color(255, 0, 0, 255), vData3, true);
				//}
			}
		}
	}

	//m_chartContainer.RefreshWnd();
	//m_chartContainer.ShowAxisXBoundaries(true, true);
	//m_chartContainer.ShowNamesLegend();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgChartView::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	SetSize(cx, cy);
}

void CDlgChartView::SetSize(int cx, int cy)
{
	if (m_pCurveCtrl)
	{
		int btn_height = 25;
		int btn_width = 70;
		int btn_offset = 5;

		if (m_aButtons.size() > 0)
		{
			btn_width = cx / m_aButtons.size();
			for (int i = 0; i < m_aButtons.size(); i++)
			{
				m_aButtons[i]->MoveWindow(i * btn_width, 0, btn_width, btn_height);
			}
			m_pCurveCtrl->MoveWindow(0, btn_height, cx, cy - (btn_height));
		}
		else
		{
			m_pCurveCtrl->MoveWindow(0, 0, cx, cy);
		}
		m_pCurveCtrl->Invalidate();
	}
}

BOOL CDlgChartView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if (wParam >= WM_USER && wParam < WM_USER + m_aButtons.size())
	{
		for (int i = 0; i < m_aButtons.size(); i++)
		{
			if (wParam == WM_USER + i)
			{
				LoadFile(i);
			}
		}
	}

	return CDialog::OnCommand(wParam, lParam);
}

void CDlgChartView::LoadFile(int index)
{
	int col_index = 0;
	int current_index = 0;
	if (index >= 0 && index < satellite_files.size())
	{
		//m_chartContainer.ResetChartContainer();
		FILE* fp;
		fopen_s(&fp, satellite_files[index], "rt");
		if (fp)
		{
			m_pCurveCtrl->RemoveAll();
			int chart_index = m_pCurveCtrl->AddCurve(satellite_files_option[0][index], RGB(0, 0, 0));
			m_pCurveCtrl->GetCurve(chart_index)->ShowCurve();
			m_pCurveCtrl->GetCurve(chart_index)->Select();
			m_pCurveCtrl->GetCurve(chart_index)->SetCurveWidth(2);
			//V_CHARTDATAD vData1;
			COptImportExportBase ifp;
			vector< float > resiatance;
			ifp.m_fp_input = fp;
			ifp.m_array_strSplit.push_back(' ');
			ifp.ReadOneLineFromFile();	//	첫줄 타이틀
			int count = ifp.ReadOneLineFromFile();
			current_index = 0;
			while (count > 0)
			{
				//vData1.push_back(PointD(atof(ifp.m_array_strOutput[0]), atof(ifp.m_array_strOutput[count-1])));
				m_pCurveCtrl->AddData(satellite_files_option[0][index], atof(ifp.m_array_strOutput[0]), atof(ifp.m_array_strOutput[count - 1]));

				resiatance.push_back(atof(ifp.m_array_strOutput[count - 1]));
				count = ifp.ReadOneLineFromFile();
			}

			//int chartIdx = m_chartContainer.AddChart(true, true, string(satellite_files_option[0][index]), "Y", 3, DashStyleSolid, 2, float(0), Color(255, 0, 0, 0), vData1, true);
			if (resiatance.size() > 15)
			{
				if (m_pCurrentView)
				{
					if (m_pCurrentView->m_fTargetResistance > 0.0f)
					{
						if (m_pCurrentView->m_fTargetResistance < resiatance[0])
						{
							m_pCurrentView->m_fEstimationSpeed = 0.0f;
						}
						else if (m_pCurrentView->m_fTargetResistance > resiatance[resiatance.size() - 1])
						{
							float offset = resiatance[resiatance.size() - 1] - resiatance[resiatance.size() - 2];
							float target_offset = m_pCurrentView->m_fTargetResistance - resiatance[resiatance.size() - 1];
							float ratio = target_offset / offset;
							int up_speed = round(ratio);
							m_pCurrentView->m_fEstimationSpeed = 16 + up_speed;
						}
						else
						{
							for (int i = 0; i < 15; i++)
							{
								if (m_pCurrentView->m_fTargetResistance >= resiatance[i] && m_pCurrentView->m_fTargetResistance <= resiatance[i + 1])
								{
									float offset = resiatance[i + 1] - resiatance[i];
									float target_offset = m_pCurrentView->m_fTargetResistance - resiatance[i];
									float ratio = target_offset / offset;
									m_pCurrentView->m_fEstimationSpeed = (float)(i + 1) + ratio;
									break;
								}
							}
						}

						//	추정 속도 찾음
						//V_CHARTDATAD vData2;
						//vData2.push_back(PointD(0.0, m_pCurrentView->m_fTargetResistance));
						//vData2.push_back(PointD(17.0, m_pCurrentView->m_fTargetResistance));
						//chartIdx = m_chartContainer.AddChart(true, true, string(satellite_files_option[0][index] + "_Target"), "Y", 3, DashStyleSolid, 2, float(0), Color(255, 255, 0, 0), vData2, true);
						chart_index = m_pCurveCtrl->AddCurve(satellite_files_option[0][index] + "_Target", RGB(255, 0, 0));
						m_pCurveCtrl->GetCurve(chart_index)->ShowCurve();
						m_pCurveCtrl->AddData(satellite_files_option[0][index] + "_Target", 0.0, m_pCurrentView->m_fTargetResistance);
						m_pCurveCtrl->AddData(satellite_files_option[0][index] + "_Target", 17.0, m_pCurrentView->m_fTargetResistance);
					}
				}
			}
			m_pCurveCtrl->Restore();
			m_pCurveCtrl->Invalidate();
		}
	}
}