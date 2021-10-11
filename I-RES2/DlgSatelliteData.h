#pragma once
#include "Mycug.h"


// CDlgSatelliteData 대화 상자

class CDlgSatelliteData : public CDialog
{
	DECLARE_DYNAMIC(CDlgSatelliteData)

public:
	CDlgSatelliteData(int type, CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgSatelliteData();
	int m_iType;
	MyCug m_wndDataView;
	CString m_strInputFile;
	void SetSize(int cx, int cy);

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SATELLITE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonFile();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
