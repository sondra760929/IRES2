#pragma once
#include "Mycug.h"

// CDlgExcelView 대화 상자

class CDlgExcelView : public CDialog
{
	DECLARE_DYNAMIC(CDlgExcelView)

public:
	CDlgExcelView(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgExcelView();
	MyCug m_wndExcelView;
	vector< CButton* > m_aButtons;
	void SetSize(int cx, int cy);
	CString m_strJobName;
	CString m_strInputFile;
	int m_iCurrentFileIndex;
	int m_iType;
	vector < CString > satellite_files;
	vector < CString > satellite_files_option[3];
	void LoadFile(int index);

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_EXCEL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonSave();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};
