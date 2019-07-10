
#include "stdafx.h"
#include "MainFrm.h"
#include "ClassView.h"
#include "Resource.h"
#include "I-RES2.h"
#include "DlgDraftSection.h"
#include "DlgCrossSection.h"
#include "DlgCondition.h"

class CClassViewMenuButton : public CMFCToolBarMenuButton
{
	friend class CClassView;

	DECLARE_SERIAL(CClassViewMenuButton)

public:
	CClassViewMenuButton(HMENU hMenu = nullptr) noexcept : CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
	{
	}

	virtual void OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz = TRUE,
		BOOL bCustomizeMode = FALSE, BOOL bHighlight = FALSE, BOOL bDrawBorder = TRUE, BOOL bGrayDisabledButtons = TRUE)
	{
		pImages = CMFCToolBar::GetImages();

		CAfxDrawState ds;
		pImages->PrepareDrawImage(ds);

		CMFCToolBarMenuButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);

		pImages->EndDrawImage(ds);
	}
};

IMPLEMENT_SERIAL(CClassViewMenuButton, CMFCToolBarMenuButton, 1)

//////////////////////////////////////////////////////////////////////
// 생성/소멸
//////////////////////////////////////////////////////////////////////

CClassView::CClassView() noexcept
{
	m_nCurrSort = ID_SORTING_GROUPBYTYPE;
	itemModelStatus = false;
	itemHullStatus = false;
	itemSectionStatus = false;
	itemDraftSectionStatus = false;
	itemCrossSectionStatus = false;
	itemMaterialStatus = false;
	itemConditionStatus = false;
	itemAnalysisStatus = false;
	m_iCurrentToolbar = -1;
}

CClassView::~CClassView()
{
}

BEGIN_MESSAGE_MAP(CClassView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CLASS_ADD_MEMBER_FUNCTION, OnClassAddMemberFunction)
	ON_COMMAND(ID_CLASS_ADD_MEMBER_VARIABLE, OnClassAddMemberVariable)
	ON_COMMAND(ID_CLASS_DEFINITION, OnClassDefinition)
	ON_COMMAND(ID_CLASS_PROPERTIES, OnClassProperties)
	ON_COMMAND(ID_NEW_FOLDER, OnNewFolder)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnSort)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnUpdateSort)
	ON_NOTIFY(TVN_SELCHANGED, 2, &CClassView::OnTvnSelchanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassView 메시지 처리기

int CClassView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_MainToolbar.Create(IDD_DIALOG_MAIN_TOOLBAR, this);
	m_MainToolbar.ShowWindow(SW_HIDE);

	m_SectionToolbar.Create(IDD_DIALOG_SECTION_TOOLBAR, this);
	m_SectionToolbar.ShowWindow(SW_HIDE);

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 뷰를 만듭니다.
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | WS_BORDER;

	if (!m_wndClassView.Create(dwViewStyle, rectDummy, this, 2))
	{
		TRACE0("클래스 뷰를 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}
	m_wndClassView.ModifyStyle(0, TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS);
	m_wndClassView.Initialize(0, 0);
	m_wndClassView.SetHtml(TRUE);

	// 이미지를 로드합니다.
	//m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SORT);
	//m_wndToolBar.LoadToolBar(IDR_SORT, 0, 0, TRUE /* 잠금 */);

	OnChangeVisualStyle();

	//m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	//m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	//m_wndToolBar.SetOwner(this);

	//// 모든 명령은 부모 프레임이 아닌 이 컨트롤을 통해 라우팅됩니다.
	//m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	//CMenu menuSort;
	//menuSort.LoadMenu(IDR_POPUP_SORT);

	//m_wndToolBar.ReplaceButton(ID_SORT_MENU, CClassViewMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));

	//CClassViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CClassViewMenuButton, m_wndToolBar.GetButton(0));

	//if (pButton != nullptr)
	//{
	//	pButton->m_bText = FALSE;
	//	pButton->m_bImage = TRUE;
	//	pButton->SetImage(GetCmdMgr()->GetCmdImage(m_nCurrSort));
	//	pButton->SetMessageWnd(this);
	//}

	// 정적 트리 뷰 데이터를 더미 코드로 채웁니다.
	FillClassView();

	return 0;
}

void CClassView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CClassView::FillClassView()
{
	itemModel = m_wndClassView.InsertItem(_T("Model <font color = \"red\">[?]</font></b>"), 0, 0);
	itemHull = m_wndClassView.InsertItem(_T("<b>Hull <font color = \"red\">[?]</font>"), 1, 1, itemModel);
	itemSection = m_wndClassView.InsertItem(_T("<b>Section Assignments <font color = \"red\">[?]</font>"), 2, 2, itemModel);
	itemDraftSection = m_wndClassView.InsertItem(_T("<b>Draft Section <font color = \"red\">[?]</font>"), 5, 5, itemSection);
	itemCrossSection = m_wndClassView.InsertItem(_T("<b>Cross Section <font color = \"red\">[?]</font>"), 5, 5, itemSection);
	itemMaterial = m_wndClassView.InsertItem(_T("<b>Material <font color = \"red\">[?]</font>"), 3, 1, itemModel);
	itemCondition = m_wndClassView.InsertItem(_T("<b>Condition <font color = \"red\">[?]</font>"), 4, 1, itemModel);
	m_wndClassView.Expand(itemModel, TVE_EXPAND);
	m_wndClassView.Expand(itemSection, TVE_EXPAND);

	itemAnalysis = m_wndClassView.InsertItem(_T("Analysis"), 0, 0);
}

void CClassView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndClassView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// 클릭한 항목을 선택합니다.
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != nullptr)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
	CMenu menu;
	menu.LoadMenu(IDR_POPUP_SORT);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}
}

