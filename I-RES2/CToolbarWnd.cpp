#include "stdafx.h"
#include "CToolbarWnd.h"
#include "resource.h"


CToolbarWnd::CToolbarWnd()
{
}


CToolbarWnd::~CToolbarWnd()
{
}
BEGIN_MESSAGE_MAP(CToolbarWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
END_MESSAGE_MAP()


int CToolbarWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_MainToolbar.Create(IDD_DIALOG_MAIN_TOOLBAR, this);
	m_MainToolbar.ShowWindow(SW_HIDE);

	m_SectionToolbar.Create(IDD_DIALOG_SECTION_TOOLBAR, this);
	m_SectionToolbar.ShowWindow(SW_HIDE);

	return 0;
}


void CToolbarWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_MainToolbar.SetFocus();
	m_SectionToolbar.SetFocus();
}

void CToolbarWnd::SetToolbar(int index)
{
	m_MainToolbar.ShowWindow(SW_HIDE);
	m_SectionToolbar.ShowWindow(SW_HIDE);
	switch (index)
	{
	case 0:
	{
		m_MainToolbar.ShowWindow(SW_SHOW);
	}
	break;
	case 1:
	{
		m_SectionToolbar.Init();
		m_SectionToolbar.ShowWindow(SW_SHOW);
	}
	break;
	case 2:
	{
	}
	break;
	case 3:
	{
	}
	break;
	}
}

BOOL CToolbarWnd::OnBeforeFloat(CRect& rectFloat, AFX_DOCK_METHOD dockMethod)
{
	return FALSE;
}


void CToolbarWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	if (m_MainToolbar)
	{
		m_MainToolbar.MoveWindow(0, 0, cx, cy);
		m_SectionToolbar.MoveWindow(0, 0, cx, cy);
	}
}
