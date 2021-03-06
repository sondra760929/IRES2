// Dlg1.cpp: 구현 파일
//

#include "stdafx.h"
#include "I-RES2.h"
#include "Dlg1.h"
#include "afxdialogex.h"


// CDlg1 대화 상자

IMPLEMENT_DYNAMIC(CDlg1, CDialog)

CDlg1::CDlg1(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_1, pParent)
	, m_fLength(0)
	, m_fMoldedDepth(0)
	, m_fMoldedBreadth(0)
	, m_fDesignDraft(0)
	, m_fInitialSpeed(0)
	, m_fMaximumSpeed(0)
	, m_fSpeedIncrements(0)
{

}

CDlg1::~CDlg1()
{
}

void CDlg1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LENGTH, m_fLength);
	DDX_Text(pDX, IDC_EDIT_MODELED_DEPTH, m_fMoldedDepth);
	DDX_Text(pDX, IDC_EDIT_MODELED_BREADTH, m_fMoldedBreadth);
	DDX_Text(pDX, IDC_EDIT_DESIGN_DRAFT, m_fDesignDraft);
	DDX_Text(pDX, IDC_EDIT_INITIAL_SPEED, m_fInitialSpeed);
	DDX_Text(pDX, IDC_EDIT_MAXIMUM_SPEED, m_fMaximumSpeed);
	DDX_Text(pDX, IDC_EDIT_SPEED_INCREMENTS, m_fSpeedIncrements);
}


BEGIN_MESSAGE_MAP(CDlg1, CDialog)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CDlg1::OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CDlg1::OnDeltaposSpin2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &CDlg1::OnDeltaposSpin3)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN4, &CDlg1::OnDeltaposSpin4)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN5, &CDlg1::OnDeltaposSpin5)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN6, &CDlg1::OnDeltaposSpin6)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN7, &CDlg1::OnDeltaposSpin7)
	ON_EN_CHANGE(IDC_EDIT_LENGTH, &CDlg1::OnEnChangeEditLength)
	ON_EN_CHANGE(IDC_EDIT_MODELED_DEPTH, &CDlg1::OnEnChangeEditModeledDepth)
	ON_EN_CHANGE(IDC_EDIT_MODELED_BREADTH, &CDlg1::OnEnChangeEditModeledBreadth)
	ON_EN_CHANGE(IDC_EDIT_DESIGN_DRAFT, &CDlg1::OnEnChangeEditDesignDraft)
	ON_EN_CHANGE(IDC_EDIT_INITIAL_SPEED, &CDlg1::OnEnChangeEditInitialSpeed)
	ON_EN_CHANGE(IDC_EDIT_MAXIMUM_SPEED, &CDlg1::OnEnChangeEditMaximumSpeed)
	ON_EN_CHANGE(IDC_EDIT_SPEED_INCREMENTS, &CDlg1::OnEnChangeEditSpeedIncrements)
END_MESSAGE_MAP()


// CDlg1 메시지 처리기


BOOL CDlg1::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

void CDlg1::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if (pNMUpDown->iDelta < 0)
	{
		m_fLength += 0.01;
	}
	else
	{
		m_fLength -= 0.01;
	}
	UpdateData(FALSE);
	*pResult = 0;
}

void CDlg1::OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if (pNMUpDown->iDelta < 0)
	{
		m_fMoldedDepth += 0.01;
	}
	else
	{
		m_fMoldedDepth -= 0.01;
	}
	UpdateData(FALSE);
	*pResult = 0;
}

void CDlg1::OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if (pNMUpDown->iDelta < 0)
	{
		m_fMoldedBreadth += 0.01;
	}
	else
	{
		m_fMoldedBreadth -= 0.01;
	}
	UpdateData(FALSE);
	*pResult = 0;
}

void CDlg1::OnDeltaposSpin4(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if (pNMUpDown->iDelta < 0)
	{
		m_fDesignDraft += 0.01;
	}
	else
	{
		m_fDesignDraft -= 0.01;
	}
	UpdateData(FALSE);
	*pResult = 0;
}

void CDlg1::OnDeltaposSpin5(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if (pNMUpDown->iDelta < 0)
	{
		m_fInitialSpeed += 0.01;
	}
	else
	{
		m_fInitialSpeed -= 0.01;
	}
	UpdateData(FALSE);
	*pResult = 0;
}

void CDlg1::OnDeltaposSpin6(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if (pNMUpDown->iDelta < 0)
	{
		m_fMaximumSpeed += 0.01;
	}
	else
	{
		m_fMaximumSpeed -= 0.01;
	}
	UpdateData(FALSE);
	*pResult = 0;
}

void CDlg1::OnDeltaposSpin7(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if (pNMUpDown->iDelta < 0)
	{
		m_fSpeedIncrements += 0.01;
	}
	else
	{
		m_fSpeedIncrements -= 0.01;
	}
	UpdateData(FALSE);
	*pResult = 0;
}


void CDlg1::OnEnChangeEditLength()
{
	UpdateData();
}


void CDlg1::OnEnChangeEditModeledDepth()
{
	UpdateData();
}


void CDlg1::OnEnChangeEditModeledBreadth()
{
	UpdateData();
}


void CDlg1::OnEnChangeEditDesignDraft()
{
	UpdateData();
}


void CDlg1::OnEnChangeEditInitialSpeed()
{
	UpdateData();
}


void CDlg1::OnEnChangeEditMaximumSpeed()
{
	UpdateData();
}


void CDlg1::OnEnChangeEditSpeedIncrements()
{
	UpdateData();
}
