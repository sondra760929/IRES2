#pragma once


// CDlg1 대화 상자

class CDlg1 : public CDialog
{
	DECLARE_DYNAMIC(CDlg1)

public:
	CDlg1(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlg1();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	double m_fLength;
	double m_fMoldedDepth;
	double m_fMoldedBreadth;
	double m_fDesignDraft;
	double m_fInitialSpeed;
	double m_fMaximumSpeed;
	double m_fSpeedIncrements;
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin5(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin6(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin7(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditLength();
	afx_msg void OnEnChangeEditModeledDepth();
	afx_msg void OnEnChangeEditModeledBreadth();
	afx_msg void OnEnChangeEditDesignDraft();
	afx_msg void OnEnChangeEditInitialSpeed();
	afx_msg void OnEnChangeEditMaximumSpeed();
	afx_msg void OnEnChangeEditSpeedIncrements();
};
