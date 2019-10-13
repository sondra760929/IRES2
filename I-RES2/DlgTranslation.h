#pragma once


// CDlgTranslation 대화 상자

class CDlgTranslation : public CDialog
{
	DECLARE_DYNAMIC(CDlgTranslation)

public:
	CDlgTranslation(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgTranslation();
	void ResizeControl(int cx, int cy);
	void SetCaption(CString caption, CString title, int data_count);
	void SetValueString(CString value_str);
	int m_iInputDataCount;
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TRANSLATE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strCaption;
	CString m_strVector;
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonOk();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	virtual void OnCancel();
};
