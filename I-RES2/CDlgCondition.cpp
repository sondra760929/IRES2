// CDlgCondition.cpp: 구현 파일
//

#include "stdafx.h"
#include "I-RES2.h"
#include "DlgCondition.h"
#include "afxdialogex.h"


// CDlgCondition 대화 상자

IMPLEMENT_DYNAMIC(CDlgCondition, CDialog)

CDlgCondition::CDlgCondition(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_CONDITION, pParent)
	, m_fShipSpeed(0)
	, m_fShipMaxSpeed(0)
	, m_fShipIncSpeed(0)
	, m_fThickness(0)
	, m_fMaxThickness(0)
	, m_fIncThickness(0)
{
	m_pView = (CIRES2View*)pParent;
}

CDlgCondition::~CDlgCondition()
{
}

void CDlgCondition::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INITIAL_SPEED, m_fShipSpeed);
	DDX_Text(pDX, IDC_EDIT_MAXIMUM_SPEED, m_fShipMaxSpeed);
	DDX_Text(pDX, IDC_EDIT_SPEED_INCREMENTS, m_fShipIncSpeed);
	DDX_Text(pDX, IDC_EDIT_INITIAL_THICKNESS, m_fThickness);
	DDX_Text(pDX, IDC_EDIT_MAXIMUM_THICKNESS2, m_fMaxThickness);
	DDX_Text(pDX, IDC_EDIT_THICKNESS_INCREMENTS2, m_fIncThickness);
	DDX_Control(pDX, IDC_RADIO_CONSTANT, m_radioConstant);
	DDX_Control(pDX, IDC_RADIO_PARAMETRIC, m_radioParametric);
}


BEGIN_MESSAGE_MAP(CDlgCondition, CDialog)
	ON_BN_CLICKED(IDC_RADIO_CONSTANT, &CDlgCondition::OnBnClickedRadioConstant)
	ON_BN_CLICKED(IDC_RADIO_PARAMETRIC, &CDlgCondition::OnBnClickedRadioParametric)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN5, &CDlgCondition::OnDeltaposSpin5)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN6, &CDlgCondition::OnDeltaposSpin6)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN7, &CDlgCondition::OnDeltaposSpin7)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN11, &CDlgCondition::OnDeltaposSpin11)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN8, &CDlgCondition::OnDeltaposSpin8)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN12, &CDlgCondition::OnDeltaposSpin12)
	ON_BN_CLICKED(IDOK, &CDlgCondition::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgCondition 메시지 처리기
