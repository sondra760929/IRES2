#pragma once
#include "I-RES2View.h"


// CDlgMaterial 대화 상자

class CDlgMaterial : public CDialog
{
	DECLARE_DYNAMIC(CDlgMaterial)

public:
	CDlgMaterial(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgMaterial();
	CIRES2View* m_pView;
	int HULL_TYPE;
	float GG;
	float XK1H1;
	float XK1H2;
	float XK2H1;
	float XK2H2;
	float XK3H1;
	float XK3H2;
	double m_fMaximumStrength;
	double m_fStrengthIncrements;
	double m_fInitialThickness;
	double m_fMaximumThickness;
	double m_fThicknessIncrements;
	double m_fDesignDraft;
	double m_fMoldedBreadth;
	double m_fInitialSpeed;
	double m_fMaximumSpeed;
	double m_fSpeedIncrements;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MATERIAL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	float m_fInitialStrength;
	float m_fFrictionCoef;
	float m_fEsigma;
	float m_fIcdDensity;
	float m_fWaterDensity;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
