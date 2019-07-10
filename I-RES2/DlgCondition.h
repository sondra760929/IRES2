#pragma once
#include "I-RES2View.h"

// CDlgCondition 대화 상자

class CDlgCondition : public CDialog
{
	DECLARE_DYNAMIC(CDlgCondition)

public:
	CDlgCondition(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgCondition();
	CIRES2View* m_pView;
	bool m_isConstant;
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CONDITION };
#endif
	int HULL_TYPE;
	double m_fFrictionCoeff;
	double m_fInitialStrength;
	double m_fMaximumStrength;
	double m_fStrengthIncrements;
	double m_fDesignDraft;
	double m_fMoldedBreadth;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadioConstant();
	afx_msg void OnBnClickedRadioParametric();
	afx_msg void OnDeltaposSpin5(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin6(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin7(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin11(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin8(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin12(NMHDR *pNMHDR, LRESULT *pResult);
	float m_fShipSpeed;
	float m_fShipMaxSpeed;
	float m_fShipIncSpeed;
	float m_fThickness;
	float m_fMaxThickness;
	float m_fIncThickness;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CButton m_radioConstant;
	CButton m_radioParametric;
};
