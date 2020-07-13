#include "stdafx.h"
#include "DlgCondition.h"
#include "resource.h"
#include "OptImportExportBase.h"
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
	ON_BN_CLICKED(IDOK, &CDlgCondition::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgCondition 메시지 처리기

void CDlgCondition::OnBnClickedRadioConstant()
{
	GetDlgItem(IDC_EDIT_MAXIMUM_SPEED)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_SPEED_INCREMENTS)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_MAXIMUM_THICKNESS2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_THICKNESS_INCREMENTS2)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_EDIT_INITIAL_THICKNESS)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATIC_SHIP_SPEED)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_INC_SHIP_SPEED)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_MAX_THICK)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_THICK_INC)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_STATIC_INC_THICK)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATIC_INI_SHIP_SPEED)->SetWindowText("Ship Speed (knots)");
	GetDlgItem(IDC_STATIC_INC_THICK)->SetWindowText("Thickness (m)");
	m_pCurrentView->m_bConditionConstant = true;
}


void CDlgCondition::OnBnClickedRadioParametric()
{
	GetDlgItem(IDC_EDIT_MAXIMUM_SPEED)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_SPEED_INCREMENTS)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_MAXIMUM_THICKNESS2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_THICKNESS_INCREMENTS2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_INITIAL_THICKNESS)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_SHIP_SPEED)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_INC_SHIP_SPEED)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_MAX_THICK)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_THICK_INC)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_INC_THICK)->ShowWindow(SW_SHOW);

	GetDlgItem(IDC_STATIC_INI_SHIP_SPEED)->SetWindowText("Initial Ship Speed (knots)");
	GetDlgItem(IDC_STATIC_INC_THICK)->SetWindowText("Initial Thickness (m)");
	m_pCurrentView->m_bConditionConstant = false;
}


BOOL CDlgCondition::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (m_pCurrentView)
	{
		if (m_pCurrentView->m_bConditionConstant)
		{
			m_radioConstant.SetCheck(TRUE);
			OnBnClickedRadioConstant();
		}
		else
		{
			m_radioParametric.SetCheck(TRUE);
			OnBnClickedRadioParametric();
		}
	}
	FILE* fp_4;
	fopen_s(&fp_4, m_strProjectPath + "\\ICE_INPUT.inp", "rt");
	if (fp_4)
	{
		COptImportExportBase ifp;
		ifp.m_fp_input = fp_4;
		ifp.m_array_strSplit.push_back(' ');
		if (ifp.ReadOneLineFromFile() > 0)
		{
			HULL_TYPE = atoi(ifp.m_array_strOutput[0]);
		}
		if (ifp.ReadOneLineFromFile() > 0)
		{
			m_fFrictionCoeff = atof(ifp.m_array_strOutput[0]);
		}
		if (ifp.ReadOneLineFromFile() > 2)
		{
			m_fInitialStrength = atof(ifp.m_array_strOutput[0]);
			m_fMaximumStrength = atof(ifp.m_array_strOutput[1]);
			m_fStrengthIncrements = atof(ifp.m_array_strOutput[2]);
		}
		if (ifp.ReadOneLineFromFile() > 2)
		{
			m_fThickness = atof(ifp.m_array_strOutput[0]);
			m_fMaxThickness = atof(ifp.m_array_strOutput[1]);
			m_fIncThickness = atof(ifp.m_array_strOutput[2]);
		}
		ifp.ReadOneLineFromFile();
		if (ifp.ReadOneLineFromFile() > 1)
		{
			m_fDesignDraft = atof(ifp.m_array_strOutput[0]);
			m_fMoldedBreadth = atof(ifp.m_array_strOutput[1]);
		}
		if (ifp.ReadOneLineFromFile() > 2)
		{
			m_fShipSpeed = atof(ifp.m_array_strOutput[0]);
			m_fShipMaxSpeed = atof(ifp.m_array_strOutput[1]);
			m_fShipIncSpeed = atof(ifp.m_array_strOutput[2]);
		}
	}

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgCondition::OnBnClickedOk()
{
	UpdateData();
	FILE* fp_4;
	fopen_s(&fp_4, m_strProjectPath + "\\ICE_INPUT.inp", "wt");
	if (fp_4)
	{
		fprintf_s(fp_4, "        %d\n", HULL_TYPE);
		fprintf_s(fp_4, "%*.6lf\n", 16, m_fFrictionCoeff);
		fprintf_s(fp_4, "%*.6lf%*.6lf%*.6lf\n", 16, m_fInitialStrength, 16, m_fInitialStrength, 16, 0.0f);
		fprintf_s(fp_4, "%*.6lf%*.6lf%*.6lf\n\n", 16, m_fThickness, 16, m_fMaxThickness, 16, m_fIncThickness);
		fprintf_s(fp_4, "%*.6lf%*.6lf\n", 16, m_pCurrentView->DRAFT, 16, m_pCurrentView->BREADTH);
		fprintf_s(fp_4, "%*.6lf%*.6lf%*.6lf\n", 16, m_fShipSpeed, 16, m_fShipMaxSpeed, 16, m_fShipIncSpeed);
		fclose(fp_4);
	}

	CDialog::OnOK();
}
