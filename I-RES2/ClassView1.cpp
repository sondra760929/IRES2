
#include "stdafx.h"
#include "MainFrm.h"
#include "ClassView1.h"
#include "Resource.h"
#include "I-RES2.h"
#include "DlgDraftSection.h"
#include "DlgCrossSection.h"
#include "DlgCondition.h"
#include "DlgMaterial.h"
#include "DlgCreateJob.h"
#include "DlgSatelliteData.h"

class CClassView1MenuButton : public CMFCToolBarMenuButton
{
	friend class CClassView1;

	DECLARE_SERIAL(CClassView1MenuButton)

public:
	CClassView1MenuButton(HMENU hMenu = nullptr) noexcept : CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
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

IMPLEMENT_SERIAL(CClassView1MenuButton, CMFCToolBarMenuButton, 1)

//////////////////////////////////////////////////////////////////////
// 생성/소멸
//////////////////////////////////////////////////////////////////////

CClassView1::CClassView1() noexcept
{
	m_nCurrSort = ID_SORTING_GROUPBYTYPE;
	itemModelStatus = false;
	itemHullStatus = false;
	//itemSectionStatus = false;
	itemDraftSectionStatus = false;
	itemCrossSectionStatus = false;
	itemMaterialStatus = false;
	//itemConditionStatus = false;
	itemSatelliteStatus = false;
	itemAnalysisStatus = false;
	m_iCurrentToolbar = -1;
	m_pView = NULL;
}

CClassView1::~CClassView1()
{
}

BEGIN_MESSAGE_MAP(CClassView1, CDockablePane)
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
	ON_NOTIFY(TVN_SELCHANGED, 2, &CClassView1::OnTvnSelchanged)
	ON_NOTIFY(NM_DBLCLK, 2, &CClassView1::OnTvnDoubleClicked)
	ON_COMMAND(ID_ANALYSIS_ADD, &CClassView1::OnAnalysisAdd)
	ON_COMMAND(ID_ANALYSIS_CLEAR, &CClassView1::OnAnalysisClear)
	ON_COMMAND(ID_ANALYSIS1_DELETE, &CClassView1::OnAnalysis1Delete)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassView1 메시지 처리기

int CClassView1::OnCreate(LPCREATESTRUCT lpCreateStruct)
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

	//m_wndToolBar.ReplaceButton(ID_SORT_MENU, CClassView1MenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));

	//CClassView1MenuButton* pButton =  DYNAMIC_DOWNCAST(CClassView1MenuButton, m_wndToolBar.GetButton(0));

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

void CClassView1::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CClassView1::FillClassView()
{
	itemModel = m_wndClassView.InsertItem(_T("Model <font color = \"red\">[?]</font></b>"), 0, 0);
	itemHull = m_wndClassView.InsertItem(_T("<b>Hull <font color = \"red\">[?]</font>"), 1, 1, itemModel);
	//itemSection = m_wndClassView.InsertItem(_T("<b>Section Assignments <font color = \"red\">[?]</font>"), 2, 2, itemModel);
	itemDraftSection = m_wndClassView.InsertItem(_T("<b>Draft Section <font color = \"red\">[?]</font>"), 5, 5, itemModel);
	itemCrossSection = m_wndClassView.InsertItem(_T("<b>Cross Section <font color = \"red\">[?]</font>"), 5, 5, itemModel);
	itemMaterial = m_wndClassView.InsertItem(_T("<b>Material <font color = \"red\">[?]</font>"), 3, 3, itemModel);
	//itemCondition = m_wndClassView.InsertItem(_T("<b>Condition <font color = \"red\">[?]</font>"), 4, 4, itemModel);
	itemSatellite = m_wndClassView.InsertItem(_T("<b>Satellite data input <font color = \"red\">[?]</font>"), 4, 4, itemModel);
	
	m_wndClassView.Expand(itemModel, TVE_EXPAND);
	//m_wndClassView.Expand(itemSection, TVE_EXPAND);

	itemAnalysis = m_wndClassView.InsertItem(_T("Analysis"), 0, 0);
}

void CClassView1::OnContextMenu(CWnd* pWnd, CPoint point)
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
		HTREEITEM hParentItem = pWndTree->GetParentItem(hTreeItem);
		if (hTreeItem != nullptr)
		{
			pWndTree->SelectItem(hTreeItem);

			if (hTreeItem == itemAnalysis)
			{
				if (GetModelStatus())
				{
					CMenu menu;
					CMenu* pPopup;

					// the font popup is stored in a resource 
					menu.LoadMenu(IDR_POPUP_ANALYSIS);
					pPopup = menu.GetSubMenu(0);
					//ClientToScreen(&ptMousePos);
					pPopup->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
				}
			}
			else if (hParentItem == itemAnalysis)
			{
				if (GetModelStatus())
				{
					CMenu menu;
					CMenu* pPopup;

					// the font popup is stored in a resource 
					menu.LoadMenu(IDR_POPUP_ANALYSIS);
					pPopup = menu.GetSubMenu(1);
					//ClientToScreen(&ptMousePos);
					pPopup->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
				}
			}
		}
	}

	pWndTree->SetFocus();
	//CMenu menu;
	//menu.LoadMenu(IDR_POPUP_SORT);

	//CMenu* pSumMenu = menu.GetSubMenu(0);

	//if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	//{
	//	CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

	//	if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
	//		return;

	//	((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
	//	UpdateDialogControls(this, FALSE);
	//}
}

