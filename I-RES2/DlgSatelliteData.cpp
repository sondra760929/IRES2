// DlgSatelliteData.cpp: 구현 파일
//

#include "stdafx.h"
#include "I-RES2.h"
#include "DlgSatelliteData.h"
#include "afxdialogex.h"
#include "OptImportExportBase.h"


// CDlgSatelliteData 대화 상자

IMPLEMENT_DYNAMIC(CDlgSatelliteData, CDialog)

CDlgSatelliteData::CDlgSatelliteData(int type, CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_SATELLITE, pParent)
{
	m_iType = type;
}

CDlgSatelliteData::~CDlgSatelliteData()
{
}

void CDlgSatelliteData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgSatelliteData, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_FILE, &CDlgSatelliteData::OnBnClickedButtonFile)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgSatelliteData 메시지 처리기


void CDlgSatelliteData::OnBnClickedButtonFile()
{
	CString file_path;
	GetDlgItem(IDC_EDIT_DATA_FILE)->GetWindowText(file_path);

	CFileDialog dlg(TRUE,
		NULL,
		file_path,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"Satellite data files (*.csv)|*.csv|All Files (*.*)|*.*||",
		NULL);

	if (dlg.DoModal() == IDOK)
	{
		file_path = dlg.GetPathName();
		GetDlgItem(IDC_EDIT_DATA_FILE)->SetWindowText(file_path);

		if (PathFileExists(file_path))
		{
			ifstream ifs;
			ifs.open(file_path);
			if (ifs.is_open())
			{
				m_wndDataView.SetPaintMode(0);
				m_wndDataView.SetNumberCols(9);
				int row_count = 0;
				int max_col_index = 0;
				string line;
				while (getline(ifs, line))
				{
					row_count++;
					m_wndDataView.SetNumberRows(row_count);
					m_wndDataView.QuickSetNumber(-1, row_count - 1, row_count);

					int col_index = 0;
					stringstream ss(line);
					string temp;
					while (getline(ss, temp, ','))
					{
						m_wndDataView.QuickSetText(col_index, row_count - 1, temp.c_str());
						col_index++;
						if (col_index > max_col_index)
							max_col_index = col_index;
					}
				}
				ifs.close();
				if (max_col_index > 0)
				{
					m_wndDataView.SetNumberCols(max_col_index);
					m_wndDataView.BestFit(-1, 3, 0, UG_BESTFIT_TOPHEADINGS);
				}
				m_wndDataView.SetPaintMode(1);
			}
		}
	}
}

void CDlgSatelliteData::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	SetSize(cx, cy);
}


