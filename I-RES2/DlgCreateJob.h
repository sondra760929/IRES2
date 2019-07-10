#pragma once


// CDlgCreateJob 대화 상자

class CDlgCreateJob : public CDialog
{
	DECLARE_DYNAMIC(CDlgCreateJob)

public:
	CDlgCreateJob(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgCreateJob();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CREATE_JOB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strJobName;
};
