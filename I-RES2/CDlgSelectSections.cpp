// CDlgSelectSections.cpp: 구현 파일
//

#include "stdafx.h"
#include "I-RES2.h"
#include "CDlgSelectSections.h"
#include "afxdialogex.h"
#include "I-RES2View.h"

// CDlgSelectSections 대화 상자

IMPLEMENT_DYNAMIC(CDlgSelectSections, CDialog)

CDlgSelectSections::CDlgSelectSections(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_SELECT_SECTIONS, pParent)
{
	m_pView = (CIRES2View*)pParent;
}

CDlgSelectSections::~CDlgSelectSections()
{
}

void CDlgSelectSections::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_lstSections);
}


BEGIN_MESSAGE_MAP(CDlgSelectSections, CDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDOK, &CDlgSelectSections::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgSelectSections 메시지 처리기


BOOL CDlgSelectSections::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_lstSections.SetCheckStyle(BS_AUTOCHECKBOX);

	CString item_string;
	int count = m_pView->osgSectionPosList.size();
	for (int i = 0; i < count; i++)
	{
		if (m_pView->m_bBowBreaking)
			item_string.Format("%.6lf", m_pView->osgSectionPosList[i].x() * UNIT_TO_M);
		else
			item_string.Format("%.6lf", m_pView->osgSectionPosList[i].y() * UNIT_TO_M);
		m_lstSections.InsertString(i, item_string);
		m_lstSections.SetCheck(i, m_pView->osgSectionEnable[i]);
	}

	m_lstSections.RedrawWindow();

	CRect rect;
	GetClientRect(&rect);
	ResizeControl(rect.Width(), rect.Height());
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgSelectSections::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	ResizeControl(cx, cy);
}


void CDlgSelectSections::OnBnClickedOk()
{
	int count = m_pView->osgSectionPosList.size();
	bool is_update = false;
	osg::Vec3 diff(0, 0, -m_pView->bbLength[2] * 2.0f);
	for (int i = 0; i < count; i++)
	{
		if (m_pView->osgSectionEnable[i] != m_lstSections.GetCheck(i))
		{
			m_pView->osgSectionEnable[i] = m_lstSections.GetCheck(i);
			is_update = true;

			osg::Matrix m;
			if (m_pView->osgSectionEnable[i])
			{
				m.setTrans(m_pView->osgSectionPosList[i]);
			}
			else
			{
				m.setTrans(m_pView->osgSectionPosList[i] + diff);
			}
			m.setRotate(m_pView->osgSectionRotation);
			//m_pView->osgSections[i]->setMatrix(m);
		}
	}

	if (is_update)
	{
		//m_pView->SetCurrentStep(2);
		//m_pView->ClearSectionPoints();
	}
	CDialog::OnOK();
}

void CDlgSelectSections::ResizeControl(int cx, int cy)
{
	if (m_lstSections)
	{
		int gap = 3;
		int button_width = 50;
		int button_height = 20;
		m_lstSections.MoveWindow(gap, gap, cx - gap - gap, cy - gap - gap - button_height);
		GetDlgItem(IDCANCEL)->MoveWindow(cx - gap - button_width, cy - gap - button_height, button_width, button_height);
		GetDlgItem(IDOK)->MoveWindow(cx - gap - button_width - gap - button_width, cy - gap - button_height, button_width, button_height);
	}
}