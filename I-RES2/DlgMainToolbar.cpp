﻿// DlgMainToolbar.cpp: 구현 파일
//

#include "stdafx.h"
#include "I-RES2.h"
#include "DlgMainToolbar.h"
#include "afxdialogex.h"


// CDlgMainToolbar 대화 상자

IMPLEMENT_DYNAMIC(CDlgMainToolbar, CDialog)

CDlgMainToolbar::CDlgMainToolbar(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_MAIN_TOOLBAR, pParent)
	, m_pView(0)
{

}

CDlgMainToolbar::~CDlgMainToolbar()
{
}

void CDlgMainToolbar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCBUTTON_CLEAR, m_btnClear);
	DDX_Control(pDX, IDC_MFCBUTTON_TRANS, m_btnTrans);
	DDX_Control(pDX, IDC_MFCBUTTON_ROT, m_btnRot);
	DDX_Control(pDX, IDC_MFCBUTTON_DATUM, m_btnDatum);
	DDX_Control(pDX, IDC_MFCBUTTON_NORMAL, m_btnNormal);
	DDX_Control(pDX, IDC_CHECK_UNIT_M, m_chkUnitM);
	DDX_Control(pDX, IDC_CHECK_UNIT_MM, m_chkUnitmm);
}


BEGIN_MESSAGE_MAP(CDlgMainToolbar, CDialog)
	ON_BN_CLICKED(IDC_MFCBUTTON_CLEAR, &CDlgMainToolbar::OnBnClickedMfcbuttonClear)
	ON_BN_CLICKED(IDC_MFCBUTTON_TRANS, &CDlgMainToolbar::OnBnClickedMfcbuttonTrans)
	ON_BN_CLICKED(IDC_MFCBUTTON_ROT, &CDlgMainToolbar::OnBnClickedMfcbuttonRot)
	ON_BN_CLICKED(IDC_MFCBUTTON_DATUM, &CDlgMainToolbar::OnBnClickedMfcbuttonDatum)
	ON_BN_CLICKED(IDC_MFCBUTTON_NORMAL, &CDlgMainToolbar::OnBnClickedMfcbuttonNormal)
	ON_BN_CLICKED(IDC_CHECK_UNIT_M, &CDlgMainToolbar::OnBnClickedCheckUnitM)
	ON_BN_CLICKED(IDC_CHECK_UNIT_MM, &CDlgMainToolbar::OnBnClickedCheckUnitMm)
END_MESSAGE_MAP()


// CDlgMainToolbar 메시지 처리기


BOOL CDlgMainToolbar::OnInitDialog()
{
	CDialog::OnInitDialog();

	CImage btn_image1;
	CImage btn_image2;
	CImage btn_image3;
	CImage btn_image4;
	CImage btn_image5;
	btn_image1.LoadFromResource(AfxGetInstanceHandle(), IDB_BITMAP_NEW);
	btn_image2.LoadFromResource(AfxGetInstanceHandle(), IDB_BITMAP_TRANS);
	btn_image3.LoadFromResource(AfxGetInstanceHandle(), IDB_BITMAP_ROT);
	btn_image4.LoadFromResource(AfxGetInstanceHandle(), IDB_BITMAP_DATUM);
	btn_image5.LoadFromResource(AfxGetInstanceHandle(), IDB_BITMAP_NORMAL);
	HBITMAP hBit1;
	HBITMAP hBit2;
	HBITMAP hBit3;
	HBITMAP hBit4;
	HBITMAP hBit5;
	hBit1 = btn_image1.Detach();
	hBit2 = btn_image2.Detach();
	hBit3 = btn_image3.Detach();
	hBit4 = btn_image4.Detach();
	hBit5 = btn_image5.Detach();

	m_btnClear.SetImage(hBit1, TRUE, hBit1);
	m_btnTrans.SetImage(hBit2, TRUE, hBit2);
	m_btnRot.SetImage(hBit3, TRUE, hBit3);
	m_btnDatum.SetImage(hBit4, TRUE, hBit4);
	m_btnNormal.SetImage(hBit5, TRUE, hBit5);

	m_btnClear.SetTooltip(_T("New Model"));
	m_btnClear.EnableFullTextTooltip(FALSE);
	m_btnTrans.SetTooltip(_T("Translate Hull"));
	m_btnTrans.EnableFullTextTooltip(FALSE);
	m_btnRot.SetTooltip(_T("Rotate Hull"));
	m_btnRot.EnableFullTextTooltip(FALSE);
	m_btnDatum.SetTooltip(_T("Create datum CSYS(coordinate system)"));
	m_btnDatum.EnableFullTextTooltip(FALSE);
	m_btnNormal.SetTooltip(_T("Change normal direction"));
	m_btnNormal.EnableFullTextTooltip(FALSE);

	m_chkUnitM.SetCheck(0);
	m_chkUnitmm.SetCheck(1);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgMainToolbar::OnBnClickedMfcbuttonClear()
{
	if (m_pView)
	{
		m_pView->OnButtonImportHull();
	}
}


void CDlgMainToolbar::OnBnClickedMfcbuttonTrans()
{
	if (m_pView)
	{
		m_pView->OnButtonTranslate();
	}
}


void CDlgMainToolbar::OnBnClickedMfcbuttonRot()
{
	if (m_pView)
	{
		m_pView->OnButtonRotate();
	}
}


void CDlgMainToolbar::OnBnClickedMfcbuttonDatum()
{
	if (m_pView)
	{
		m_pView->OnButtonMakeDatum();
	}
}


void CDlgMainToolbar::OnBnClickedMfcbuttonNormal()
{
	if (m_pView)
	{
		m_pView->OnButtonSetNormal();
	}
}


void CDlgMainToolbar::OnBnClickedCheckUnitM()
{
	if (m_pView)
	{
		m_pView->OnButtonSetUnit(UNIT_M);
		m_chkUnitM.SetCheck(1);
		m_chkUnitmm.SetCheck(0);
	}
}


void CDlgMainToolbar::OnBnClickedCheckUnitMm()
{
	if (m_pView)
	{
		m_pView->OnButtonSetUnit(UNIT_MM);
		m_chkUnitM.SetCheck(0);
		m_chkUnitmm.SetCheck(1);
	}
}
