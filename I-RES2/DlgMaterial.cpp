// DlgMaterial.cpp: 구현 파일
//

#include "stdafx.h"
#include "I-RES2.h"
#include "DlgMaterial.h"
#include "afxdialogex.h"
#include "OptImportExportBase.h"


// CDlgMaterial 대화 상자

IMPLEMENT_DYNAMIC(CDlgMaterial, CDialog)

CDlgMaterial::CDlgMaterial(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_MATERIAL, pParent)
	, m_fInitialStrength(0)
	, m_fFrictionCoef(0)
	, m_fEsigma(0)
	, m_fIcdDensity(0)
	, m_fWaterDensity(0)
{

}

CDlgMaterial::~CDlgMaterial()
{
}

void CDlgMaterial::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INITIAL_STRENGTH, m_fInitialStrength);
	DDX_Text(pDX, IDC_EDIT_FRICTION_COEFF, m_fFrictionCoef);
	DDX_Text(pDX, IDC_EDIT_E_SIGMA, m_fEsigma);
	DDX_Text(pDX, IDC_EDIT_ICE_DENSITY, m_fIcdDensity);
	DDX_Text(pDX, IDC_EDIT_WATER_DENSITY, m_fWaterDensity);
}


BEGIN_MESSAGE_MAP(CDlgMaterial, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgMaterial::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgMaterial 메시지 처리기


BOOL CDlgMaterial::OnInitDialog()
{
	CDialog::OnInitDialog();

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
			m_fFrictionCoef = atof(ifp.m_array_strOutput[0]);
		}
		if (ifp.ReadOneLineFromFile() > 2)
		{
			m_fInitialStrength = atof(ifp.m_array_strOutput[0]);
			m_fMaximumStrength = atof(ifp.m_array_strOutput[1]);
			m_fStrengthIncrements = atof(ifp.m_array_strOutput[2]);
		}
		if (ifp.ReadOneLineFromFile() > 2)
		{
			m_fInitialThickness = atof(ifp.m_array_strOutput[0]);
			m_fMaximumThickness = atof(ifp.m_array_strOutput[1]);
			m_fThicknessIncrements = atof(ifp.m_array_strOutput[2]);
		}
		ifp.ReadOneLineFromFile();
		if (ifp.ReadOneLineFromFile() > 1)
		{
			m_fDesignDraft = atof(ifp.m_array_strOutput[0]);
			m_fMoldedBreadth = atof(ifp.m_array_strOutput[1]);
		}
		if (ifp.ReadOneLineFromFile() > 2)
		{
			m_fInitialSpeed = atof(ifp.m_array_strOutput[0]);
			m_fMaximumSpeed = atof(ifp.m_array_strOutput[1]);
			m_fSpeedIncrements = atof(ifp.m_array_strOutput[2]);
		}
	}

	FILE* fp_9;
	fopen_s(&fp_9, m_strProjectPath + "\\ICECOFF_INPUT.inp", "rt");
	if (fp_9)
	{
		COptImportExportBase ifp;
		ifp.m_fp_input = fp_9;
		ifp.m_array_strSplit.push_back(' ');
		ifp.m_array_strSplit.push_back(':');
		if (ifp.ReadOneLineFromFile() > 1)
		{
			GG = atof(ifp.m_array_strOutput[1]);
		}
		if (ifp.ReadOneLineFromFile() > 1)
		{
			XK1H1 = atof(ifp.m_array_strOutput[1]);
		}
		if (ifp.ReadOneLineFromFile() > 1)
		{
			XK1H2 = atof(ifp.m_array_strOutput[1]);
		}
		if (ifp.ReadOneLineFromFile() > 1)
		{
			XK2H1 = atof(ifp.m_array_strOutput[1]);
		}
		if (ifp.ReadOneLineFromFile() > 1)
		{
			XK2H2 = atof(ifp.m_array_strOutput[1]);
		}
		if (ifp.ReadOneLineFromFile() > 1)
		{
			XK3H1 = atof(ifp.m_array_strOutput[1]);
		}
		if (ifp.ReadOneLineFromFile() > 1)
		{
			XK3H2 = atof(ifp.m_array_strOutput[1]);
		}
		if (ifp.ReadOneLineFromFile() > 1)
		{
			m_fWaterDensity = atof(ifp.m_array_strOutput[1]);
		}
		if (ifp.ReadOneLineFromFile() > 1)
		{
			m_fIcdDensity = atof(ifp.m_array_strOutput[1]);
		}
		if (ifp.ReadOneLineFromFile() > 1)
		{
			m_fEsigma = atof(ifp.m_array_strOutput[1]) / 1000000.0f;
		}
	}
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgMaterial::OnBnClickedOk()
{
	UpdateData();
	FILE* fp_4;
	fopen_s(&fp_4, m_strProjectPath + "\\ICE_INPUT.inp", "wt");
	if (fp_4)
	{
		fprintf_s(fp_4, "        %d\n", HULL_TYPE);
		fprintf_s(fp_4, "%*.6lf\n", 16, m_fFrictionCoef);
		fprintf_s(fp_4, "%*.6lf%*.6lf%*.6lf\n", 16, m_fInitialStrength, 16, m_fInitialStrength, 16, 0.0f);
		fprintf_s(fp_4, "%*.6lf%*.6lf%*.6lf\n\n", 16, m_fInitialThickness, 16, m_fMaximumThickness, 16, m_fThicknessIncrements);
		fprintf_s(fp_4, "%*.6lf%*.6lf\n", 16, m_pCurrentView->DRAFT, 16, m_pCurrentView->BREADTH);
		fprintf_s(fp_4, "%*.6lf%*.6lf%*.6lf\n", 16, m_fInitialSpeed, 16, m_fMaximumSpeed, 16, m_fSpeedIncrements);
		fclose(fp_4);
	}

	FILE* fp_9;
	fopen_s(&fp_9, m_strProjectPath + "\\ICECOFF_INPUT.inp", "wt");
	if (fp_9)
	{
		fprintf_s(fp_9, "GG        :   %g\n", GG);
		fprintf_s(fp_9, "XK1H1     :   %g\n", XK1H1);
		fprintf_s(fp_9, "XK1H2     :   %g\n", XK1H2);
		fprintf_s(fp_9, "XK2H1     :   %g\n", XK2H1);
		fprintf_s(fp_9, "XK2H2     :   %g\n", XK2H2);
		fprintf_s(fp_9, "XK3H1     :   %g\n", XK3H1);
		fprintf_s(fp_9, "XK3H2     :   %g\n", XK3H2);
		fprintf_s(fp_9, "RHO       :   %g\n", m_fWaterDensity);
		fprintf_s(fp_9, "RHOL      :   %g\n", m_fIcdDensity);
		fprintf_s(fp_9, "E_young   :   %.0lf\n", m_fEsigma * 1000000.0f);
		fclose(fp_9);
	}

	CDialog::OnOK();
}
