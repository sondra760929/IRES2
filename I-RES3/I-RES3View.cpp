
// I-RES3View.cpp: CIRES3View 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "I-RES3.h"
#endif

#include "I-RES3Doc.h"
#include "I-RES3View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "vtkWin32OpenGLRenderWindow.h"
#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkCellArray.h"
#include "vtkFloatArray.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkUnstructuredGrid.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"
#include "vtkTransform.h"
#include <vtkCullerCollection.h>
#include <vtkFrustumCoverageCuller.h>
#include <vtkQuadricLODActor.h>
#include "vtkProperty.h"
#include "vtkInteractorStyleTrackballCamera.h"

// CIRES3View

IMPLEMENT_DYNCREATE(CIRES3View, CView)

BEGIN_MESSAGE_MAP(CIRES3View, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CIRES3View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CIRES3View 생성/소멸
map< CString, vtkActor* > map_geo;

CIRES3View::CIRES3View() noexcept
{
	m_pvtkMFCWindow = NULL;
	m_pvtkRenderer = vtkRenderer::New();

}

CIRES3View::~CIRES3View()
{
	if (NULL != m_pvtkMFCWindow)
		delete m_pvtkMFCWindow;
}

BOOL CIRES3View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CIRES3View 그리기

void CIRES3View::OnDraw(CDC* /*pDC*/)
{
	CIRES3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CIRES3View 인쇄


void CIRES3View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CIRES3View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CIRES3View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CIRES3View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CIRES3View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CIRES3View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CIRES3View 진단

#ifdef _DEBUG
void CIRES3View::AssertValid() const
{
	CView::AssertValid();
}

void CIRES3View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIRES3Doc* CIRES3View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIRES3Doc)));
	return (CIRES3Doc*)m_pDocument;
}
#endif //_DEBUG


// CIRES3View 메시지 처리기


void CIRES3View::OnDestroy()
{
	if (NULL != m_pvtkRenderer)
		m_pvtkRenderer->Delete();

	CView::OnDestroy();
}


BOOL CIRES3View::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;

	//return CView::OnEraseBkgnd(pDC);
}


void CIRES3View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (NULL != m_pvtkMFCWindow)
		m_pvtkMFCWindow->MoveWindow(0, 0, cx, cy);
}


void CIRES3View::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	m_pvtkMFCWindow = new vtkMFCWindow(this);
	m_pvtkMFCWindow->GetRenderWindow()->AddRenderer(m_pvtkRenderer);
	m_pvtkRenderer->SetBackground(0.0, 0.0, 0.5);

	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(m_pvtkMFCWindow->GetRenderWindow());

	vtkInteractorStyleTrackballCamera *style =
	vtkInteractorStyleTrackballCamera::New();
	iren->SetInteractorStyle(style);
	iren->Initialize();
	iren->Start();
}