void CClassView::AdjustLayout()
{
	if (GetSafeHwnd() == nullptr)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	//int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;
	int toolbar_width = 35;
	m_MainToolbar.ShowWindow(SW_HIDE);
	m_SectionToolbar.ShowWindow(SW_HIDE);
	if (m_iCurrentToolbar > -1)
	{
		m_wndClassView.SetWindowPos(nullptr, rectClient.left + 1, rectClient.top + 1, rectClient.Width() - 2 - toolbar_width, rectClient.Height() - 2, SWP_NOACTIVATE | SWP_NOZORDER);
		switch (m_iCurrentToolbar)
		{
		case 0:
		{
			m_MainToolbar.ShowWindow(SW_SHOW);
			m_MainToolbar.SetWindowPos(nullptr, rectClient.left + rectClient.Width() - toolbar_width, rectClient.top + 1,toolbar_width, rectClient.Height() - 2, SWP_NOACTIVATE | SWP_NOZORDER);
		}
		break;
		case 1:
		{
			m_SectionToolbar.Init();
			m_SectionToolbar.ShowWindow(SW_SHOW);
			m_SectionToolbar.SetWindowPos(nullptr, rectClient.left + rectClient.Width() - toolbar_width, rectClient.top + 1,toolbar_width, rectClient.Height() - 2, SWP_NOACTIVATE | SWP_NOZORDER);
		}
		break;
		}
	}
	else
	{
		m_wndClassView.SetWindowPos(nullptr, rectClient.left + 1, rectClient.top + 1, rectClient.Width() - 2, rectClient.Height() - 2, SWP_NOACTIVATE | SWP_NOZORDER);
	}
}

BOOL CClassView::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}

void CClassView::OnSort(UINT id)
{
	if (m_nCurrSort == id)
	{
		return;
	}

	m_nCurrSort = id;

	//CClassViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CClassViewMenuButton, m_wndToolBar.GetButton(0));

	//if (pButton != nullptr)
	//{
	//	pButton->SetImage(GetCmdMgr()->GetCmdImage(id));
	//	m_wndToolBar.Invalidate();
	//	m_wndToolBar.UpdateWindow();
	//}
}

void CClassView::OnUpdateSort(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(pCmdUI->m_nID == m_nCurrSort);
}

void CClassView::OnClassAddMemberFunction()
{
	AfxMessageBox(_T("멤버 함수 추가..."));
}

void CClassView::OnClassAddMemberVariable()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CClassView::OnClassDefinition()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CClassView::OnClassProperties()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CClassView::OnNewFolder()
{
	AfxMessageBox(_T("새 폴더..."));
}