void CDlgSatelliteData::SetSize(int cx, int cy)
{
	if (m_wndDataView)
	{
		int btn_height = 25;
		int btn_width = 80;
		int btn_offset = 5;

		int w1 = btn_width * 2;
		int w2 = cx - (btn_width * 3 + btn_offset * 4);
		int w3 = btn_width;
		int w4 = (cx - (btn_width * 5 + btn_offset * 7)) / 2;
		GetDlgItem(IDC_STATIC_TITLE)->MoveWindow(btn_offset, btn_offset, w1, btn_height);
		GetDlgItem(IDC_EDIT_DATA_FILE)->MoveWindow(btn_offset * 2 + btn_width * 2, btn_offset, w2, btn_height);
		GetDlgItem(IDC_BUTTON_FILE)->MoveWindow(cx - btn_offset - btn_width, btn_offset, w3, btn_height);

		if (m_iType == 0)
		{
			GetDlgItem(IDC_STATIC_TITLE2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_TITLE3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_TITLE4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_TITLE5)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_TITLE6)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_TITLE7)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_TITLE8)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_TITLE9)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_EDIT_TARGET)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_TARGET2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_TARGET3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_TARGET4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_TARGET5)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_TARGET6)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_TARGET7)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_TARGET8)->ShowWindow(SW_HIDE);
			m_wndDataView.MoveWindow(btn_offset, btn_height + btn_offset * 2, cx - btn_offset - btn_offset, cy - (btn_height * 2 + btn_offset * 4));
		}
		else
		{
			GetDlgItem(IDC_STATIC_TITLE2)->MoveWindow(btn_offset, btn_height + btn_offset * 2, w1, btn_height);
			GetDlgItem(IDC_STATIC_TITLE3)->MoveWindow(btn_offset, btn_height * 2 + btn_offset * 3, w1, btn_height);
			GetDlgItem(IDC_STATIC_TITLE4)->MoveWindow(btn_offset, btn_height * 3 + btn_offset * 4, w1, btn_height);
			GetDlgItem(IDC_STATIC_TITLE5)->MoveWindow(btn_offset, btn_height * 4 + btn_offset * 5, w1, btn_height);

			GetDlgItem(IDC_STATIC_TITLE6)->MoveWindow(btn_offset * 3 + w1 + w4, btn_height + btn_offset * 2, w1, btn_height);
			GetDlgItem(IDC_STATIC_TITLE7)->MoveWindow(btn_offset * 3 + w1 + w4, btn_height * 2 + btn_offset * 3, w1, btn_height);
			GetDlgItem(IDC_STATIC_TITLE8)->MoveWindow(btn_offset * 3 + w1 + w4, btn_height * 3 + btn_offset * 4, w1, btn_height);
			GetDlgItem(IDC_STATIC_TITLE9)->MoveWindow(btn_offset * 3 + w1 + w4, btn_height * 4 + btn_offset * 5, w1, btn_height);

			GetDlgItem(IDC_EDIT_TARGET)->MoveWindow(btn_offset * 2 + btn_width * 2, btn_height + btn_offset * 2, w4, btn_height);
			GetDlgItem(IDC_EDIT_TARGET2)->MoveWindow(btn_offset * 2 + btn_width * 2, btn_height * 2 + btn_offset * 3, w4, btn_height);
			GetDlgItem(IDC_EDIT_TARGET3)->MoveWindow(btn_offset * 2 + btn_width * 2, btn_height * 3 + btn_offset * 4, w4, btn_height);
			GetDlgItem(IDC_EDIT_TARGET4)->MoveWindow(btn_offset * 2 + btn_width * 2, btn_height * 4 + btn_offset * 5, w4, btn_height);

			GetDlgItem(IDC_EDIT_TARGET5)->MoveWindow(btn_offset * 5 + w1 * 2 + w4, btn_height + btn_offset * 2, w4, btn_height);
			GetDlgItem(IDC_EDIT_TARGET6)->MoveWindow(btn_offset * 5 + w1 * 2 + w4, btn_height * 2 + btn_offset * 3, w4, btn_height);
			GetDlgItem(IDC_EDIT_TARGET7)->MoveWindow(btn_offset * 5 + w1 * 2 + w4, btn_height * 3 + btn_offset * 4, w4, btn_height);
			GetDlgItem(IDC_EDIT_TARGET8)->MoveWindow(btn_offset * 5 + w1 * 2 + w4, btn_height * 4 + btn_offset * 5, w4, btn_height);

			m_wndDataView.MoveWindow(btn_offset, btn_height*5 + btn_offset * 6, cx - btn_offset - btn_offset, cy - (btn_height * 6 + btn_offset * 8));
		}
		GetDlgItem(IDOK)->MoveWindow(cx - (btn_width * 2 + btn_offset * 2), cy - btn_height - btn_offset, btn_width, btn_height);
		GetDlgItem(IDCANCEL)->MoveWindow(cx - (btn_width + btn_offset), cy - btn_height - btn_offset, btn_width, btn_height);
	}
}

