#pragma once
#include "I-RES2View.h"

// CDlgMainToolbar 대화 상자

class CDlgMainToolbar : public CDialog
{
	DECLARE_DYNAMIC(CDlgMainToolbar)

public:
	CDlgMainToolbar(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgMainToolbar();
	CIRES2View* m_pView;

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
	CMFCButton m_btnCalc;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedMfcbuttonClear();
	afx_msg void OnBnClickedMfcbuttonTrans();
	afx_msg void OnBnClickedMfcbuttonRot();
	afx_msg void OnBnClickedMfcbuttonDatum();
	afx_msg void OnBnClickedMfcbuttonNormal();
	afx_msg void OnBnClickedMfcbuttonSetunit();
	afx_msg void OnBnClickedMfcbuttonCalc();
	CMFCButton m_btnSetUnit;
};
