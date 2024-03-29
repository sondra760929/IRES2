﻿// DlgSectionToolbar.cpp: 구현 파일
//

#include "stdafx.h"
#include "I-RES2.h"
#include "DlgSectionToolbar.h"
#include "afxdialogex.h"


// CDlgSectionToolbar 대화 상자

IMPLEMENT_DYNAMIC(CDlgSectionToolbar, CDialog)

CDlgSectionToolbar::CDlgSectionToolbar(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_SECTION_TOOLBAR, pParent)
	, m_pView(0)
{

}

CDlgSectionToolbar::~CDlgSectionToolbar()
{
}

void CDlgSectionToolbar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_CHECK_VIEW_DRAFT_PLANE, m_chkDraftPlane);
	DDX_Control(pDX, IDC_CHECK_VIEW_DRAFT_POINTS, m_chkDraftPoints);
	//DDX_Control(pDX, IDC_CHECK_VIEW_CROSS_PLANE, m_chkCrossPlane);
	DDX_Control(pDX, IDC_CHECK_VIEW_CROSS_POINTS, m_chkCrossPoints);
	DDX_Control(pDX, IDC_CHECK_VIEW_CROSS_POINTS, m_chkCrossPoints);
	DDX_Control(pDX, IDC_MFCBUTTON_SAVE_SECTION, m_btnSaveSection);
}


BEGIN_MESSAGE_MAP(CDlgSectionToolbar, CDialog)
	//ON_BN_CLICKED(IDC_CHECK_VIEW_DRAFT_PLANE, &CDlgSectionToolbar::OnBnClickedCheckViewDraftPlane)
	ON_BN_CLICKED(IDC_CHECK_VIEW_DRAFT_POINTS, &CDlgSectionToolbar::OnBnClickedCheckViewDraftPoints)
	//ON_BN_CLICKED(IDC_CHECK_VIEW_CROSS_PLANE, &CDlgSectionToolbar::OnBnClickedCheckViewCrossPlane)
	ON_BN_CLICKED(IDC_CHECK_VIEW_CROSS_POINTS, &CDlgSectionToolbar::OnBnClickedCheckViewCrossPoints)
	ON_BN_CLICKED(IDC_MFCBUTTON_SAVE_SECTION, &CDlgSectionToolbar::OnBnClickedMfcbuttonSaveSection)
END_MESSAGE_MAP()


// CDlgSectionToolbar 메시지 처리기


BOOL CDlgSectionToolbar::OnInitDialog()
{
	CDialog::OnInitDialog();

	//CImage btn_image1;
	CImage btn_image2;
	//CImage btn_image3;
	CImage btn_image4;
	CImage btn_image5;
	//btn_image1.LoadFromResource(AfxGetInstanceHandle(), IDB_BITMAP_HOR);
	btn_image2.LoadFromResource(AfxGetInstanceHandle(), IDB_BITMAP_HOR_PT);
	//btn_image3.LoadFromResource(AfxGetInstanceHandle(), IDB_BITMAP_VER);
	btn_image4.LoadFromResource(AfxGetInstanceHandle(), IDB_BITMAP_VER_PT);
	btn_image5.LoadFromResource(AfxGetInstanceHandle(), IDB_BITMAP_NEW);
	//HBITMAP hBit1;
	HBITMAP hBit2;
	//HBITMAP hBit3;
	HBITMAP hBit4;
	HBITMAP hBit5;
	//hBit1 = btn_image1.Detach();
	hBit2 = btn_image2.Detach();
	//hBit3 = btn_image3.Detach();
	hBit4 = btn_image4.Detach();
	hBit5 = btn_image5.Detach();

	//m_chkDraftPlane.SetBitmap(hBit1);
	m_chkDraftPoints.SetBitmap(hBit2);
	//m_chkCrossPlane.SetBitmap(hBit3);
	m_chkCrossPoints.SetBitmap(hBit4);
	m_btnSaveSection.SetBitmap(hBit5);

	//m_chkDraftPlane.SetCheck(m_pView->m_bShowWaterline);

	int width = 28;
	int top = 0;
	//m_chkDraftPlane.MoveWindow(0, top, width, width);
	//top += width;
	m_chkDraftPoints.MoveWindow(0, top, width, width);
	top += width;
	top += 10;
	//m_chkCrossPlane.MoveWindow(0, top, width, width);
	//top += width;
	m_chkCrossPoints.MoveWindow(0, top, width, width);
	top += width;
	top += 10;
	//m_chkCrossPlane.MoveWindow(0, top, width, width);
	//top += width;
	m_btnSaveSection.MoveWindow(0, top, width, width);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgSectionToolbar::Init()
{
	//m_chkDraftPlane.SetCheck(m_pView->m_bShowWaterline);
	m_chkDraftPoints.SetCheck(m_pView->m_bShowWaterlineData);
	//m_chkCrossPlane.SetCheck(m_pView->m_bShowSection);
	m_chkCrossPoints.SetCheck(m_pView->m_bShowSectionData);
}
//
//void CDlgSectionToolbar::OnBnClickedCheckViewDraftPlane()
//{
//	m_pView->OnButtonHideShowWaterlines();
//	m_chkDraftPlane.SetCheck(m_pView->m_bShowWaterline);
//}


void CDlgSectionToolbar::OnBnClickedCheckViewDraftPoints()
{
	m_pView->OnButtonShowHideWaterlineCut();
	m_chkDraftPoints.SetCheck(m_pView->m_bShowWaterlineData);
}


//void CDlgSectionToolbar::OnBnClickedCheckViewCrossPlane()
//{
//	m_pView->OnButtonShowHideSections();
//	m_chkCrossPlane.SetCheck(m_pView->m_bShowSection);
//}


void CDlgSectionToolbar::OnBnClickedCheckViewCrossPoints()
{
	m_pView->OnButtonShowHideSectionCut();
	m_chkCrossPoints.SetCheck(m_pView->m_bShowSectionData);
}


void CDlgSectionToolbar::OnBnClickedMfcbuttonSaveSection()
{
	m_pView->OnButtonSaveSectionData();
}