void CClassView1::AdjustLayout()
{
	if (GetSafeHwnd() == nullptr)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	//int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;
	int toolbar_width = 28;
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
			m_SectionToolbar.SetWindowPos(nullptr, rectClient.left + rectClient.Width() - toolbar_width, rectClient.top + 1, toolbar_width, rectClient.Height() - 2, SWP_NOACTIVATE | SWP_NOZORDER);
		}
		break;
		}
	}
	else
	{
		m_wndClassView.SetWindowPos(nullptr, rectClient.left + 1, rectClient.top + 1, rectClient.Width() - 2, rectClient.Height() - 2, SWP_NOACTIVATE | SWP_NOZORDER);
	}
}

BOOL CClassView1::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}

void CClassView1::OnSort(UINT id)
{
	if (m_nCurrSort == id)
	{
		return;
	}

	m_nCurrSort = id;

	//CClassView1MenuButton* pButton =  DYNAMIC_DOWNCAST(CClassView1MenuButton, m_wndToolBar.GetButton(0));

	//if (pButton != nullptr)
	//{
	//	pButton->SetImage(GetCmdMgr()->GetCmdImage(id));
	//	m_wndToolBar.Invalidate();
	//	m_wndToolBar.UpdateWindow();
	//}
}

void CClassView1::OnUpdateSort(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(pCmdUI->m_nID == m_nCurrSort);
}

void CClassView1::OnClassAddMemberFunction()
{
	AfxMessageBox(_T("멤버 함수 추가..."));
}

void CClassView1::OnClassAddMemberVariable()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CClassView1::OnClassDefinition()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CClassView1::OnClassProperties()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CClassView1::OnNewFolder()
{
	AfxMessageBox(_T("새 폴더..."));
}

void CClassView1::OnPaint()
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

	CRect rectTree;
	m_wndClassView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CClassView1::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndClassView.SetFocus();
}

void CClassView1::OnChangeVisualStyle()
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

int CClassView1::CountTreeItems(HTREEITEM hItem, bool Recurse)
{
	int count = 0;
	if (hItem == NULL)
		hItem = m_wndClassView.GetSelectedItem();

	if (m_wndClassView.ItemHasChildren(hItem))
	{
		hItem = m_wndClassView.GetNextItem(hItem, TVGN_CHILD);
		while (hItem)
		{
			count++;
			if (Recurse)
			{
				count += CountTreeItems(hItem, Recurse);
			}
			hItem = m_wndClassView.GetNextItem(hItem, TVGN_NEXT);
		}
	}
	return count;
}

void CClassView1::ClearJobList(bool delete_job_folder)
{
	if (m_wndClassView.ItemHasChildren(itemAnalysis))
	{
		HTREEITEM hNextItem;
		HTREEITEM hChildItem = m_wndClassView.GetChildItem(itemAnalysis);

		while (hChildItem != NULL)
		{
			hNextItem = m_wndClassView.GetNextItem(hChildItem, TVGN_NEXT);
			if (delete_job_folder)
			{
				DeleteJob(hChildItem);
			}
			else
			{
				m_wndClassView.DeleteItem(hChildItem);
			}
			hChildItem = hNextItem;
		}
	}
}