BOOL CDlgSatelliteData::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_wndDataView.AttachGrid(this, IDC_GRID_DATA);
	m_wndDataView.m_pParentDlg = this;
	m_wndDataView.SetVScrollMode(UG_SCROLLTRACKING);
	m_wndDataView.SetHScrollMode(UG_SCROLLTRACKING);
	if (m_iType == 1)
	{
		SetWindowText("Estimation data input");
		m_wndDataView.SetNumberCols(7);
		m_wndDataView.QuickSetText(0, -1, "Concentration");
		m_wndDataView.QuickSetText(1, -1, "Flexural strength");
		m_wndDataView.QuickSetText(2, -1, "Ice thickness");
		m_wndDataView.QuickSetText(3, -1, "Longitude");
		m_wndDataView.QuickSetText(4, -1, "Latitude");
		m_wndDataView.QuickSetText(5, -1, "X");
		m_wndDataView.QuickSetText(6, -1, "Y");
	}
	else
	{
		m_wndDataView.SetNumberCols(4);
		m_wndDataView.QuickSetText(0, -1, "Concentration");
		m_wndDataView.QuickSetText(1, -1, "Flexural strength");
		m_wndDataView.QuickSetText(2, -1, "Ice thickness");
		m_wndDataView.QuickSetText(3, -1, "Ship speed");
	}


	CRect rect;
	GetClientRect(&rect);
	SetSize(rect.Width(), rect.Height());

	if (m_pCurrentView->m_fConcentration.size() > 0)
	{
		int row_count = m_pCurrentView->m_fConcentration.size();
		m_wndDataView.SetNumberRows(row_count);
		CString value_string;
		for (int i = 0; i < row_count; i++)
		{
			m_wndDataView.QuickSetNumber(-1, i, i+1);
			value_string.Format("%lf", m_pCurrentView->m_fConcentration[i]);
			m_wndDataView.QuickSetText(0, i, value_string);
			value_string.Format("%lf", m_pCurrentView->m_fFlexuralStrength[i]);
			m_wndDataView.QuickSetText(1, i, value_string);
			value_string.Format("%lf", m_pCurrentView->m_fIceThickness[i]);
			m_wndDataView.QuickSetText(2, i, value_string);
			value_string.Format("%lf", m_pCurrentView->m_fShipSpeed[i]);
			m_wndDataView.QuickSetText(3, i, value_string);
		}
	}

	m_wndDataView.BestFit(-1, 3, 0, UG_BESTFIT_TOPHEADINGS);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgSatelliteData::OnOK()
{
	if (m_pCurrentView)
	{
		m_pCurrentView->m_fConcentration.clear();
		m_pCurrentView->m_fFlexuralStrength.clear();
		m_pCurrentView->m_fIceThickness.clear();
		m_pCurrentView->m_fShipSpeed.clear();
		m_pCurrentView->m_fLongitude.clear();
		m_pCurrentView->m_fLatitude.clear();
		m_pCurrentView->m_fX.clear();
		m_pCurrentView->m_fY.clear();
		m_pCurrentView->m_fedgeX.clear();
		m_pCurrentView->m_fedgeY.clear();

		CString file_path;
		double edit_value;
		GetDlgItem(IDC_EDIT_TARGET)->GetWindowText(file_path);
		if (file_path != "" && parse_double(string(file_path), edit_value))
		{
			m_pCurrentView->m_fTargetResistance = edit_value;
		}
		else
		{
			m_pCurrentView->m_fTargetResistance = 0;
		}
		GetDlgItem(IDC_EDIT_TARGET2)->GetWindowText(file_path);
		if (file_path != "" && parse_double(string(file_path), edit_value))
		{
			m_pCurrentView->m_fInitSpeed = edit_value;
		}
		else
		{
			m_pCurrentView->m_fInitSpeed = 0;
		}
		GetDlgItem(IDC_EDIT_TARGET3)->GetWindowText(file_path);
		if (file_path != "" && parse_double(string(file_path), edit_value))
		{
			m_pCurrentView->m_fMaxSpeed = edit_value;
		}
		else
		{
			m_pCurrentView->m_fMaxSpeed = 0;
		}
		GetDlgItem(IDC_EDIT_TARGET4)->GetWindowText(file_path);
		if (file_path != "" && parse_double(string(file_path), edit_value))
		{
			m_pCurrentView->m_fIncreSpeed = edit_value;
		}
		else
		{
			m_pCurrentView->m_fIncreSpeed = 0;
		}
		GetDlgItem(IDC_EDIT_TARGET5)->GetWindowText(file_path);
		if (file_path != "" && parse_double(string(file_path), edit_value))
		{
			m_pCurrentView->startRow = edit_value;
		}
		else
		{
			m_pCurrentView->startRow = 0;
		}
		GetDlgItem(IDC_EDIT_TARGET6)->GetWindowText(file_path);
		if (file_path != "" && parse_double(string(file_path), edit_value))
		{
			m_pCurrentView->startCol = edit_value;
		}
		else
		{
			m_pCurrentView->startCol = 0;
		}
		GetDlgItem(IDC_EDIT_TARGET7)->GetWindowText(file_path);
		if (file_path != "" && parse_double(string(file_path), edit_value))
		{
			m_pCurrentView->goalRow = edit_value;
		}
		else
		{
			m_pCurrentView->goalRow = 0;
		}
		GetDlgItem(IDC_EDIT_TARGET8)->GetWindowText(file_path);
		if (file_path != "" && parse_double(string(file_path), edit_value))
		{
			m_pCurrentView->goalCol = edit_value;
		}
		else
		{
			m_pCurrentView->goalCol = 0;
		}


		int rows = m_wndDataView.GetNumberRows();
		bool use_map = false;
		int max_x = 0;
		int max_y = 0;
		for (int i = 0; i < rows; i++)
		{
			double d;
			CString value = m_wndDataView.QuickGetText(0, i);
			if (value != "")
			{

				if (parse_double(string(value), d))
				{
					m_pCurrentView->m_fConcentration.push_back(d);
				}
				else
				{
					m_pCurrentView->m_fConcentration.push_back(0.0f);
				}
				value = m_wndDataView.QuickGetText(1, i);
				if (parse_double(string(value), d))
				{
					m_pCurrentView->m_fFlexuralStrength.push_back(d);
				}
				else
				{
					m_pCurrentView->m_fFlexuralStrength.push_back(0.0f);
				}
				value = m_wndDataView.QuickGetText(2, i);
				if (parse_double(string(value), d))
				{
					m_pCurrentView->m_fIceThickness.push_back(d);
				}
				else
				{
					m_pCurrentView->m_fIceThickness.push_back(0.0f);
				}
			}

			if (m_iType == 0)
			{
				CString value = m_wndDataView.QuickGetText(3, i);
				//string value(m_wndDataView.QuickGetText(3, i));
				if (value != "" && parse_double(string(value), d))
				{
					m_pCurrentView->m_fShipSpeed.push_back(d);
				}
				else
				{
					m_pCurrentView->m_fShipSpeed.push_back(1.0f);
				}
			}
			else
			{
				int col_size = m_wndDataView.GetNumberCols();
				if (col_size > 6)
				{ 
					use_map = true;
					value = m_wndDataView.QuickGetText(3, i);
					if (value != "")
					{
						if (parse_double(string(value), d))
						{
							m_pCurrentView->m_fLongitude.push_back(d);
						}
						else
						{
							m_pCurrentView->m_fLongitude.push_back(0.0f);
						}

						value = m_wndDataView.QuickGetText(4, i);
						if (parse_double(string(value), d))
						{
							m_pCurrentView->m_fLatitude.push_back(d);
						}
						else
						{
							m_pCurrentView->m_fLatitude.push_back(0.0f);
						}

						value = m_wndDataView.QuickGetText(5, i);
						if (parse_double(string(value), d))
						{
							m_pCurrentView->m_fX.push_back(d);
							if (d > max_x)
								max_x = d;
						}
						else
						{
							m_pCurrentView->m_fX.push_back(0.0f);
						}

						value = m_wndDataView.QuickGetText(6, i);
						if (parse_double(string(value), d))
						{
							m_pCurrentView->m_fY.push_back(d);
							if (d > max_y)
								max_y = d;
						}
						else
						{
							m_pCurrentView->m_fY.push_back(0.0f);
						}
					}

					value = m_wndDataView.QuickGetText(7, i);
					if (value != "")
					{
						if (parse_double(string(value), d))
						{
							m_pCurrentView->m_fedgeX.push_back(d);
							value = m_wndDataView.QuickGetText(8, i);
							if (parse_double(string(value), d))
							{
								m_pCurrentView->m_fedgeY.push_back(d);
							}
							else
							{
								m_pCurrentView->m_fedgeY.push_back(0);
							}
						}
					}
				}
			}
			//m_pCurrentView->m_fConcentration.push_back(atof(m_wndDataView.QuickGetText(0, i)));
			//m_pCurrentView->m_fFlexuralStrength.push_back(atof(m_wndDataView.QuickGetText(1, i)));
			//m_pCurrentView->m_fIceThickness.push_back(atof(m_wndDataView.QuickGetText(2, i)));
			//m_pCurrentView->m_fShipSpeed.push_back(atof(m_wndDataView.QuickGetText(3, i)));
		}

		if (use_map && max_x > 0 && max_y > 0)
		{
			m_pCurrentView->m_fExSpeed.resize(max_x + 1, vector< float >(max_y + 1, 0));
			m_pCurrentView->realMap.resize(max_x + 1, vector< int >(max_y + 1, 0));
		}
	}
	CDialog::OnOK();
}
