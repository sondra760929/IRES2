#pragma once


// CDlgSelectSections 대화 상자
class CIRES2View;

class CDlgSelectSections : public CDialog
{
	DECLARE_DYNAMIC(CDlgSelectSections)
	CIRES2View* m_pView;
public:
	CDlgSelectSections(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgSelectSections();
	void ResizeControl(int cx, int cy);
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SELECT_SECTIONS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CCheckListBox m_lstSections;
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedOk();
};