void CClassView1::AddJobItem(CString job_name)
{
	m_wndClassView.InsertItem(job_name, 6, 6, itemAnalysis);
	m_wndClassView.Expand(itemAnalysis, TVE_EXPAND);
}

void CClassView1::CreateJob(HTREEITEM current_item)
{
	CMainFrame* m_pFrame = (CMainFrame*)AfxGetMainWnd();

	if (m_pFrame)
	{
		CString temp_name;
		temp_name.Format("Job(Satellite)_%d", CountTreeItems(current_item) + 1);
		CDlgCreateJob pDlg;
		pDlg.m_strJobName = temp_name;
		if (pDlg.DoModal() == IDOK)
		{
			//	Job 생성
			CString job_path = m_strProjectPath + "\\JOB\\" + pDlg.m_strJobName + "\\";
			if (PathFileExists(job_path))
			{
				AfxMessageBox("Folder exists. Rename Job");
				return;
			}

			AddJobItem(pDlg.m_strJobName);
			m_pFrame->m_wndFileView.AddItem(pDlg.m_strJobName);

			m_pView->CreateJob(pDlg.m_strJobName, 1);
		}
	}
}

void CClassView1::DeleteJob(HTREEITEM current_item)
{
	CMainFrame* m_pFrame = (CMainFrame*)AfxGetMainWnd();

	if (m_pFrame)
	{
		CString job_name = m_wndClassView.GetItemText(current_item);
		m_wndClassView.DeleteItem(current_item);
		m_pFrame->m_wndFileView.DeleteItem(job_name);
		m_pView->DeleteJob(job_name);
	}
}

void CClassView1::SelectJob(HTREEITEM current_item)
{
	CString job_name = m_wndClassView.GetItemText(current_item);
	if (AfxMessageBox("Reset all setting from [" + job_name + "]", MB_YESNO) == IDYES)
	{
		m_pView->SelectJob(job_name);
	}
	//else if (AfxMessageBox("Delete all setting from [" + job_name + "]", MB_YESNO) == IDYES)
	//{
	//	DeleteJob(current_item);
	//}
}

void CClassView1::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	HTREEITEM current_item = m_wndClassView.GetSelectedItem();
	if (current_item)
	{
		//CString item_text = m_wndClassView.GetItemText(current_item);
		CMainFrame* m_pFrame = (CMainFrame*)AfxGetMainWnd();
		if(m_pView)
			m_pView->HideOutputSummury();

		if (m_pFrame)
		{
			if (current_item == itemHull)
			{
				m_iCurrentToolbar = 0;
				AdjustLayout();
			}
			else if (current_item == itemDraftSection/* || current_item == itemSection*/)
			{
				m_iCurrentToolbar = 1;
				AdjustLayout();

				if (!GetDraftStatus())
				{
					if (m_pView->osgHull->getNumChildren() < 1)
					{
						AfxMessageBox("Import HULL data first.");
						m_pView->CalculateOutputResult(1, false);
						return;
					}
					CDlgDraftSection pDlg(m_pView);
					if (pDlg.DoModal() == IDOK)
					{
						//	입력한 파라미터 유지되도록 프로그램 폴더로 복사
						CopyFiles(m_strProjectPath, m_strAppPath);
					}
				}
			}
			else if (current_item == itemCrossSection)
			{
				m_iCurrentToolbar = 1;
				AdjustLayout();
				if (!GetCrossStatus())
				{
					if (m_pView->osgHull->getNumChildren() < 1)
					{
						AfxMessageBox("Import HULL data first.");
						m_pView->CalculateOutputResult(1, false);
						return;
					}

					CDlgCrossSection pDlg(m_pView);
					if (pDlg.DoModal() == IDOK)
					{
						CopyFiles(m_strProjectPath, m_strAppPath);
					}
				}
			}
			//else if (current_item == itemCondition)
			//{
			//	m_iCurrentToolbar = -1;
			//	AdjustLayout();
			//	if (!GetConditionStatus())
			//	{
			//		CDlgCondition pDlg(m_pView);
			//		if (pDlg.DoModal() == IDOK)
			//		{
			//			SetConditionStatus(true);
			//			CopyFiles(m_strProjectPath, m_strAppPath);
			//		}
			//	}
			//}
			else if (current_item == itemMaterial)
			{
				m_iCurrentToolbar = -1;
				AdjustLayout();
				if (!GetMaterialStatus())
				{
					CDlgMaterial pDlg(m_pView);
					if (pDlg.DoModal() == IDOK)
					{
						SetMaterialStatus(true);
						CopyFiles(m_strProjectPath, m_strAppPath);
					}
				}
			}
			else if (current_item == itemSatellite)
			{
				m_iCurrentToolbar = -1;
				AdjustLayout();
				if (!GetSatelliteStatus())
				{
					CDlgSatelliteData pDlg(0, m_pView);
					if (pDlg.DoModal() == IDOK)
					{
						SetSatelliteStatus(true);
						CopyFiles(m_strProjectPath, m_strAppPath);
					}
				}
			}
			else if (current_item == itemAnalysis)
			{
				m_iCurrentToolbar = -1;
				AdjustLayout();

				if (!GetModelStatus())
				{
					AfxMessageBox("Complete All Settting first.");
					return;
				}

				//CreateJob(current_item);
			}
			else
			{
				m_iCurrentToolbar = -1;
				AdjustLayout();

				HTREEITEM parent_item = m_wndClassView.GetParentItem(current_item);
				if (parent_item == itemAnalysis)
				{
					//	job 선택
					SelectJob(current_item);
				}
			}
		}
	}
	*pResult = 0;
}