void CClassView::OnPaint()
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

	CRect rectTree;
	m_wndClassView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CClassView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndClassView.SetFocus();
}

void CClassView::OnChangeVisualStyle()
{
	m_ClassViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_CLASS_VIEW_24 : IDB_CLASS_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("비트맵을 로드할 수 없습니다. %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_ClassViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_ClassViewImages.Add(&bmp, RGB(255, 0, 0));

	m_wndClassView.SetImageList(&m_ClassViewImages, TVSIL_NORMAL);

	//m_wndToolBar.CleanUpLockedImages();
	//m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_SORT_24 : IDR_SORT, 0, 0, TRUE /* 잠금 */);
}

void CClassView::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	HTREEITEM current_item = m_wndClassView.GetSelectedItem();
	if (current_item)
	{
		//CString item_text = m_wndClassView.GetItemText(current_item);
		CMainFrame* m_pFrame = (CMainFrame*)AfxGetMainWnd();

		if (m_pFrame)
		{
			if (current_item == itemHull)
			{
				m_iCurrentToolbar = 0;
				AdjustLayout();
				//m_pFrame->m_wndDlgToolbar.SetToolbar(0);
				//CRect rect;
				//m_pFrame->m_wndDlgToolbar.GetWindowRect(&rect);
				//m_pFrame->m_wndDlgToolbar.MoveWindow(CRect(rect.left, rect.top, rect.left + 30, rect.bottom));
				//m_pFrame->m_wndDlgToolbar.ShowPane(TRUE, FALSE, TRUE);
				//m_pFrame->m_wndDlgToolbar.SetFocus();
			}
			else if (current_item == itemDraftSection || current_item == itemSection)
			{
				m_iCurrentToolbar = 1;
				AdjustLayout();
				//m_pFrame->m_wndDlgToolbar.SetToolbar(1);
				//CRect rect;
				//m_pFrame->m_wndDlgToolbar.GetWindowRect(&rect);
				//m_pFrame->m_wndDlgToolbar.MoveWindow(CRect(rect.left, rect.top, rect.left + 30, rect.bottom));
				//m_pFrame->m_wndDlgToolbar.ShowPane(TRUE, FALSE, TRUE);
				//m_pFrame->m_wndDlgToolbar.SetFocus();

				CDlgDraftSection pDlg(m_pView);
				pDlg.DoModal();
			}
			else if (current_item == itemCrossSection)
			{
				m_iCurrentToolbar = 1;
				AdjustLayout();
				//m_pFrame->m_wndDlgToolbar.SetToolbar(1);
				//CRect rect;
				//m_pFrame->m_wndDlgToolbar.GetWindowRect(&rect);
				//m_pFrame->m_wndDlgToolbar.MoveWindow(CRect(rect.left, rect.top, rect.left + 30, rect.bottom));
				//m_pFrame->m_wndDlgToolbar.ShowPane(TRUE, FALSE, TRUE);
				//m_pFrame->m_wndDlgToolbar.SetFocus();

				CDlgCrossSection pDlg(m_pView);
				pDlg.DoModal();
			}
			else if (current_item == itemCondition)
			{
				m_iCurrentToolbar = -1;
				AdjustLayout();
				//m_pFrame->m_wndDlgToolbar.SetToolbar(1);
				//CRect rect;
				//m_pFrame->m_wndDlgToolbar.GetWindowRect(&rect);
				//m_pFrame->m_wndDlgToolbar.MoveWindow(CRect(rect.left, rect.top, rect.left + 30, rect.bottom));
				//m_pFrame->m_wndDlgToolbar.ShowPane(TRUE, FALSE, TRUE);
				//m_pFrame->m_wndDlgToolbar.SetFocus();

				CDlgCondition pDlg(m_pView);
				pDlg.DoModal();
			}
			else
			{
				m_iCurrentToolbar = -1;
				AdjustLayout();
				//m_pFrame->m_wndDlgToolbar.ShowPane(FALSE, FALSE, FALSE);
			}
		}
		//HTREEITEM parent_item = m_wndClassView.GetParentItem(current_item);
		//if (parent_item)
		//{
		//	CMainFrame* m_pFrame = (CMainFrame*)AfxGetMainWnd();

		//	if (m_pFrame)
		//	{
		//		COSSimulatorView* pview = (COSSimulatorView*)(m_pFrame->GetActiveView());
		//		if (pview)
		//		{
		//			pview->SelectResultFromTree(parent_item, current_item);
		//		}
		//	}
		//}
	}
	*pResult = 0;
}

