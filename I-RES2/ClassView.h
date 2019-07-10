
#pragma once

#include "ViewTree.h"
#include "XHtmlTree.h"
#include "DlgMainToolbar.h"
#include "DlgSectionToolbar.h"

class CIRES2View;
class CClassToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CClassView : public CDockablePane
{
public:
	CClassView() noexcept;
	virtual ~CClassView();

	void AdjustLayout();
	void OnChangeVisualStyle();
	HTREEITEM itemModel;
	HTREEITEM itemHull;
	HTREEITEM itemSection;
	HTREEITEM itemDraftSection;
	HTREEITEM itemCrossSection;
	HTREEITEM itemMaterial;
	HTREEITEM itemCondition;
	HTREEITEM itemAnalysis;
	bool itemModelStatus;
	bool itemHullStatus;
	bool itemSectionStatus;
	bool itemDraftSectionStatus;
	bool itemCrossSectionStatus;
	bool itemMaterialStatus;
	bool itemConditionStatus;
	bool itemAnalysisStatus;

	void SetModelStatus(bool is_on);
	void SetHulllStatus(bool is_on);
	void SetSectionStatus(bool is_on);
	void SetDraftStatus(bool is_on);
	void SetCrossStatus(bool is_on);
	void SetMaterialStatus(bool is_on);
	void SetConditionStatus(bool is_on);
	void SetAnalysisStatus(bool is_on);
	int GetModelStatus();
	int GetHulllStatus();
	int GetSectionStatus();
	int GetDraftStatus();
	int GetCrossStatus();
	int GetMaterialStatus();
	int GetConditionStatus();
	int GetAnalysisStatus();

	CIRES2View* m_pView;

	int m_iCurrentToolbar;
	CDlgMainToolbar m_MainToolbar;
	CDlgSectionToolbar m_SectionToolbar;
	void SetToolbar(int index);

protected:
	//CClassToolBar m_wndToolBar;
	CXHtmlTree m_wndClassView;
	CImageList m_ClassViewImages;
	UINT m_nCurrSort;

	void FillClassView();

// 재정의입니다.
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnClassAddMemberFunction();
	afx_msg void OnClassAddMemberVariable();
	afx_msg void OnClassDefinition();
	afx_msg void OnClassProperties();
	afx_msg void OnNewFolder();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg LRESULT OnChangeActiveTab(WPARAM, LPARAM);
	afx_msg void OnSort(UINT id);
	afx_msg void OnUpdateSort(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
};

