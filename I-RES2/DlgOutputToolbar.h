#pragma once


// CDlgOutputToolbar 대화 상자

class CDlgOutputToolbar : public CDialog
{
	DECLARE_DYNAMIC(CDlgOutputToolbar)

public:
	CDlgOutputToolbar(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgOutputToolbar();
	CIRES2View* m_pView;
	CString m_strJobName;
	void ResizeControl(int cx, int cy);
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_OUTPUT_TOOLBAR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedMfcbuttonExcel1();
	afx_msg void OnBnClickedMfcbuttonGraph1();
	afx_msg void OnBnClickedMfcbuttonExcel2();
	afx_msg void OnBnClickedMfcbuttonGraph2();
	CMFCButton m_btnExcel1;
	CMFCButton m_btnGraph1;
	CMFCButton m_btnExcel2;
	CMFCButton m_btnGraph2;
};
