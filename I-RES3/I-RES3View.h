
// I-RES3View.h: CIRES3View 클래스의 인터페이스
//

#pragma once

#include "vtkMFCWindow.h"
#include "vtkRenderer.h"
#include "vtkAssembly.h"
#include "VTKEngine.h"

class CMainFrame;
class CIRES3Doc;

class CIRES3View : public CView
{
protected: // serialization에서만 만들어집니다.
	CIRES3View() noexcept;
	DECLARE_DYNCREATE(CIRES3View)

// 특성입니다.
public:
	CIRES3Doc* GetDocument() const;
	//vtkMFCWindow* m_pvtkMFCWindow;
	//vtkRenderer* m_pvtkRenderer;
	CMainFrame* m_pMainFrame;
	CVTKEngine* vtk_engine;
// 작업입니다.
public:
	bool LoadShapesGeo(const TopoDS_Shape& aShape, vtkAssembly* goede);
	vtkAssembly* FaceToGeometry(const TopoDS_Face& aFace, float face_deflection);
	//vtkAssembly* CurveToGeometry(const TopoDS_Edge& s, const TopoDS_Face& f);
	//vtkAssembly* CurveToGeometry(const TopoDS_Edge& s, float edge_deflection);
	void FindLimits(const Adaptor3d_Curve& aCurve,
		const Standard_Real  aLimit,
		Standard_Real&       First,
		Standard_Real&       Last);

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CIRES3View();
	void OnImportModel();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnInitialUpdate();
	afx_msg void OnViewFront();
	afx_msg void OnViewRear();
	afx_msg void OnViewLeft();
	afx_msg void OnViewRight();
	afx_msg void OnViewTop();
	afx_msg void OnViewBottom();
	afx_msg void OnViewISO();
	afx_msg void OnViewPerspective();
	afx_msg void OnViewOrtho();
};

#ifndef _DEBUG  // I-RES3View.cpp의 디버그 버전
inline CIRES3Doc* CIRES3View::GetDocument() const
   { return reinterpret_cast<CIRES3Doc*>(m_pDocument); }
#endif