void CClassView::SetModelStatus(bool is_on)
{
	itemModelStatus = is_on;
	if (is_on)
	{
		m_wndClassView.SetItemText(itemModel, _T("Model <font color = \"green\">[O]</font></b>"));
	}
	else
	{
		m_wndClassView.SetItemText(itemModel, _T("Model <font color = \"red\">[?]</font></b>"));
	}
}

void CClassView::SetHulllStatus(bool is_on)
{
	itemHullStatus = is_on;
	if (is_on)
	{
		m_wndClassView.SetItemText(itemHull, _T("<b>Hull <font color = \"green\">[O]</font>"));
	}
	else
	{
		m_wndClassView.SetItemText(itemHull, _T("<b>Hull <font color = \"red\">[?]</font>"));
	}
}

void CClassView::SetSectionStatus(bool is_on)
{
	itemSectionStatus = is_on;
	if (is_on)
	{
		m_wndClassView.SetItemText(itemSection, _T("<b>Section Assignments <font color = \"green\">[O]</font>"));
	}
	else
	{
		m_wndClassView.SetItemText(itemSection, _T("<b>Section Assignments <font color = \"red\">[?]</font>"));
	}
}

void CClassView::SetDraftStatus(bool is_on)
{
	itemDraftSectionStatus = is_on;
	if (is_on)
	{
		m_wndClassView.SetItemText(itemDraftSection, _T("<b>Draft Section <font color = \"green\">[O]</font>"));
		if (itemCrossSectionStatus)
		{
			SetSectionStatus(true);
		}
	}
	else
	{
		m_wndClassView.SetItemText(itemDraftSection, _T("<b>Draft Section <font color = \"red\">[?]</font>"));
	}
}

void CClassView::SetCrossStatus(bool is_on)
{
	itemCrossSectionStatus = is_on;
	if (is_on)
	{
		m_wndClassView.SetItemText(itemCrossSection, _T("<b>Cross Section <font color = \"green\">[O]</font>"));
		if (itemDraftSectionStatus)
		{
			SetSectionStatus(true);
		}
	}
	else
	{
		m_wndClassView.SetItemText(itemCrossSection, _T("<b>Cross Section <font color = \"red\">[?]</font>"));
	}
}

void CClassView::SetMaterialStatus(bool is_on)
{
	itemMaterialStatus = is_on;
	if (is_on)
	{
		m_wndClassView.SetItemText(itemMaterial, _T("<b>Material <font color = \"green\">[O]</font>"));
	}
	else
	{
		m_wndClassView.SetItemText(itemMaterial, _T("<b>Material <font color = \"red\">[?]</font>"));
	}
}

void CClassView::SetConditionStatus(bool is_on)
{
	itemConditionStatus = is_on;
	if (is_on)
	{
		m_wndClassView.SetItemText(itemCondition, _T("<b>Condition <font color = \"green\">[O]</font>"));
	}
	else
	{
		m_wndClassView.SetItemText(itemCondition, _T("<b>Condition < font color = \"red\">[?]</font>"));
	}
}

void CClassView::SetAnalysisStatus(bool is_on)
{
	itemAnalysisStatus = is_on;
}

int CClassView::GetModelStatus()
{
	return itemModelStatus;
}

int CClassView::GetHulllStatus()
{
	return itemHullStatus;
}

int CClassView::GetSectionStatus()
{
	return itemSectionStatus;
}

int CClassView::GetDraftStatus()
{
	return itemDraftSectionStatus;
}

int CClassView::GetCrossStatus()
{
	return itemCrossSectionStatus;
}

int CClassView::GetMaterialStatus()
{
	return itemMaterialStatus;
}

int CClassView::GetConditionStatus()
{
	return itemConditionStatus;
}

int CClassView::GetAnalysisStatus()
{
	return itemAnalysisStatus;
}