#pragma once


// CDlgOutputToolbar 대화 상자

class CDlgOutputToolbar : public CDialog
{
	DECLARE_DYNAMIC(CDlgOutputToolbar)

public:
	CDlgOutputToolbar(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgOutputToolbar();
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
	vector< CMFCButton* > m_aExcelButton;
	vector< CMFCButton* > m_aGraphButton;
	//CMFCButton m_btnIceExcel;
	//CMFCButton m_btnIceGraph;
	//CMFCButton m_btnSpeedExcel;
	//CMFCButton m_btnSpeedGraph;
	//afx_msg void OnBnClickedMfcbuttonIceExcel();
	//afx_msg void OnBnClickedMfcbuttonIcdGraph();
	//afx_msg void OnBnClickedMfcbuttonSpeedExcel();
	//afx_msg void OnBnClickedMfcbuttonSpeedGraph();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};