void CClassView1::OnTvnDoubleClicked(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	HTREEITEM current_item = m_wndClassView.GetSelectedItem();
	if (current_item)
	{
		//CString item_text = m_wndClassView.GetItemText(current_item);
		CMainFrame* m_pFrame = (CMainFrame*)AfxGetMainWnd();

		if (m_pFrame)
		{
			if (current_item == itemDraftSection && GetDraftStatus())
			{
				if (m_pView->osgHull->getNumChildren() < 1)
				{
					AfxMessageBox("Import HULL data first.");
					m_pView->CalculateOutputResult(1, false);
					return;
				}

				CDlgDraftSection pDlg(m_pView);
				if (pDlg.DoModal() == IDOK)
				{
					CopyFiles(m_strProjectPath, m_strAppPath);
				}
			}
			else if (current_item == itemCrossSection && GetCrossStatus())
			{
				if (m_pView->osgHull->getNumChildren() < 1)
				{
					AfxMessageBox("Import HULL data first.");
					m_pView->CalculateOutputResult(1, false);
					return;
				}

				CDlgCrossSection pDlg(m_pView);
				if (pDlg.DoModal() == IDOK)
				{
					CopyFiles(m_strProjectPath, m_strAppPath);
				}
			}
			//else if (current_item == itemCondition && GetConditionStatus())
			//{
			//	CDlgCondition pDlg(m_pView);
			//	if (pDlg.DoModal() == IDOK)
			//	{
			//		SetConditionStatus(true);
			//		CopyFiles(m_strProjectPath, m_strAppPath);
			//	}
			//}
			else if (current_item == itemMaterial && GetMaterialStatus())
			{
				CDlgMaterial pDlg(m_pView);
				if (pDlg.DoModal() == IDOK)
				{
					SetMaterialStatus(true);
					CopyFiles(m_strProjectPath, m_strAppPath);
				}
			}
			else if (current_item == itemSatellite && GetSatelliteStatus())
			{
				CDlgSatelliteData pDlg(0, m_pView);
				if (pDlg.DoModal() == IDOK)
				{
					SetSatelliteStatus(true);
					CopyFiles(m_strProjectPath, m_strAppPath);
				}
			}
			else if (current_item == itemAnalysis && GetModelStatus())
			{
				CreateJob(current_item);
			}
			else
			{
				HTREEITEM parent_item = m_wndClassView.GetParentItem(current_item);
				if (parent_item == itemAnalysis)
				{
					//	job 선택
					SelectJob(current_item);
				}
			}
		}
	}
	*pResult = 0;
}

void CClassView1::SetModelStatus(bool is_on)
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

void CClassView1::SetHulllStatus(bool is_on)
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

