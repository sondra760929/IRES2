// DlgCrossSection.cpp: 구현 파일
//

#include "stdafx.h"
#include "I-RES2.h"
#include "DlgCrossSection.h"
#include "afxdialogex.h"
#include "I-RES2View.h"


// CDlgCrossSection 대화 상자

IMPLEMENT_DYNAMIC(CDlgCrossSection, CDialog)

CDlgCrossSection::CDlgCrossSection(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_CROSSSECTION, pParent)
	, m_fStart(0)
	, m_fEnd(0)
	, m_fInterval(0)
	, m_fPointDistance(0)
	, m_fAxisDistance(0)
{
	m_pView = (CIRES2View*)pParent;
}

CDlgCrossSection::~CDlgCrossSection()
{
}

void CDlgCrossSection::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCBUTTON_CALCULATE, m_btnCalc);
	DDX_Text(pDX, IDC_EDIT_DRAFT, m_fStart);
	DDX_Text(pDX, IDC_EDIT_DRAFT2, m_fEnd);
	DDX_Text(pDX, IDC_EDIT_DRAFT3, m_fInterval);
	DDX_Text(pDX, IDC_EDIT_POINT_DISTANCE, m_fPointDistance);
	DDX_Text(pDX, IDC_EDIT_AXIS_DISTANCE, m_fAxisDistance);
	DDX_Control(pDX, IDC_CHECK_POINT, m_chkPointDistance);
	DDX_Control(pDX, IDC_CHECK_AXIS, m_chkAxisDistance);
}


BEGIN_MESSAGE_MAP(CDlgCrossSection, CDialog)
	ON_BN_CLICKED(IDC_MFCBUTTON_CALCULATE, &CDlgCrossSection::OnBnClickedMfcbuttonCalculate)
	ON_BN_CLICKED(IDC_CHECK_POINT, &CDlgCrossSection::OnBnClickedCheckPoint)
	ON_BN_CLICKED(IDC_CHECK_AXIS, &CDlgCrossSection::OnBnClickedCheckAxis)
END_MESSAGE_MAP()


// CDlgCrossSection 메시지 처리기


BOOL CDlgCrossSection::OnInitDialog()
{
	CDialog::OnInitDialog();

	CImage btn_image1;
	btn_image1.LoadFromResource(AfxGetInstanceHandle(), IDB_BITMAP_CALC);
	HBITMAP hBit1;
	hBit1 = btn_image1.Detach();
	m_btnCalc.SetImage(hBit1, TRUE, hBit1);
	m_btnCalc.SetTooltip(_T("Calculate Section Points"));
	m_btnCalc.EnableFullTextTooltip(FALSE);

	if (m_pView)
	{
		m_fStart = m_pView->m_fCrossSectionStart * UNIT_TO_M;
		m_fEnd = m_pView->m_fCrossSectionEnd  * UNIT_TO_M;
		m_fInterval = m_pView->m_fCrossSectionOffset * UNIT_TO_M;
		m_fPointDistance = m_pView->m_fCrossSectionPointGap * UNIT_TO_M;
		m_fAxisDistance = m_pView->m_fCrossSectionPointGap * UNIT_TO_M;
		if (m_pView->m_bUseDistanceForAxis)
		{
			m_chkPointDistance.SetCheck(TRUE);
			m_chkAxisDistance.SetCheck(FALSE);
		}
		else
		{
			m_chkAxisDistance.SetCheck(TRUE);
			m_chkPointDistance.SetCheck(FALSE);
		}
	}
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgCrossSection::OnBnClickedMfcbuttonCalculate()
{
	UpdateData();
	m_pView->m_fCrossSectionStart = m_fStart * M_TO_UNIT;
	m_pView->m_fCrossSectionEnd = m_fEnd * M_TO_UNIT;
	m_pView->m_fCrossSectionOffset = m_fInterval * M_TO_UNIT;
	if (m_pView->m_bUseDistanceForAxis)
	{
		m_pView->m_fCrossSectionPointGap = m_fPointDistance * M_TO_UNIT;
	}
	else
	{
		m_pView->m_fCrossSectionPointGap = m_fAxisDistance * M_TO_UNIT;
	}

	m_pView->SaveCrossSectionSetting();
	m_pView->CalculateSectionPoint();

	CDialog::OnOK();
}


void CDlgCrossSection::OnBnClickedCheckPoint()
{
	if (m_pView)
	{
		m_pView->m_bUseDistanceForAxis = true;
		if (m_pView->m_bUseDistanceForAxis)
		{
			m_chkPointDistance.SetCheck(TRUE);
			m_chkAxisDistance.SetCheck(FALSE);
		}
		else
		{
			m_chkAxisDistance.SetCheck(TRUE);
			m_chkPointDistance.SetCheck(FALSE);
		}
	}
}


void CDlgCrossSection::OnBnClickedCheckAxis()
{
	if (m_pView)
	{
		m_pView->m_bUseDistanceForAxis = false;
		if (m_pView->m_bUseDistanceForAxis)
		{
			m_chkPointDistance.SetCheck(TRUE);
			m_chkAxisDistance.SetCheck(FALSE);
		}
		else
		{
			m_chkAxisDistance.SetCheck(TRUE);
			m_chkPointDistance.SetCheck(FALSE);
		}
	}
}
