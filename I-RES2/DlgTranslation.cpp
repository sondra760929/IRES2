// DlgTranslation.cpp: 구현 파일
//

#include "stdafx.h"
#include "I-RES2.h"
#include "DlgTranslation.h"
#include "afxdialogex.h"


// CDlgTranslation 대화 상자

IMPLEMENT_DYNAMIC(CDlgTranslation, CDialog)

CDlgTranslation::CDlgTranslation(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_TRANSLATE, pParent)
	, m_strCaption(_T(""))
	, m_strVector(_T(""))
{

}

CDlgTranslation::~CDlgTranslation()
{
}

void CDlgTranslation::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_CAPTION, m_strCaption);
	DDX_Text(pDX, IDC_EDIT1, m_strVector);
}


BEGIN_MESSAGE_MAP(CDlgTranslation, CDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_OK, &CDlgTranslation::OnBnClickedButtonOk)
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()


// CDlgTranslation 메시지 처리기


BOOL CDlgTranslation::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_strCaption = "   << Select a start point for the translation vector or enter X, Y, Z ";
	UpdateData(FALSE);

	CRect rect;
	GetClientRect(&rect);
	ResizeControl(rect.Width(), rect.Height());
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgTranslation::ResizeControl(int cx, int cy)
{
	CWnd* p_cpation = GetDlgItem(IDC_STATIC_CAPTION);
	if (p_cpation)
	{
		int width = 450;
		int button_width = 40;
		p_cpation->MoveWindow(0, 0, width, cy);
		GetDlgItem(IDC_EDIT1)->MoveWindow(width, 0, cx-width - button_width, cy);
		GetDlgItem(IDC_BUTTON_OK)->MoveWindow(cx - button_width, 0, button_width, cy);
	}
}

void CDlgTranslation::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	ResizeControl(cx, cy);
}


void CDlgTranslation::OnBnClickedButtonOk()
{
	UpdateData();
	CString value_string;
	value_string = m_strVector;
	vector< float > values;
	int index = value_string.Find(",");
	while (index > 0)
	{
		values.push_back(atof(value_string.Left(index)));
		value_string.Delete(0, index + 1);
		index = value_string.Find(",");
	}
	if (value_string != "")
	{
		values.push_back(atof(value_string));
	}

	if (values.size() == m_iInputDataCount)
	{

		switch (m_iInputDataCount)
		{
		case 1:
		{
			float x = values[0];
			m_pCurrentView->SetDlgPoint(x * M_TO_UNIT, 0, 0);
		}
		break;
		case 2:
		{
			float x = values[0];
			float y = values[1];
			m_pCurrentView->SetDlgPoint(x * M_TO_UNIT, y * M_TO_UNIT, 0);
		}
		break;
		case 3:
		{
			float x = values[0];
			float y = values[1];
			float z = values[2];
			m_pCurrentView->SetDlgPoint(x * M_TO_UNIT, y * M_TO_UNIT, z * M_TO_UNIT);
		}
		break;
		}
	}
}

void CDlgTranslation::SetCaption(CString caption, CString title, int data_count)
{
	m_iInputDataCount = data_count;
	m_strCaption = caption;
	SetWindowText(title);
	UpdateData(FALSE);
}

void CDlgTranslation::SetValueString(CString value_str)
{
	m_strVector = value_str;
	UpdateData(FALSE);
}

void CDlgTranslation::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	lpMMI->ptMinTrackSize.x = 28 * 4;
	lpMMI->ptMinTrackSize.y = 65;

	lpMMI->ptMaxTrackSize.x = 1024;
	lpMMI->ptMaxTrackSize.y = 65;

	CDialog::OnGetMinMaxInfo(lpMMI);
}


void CDlgTranslation::OnCancel()
{
	m_pCurrentView->OnKeyESC();

	//CDialog::OnCancel();
}


void CDlgTranslation::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	//CDialog::OnOK();
}
