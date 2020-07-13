#pragma once
#include "Mycug.h"
#include "ChartDef.h"
#include "ChartContainer.h"


// CDlgCalc 대화 상자

class CDlgCalc : public CDialog
{
	DECLARE_DYNAMIC(CDlgCalc)

public:
	CDlgCalc(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgCalc();
	MyCug m_wndExcelView;
	CChartContainer m_chartContainer;

	void SetSize(int cx, int cy);
	void SetNumRows(int irow);
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CALC };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	float m_fAvg;
	float m_fMean;
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	int m_iRows;
	afx_msg void OnBnClickedButtonLoadTxt();
	afx_msg void OnBnClickedButtonSetRows();
	afx_msg void OnBnClickedButtonDoCalc();
};
