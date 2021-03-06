#pragma once


// CDlg2 대화 상자

class CDlg2 : public CDialog
{
	DECLARE_DYNAMIC(CDlg2)

public:
	CDlg2(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlg2();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	double m_fInitialStrength;
	double m_fMaximumStrength;
	double m_fStrengthIncrements;
	double m_fFrictionCoeff;
	double m_fESigma;
	double m_fInitialThickness;
	double m_fMaximumThickness;
	double m_fThicknessIncrements;
	double m_fIceDensity;
	double m_fWaterDensity;
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin5(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin6(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin7(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin8(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin9(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin10(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditInitialStrength();
	afx_msg void OnEnChangeEditMaximumStrength();
	afx_msg void OnEnChangeEditStrengthIncrements();
	afx_msg void OnEnChangeEditFrictionCoeff();
	afx_msg void OnEnChangeEditESigma();
	afx_msg void OnEnChangeEditInitialThickness();
	afx_msg void OnEnChangeEditMaximumThickness();
	afx_msg void OnEnChangeEditThicknessIncrements();
	afx_msg void OnEnChangeEditIceDensity();
	afx_msg void OnEnChangeEditWaterDensity();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
