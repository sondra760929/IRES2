#pragma once
#include "I-RES2View.h"


// CDlgSectionToolbar 대화 상자

class CDlgSectionToolbar : public CDialog
{
	DECLARE_DYNAMIC(CDlgSectionToolbar)

public:
	CDlgSectionToolbar(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgSectionToolbar();
	CIRES2View* m_pView;
	void Init();
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SECTION_TOOLBAR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CButton m_chkDraftPlane;
	afx_msg void OnBnClickedCheckViewDraftPlane();
	CButton m_chkDraftPoints;
	CButton m_chkCrossPlane;
	CButton m_chkCrossPoints;
	afx_msg void OnBnClickedCheckViewDraftPoints();
	afx_msg void OnBnClickedCheckViewCrossPlane();
	afx_msg void OnBnClickedCheckViewCrossPoints();
};
