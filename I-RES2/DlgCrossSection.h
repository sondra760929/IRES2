#pragma once


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

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CMFCButton m_btnCalc;
	virtual BOOL OnInitDialog();
};
