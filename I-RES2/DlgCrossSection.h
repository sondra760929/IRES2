#pragma once
class CIRES2View;


// CDlgCrossSection 대화 상자

class CDlgCrossSection : public CDialog
{
	DECLARE_DYNAMIC(CDlgCrossSection)

public:
	CDlgCrossSection(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgCrossSection();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CROSSSECTION };
#endif
	CIRES2View* m_pView;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CMFCButton m_btnCalc;
	virtual BOOL OnInitDialog();
	float m_fStart;
	float m_fEnd;
	float m_fInterval;
	float m_fPointDistance;
	float m_fAxisDistance;
	CButton m_chkPointDistance;
	CButton m_chkAxisDistance;
	afx_msg void OnBnClickedMfcbuttonCalculate();
	afx_msg void OnBnClickedCheckPoint();
	afx_msg void OnBnClickedCheckAxis();
};
