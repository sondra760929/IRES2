#pragma once


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

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CMFCButton m_btnCalc;
	virtual BOOL OnInitDialog();
};
