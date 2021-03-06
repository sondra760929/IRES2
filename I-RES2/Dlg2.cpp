// Dlg2.cpp: 구현 파일
//

#include "stdafx.h"
#include "I-RES2.h"
#include "Dlg2.h"
#include "afxdialogex.h"


// CDlg2 대화 상자

IMPLEMENT_DYNAMIC(CDlg2, CDialog)

CDlg2::CDlg2(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_2, pParent)
	, m_fInitialStrength(0)
	, m_fMaximumStrength(0)
	, m_fStrengthIncrements(0)
	, m_fFrictionCoeff(0)
	, m_fESigma(0)
	, m_fInitialThickness(0)
	, m_fMaximumThickness(0)
	, m_fThicknessIncrements(0)
	, m_fIceDensity(0)
	, m_fWaterDensity(0)
{

}

CDlg2::~CDlg2()
{
}

void CDlg2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INITIAL_STRENGTH, m_fInitialStrength);
	DDX_Text(pDX, IDC_EDIT_MAXIMUM_STRENGTH, m_fMaximumStrength);
	DDX_Text(pDX, IDC_EDIT_STRENGTH_INCREMENTS, m_fStrengthIncrements);
	DDX_Text(pDX, IDC_EDIT_FRICTION_COEFF, m_fFrictionCoeff);
	DDX_Text(pDX, IDC_EDIT_E_SIGMA, m_fESigma);
	DDX_Text(pDX, IDC_EDIT_INITIAL_THICKNESS, m_fInitialThickness);
	DDX_Text(pDX, IDC_EDIT_MAXIMUM_THICKNESS, m_fMaximumThickness);
	DDX_Text(pDX, IDC_EDIT_THICKNESS_INCREMENTS, m_fThicknessIncrements);
	DDX_Text(pDX, IDC_EDIT_ICE_DENSITY, m_fIceDensity);
	DDX_Text(pDX, IDC_EDIT_WATER_DENSITY, m_fWaterDensity);
}


BEGIN_MESSAGE_MAP(CDlg2, CDialog)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CDlg2::OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CDlg2::OnDeltaposSpin2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &CDlg2::OnDeltaposSpin3)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN4, &CDlg2::OnDeltaposSpin4)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN5, &CDlg2::OnDeltaposSpin5)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN6, &CDlg2::OnDeltaposSpin6)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN7, &CDlg2::OnDeltaposSpin7)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN8, &CDlg2::OnDeltaposSpin8)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN9, &CDlg2::OnDeltaposSpin9)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN10, &CDlg2::OnDeltaposSpin10)
	ON_EN_CHANGE(IDC_EDIT_INITIAL_STRENGTH, &CDlg2::OnEnChangeEditInitialStrength)
	ON_EN_CHANGE(IDC_EDIT_MAXIMUM_STRENGTH, &CDlg2::OnEnChangeEditMaximumStrength)
	ON_EN_CHANGE(IDC_EDIT_STRENGTH_INCREMENTS, &CDlg2::OnEnChangeEditStrengthIncrements)
	ON_EN_CHANGE(IDC_EDIT_FRICTION_COEFF, &CDlg2::OnEnChangeEditFrictionCoeff)
	ON_EN_CHANGE(IDC_EDIT_E_SIGMA, &CDlg2::OnEnChangeEditESigma)
	ON_EN_CHANGE(IDC_EDIT_INITIAL_THICKNESS, &CDlg2::OnEnChangeEditInitialThickness)
	ON_EN_CHANGE(IDC_EDIT_MAXIMUM_THICKNESS, &CDlg2::OnEnChangeEditMaximumThickness)
	ON_EN_CHANGE(IDC_EDIT_THICKNESS_INCREMENTS, &CDlg2::OnEnChangeEditThicknessIncrements)
	ON_EN_CHANGE(IDC_EDIT_ICE_DENSITY, &CDlg2::OnEnChangeEditIceDensity)
	ON_EN_CHANGE(IDC_EDIT_WATER_DENSITY, &CDlg2::OnEnChangeEditWaterDensity)
