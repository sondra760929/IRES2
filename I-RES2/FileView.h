﻿
#pragma once

#include "ViewTree.h"
#include "XHtmlTree.h"
#include "DlgOutputToolbar.h"
class CIRES2View;

class CFileViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CFileView : public CDockablePane
{
// 생성입니다.
public:
	CFileView() noexcept;
	CDlgOutputToolbar m_OutputToolbar;

	void AdjustLayout();
	void OnChangeVisualStyle();
	CXHtmlTree m_wndFileView;
	HTREEITEM m_itemRoot;
	int item_count;
	void Clear();
	void AddItem(CString job_name);
	void DeleteItem(CString job_name);
	bool m_bSetJobName;
	CString m_strJobName;
	CIRES2View* m_pView;

// 특성입니다.
protected:

	CImageList m_FileViewImages;
	//CFileViewToolBar m_wndToolBar;

protected:
	void FillFileView();

// 구현입니다.
public:
	virtual ~CFileView();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnProperties();
	afx_msg void OnFileOpen();
	afx_msg void OnFileOpenWith();
	afx_msg void OnDummyCompile();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
};

