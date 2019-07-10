#pragma once
#include "ugctrl.h"

#define USE_SETMASK	44000
#define USE_CHARLIMIT 44001
#define USE_FINISHTEST 44002

class CPatternDBDlg;
class MyCug :public CUGCtrl
{

public:
	MyCug();
	~MyCug();
	void SetType1();
	void SetType2();

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	//}}AFX_VIRTUAL

	void OnLClicked(int col, long row, int updn, RECT *rect, POINT *point, BOOL processed);
	void OnDClicked(int col, long row, RECT *rect, POINT *point, BOOL processed);
	void OnMouseMove(int col, long row, POINT *point, UINT nFlags, BOOL processed);
	void OnTH_LClicked(int col, long row, int updn, RECT *rect, POINT *point, BOOL processed);
	CDialog* m_pParentDlg;
	//{{AFX_MSG(MyCug)
	// NOTE - the ClassWizard will add and remove member functions here.
	//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	void OnCharDown(UINT* vcKey, BOOL processed);
	virtual void OnSetup();
	//virtual void OnGetCell(int col, long row, CUGCell *cell);
	virtual int OnCellTypeNotify(long ID, int col, long row, long msg, LONG_PTR param);
	
	virtual int OnEditKeyDown(int col, long row, CWnd *edit, UINT *vcKey);
	
	virtual int OnEditStart(int col, long row, CWnd **edit);
	virtual int OnEditVerify(int col, long row, CWnd *edit, UINT *vcKey);
	virtual int OnEditFinish(int col, long row, CWnd *edit, LPCTSTR string, BOOL cancelFlag);
	virtual int OnKeyDown(int col, long row, CWnd *edit, LPCTSTR string, BOOL cancelFlag);
	CString origin_name;
	CUGEdit m_myCUGEdit;


};
