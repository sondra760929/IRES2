#pragma once
#include <afxdockablepane.h>
#include "DlgMainToolbar.h"
#include "DlgSectionToolbar.h"

class CToolbarWnd :
	public CDockablePane
{
public:
	CToolbarWnd();
	~CToolbarWnd();

	CDlgMainToolbar m_MainToolbar;
	CDlgSectionToolbar m_SectionToolbar;

	void SetToolbar(int index);
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	virtual BOOL OnBeforeFloat(CRect& rectFloat, AFX_DOCK_METHOD dockMethod);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

