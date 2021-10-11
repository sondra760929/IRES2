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
	satellite_files.clear();
	satellite_files_option[0].clear();
	satellite_files_option[1].clear();
	satellite_files_option[2].clear();

	CRect rect;
	GetClientRect(&rect);
	SetSize(rect.Width(), rect.Height());
	int row_count = 0;
	int col_count = 0;
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

			//m_wndExcelView.SetNumberCols(10);
			//col_count = 10;
			//int col_index = 0;
			//int current_index = 0;
			//int option_index = 0;
			//for (int i = 0; i < satellite_files.size(); i++)
			//{
			//	FILE* fp;
			//	fopen_s(&fp, satellite_files[i], "rt");
			//	if (fp)
			//	{
			//		row_count++;
			//		m_wndExcelView.SetNumberRows(row_count);
			//		option_index = row_count - 1;
			//		m_wndExcelView.QuickSetText(0, row_count - 1, satellite_files_option[0][i]);
			//		m_wndExcelView.QuickSetText(1, row_count - 1, satellite_files_option[1][i]);
			//		m_wndExcelView.QuickSetText(2, row_count - 1, satellite_files_option[2][i]);

			//		COptImportExportBase ifp;
			//		vector< float > resiatance;
			//		ifp.m_fp_input = fp;
			//		ifp.m_array_strSplit.push_back(' ');
			//		int count = ifp.ReadOneLineFromFile();
			//		current_index = 0;
			//		while (count > 0)
			//		{
			//			row_count++;
			//			m_wndExcelView.SetNumberRows(row_count);
			//			m_wndExcelView.QuickSetNumber(-1, row_count - 1, current_index);

			//			for (int j = 0; j < count; j++)
			//			{
			//				m_wndExcelView.QuickSetText(j, row_count - 1, ifp.m_array_strOutput[j]);
			//			}
			//			if (current_index > 0)
			//			{
			//				resiatance.push_back(atof(ifp.m_array_strOutput[count-1]));
			//			}
			//			count = ifp.ReadOneLineFromFile();
			//			current_index++;
			//		}

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
			//					m_wndExcelView.QuickSetText(4, option_index, "Estimation Speed");
			//					m_wndExcelView.QuickSetNumber(5, option_index, m_pCurrentView->m_fEstimationSpeed);
			//				}
			//			}
			//		}
			//	}
			//}
		}
		else if (PathFileExists(job_file))
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

		if (m_aButtons.size() > 0)
		{
			btn_width = cx / m_aButtons.size();
			for (int i = 0; i < m_aButtons.size(); i++)
			{
				m_aButtons[i]->MoveWindow(i * btn_width, btn_height, btn_width, btn_height);
			}
			m_wndExcelView.MoveWindow(0, btn_height * 2, cx, cy - (btn_height * 2));
		}
		else
		{
			m_wndExcelView.MoveWindow(0, btn_height, cx, cy - btn_height);
		}
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

void CDlgExcelView::LoadFile(int index)
{
	m_wndExcelView.SetNumberCols(10);
	int col_count = 10;
	int col_index = 0;
	int current_index = 0;
	int option_index = 0;
	int row_count = 0;
	if(index >= 0 && index < satellite_files.size())
	{
		FILE* fp;
		fopen_s(&fp, satellite_files[index], "rt");
		if (fp)
		{
			row_count++;
			m_wndExcelView.SetNumberRows(row_count);
			option_index = row_count - 1;
			m_wndExcelView.QuickSetText(0, row_count - 1, satellite_files_option[0][index]);
			m_wndExcelView.QuickSetText(1, row_count - 1, satellite_files_option[1][index]);
			m_wndExcelView.QuickSetText(2, row_count - 1, satellite_files_option[2][index]);

			COptImportExportBase ifp;
			vector< float > resiatance;
			ifp.m_fp_input = fp;
			ifp.m_array_strSplit.push_back(' ');
			int count = ifp.ReadOneLineFromFile();
			current_index = 0;
			while (count > 0)
			{
				row_count++;
				m_wndExcelView.SetNumberRows(row_count);
				m_wndExcelView.QuickSetNumber(-1, row_count - 1, current_index);

				for (int j = 0; j < count; j++)
				{
					m_wndExcelView.QuickSetText(j, row_count - 1, ifp.m_array_strOutput[j]);
				}
				if (current_index > 0)
				{
					resiatance.push_back(atof(ifp.m_array_strOutput[count - 1]));
				}
				count = ifp.ReadOneLineFromFile();
				current_index++;
			}

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
						m_wndExcelView.QuickSetText(4, option_index, "Estimation Speed");
						m_wndExcelView.QuickSetNumber(5, option_index, m_pCurrentView->m_fEstimationSpeed);
					}
				}
			}
		}
		m_wndExcelView.BestFit(-1, col_count - 1, 0, UG_BESTFIT_TOPHEADINGS);
	}
}

BOOL CDlgExcelView::OnCommand(WPARAM wParam, LPARAM lParam)
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
