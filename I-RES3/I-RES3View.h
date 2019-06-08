
// I-RES3View.h: CIRES3View 클래스의 인터페이스
//

#pragma once

#include "vtkMFCWindow.h"
#include "vtkRenderer.h"
#include "vtkAssembly.h"

class CIRES3View : public CView
{
protected: // serialization에서만 만들어집니다.
	CIRES3View() noexcept;
	DECLARE_DYNCREATE(CIRES3View)

// 특성입니다.
public:
	CIRES3Doc* GetDocument() const;
	vtkMFCWindow* m_pvtkMFCWindow;
	vtkRenderer* m_pvtkRenderer;

// 작업입니다.
public:

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
};

#ifndef _DEBUG  // I-RES3View.cpp의 디버그 버전
inline CIRES3Doc* CIRES3View::GetDocument() const
   { return reinterpret_cast<CIRES3Doc*>(m_pDocument); }
#endif