END_MESSAGE_MAP()


// CDlg2 메시지 처리기


void CDlg2::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if (pNMUpDown->iDelta < 0)
	{
		m_fInitialStrength += 1.0;
	}
	else
	{
		m_fInitialStrength -= 1.0;
	}
	UpdateData(FALSE);
	*pResult = 0;
}

void CDlg2::OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if (pNMUpDown->iDelta < 0)
	{
		m_fMaximumStrength += 1.0;
	}
	else
	{
		m_fMaximumStrength -= 1.0;
	}
	UpdateData(FALSE);
	*pResult = 0;
}

void CDlg2::OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if (pNMUpDown->iDelta < 0)
	{
		m_fStrengthIncrements += 1.0;
	}
	else
	{
		m_fStrengthIncrements -= 1.0;
	}
	UpdateData(FALSE);
	*pResult = 0;
}

void CDlg2::OnDeltaposSpin4(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if (pNMUpDown->iDelta < 0)
	{
		m_fFrictionCoeff += 0.01;
	}
	else
	{
		m_fFrictionCoeff -= 0.01;
	}
	UpdateData(FALSE);
	*pResult = 0;
}

void CDlg2::OnDeltaposSpin8(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if (pNMUpDown->iDelta < 0)
	{
		m_fESigma += 0.01;
	}
	else
	{
		m_fESigma -= 0.01;
	}
	UpdateData(FALSE);
	*pResult = 0;
}

void CDlg2::OnDeltaposSpin5(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if (pNMUpDown->iDelta < 0)
	{
		m_fInitialThickness += 0.01;
	}
	else
	{
		m_fInitialThickness -= 0.01;
	}
	UpdateData(FALSE);
	*pResult = 0;
}

void CDlg2::OnDeltaposSpin6(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if (pNMUpDown->iDelta < 0)
	{
		m_fMaximumThickness += 0.01;
	}
	else
	{
		m_fMaximumThickness -= 0.01;
	}
	UpdateData(FALSE);
	*pResult = 0;
}

void CDlg2::OnDeltaposSpin7(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if (pNMUpDown->iDelta < 0)
	{
		m_fThicknessIncrements += 0.01;
	}
	else
	{
		m_fThicknessIncrements -= 0.01;
	}
	UpdateData(FALSE);
	*pResult = 0;
}

void CDlg2::OnDeltaposSpin9(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if (pNMUpDown->iDelta < 0)
	{
		m_fIceDensity += 0.01;
	}
	else
	{
		m_fIceDensity -= 0.01;
	}
	UpdateData(FALSE);
	*pResult = 0;
}

void CDlg2::OnDeltaposSpin10(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if (pNMUpDown->iDelta < 0)
	{
		m_fWaterDensity += 0.01;
	}
	else
	{
		m_fWaterDensity -= 0.01;
	}
	UpdateData(FALSE);
	*pResult = 0;
}

void CDlg2::OnEnChangeEditInitialStrength()
{
	UpdateData();
}


void CDlg2::OnEnChangeEditMaximumStrength()
{
	UpdateData();
}


void CDlg2::OnEnChangeEditStrengthIncrements()
{
	UpdateData();
}


void CDlg2::OnEnChangeEditFrictionCoeff()
{
	UpdateData();
}


void CDlg2::OnEnChangeEditESigma()
{
	UpdateData();
}


void CDlg2::OnEnChangeEditInitialThickness()
{
	UpdateData();
}


void CDlg2::OnEnChangeEditMaximumThickness()
{
	UpdateData();
}


void CDlg2::OnEnChangeEditThicknessIncrements()
{
	UpdateData();
}


void CDlg2::OnEnChangeEditIceDensity()
{
	UpdateData();
}


void CDlg2::OnEnChangeEditWaterDensity()
{
	UpdateData();
}


BOOL CDlg2::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}
