#pragma once


// CDlgMainToolbar 대화 상자

class CDlgMainToolbar : public CDialog
{
	DECLARE_DYNAMIC(CDlgMainToolbar)

public:
	CDlgMainToolbar(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgMainToolbar();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MAIN_TOOLBAR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CMFCButton m_btnClear;
	CMFCButton m_btnTrans;
	CMFCButton m_btnRot;
	CMFCButton m_btnDatum;
	CMFCButton m_btnNormal;
	virtual BOOL OnInitDialog();
};
