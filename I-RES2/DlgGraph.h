#pragma once
#include "CMPGraphCtrl.h"
#include "CMPGraphCtrl0.h"

// CDlgGraph 대화 상자

class CDlgGraph : public CDialog
{
	DECLARE_DYNAMIC(CDlgGraph)

public:
	CDlgGraph(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgGraph();
	CString m_strJobName;
	int m_iType;
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_GRAPH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CMPGraphCtrl0 m_ctrlGraph;
};
