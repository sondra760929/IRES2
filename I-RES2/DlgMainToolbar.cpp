﻿// DlgMainToolbar.cpp: 구현 파일
//

#include "stdafx.h"
#include "I-RES2.h"
#include "DlgMainToolbar.h"
#include "afxdialogex.h"
#include "resource.h"


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
	DDX_Control(pDX, IDC_MFCBUTTON_SETUNIT, m_btnSetUnit);
	DDX_Control(pDX, IDC_MFCBUTTON_CALC, m_btnCalc);
	DDX_Control(pDX, IDC_CHECK_OBLIQUE, m_chkOblique);
}


BEGIN_MESSAGE_MAP(CDlgMainToolbar, CDialog)
	ON_BN_CLICKED(IDC_MFCBUTTON_CLEAR, &CDlgMainToolbar::OnBnClickedMfcbuttonClear)
	ON_BN_CLICKED(IDC_MFCBUTTON_TRANS, &CDlgMainToolbar::OnBnClickedMfcbuttonTrans)
	ON_BN_CLICKED(IDC_MFCBUTTON_ROT, &CDlgMainToolbar::OnBnClickedMfcbuttonRot)
	ON_BN_CLICKED(IDC_MFCBUTTON_DATUM, &CDlgMainToolbar::OnBnClickedMfcbuttonDatum)
	ON_BN_CLICKED(IDC_MFCBUTTON_NORMAL, &CDlgMainToolbar::OnBnClickedMfcbuttonNormal)
	ON_BN_CLICKED(IDC_MFCBUTTON_SETUNIT, &CDlgMainToolbar::OnBnClickedMfcbuttonSetunit)
	ON_BN_CLICKED(IDC_MFCBUTTON_CALC, &CDlgMainToolbar::OnBnClickedMfcbuttonCalc)
	ON_BN_CLICKED(IDC_CHECK_OBLIQUE, &CDlgMainToolbar::OnBnClickedCheckOblique)
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
	CImage btn_image6;
	CImage btn_image7;
	CImage btn_image8;
	CImage btn_image9;
	btn_image1.LoadFromResource(AfxGetInstanceHandle(), IDB_BITMAP_NEW);
	btn_image2.LoadFromResource(AfxGetInstanceHandle(), IDB_BITMAP_TRANS);
	btn_image3.LoadFromResource(AfxGetInstanceHandle(), IDB_BITMAP_ROT);
	btn_image4.LoadFromResource(AfxGetInstanceHandle(), IDB_BITMAP_DATUM);
	btn_image5.LoadFromResource(AfxGetInstanceHandle(), IDB_BITMAP_NORMAL);
	btn_image6.LoadFromResource(AfxGetInstanceHandle(), IDB_BITMAP_SETUNIT);
	btn_image7.LoadFromResource(AfxGetInstanceHandle(), IDB_BITMAP_GRAPH);
	btn_image8.LoadFromResource(AfxGetInstanceHandle(), IDB_BITMAP_OBLIQUE1);
	btn_image9.LoadFromResource(AfxGetInstanceHandle(), IDB_BITMAP_OBLIQUE2);
	HBITMAP hBit1;
	HBITMAP hBit2;
	HBITMAP hBit3;
	HBITMAP hBit4;
	HBITMAP hBit5;
	HBITMAP hBit6;
	HBITMAP hBit7;
	hBit1 = btn_image1.Detach();
	hBit2 = btn_image2.Detach();
	hBit3 = btn_image3.Detach();
	hBit4 = btn_image4.Detach();
	hBit5 = btn_image5.Detach();
	hBit6 = btn_image6.Detach();
	hBit7 = btn_image7.Detach();
	hBit8 = btn_image8.Detach();
	hBit9 = btn_image9.Detach();

	m_btnClear.SetImage(hBit1, TRUE, hBit1);
	m_btnTrans.SetImage(hBit2, TRUE, hBit2);
	m_btnRot.SetImage(hBit3, TRUE, hBit3);
	m_btnDatum.SetImage(hBit4, TRUE, hBit4);
	m_btnNormal.SetImage(hBit5, TRUE, hBit5);
	m_btnSetUnit.SetImage(hBit6, TRUE, hBit6);
	m_btnCalc.SetImage(hBit7, TRUE, hBit7);
	m_chkOblique.SetBitmap(hBit8);
	//m_chkOblique.SetImage(hBit8, TRUE, hBit8);

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
	m_btnSetUnit.SetTooltip(_T("Change unit to M"));
	m_btnSetUnit.EnableFullTextTooltip(FALSE);
	m_btnCalc.SetTooltip(_T("Calculation Module"));
	m_btnCalc.EnableFullTextTooltip(FALSE);
	//m_chkOblique.SetTooltip(_T("Oblique Mode"));
	//m_chkOblique.EnableFullTextTooltip(FALSE);

	int width = 28;
	int top = 0;
	m_btnClear.MoveWindow(0, top, width, width);
	top += width;
	m_btnTrans.MoveWindow(0, top, width, width);
	top += width;
	m_btnRot.MoveWindow(0, top, width, width);
	top += width;
	m_btnDatum.MoveWindow(0, top, width, width);
	top += width;
	m_btnNormal.MoveWindow(0, top, width, width);
	top += width;
	m_btnSetUnit.MoveWindow(0, top, width, width);
	top += width + 20;
	m_btnCalc.MoveWindow(0, top, width, width);
	top += width + 20;
	m_chkOblique.MoveWindow(0, top, width, width);
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


void CDlgMainToolbar::OnBnClickedMfcbuttonSetunit()
{
	if (m_pView)
	{
		m_pView->OnButtonSetUnit();
	}
}

void CDlgMainToolbar::OnBnClickedMfcbuttonCalc()
{
	if (m_pView)
	{
		m_pView->OnButtonCalc();
	}
}

void CDlgMainToolbar::OnBnClickedCheckOblique()
{
	if (m_pView)
	{
		m_pView->OnButtonToggleOblique();
		m_chkOblique.SetCheck(m_pView->m_bDoubleCalc);
		if (m_pView->m_bDoubleCalc)
		{
			m_chkOblique.SetBitmap(hBit9);
		}
		else
		{
			m_chkOblique.SetBitmap(hBit8);
		}
	}
}
