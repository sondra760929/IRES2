// DlgDraftSection.cpp: 구현 파일
//

#include "stdafx.h"
#include "I-RES2.h"
#include "DlgDraftSection.h"
#include "afxdialogex.h"
#include "I-RES2View.h"

// CDlgDraftSection 대화 상자

IMPLEMENT_DYNAMIC(CDlgDraftSection, CDialog)

CDlgDraftSection::CDlgDraftSection(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_DRAFTSECTION, pParent)
	, m_fDraft(0)
	, m_fPointDistance(0)
	, m_fAxisDistance(0)
{
	m_pView = (CIRES2View*)pParent;
}

CDlgDraftSection::~CDlgDraftSection()
{
}

void CDlgDraftSection::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCBUTTON_CALCULATE, m_btnCalc);
	DDX_Control(pDX, IDC_CHECK_POINT, m_chkDistance);
	DDX_Control(pDX, IDC_CHECK_AXIS, m_chkAxis);
	DDX_Text(pDX, IDC_EDIT_DRAFT, m_fDraft);
	DDX_Text(pDX, IDC_EDIT_POINT_DISTANCE, m_fPointDistance);
	DDX_Text(pDX, IDC_EDIT_AXIS_DISTANCE, m_fAxisDistance);
}


BEGIN_MESSAGE_MAP(CDlgDraftSection, CDialog)
	ON_BN_CLICKED(IDC_CHECK_POINT, &CDlgDraftSection::OnBnClickedCheckPoint)
	ON_BN_CLICKED(IDC_CHECK_AXIS, &CDlgDraftSection::OnBnClickedCheckAxis)
	ON_BN_CLICKED(IDC_MFCBUTTON_CALCULATE, &CDlgDraftSection::OnBnClickedMfcbuttonCalculate)
END_MESSAGE_MAP()


// CDlgDraftSection 메시지 처리기


BOOL CDlgDraftSection::OnInitDialog()
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
		m_pView->LoadIceInput();
		m_fDraft = m_pView->DRAFT;
		m_fPointDistance = m_pView->m_fWaterlinePointGap * UNIT_TO_M;
		m_fAxisDistance = m_pView->m_fWaterlinePointGap * UNIT_TO_M;
		if (m_pView->m_bUseDistanceForAxisWaterline)
		{
			m_chkDistance.SetCheck(TRUE);
			m_chkAxis.SetCheck(FALSE);
		}
		else
		{
			m_chkAxis.SetCheck(TRUE);
			m_chkDistance.SetCheck(FALSE);
		}
	}
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgDraftSection::OnBnClickedCheckPoint()
{
	if (m_pView)
	{
		m_pView->m_bUseDistanceForAxisWaterline = true;
		if (m_pView->m_bUseDistanceForAxisWaterline)
		{
			m_chkDistance.SetCheck(TRUE);
			m_chkAxis.SetCheck(FALSE);
		}
		else
		{
			m_chkAxis.SetCheck(TRUE);
			m_chkDistance.SetCheck(FALSE);
		}
	}
}


void CDlgDraftSection::OnBnClickedCheckAxis()
{
	if (m_pView)
	{
		m_pView->m_bUseDistanceForAxisWaterline = false;
		if (m_pView->m_bUseDistanceForAxisWaterline)
		{
			m_chkDistance.SetCheck(TRUE);
			m_chkAxis.SetCheck(FALSE);
		}
		else
		{
			m_chkAxis.SetCheck(TRUE);
			m_chkDistance.SetCheck(FALSE);
		}
	}
}


void CDlgDraftSection::OnBnClickedMfcbuttonCalculate()
{
	UpdateData();
	m_pView->DRAFT = m_fDraft;
	if (m_pView->m_bUseDistanceForAxisWaterline)
	{
		m_pView->m_fWaterlinePointGap = m_fPointDistance * M_TO_UNIT;
	}
	else
	{
		m_pView->m_fWaterlinePointGap = m_fAxisDistance * M_TO_UNIT;
	}

	m_pView->SaveDraftSectionSetting();
	m_pView->SaveIceInput();
	m_pView->CalculateWaterSectionPoint();

	CDialog::OnOK();
}
