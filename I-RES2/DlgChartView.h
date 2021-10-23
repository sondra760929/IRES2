#pragma once
//#include "ChartDef.h"
//#include "ChartContainer.h"
#include "CurveCtrl.h"


// CDlgChartView 대화 상자

class CDlgChartView : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgChartView)

public:
	CDlgChartView(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgChartView();
	CString m_strJobName;
	CString m_strInputFile;
	int m_iType;
	vector < CString > satellite_files;
	vector < CString > satellite_files_option[3];
	vector< CButton* > m_aButtons;
	void SetSize(int cx, int cy);
	void LoadFile(int index);
	CCurveCtrl* m_pCurveCtrl;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CHART };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	//CChartContainer m_chartContainer;
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};
