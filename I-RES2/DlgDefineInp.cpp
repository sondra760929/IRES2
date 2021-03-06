// DlgDefineInp.cpp: 구현 파일
//

#include "stdafx.h"
#include "I-RES2.h"
#include "DlgDefineInp.h"
#include "afxdialogex.h"
#include "OptImportExportBase.h"

// CDlgDefineInp 대화 상자

IMPLEMENT_DYNAMIC(CDlgDefineInp, CDialog)

CDlgDefineInp::CDlgDefineInp(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_DEFINE_INP, pParent)
{

}

CDlgDefineInp::~CDlgDefineInp()
{
}

void CDlgDefineInp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}


BEGIN_MESSAGE_MAP(CDlgDefineInp, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CDlgDefineInp::OnTcnSelchangeTab1)
	ON_BN_CLICKED(IDOK, &CDlgDefineInp::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgDefineInp 메시지 처리기

BOOL CDlgDefineInp::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Tab.InsertItem(1, "Principal Dimensions");
	m_Tab.InsertItem(2, "Icd Data");

	CRect rect;
	m_Tab.GetClientRect(&rect);
	m_Dlg1.Create(IDD_DIALOG_1, &m_Tab);
	m_Dlg1.SetWindowPos(NULL, 0, 20, rect.Width(), rect.Height() - 20, SWP_SHOWWINDOW | SWP_NOZORDER);
	m_pwndShow = &m_Dlg1;

	m_Dlg2.Create(IDD_DIALOG_2, &m_Tab);
	m_Dlg2.SetWindowPos(NULL, 0, 20, rect.Width(), rect.Height() - 20, SWP_NOZORDER);

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
			m_Dlg2.m_fFrictionCoeff = atof(ifp.m_array_strOutput[0]);
		}
		if (ifp.ReadOneLineFromFile() > 2)
		{
			m_Dlg2.m_fInitialStrength = atof(ifp.m_array_strOutput[0]);
			m_Dlg2.m_fMaximumStrength = atof(ifp.m_array_strOutput[1]);
			m_Dlg2.m_fStrengthIncrements = atof(ifp.m_array_strOutput[2]);
		}
		if (ifp.ReadOneLineFromFile() > 2)
		{
			m_Dlg2.m_fInitialThickness = atof(ifp.m_array_strOutput[0]);
			m_Dlg2.m_fMaximumThickness = atof(ifp.m_array_strOutput[1]);
			m_Dlg2.m_fThicknessIncrements = atof(ifp.m_array_strOutput[2]);
		}
		ifp.ReadOneLineFromFile();
		if (ifp.ReadOneLineFromFile() > 1)
		{
			m_Dlg1.m_fDesignDraft = atof(ifp.m_array_strOutput[0]);
			m_Dlg1.m_fMoldedBreadth = atof(ifp.m_array_strOutput[1]);
		}
		if (ifp.ReadOneLineFromFile() > 2)
		{
			m_Dlg1.m_fInitialSpeed = atof(ifp.m_array_strOutput[0]);
			m_Dlg1.m_fMaximumSpeed = atof(ifp.m_array_strOutput[1]);
			m_Dlg1.m_fSpeedIncrements = atof(ifp.m_array_strOutput[2]);
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
			m_Dlg2.m_fWaterDensity = atof(ifp.m_array_strOutput[1]);
		}
		if (ifp.ReadOneLineFromFile() > 1)
		{
			m_Dlg2.m_fIceDensity = atof(ifp.m_array_strOutput[1]);
		}
		if (ifp.ReadOneLineFromFile() > 1)
		{
			m_Dlg2.m_fESigma = atof(ifp.m_array_strOutput[1]) / 1000000.0f;
		}
	}
	m_Dlg1.UpdateData(FALSE);
	m_Dlg2.UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgDefineInp::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (m_pwndShow != NULL)
	{
		m_pwndShow->ShowWindow(SW_HIDE);
		m_pwndShow = NULL;
	}

	int nIndex = m_Tab.GetCurSel();
	switch (nIndex)
	{
	case 0:
		m_Dlg1.ShowWindow(SW_SHOW);
		m_pwndShow = &m_Dlg1;
		break;
	case 1:
		m_Dlg2.ShowWindow(SW_SHOW);
		m_pwndShow = &m_Dlg2;
		break;
	}

	*pResult = 0;
}


void CDlgDefineInp::OnBnClickedOk()
{
	FILE* fp_4;
	fopen_s(&fp_4, m_strProjectPath + "\\ICE_INPUT.inp", "wt");
	if (fp_4)
	{
		fprintf_s(fp_4, "        %d\n", HULL_TYPE);
		fprintf_s(fp_4, "%*.6lf\n", 16, m_Dlg2.m_fFrictionCoeff);
		fprintf_s(fp_4, "%*.6lf%*.6lf%*.6lf\n", 16, m_Dlg2.m_fInitialStrength, 16, m_Dlg2.m_fMaximumStrength, 16, m_Dlg2.m_fStrengthIncrements);
		fprintf_s(fp_4, "%*.6lf%*.6lf%*.6lf\n\n", 16, m_Dlg2.m_fInitialThickness, 16, m_Dlg2.m_fMaximumThickness, 16, m_Dlg2.m_fThicknessIncrements);
		fprintf_s(fp_4, "%*.6lf%*.6lf\n", 16, m_Dlg1.m_fDesignDraft, 16, m_Dlg1.m_fMoldedBreadth);
		fprintf_s(fp_4, "%*.6lf%*.6lf%*.6lf\n", 16, m_Dlg1.m_fInitialSpeed, 16, m_Dlg1.m_fMaximumSpeed, 16, m_Dlg1.m_fSpeedIncrements);
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
		fprintf_s(fp_9, "RHO       :   %g\n", m_Dlg2.m_fWaterDensity);
		fprintf_s(fp_9, "RHOL      :   %g\n", m_Dlg2.m_fIceDensity);
		fprintf_s(fp_9, "E_young   :   %.0lf\n", m_Dlg2.m_fESigma * 1000000.0f);
		fclose(fp_9);
	}


	CDialog::OnOK();
}
