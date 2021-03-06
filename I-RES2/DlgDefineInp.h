#pragma once
#include "Dlg1.h"
#include "Dlg2.h"

// CDlgDefineInp 대화 상자

class CDlgDefineInp : public CDialog
{
	DECLARE_DYNAMIC(CDlgDefineInp)

public:
	CDlgDefineInp(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgDefineInp();
	CDlg1 m_Dlg1;
	CDlg2 m_Dlg2;
	CWnd*        m_pwndShow;
	int HULL_TYPE;
	float GG;
	float XK1H1;
	float XK1H2;
	float XK2H1;
	float XK2H2;
	float XK3H1;
	float XK3H2;

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DEFINE_INP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_Tab;
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
};
