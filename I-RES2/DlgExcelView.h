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
	void SetSize(int cx, int cy);
	CString m_strJobName;
	int m_iType;
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
};