//void CClassView1::SetSectionStatus(bool is_on)
//{
//	itemSectionStatus = is_on;
//	if (is_on)
//	{
//		m_wndClassView.SetItemText(itemSection, _T("<b>Section Assignments <font color = \"green\">[O]</font>"));
//	}
//	else
//	{
//		m_wndClassView.SetItemText(itemSection, _T("<b>Section Assignments <font color = \"red\">[?]</font>"));
//	}
//}
//
void CClassView1::UpdateStatus()
{
	//if (itemCrossSectionStatus && itemDraftSectionStatus)
	//{
	//	SetSectionStatus(true);
	//}

	if (itemHullStatus && itemCrossSectionStatus && itemDraftSectionStatus && itemMaterialStatus/* && itemConditionStatus*/)
	{
		SetModelStatus(true);
	}
}
void CClassView1::SetDraftStatus(bool is_on)
{
	itemDraftSectionStatus = is_on;
	if (is_on)
	{
		m_wndClassView.SetItemText(itemDraftSection, _T("<b>Draft Section <font color = \"green\">[O]</font>"));
		UpdateStatus();
	}
	else
	{
		m_wndClassView.SetItemText(itemDraftSection, _T("<b>Draft Section <font color = \"red\">[?]</font>"));
	}
}

void CClassView1::SetCrossStatus(bool is_on)
{
	itemCrossSectionStatus = is_on;
	if (is_on)
	{
		m_wndClassView.SetItemText(itemCrossSection, _T("<b>Cross Section <font color = \"green\">[O]</font>"));
		UpdateStatus();
	}
	else
	{
		m_wndClassView.SetItemText(itemCrossSection, _T("<b>Cross Section <font color = \"red\">[?]</font>"));
	}
}

void CClassView1::SetSatelliteStatus(bool is_on)
{
	itemSatelliteStatus = is_on;
	if (is_on)
	{
		m_wndClassView.SetItemText(itemSatellite, _T("<b>Satellite data input <font color = \"green\">[O]</font>"));
		UpdateStatus();
	}
	else
	{
		m_wndClassView.SetItemText(itemSatellite, _T("<b>Satellite data input <font color = \"red\">[?]</font>"));
	}
}

void CClassView1::SetMaterialStatus(bool is_on)
{
	itemMaterialStatus = is_on;
	if (is_on)
	{
		m_wndClassView.SetItemText(itemMaterial, _T("<b>Material <font color = \"green\">[O]</font>"));
		UpdateStatus();
	}
	else
	{
		m_wndClassView.SetItemText(itemMaterial, _T("<b>Material <font color = \"red\">[?]</font>"));
	}
}

//void CClassView1::SetConditionStatus(bool is_on)
//{
//	itemConditionStatus = is_on;
//	if (is_on)
//	{
//		m_wndClassView.SetItemText(itemCondition, _T("<b>Condition <font color = \"green\">[O]</font>"));
//		UpdateStatus();
//	}
//	else
//	{
//		m_wndClassView.SetItemText(itemCondition, _T("<b>Condition <font color = \"red\">[?]</font>"));
//	}
//}

void CClassView1::SetAnalysisStatus(bool is_on)
{
	itemAnalysisStatus = is_on;
}

int CClassView1::GetModelStatus()
{
	return itemModelStatus;
}

int CClassView1::GetHulllStatus()
{
	return itemHullStatus;
}

//int CClassView1::GetSectionStatus()
//{
//	return itemSectionStatus;
//}

int CClassView1::GetDraftStatus()
{
	return itemDraftSectionStatus;
}

int CClassView1::GetCrossStatus()
{
	return itemCrossSectionStatus;
}

int CClassView1::GetMaterialStatus()
{
	return itemMaterialStatus;
}

int CClassView1::GetSatelliteStatus()
{
	return itemSatelliteStatus;
}

//int CClassView1::GetConditionStatus()
//{
//	return itemConditionStatus;
//}

int CClassView1::GetAnalysisStatus()
{
	return itemAnalysisStatus;
}

void CClassView1::OnAnalysisAdd()
{
	CreateJob(itemAnalysis);
}


void CClassView1::OnAnalysisClear()
{
	if (AfxMessageBox("Delete All JOBs ?", MB_YESNO) == IDYES)
	{
		ClearJobList(true);
	}
}


void CClassView1::OnAnalysis1Delete()
{
	HTREEITEM current_item = m_wndClassView.GetSelectedItem();
	if (current_item)
	{
		HTREEITEM parent_item = m_wndClassView.GetParentItem(current_item);
		if (parent_item == itemAnalysis)
		{
			CString item_name = m_wndClassView.GetItemText(current_item);
			CString temp_string;
			temp_string.Format("Delete Job [%s] ?", item_name);
			if (AfxMessageBox(temp_string, MB_YESNO) == IDYES)
			{
				DeleteJob(current_item);
			}
		}
	}
}
