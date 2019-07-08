#pragma once
class CIRES3View;

// CDlgDraftSection 대화 상자

class CDlgDraftSection : public CDialog
{
	DECLARE_DYNAMIC(CDlgDraftSection)

public:
	CDlgDraftSection(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgDraftSection();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DRAFTSECTION };
#endif
	CIRES3View* m_pView;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CMFCButton m_btnCalc;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckPoint();
	afx_msg void OnBnClickedCheckAxis();
	CButton m_chkDistance;
	CButton m_chkAxis;
	float m_fDraft;
	float m_fPointDistance;
	float m_fAxisDistance;
	afx_msg void OnBnClickedMfcbuttonCalculate();
};
